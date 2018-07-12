#ifndef WAYPOINT_HPP
#define WAYPOINT_HPP

#include <dji_vehicle.hpp>
#include <QObject>
#include <QHash>

#include <utils/constset.hpp>

class Waypoint : public QObject
{
    Q_OBJECT

public:
    explicit Waypoint(DJI::OSDK::Vehicle    *vehicle = 0,
                      QObject               *parent = 0);
    ~Waypoint();

    void initWaypoint(const QHash<QString, int> &settings);
    void loadWaypoint(const QHash<QString, int> &settings);
    void startWaypoint();
    void abortWaypoint();

private:
    DJI::OSDK::WayPointSettings      wayPointDataTmp;
    DJI::OSDK::WayPointInitSettings *wpInitSettings;

    DJI::OSDK::Vehicle  *vehicle;
};

#endif // WAYPOINT_HPP
