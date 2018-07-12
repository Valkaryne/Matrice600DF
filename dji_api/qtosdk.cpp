#include "qtosdk.hpp"
#include <QDebug>

using namespace DJI;
using namespace DJI::OSDK;

QtOsdk::QtOsdk(QObject *parent)
    : QObject(parent)
{
    vehicle = nullptr;
    //qRegisterMetaType<QHash<QString, int>>("QHash<QString, int>");
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
                appIDInput = line.remove(0,3);
            else if (line.startsWith("KEY:"))
                keyInput = line.remove(0,4);
        }
        f.close();
    }

    qDebug() << "ID: " << appIDInput;
    qDebug() << "KEY: " << keyInput;
}

void QtOsdk::initVehicle()
{
    readAppIDKey();

    vehicle = new Vehicle(COMPORT.toStdString().c_str(),
                          BAUDRATE, true);
    if (vehicle) {
        emit changeInitButton("Vehicle Initialized", true);
        QThread::msleep(100);
    } else {
        emit changeInitButton("Init Failed", false);
    }
}

void QtOsdk::activate()
{
    Vehicle::ActivateData activateData;
    activateData.ID = static_cast<uint32_t>(appIDInput.toUInt());
    QByteArray key = keyInput.toLocal8Bit();
    char keyArray[65];
    activateData.encKey = keyArray;
    strcpy(activateData.encKey, key.toStdString().c_str());
    activateData.version = vehicle->getFwVersion();
    vehicle->activate(&activateData, activateCallback, this);
}

void QtOsdk::activateCallback(Vehicle *vehicle, RecvContainer recvFrame,
                              UserData userData)
{
    QtOsdk  *sdk = (QtOsdk*)userData;
    ACK::ErrorCode ack_data;
    if (recvFrame.recvInfo.len - OpenProtocol::PackageMin <= 2) {
        ack_data.data = recvFrame.recvData.ack;
        ack_data.info = recvFrame.recvInfo;

        if (ACK::getError(ack_data)) {
            emit sdk->changeActivateButton(
                        QString("Activation Error: %1").arg(ack_data.data),false);
            ACK::getErrorCodeMessage(ack_data, __func__);
        } else {
            emit sdk->changeActivateButton(QString("Activation Successful"), true);
            //QThread::msleep(100);
            sdk->initComponents();
        }
    } else {
        emit sdk->changeActivateButton(QString("ACK Decode Error"), false);
    }



    // Do the stuff the OSDK callback does, since it is private and we cannot call
    // it here
    if (ack_data.data == OpenProtocolCMD::ErrorCode::ActivationACK::SUCCESS &&
            vehicle->getAccountData().encKey) {
        vehicle->protocolLayer->setKey(vehicle->getAccountData().encKey);
    }
}

void QtOsdk::obtainCtrl(QString ctrlOperation)
{
    if (ctrlOperation == "Release Control")
        vehicle->releaseCtrlAuthority(QtOsdk::setControlCallback, this);
    else if (ctrlOperation == "Obtain Control")
        vehicle->obtainCtrlAuthority(QtOsdk::setControlCallback, this);
}

void QtOsdk::resetConnection()
{
    subscribe->stopPkgRequested();
    QThread::msleep(100);
    //vehicle->closePort();
    emit changeConnectionButtons();
}

void QtOsdk::flightRunCommandRequest(int &commandIndex)
{
    flightController->flightRunCommand(commandIndex);
}

void QtOsdk::startRotationRequest(int &yawRate)
{
    flightController->startRotation(yawRate);
}

void QtOsdk::stopRotationRequest()
{
    flightController->stopRotation();
}

void QtOsdk::initWaypointRequest(const QHash<QString, int> &settings)
{
    waypoint->initWaypoint(settings);
}

void QtOsdk::loadWaypointRequest(const QHash<QString, int> &settings)
{
    waypoint->loadWaypoint(settings);
}

void QtOsdk::startWaypointRequest()
{
    waypoint->startWaypoint();
}

void QtOsdk::abortWaypointRequest()
{
    waypoint->abortWaypoint();
}

void QtOsdk::setControlCallback(Vehicle *vehicle, RecvContainer recvFrame,
                                UserData userData)
{
    QtOsdk  *sdk = (QtOsdk*)userData;
    ACK::ErrorCode ack;
    ack.data = OpenProtocolCMD::ErrorCode::CommonACK::NO_RESPONSE_ERROR;

    if (recvFrame.recvInfo.len - OpenProtocol::PackageMin <= sizeof(uint16_t)) {
        ack.data = recvFrame.recvData.ack;
        ack.info = recvFrame.recvInfo;
    } else {
        DERROR("ACK is exception, sequence %d\n", recvFrame.recvInfo.seqNumber);
        return;
    }

    if (ack.data == OpenProtocolCMD::ErrorCode::ControlACK::SetControl::RC_MODE_ERROR) {
        if (sdk)
            qDebug() << "Switch to mode P";
        else
            DERROR("SDK not initialized.");
    }

    if (ack.data == OpenProtocolCMD::ErrorCode::ControlACK::SetControl::OBTAIN_CONTROL_SUCCESS) {
        if (sdk)
            emit sdk->changeControlAuthorityStatus("Obtained Control");
        else
            DERROR("SDK not initialized");
    }

    if (ack.data == OpenProtocolCMD::ErrorCode::ControlACK::SetControl::RELEASE_CONTROL_SUCCESS) {
        if (sdk)
            emit sdk->changeControlAuthorityStatus("Released Control");
        else
            DERROR("SDK not initialized");
    }

    if (ack.data == OpenProtocolCMD::ErrorCode::ControlACK::SetControl::OBTAIN_CONTROL_IN_PROGRESS) {
        vehicle->obtainCtrlAuthority(QtOsdk::setControlCallback, sdk);
        emit sdk->changeControlAuthorityStatus("Obtaining Control...");
    }

    if (ack.data == OpenProtocolCMD::ErrorCode::ControlACK::SetControl::RELEASE_CONTROL_IN_PROGRESS) {
        vehicle->releaseCtrlAuthority(QtOsdk::setControlCallback, sdk);
        emit sdk->changeControlAuthorityStatus("Releasing Control...");
    }
}

void QtOsdk::initComponents()
{
    this->subscribe = new Subscribe(this->vehicle, 0);
    connect(this->subscribe, SIGNAL(subscribeDataReady(const QVector<double> &)),
            SIGNAL(receiveTelemetryData(const QVector<double> &)));

    this->flightController = new FlightController(this->vehicle, 0);

    this->waypoint = new Waypoint(this->vehicle, 0);
}
