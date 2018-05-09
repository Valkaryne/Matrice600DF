#ifndef FLIGHTCONTROLLER_HPP
#define FLIGHTCONTROLLER_HPP

#include <dji_vehicle.hpp>
#include <QObject>

class FlightController : public QObject
{
    Q_OBJECT

public:
    explicit FlightController(DJI::OSDK::Vehicle    *vehicle = 0,
                              QObject               *parent = 0);
    ~FlightController();

    static void actionCallback(DJI::OSDK::Vehicle      *vehiclePtr,
                               DJI::OSDK::RecvContainer recvFrame,
                               DJI::OSDK::UserData      userData);

private slots:
    void flightRunCommand(int commandIndex);
    void commandStatusChanged(QString);

signals:
    void changeCommandStatus(QString);

private:
    DJI::OSDK::Vehicle              *vehicle;
    DJI::OSDK::Control::CtrlData     command;

};

#endif // FLIGHTCONTROLLER_HPP
