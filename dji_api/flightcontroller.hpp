#ifndef FLIGHTCONTROLLER_HPP
#define FLIGHTCONTROLLER_HPP

#include <dji_vehicle.hpp>
#include <QObject>
#include <QTimer>

class FlightController : public QObject
{
    Q_OBJECT
public:
    explicit FlightController(DJI::OSDK::Vehicle    *vehicle = 0,
                              QObject               *parent = 0);
    ~FlightController();

    static void actionCallback(DJI::OSDK::Vehicle       *vehiclePtr,
                               DJI::OSDK::RecvContainer  recvFrame,
                               DJI::OSDK::UserData       userData);

    void flightRunCommand(int &commandIndex);

    void setAutoHorizontalVelocity(int &velocity);
    void setAutoYawRate(int &rate);

    void autoYaw(int &direction);
    void autoPitch(int &direction);
    void autoRoll(int &direction);

    void stableThrust(int &direction);
    void stableYaw(int &direction);
    void stablePitch(int &direction);
    void stableRoll(int &direction);

    void slowThrust(int &direction);
    void slowYaw(int &direction);
    void slowPitch(int &direction);
    void slowRoll(int &direction);

    void fastThrust(int &direction);
    void fastYaw(int &direction);
    void fastPitch(int &direction);
    void fastRoll(int &direction);

    void resetThrust();
    void resetYaw();
    void resetPitch();
    void resetRoll();

    void setDefiniteDirection(double &heading);
    void sendFlightCommand();

private:
    DJI::OSDK::Vehicle              *vehicle;
    DJI::OSDK::Control::CtrlData     command;

    int autoHorizontalVelocity = 6;
    int autoYawRate = 10;

    int stableHorizontalVelocity = 5;
    int stableVerticalVelocity = 2;
    int stableYawRate = 6;

    int slowHorizontalVelocity = 3;
    int slowVerticalVelocity = 1;
    int slowYawRate = 3;

    int fastHorizontalVelocity = 7;
    int fastVerticalVelocity = 3;
    int fastYawRate = 12;

    //QTimer  *autoSend;
};

#endif // FLIGHTCONTROLLER_HPP
