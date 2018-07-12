#include "flightcontroller.hpp"
#include <QDebug>

using namespace DJI;
using namespace DJI::OSDK;

FlightController::FlightController(Vehicle *vehicle,
                                   QObject *parent)
    : QObject(parent),
      command(0, 0, 0, 0, 0)
{
    this->vehicle = vehicle;

    // Set yaw logic according to yaw rate
    command.flag &= 0xF7;
    command.flag |= Control::YawLogic::YAW_RATE;

    autoSend = new QTimer;
    autoSend->setInterval(100);
    connect(autoSend, SIGNAL(timeout()), SLOT(moveSend()));
}

FlightController::~FlightController() {}

void FlightController::actionCallback(Vehicle *vehiclePtr, RecvContainer recvFrame,
                                      UserData userData)
{
    FlightController *fc = (FlightController*)userData;
    ACK::ErrorCode    ack;
    Control          *controlPtr = vehiclePtr->control;

    if (recvFrame.recvInfo.len - OpenProtocol::PackageMin <= sizeof(uint16_t))
    {
        ack.info = recvFrame.recvInfo;
        ack.data = recvFrame.recvData.commandACK;

        if (ACK::getError(ack)) {
            ACK::getErrorCodeMessage(ack, __func__);
            qDebug() << "Error! See callback.";
        } else {
            qDebug() << "Success";
        }
    } else {
        DERROR("ACK is exception, sequence %d\n", recvFrame.recvInfo.seqNumber);
    }
}

void FlightController::flightRunCommand(int &commandIndex)
{
    int flightTask;
    switch (commandIndex) {
    case 0:
        flightTask = Control::FlightCommand::takeOff;
        break;
    case 1:
        flightTask = Control::FlightCommand::landing;
        break;
    case 2:
        flightTask = Control::FlightCommand::goHome;
        break;
    case 3:
        flightTask = Control::FlightCommand::exitTakeOff;
        break;
    case 4:
        flightTask = Control::FlightCommand::exitLanding;
        break;
    case 5:
        flightTask = Control::FlightCommand::exitGoHome;
        break;
    case 6:
        flightTask = Control::FlightCommand::calibrateCompass;
        break;
    case 7:
        flightTask = Control::FlightCommand::exitCalibrateCompass;
        break;
    case 8:
        flightTask = Control::FlightCommand::landingGearUp;
        break;
    case 9:
        flightTask = Control::FlightCommand::landingGearDown;
        break;
    default:
        DERROR("Unknown command %d", commandIndex);
        break;
    }
    qDebug() << commandIndex << flightTask;
    vehicle->control->action(flightTask, FlightController::actionCallback,
                             this);
}

void FlightController::startRotation(int &yawRate)
{
    this->command.yaw = yawRate;
    autoSend->start();
}

void FlightController::stopRotation()
{
    autoSend->stop();
}

void FlightController::moveSend()
{
    vehicle->control->flightCtrl(command);
}
