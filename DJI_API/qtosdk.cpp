#include "qtosdk.hpp"
#include <QDebug>

using namespace DJI;
using namespace DJI::OSDK;

QtOsdk::QtOsdk(QObject *parent)
    : QObject(parent)
{
    vehicle = 0;

    connect(this, SIGNAL(changeControlStatus(bool)), SLOT(controlStatusChanged(bool)));
    connect(this, SIGNAL(changeActivateStatus()), SLOT(activationSuccessful()));

    initVehicle();
    activate();
    obtainControl();

    // create signal-slot chains for activation and obtaining control
}

QtOsdk::~QtOsdk()
{
    if (vehicle)
        delete vehicle;
}

void QtOsdk::readAppIDKey()
{
    QFile f(":/UserConfig.txt");
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
        DSTATUS("No UserConfig.txt file found");
    else
    {
        while (!f.atEnd())
        {
            QByteArray line = f.readLine();
            if (line.startsWith("ID:"))
                appID.append(line.remove(0, 3));
            else if (line.startsWith("KEY:"))
                appKey.append(line.remove(0, 4));
        }
        f.close();
    }
}

void QtOsdk::activateCallback(Vehicle *vehicle, RecvContainer recvFrame,
                              UserData userData)
{
    QtOsdk  *sdk = (QtOsdk*)userData;
    ACK::ErrorCode  ack_data;
    if (recvFrame.recvInfo.len - OpenProtocol::PackageMin <= 2)
    {
        ack_data.data = recvFrame.recvData.ack;
        ack_data.info = recvFrame.recvInfo;

        if (ACK::getError(ack_data))
        {
            qDebug() << "Activation Error: " << ack_data.data;
            ACK::getErrorCodeMessage(ack_data, __func__);
        }
        else
        {
            qDebug() << "Activation Successful";
            emit sdk->changeActivateStatus();
        }
    }
    else
    {
        qDebug() << "ACD Decode Error";
    }

    // Do the stuff the OSDK callback does, since it is private and we cannot call
    // it here
    if (ack_data.data == OpenProtocolCMD::ErrorCode::ActivationACK::SUCCESS &&
            vehicle->getAccountData().encKey)
    {
        vehicle->protocolLayer->setKey(vehicle->getAccountData().encKey);
    }
}

void QtOsdk::setControlCallback(Vehicle *vehicle, RecvContainer recvFrame,
                                UserData userData)
{
    QtOsdk  *sdk = (QtOsdk*)userData;
    ACK::ErrorCode ack;
    ack.data = OpenProtocolCMD::ErrorCode::CommonACK::NO_RESPONSE_ERROR;

    if (recvFrame.recvInfo.len - OpenProtocol::PackageMin <= sizeof(uint16_t))
    {
        ack.data = recvFrame.recvData.ack;
        ack.info = recvFrame.recvInfo;
    }
    else
    {
        DERROR("ACK is exception, sequence %d\n", recvFrame.recvInfo.seqNumber);
        return;
    }
    if (ack.data ==
            OpenProtocolCMD::ErrorCode::ControlACK::SetControl::RC_MODE_ERROR)
    {
        if (sdk)
        {
            qDebug() << "Switch to mode P";
        }
        else
            DERROR("SDK not initialized.");
    }
    if (ack.data ==
            OpenProtocolCMD::ErrorCode::ControlACK::SetControl::OBTAIN_CONTROL_SUCCESS)
    {
        if (sdk)
        {
            qDebug() << "Obtained Control";
            emit sdk->changeControlStatus(true);
        }
        else
            DERROR("SDK not initialized.");
    }
    if (ack.data ==
            OpenProtocolCMD::ErrorCode::ControlACK::SetControl::RELEASE_CONTROL_SUCCESS)
    {
        if (sdk)
        {
            qDebug() << "Released Control";
            emit sdk->changeControlStatus(false);
        }
        else
            DERROR("SDK not initialized.");
    }
    if (ack.data ==
            OpenProtocolCMD::ErrorCode::ControlACK::SetControl::OBTAIN_CONTROL_IN_PROGRESS)
    {
        vehicle->obtainCtrlAuthority(QtOsdk::setControlCallback, sdk);
        qDebug() << "Obtaining Control...";
    }
    if (ack.data ==
            OpenProtocolCMD::ErrorCode::ControlACK::SetControl::RELEASE_CONTROL_IN_PROGRESS)
    {
        vehicle->releaseCtrlAuthority(QtOsdk::setControlCallback, sdk);
        qDebug() << "Releasing Control...";
    }
}

Subscribe* QtOsdk::getSubscribeModule()
{
    return subscribe;
}

void QtOsdk::initModules()
{
    this->subscribe = new Subscribe(this->vehicle, 0);
    connect(this->subscribe, SIGNAL(subscribeDataReady(const QVector<double>)), SIGNAL(throwSubscribeData(const QVector<double>)));

    this->flightController = new FlightController(this->vehicle, 0);
    connect(this, SIGNAL(runCommandRequest(int)), this->flightController, SLOT(flightRunCommand(int)));

    this->waypoint = new Waypoint(this->vehicle, 0);
    connect(this, SIGNAL(initWaypointRequest(QHash<QString,int>)), this->waypoint, SLOT(initWaypoint(QHash<QString,int>)));
    connect(this, SIGNAL(loadWaypointRequest(QHash<QString,int>)), this->waypoint, SLOT(loadWaypoint(QHash<QString,int>)));
    connect(this, SIGNAL(startWaypointRequest()), this->waypoint, SLOT(startWaypoint()));
    connect(this, SIGNAL(abortWaypointRequest()), this->waypoint, SLOT(abortWaypoint()));
}

void QtOsdk::initVehicle()
{
    readAppIDKey();

    vehicle = new Vehicle(COMPORT.toStdString().c_str(),
                          BAUDRATE, true);
    if (vehicle)
    {
        qDebug() << "Vehicle initialized";
        QThread::msleep(100);
    }
    else
    {
        qDebug() << "Init Failed";
    }
}

void QtOsdk::activate()
{
    Vehicle::ActivateData   activateData;
    activateData.ID = static_cast<uint32_t>(appID.toUInt());
    QByteArray key = appKey.toLocal8Bit();
    char keyArray[65];
    activateData.encKey = keyArray;
    strcpy(activateData.encKey, key.toStdString().c_str());
    activateData.version = vehicle->getFwVersion();
    vehicle->activate(&activateData, activateCallback, this);
}

void QtOsdk::obtainControl()
{
    if (controlObtained)
        vehicle->releaseCtrlAuthority(QtOsdk::setControlCallback, this);
    else
        vehicle->obtainCtrlAuthority(QtOsdk::setControlCallback, this);
}

void QtOsdk::controlStatusChanged(bool obtain)
{
    controlObtained = obtain;
}

void QtOsdk::activationSuccessful()
{
    qDebug() << "Hardware version: " << vehicle->getHwVersion();
    Version::FirmWare fwVersion = vehicle->getFwVersion();
    uint8_t ver1 = (fwVersion >> 24) & 0x000000ff;
    uint8_t ver2 = (fwVersion >> 16) & 0x000000ff;
    uint8_t ver3 = (fwVersion >> 8) & 0x000000ff;
    uint8_t ver4 = fwVersion & 0x000000ff;
    qDebug() << QString("%1.%2.%3.%4").arg(ver1).arg(ver2).arg(ver3).arg(ver4);
    // Initialize all the other parts of the SDK
    initModules();
}

