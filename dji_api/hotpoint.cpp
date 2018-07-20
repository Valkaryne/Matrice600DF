#include "hotpoint.hpp"
#include <QDebug>

using namespace DJI::OSDK;

Hotpoint::Hotpoint(Vehicle *vehicle, QObject *parent) :
    QObject(parent)
{
    this->vehicle = vehicle;
    vehicle->missionManager->init(DJI_MISSION_TYPE::HOTPOINT);
}

Hotpoint::~Hotpoint() {}

void Hotpoint::hotpointReadCallback(Vehicle *vehicle, RecvContainer recvFrame,
                                    UserData userData)
{
    Hotpoint    *sdk = (Hotpoint*)userData;
    HotpointMission::getHotpointSettingsCallback(vehicle, recvFrame, vehicle->missionManager->hpMission);
    DSTATUS("Refreshing hotpoint data");
    qDebug() << "Latitude: " << vehicle->missionManager->hpMission->getData().latitude;
    qDebug() << "Longitude: " << vehicle->missionManager->hpMission->getData().longitude;
    qDebug() << "Height: " << vehicle->missionManager->hpMission->getData().height;
    qDebug() << "Yaw rate: " << vehicle->missionManager->hpMission->getData().yawRate;
    qDebug() << "Radius: " << vehicle->missionManager->hpMission->getData().radius;
    qDebug() << "Clockwise: " << vehicle->missionManager->hpMission->getData().clockwise;
    qDebug() << "Yaw mode: " << vehicle->missionManager->hpMission->getData().yawMode;
}

void Hotpoint::initHotpoint(const QVector<double> &coordinates)
{
    this->latitude = coordinates.at(0) * DEG2RAD;
    this->longitude = coordinates.at(1) * DEG2RAD;
    this->altitude = coordinates.at(2);
}

void Hotpoint::startHotpoint(int yawRate)
{
    vehicle->missionManager->hpMission->setHotPoint(
                longitude, latitude, altitude);
    vehicle->missionManager->hpMission->setClockwise(yawRate > 0);
    vehicle->missionManager->hpMission->setYawRate(qAbs(yawRate));
    vehicle->missionManager->hpMission->setRadius(5.0);
    vehicle->missionManager->hpMission->setYawMode(HotpointMission::YawMode::YAW_OUTSIDE);
    vehicle->missionManager->hpMission->start(hotpointReadCallback, this);
}

void Hotpoint::stopHotpoint()
{
    vehicle->missionManager->hpMission->stop(hotpointReadCallback, this);
}
