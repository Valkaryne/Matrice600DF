#ifndef HOTPOINT_HPP
#define HOTPOINT_HPP

#include <QObject>
#include <dji_vehicle.hpp>

#include <utils/constset.hpp>

class Hotpoint : public QObject
{
    Q_OBJECT

public:
    explicit Hotpoint(DJI::OSDK::Vehicle    *vehicle = 0,
                      QObject               *parent = 0);
    ~Hotpoint();

    static void hotpointReadCallback(DJI::OSDK::Vehicle        *vehicle,
                                     DJI::OSDK::RecvContainer   recvFrame,
                                     DJI::OSDK::UserData        userData);

    void initHotpoint(const QVector<double> &coordinates);
    void startHotpoint();
    void stopHotpoint();

private:
    DJI::OSDK::Vehicle  *vehicle;

    double latitude, longitude, altitude, velocity;
};


#endif // HOTPOINT_HPP
