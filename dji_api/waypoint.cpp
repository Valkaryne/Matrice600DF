#include "waypoint.hpp"
#include <QDebug>

using namespace DJI::OSDK;

Waypoint::Waypoint(Vehicle *vehicle, QObject *parent) :
    QObject(parent)
{
    this->vehicle = vehicle;
    wpInitSettings = new WayPointInitSettings;

    qRegisterMetaType<QHash<QString, int>>("QHash<QString, int>");
}

Waypoint::~Waypoint() {}

void Waypoint::initWaypoint(const QHash<QString, int> &settings)
{
    wpInitSettings->indexNumber = 2;
    wpInitSettings->maxVelocity = 15;
    wpInitSettings->idleVelocity = settings.value("velocity");
    wpInitSettings->finishAction = 0; // No action
    wpInitSettings->executiveTimes = 1;
    wpInitSettings->yawMode = settings.value("yaw logic");
    wpInitSettings->traceMode = 0;
    wpInitSettings->RCLostAction = settings.value("on rc lost");
    wpInitSettings->gimbalPitch = 0;
    wpInitSettings->latitude = (settings.value("latitude") / 1000000.0) * DEG2RAD;
    wpInitSettings->longitude = (settings.value("longitude") / 1000000.0) * DEG2RAD;
    wpInitSettings->altitude = settings.value("altitude");

    qDebug() << "Init lat: " << wpInitSettings->latitude;
    qDebug() << "Init lng: " << wpInitSettings->longitude;
    qDebug() << "Init alt: " << wpInitSettings->altitude;

    vehicle->missionManager->init(DJI_MISSION_TYPE::WAYPOINT, 0, wpInitSettings);
}

void Waypoint::loadWaypoint(const QHash<QString, int> &settings)
{
    int index = 0;
    wayPointDataTmp.index = index;
    wayPointDataTmp.latitude = (settings.value("latitude") / 1000000.0 + 0.00006) * DEG2RAD;
    wayPointDataTmp.longitude = (settings.value("longitude") / 1000000.0 + 0.00006) * DEG2RAD;
    wayPointDataTmp.altitude = settings.value("altitude");
    wayPointDataTmp.damping = 0;
    wayPointDataTmp.yaw = 0;
    wayPointDataTmp.gimbalPitch = 0;
    wayPointDataTmp.turnMode = 0;
    wayPointDataTmp.hasAction = 0;
    wayPointDataTmp.actionTimeLimit = 0;
    wayPointDataTmp.actionNumber = 0;
    wayPointDataTmp.actionRepeat = 0;
    for (int i; i < 16; ++i)
    {
        wayPointDataTmp.commandList[i] = 0;
        wayPointDataTmp.commandParameter[i] = 0;
    }

    if (!vehicle->missionManager->wpMission->uploadIndexData(&wayPointDataTmp))
        qDebug() << "fail";

    index = 1;
    wayPointDataTmp.index = index;
    wayPointDataTmp.latitude = (settings.value("latitude") / 1000000.0) * DEG2RAD;
    wayPointDataTmp.longitude = (settings.value("longitude") / 1000000.0) * DEG2RAD;
    wayPointDataTmp.altitude = settings.value("altitude");
    wayPointDataTmp.damping = 0;
    wayPointDataTmp.yaw = 0;
    wayPointDataTmp.gimbalPitch = 0;
    wayPointDataTmp.turnMode = 0;
    wayPointDataTmp.hasAction = 0;
    wayPointDataTmp.actionTimeLimit = 0;
    wayPointDataTmp.actionNumber = 0;
    wayPointDataTmp.actionRepeat = 0;
    for (int i; i < 16; ++i)
    {
        wayPointDataTmp.commandList[i] = 0;
        wayPointDataTmp.commandParameter[i] = 0;
    }

    if (!vehicle->missionManager->wpMission->uploadIndexData(&wayPointDataTmp))
        qDebug() << "fail";
}

void Waypoint::startWaypoint()
{
    vehicle->missionManager->wpMission->start();
}

void Waypoint::abortWaypoint()
{
    vehicle->missionManager->wpMission->stop();
}
