#include "subscribe.hpp"
#include <QDebug>

using namespace DJI::OSDK;

Subscribe::Subscribe(Vehicle *vehiclePtr, QObject *parent)
    : QObject(parent)
{
    vehicle = vehiclePtr;

    int numTopics = 3;
    for (int i = 0; i <= numTopics; i++)
        subscribeData.append(0);

    qDebug() << "Subscribe init";
    vehicle->subscribe->verify();

    startPkgRequested();
}

Subscribe::~Subscribe() {}

void Subscribe::prepareSubscribeData(Telemetry::TopicName topicName, uint32_t id)
{
    switch (topicName) {
    case Telemetry::TOPIC_QUATERNION:
    {
        Telemetry::Quaternion q =
                vehicle->subscribe->getValue<Telemetry::TOPIC_QUATERNION>();
        subscribeData.replace(0, asin(q.q3) * 360 / M_PI);
        break;
    }
    case Telemetry::TOPIC_HEIGHT_FUSION:
    {
        float32_t h =
                vehicle->subscribe->getValue<Telemetry::TOPIC_HEIGHT_FUSION>();
        subscribeData.replace(1, (double)h);
        break;
    }
    case Telemetry::TOPIC_GPS_POSITION:
    {
        Telemetry::Vector3d p =
                vehicle->subscribe->getValue<Telemetry::TOPIC_GPS_POSITION>();
        subscribeData.replace(2, p.x);
        subscribeData.replace(3, p.y);
        break;
    }
    default:
        break;
    }
    emit subscribeDataReady(subscribeData);
}

void Subscribe::pkgUnpackCallback(Vehicle *vehiclePtr,
                                  RecvContainer recvContainer,
                                  UserData userData)
{
    Subscribe *subscribe = (Subscribe*)userData;
    int numTopics = subscribe->pkgIndices.size();
    for (int i = 0; i < numTopics; i++) {
        subscribe->prepareSubscribeData(
                    static_cast<Telemetry::TopicName>(subscribe->pkgIndices[i]), 0);
    }
}

void Subscribe::startPkgRequested()
{
    int pkgIndex = 0;
    int freq = 0;
    bool enableTimestamp = false;

    freq = 50; // Hz
    Telemetry::TopicName topicList[Telemetry::TOTAL_TOPIC_NUMBER];
    pkgIndices.push_back(0); // Quaternion
    pkgIndices.push_back(10); // Quaternion
    pkgIndices.push_back(14); // GPS Position
    for (int i = 0; i < pkgIndices.size(); i++)
        topicList[i] = static_cast<Telemetry::TopicName>(pkgIndices[i]);

    bool pkgStaus = vehicle->subscribe->initPackageFromTopicList(
                pkgIndex, pkgIndices.size(), topicList, enableTimestamp, freq);

    if (pkgStaus)
        vehicle->subscribe->startPackage(pkgIndex);
    QThread::msleep(100);
    vehicle->subscribe->registerUserPackageUnpackCallback(pkgIndex, pkgUnpackCallback,
                                                          this);
}

void Subscribe::stopPkgRequested()
{
    vehicle->subscribe->removePackage(0);
}