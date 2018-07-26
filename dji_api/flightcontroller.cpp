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

void FlightController::setControls(QChar control)
{
    char c = static_cast<char>(control.toLatin1());
    switch (c) {
    case 'W':
        if (command.z < 0) {
            command.z = 0;
            break;
        }
        if (command.z + 1 <= 5) command.z++;
        break;
    case 'A':
        if (command.yaw > 0) {
            command.yaw = 0;
            break;
        }
        //if (command.yaw - 1 >= -36) command.yaw--;
        command.yaw = -6;
        break;
    case 'S':
        if (command.z > 0) {
            command.z = 0;
            break;
        }
        if (command.z - 1 >= -5) command.z--;
        break;
    case 'D':
        if (command.yaw < 0) {
            command.yaw = 0;
            break;
        }
        //if (command.yaw + 1 <= 36) command.yaw++;
        command.yaw = 6;
        break;
    case 'I':
        if (command.x < 0) {
            command.x = 0;
            break;
        }
        if (command.x + 1 <= 10) command.x++;
        break;
    case 'J':
        if (command.y > 0) {
            command.y = 0;
            break;
        }
        if (command.y - 1 >= -10) command.y--;
        break;
    case 'K':
        if (command.x > 0) {
            command.x = 0;
            break;
        }
        if (command.x - 1 >= -10) command.x--;
        break;
    case 'L':
        if (command.y < 0) {
            command.y = 0;
            break;
        }
        if (command.y + 1 <= 10) command.y++;
        break;
    default:
        qDebug() << "Unknown controls";
        break;
    }

    qDebug() << "pitch: " << command.x;
    qDebug() << "roll: " << command.y;
    qDebug() << "thrust: " << command.z;
    qDebug() << "yaw: " << command.yaw;
}

void FlightController::sendFlightCommand()
{
    float32_t duration = 0.04;
    float32_t elapsed = 0;

    if ((command.x == 0) && (command.y == 0) && (command.z == 0) && (command.yaw == 0))
        return;

    while (elapsed < duration) {
        vehicle->control->flightCtrl(command);
        QThread::usleep(20000);
        elapsed += 0.02;
    }
}
