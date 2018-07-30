#include "flightcontroller.hpp"
#include <QDebug>

using namespace DJI;
using namespace DJI::OSDK;

FlightController::FlightController(Vehicle *vehicle,
                                   QObject *parent)
    : QObject(parent),
      command(0,0,0,0,0)
{
    this->vehicle = vehicle;

    command.flag &= 0x3F;
    command.flag |= Control::HorizontalLogic::HORIZONTAL_VELOCITY; // (-10, 10)

    command.flag &= 0xCF;
    command.flag |= Control::VerticalLogic::VERTICAL_VELOCITY; // (-5, 5)

    command.flag &= 0xF7;
    command.flag |= Control::YawLogic::YAW_RATE; // (-36, 36)

    command.flag &= 0xF9;
    command.flag |= Control::HorizontalCoordinate::HORIZONTAL_BODY;

    command.flag &= 0xFE;
    command.flag |= Control::StableMode::STABLE_ENABLE;

    //qDebug() << "Flight Cat";
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

void FlightController::stableThrust(int &direction)
{
    command.z = stableVerticalVelocity * direction;
}

void FlightController::stableYaw(int &direction)
{
    command.yaw = stableYawRate * direction;
}

void FlightController::stablePitch(int &direction)
{
    command.x = stableHorizontalVelocity * direction;
}

void FlightController::stableRoll(int &direction)
{
    command.y = stableHorizontalVelocity * direction;
}

void FlightController::slowThrust(int &direction)
{
    command.z = slowVerticalVelocity * direction;
}

void FlightController::slowYaw(int &direction)
{
    command.yaw = slowYawRate * direction;
}

void FlightController::slowPitch(int &direction)
{
    command.x = slowHorizontalVelocity * direction;
}

void FlightController::slowRoll(int &direction)
{
    command.y = slowHorizontalVelocity * direction;
}

void FlightController::fastThrust(int &direction)
{
    command.z = fastVerticalVelocity * direction;
}

void FlightController::fastYaw(int &direction)
{
    command.yaw = fastYawRate * direction;
}

void FlightController::fastPitch(int &direction)
{
    command.x = fastHorizontalVelocity * direction;
}

void FlightController::fastRoll(int &direction)
{
    command.y = fastHorizontalVelocity * direction;
}

void FlightController::resetThrust()
{
    command.z = 0;
}

void FlightController::resetYaw()
{
    command.yaw = 0;
}

void FlightController::resetPitch()
{
    command.x = 0;
}

void FlightController::resetRoll()
{
    command.y = 0;
}

void FlightController::sendFlightCommand()
{
    //float32_t duration = 0.04;
    //float32_t elapsed = 0;

    if ((command.x == 0) && (command.y == 0) && (command.z == 0) && (command.yaw == 0))
        return;

    vehicle->control->flightCtrl(command);

    /* while (elapsed < duration) {
        vehicle->control->flightCtrl(command);
        QThread::usleep(20000);
        elapsed += 0.02;
    } */
}
