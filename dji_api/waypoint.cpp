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

    initLat = settings.value("latitude");
    initLng = settings.value("longitude");

    qDebug() << "Init lat: " << wpInitSettings->latitude;
    qDebug() << "Init lng: " << wpInitSettings->longitude;
    qDebug() << "Init alt: " << wpInitSettings->altitude;

    vehicle->missionManager->init(DJI_MISSION_TYPE::WAYPOINT, 0, wpInitSettings);
}

void Waypoint::loadWaypoint(const QHash<QString, int> &settings)
{
    int destLat = settings.value("latitude");
    int destLng = settings.value("longitude");

    WayPointSettings *wayPointData = new WayPointSettings();
    wayPointData->index = 0;
    wayPointData->latitude = ((initLat + destLat) / 2000000.0) * DEG2RAD;
    wayPointData->longitude = ((initLng + destLng) / 2000000.0) * DEG2RAD;
    wayPointData->altitude = settings.value("altitude");
    wayPointData->damping = 0;
    wayPointData->yaw = 0;
    wayPointData->gimbalPitch = 0;
    wayPointData->turnMode = 0;
    wayPointData->hasAction = 0;
    wayPointData->actionTimeLimit = 0;
    wayPointData->actionNumber = 0;
    wayPointData->actionRepeat = 0;
    for (int i = 0; i < 16; ++i)
    {
        wayPointData->commandList[i] = 0;
        wayPointData->commandParameter[i] = 0;
    }

    qDebug() << "Lat1: " << QString::number((wayPointData->latitude / 0.01745329252), 'f', 6);
    qDebug() << "Lng1: " << QString::number((wayPointData->longitude / 0.01745329252), 'f', 6);

    if (!vehicle->missionManager->wpMission->uploadIndexData(wayPointData))
        qDebug() << "fail";

    wayPointData = new WayPointSettings();
    wayPointData->index = 1;
    wayPointData->latitude = (destLat / 1000000.0) * DEG2RAD;
    wayPointData->longitude = (destLng / 1000000.0) * DEG2RAD;
    wayPointData->altitude = settings.value("altitude");
    wayPointData->damping = 0;
    wayPointData->yaw = 0;
    wayPointData->gimbalPitch = 0;
    wayPointData->turnMode = 0;
    wayPointData->hasAction = 0;
    wayPointData->actionTimeLimit = 0;
    wayPointData->actionNumber = 0;
    wayPointData->actionRepeat = 0;
    for (int i = 0; i < 16; ++i)
    {
        wayPointData->commandList[i] = 0;
        wayPointData->commandParameter[i] = 0;
    }

    qDebug() << "Lat2: " << QString::number((wayPointData->latitude / 0.01745329252), 'f', 6);
    qDebug() << "Lng2: " << QString::number((wayPointData->longitude / 0.01745329252), 'f', 6);

    if (!vehicle->missionManager->wpMission->uploadIndexData(wayPointData))
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
