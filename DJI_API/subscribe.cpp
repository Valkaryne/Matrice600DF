#include "subscribe.hpp"
#include <QDebug>

using namespace DJI::OSDK;

Subscribe::Subscribe(Vehicle *vehiclePtr, QObject *parent)
    : QObject(parent)
{
    vehicle = vehiclePtr;

    int numTopics = 3;
    for (int i = 0; i < numTopics + 1; i++)
        subscribeData.append(0);

    vehicle->subscribe->verify();

    startPkg0Requested();
    //startPkg1Requested();
    //startPkg2Requested();
}

Subscribe::~Subscribe()
{}

void Subscribe::prepareSubscribeData(Telemetry::TopicName topicName, uint32_t id)
{
    switch (topicName) {
    case Telemetry::TOPIC_QUATERNION:
    {
        Telemetry::Quaternion q =
                vehicle->subscribe->getValue<Telemetry::TOPIC_QUATERNION>();
        subscribeData.replace(0, asin(q.q3) * 360 / 3.14);
        break;
    }
    case Telemetry::TOPIC_HEIGHT_FUSION:
    {
        float32_t d =
                vehicle->subscribe->getValue<Telemetry::TOPIC_HEIGHT_FUSION>();
        subscribeData.replace(1, (double)d);
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

void Subscribe::pkg0UnpackCallback(Vehicle *vehiclePtr,
                                   RecvContainer recvContainer,
                                   UserData userData)
{
    // Receive Quaternion
    Subscribe *subscribe = (Subscribe*)userData;
    int numTopics = subscribe->pkg0Indices.size();
    for (int i = 0; i < numTopics; i++)
    {
        subscribe->prepareSubscribeData(
                    static_cast<Telemetry::TopicName>(subscribe->pkg0Indices[i]), 0);
    }
}

void Subscribe::pkg1UnpackCallback(Vehicle *vehiclePtr,
                                   RecvContainer recvContainer,
                                   UserData userData)
{
    // Receive Height Fusion
    Subscribe *subscribe = (Subscribe*)userData;
    int numTopics = 1;
    for (int i = 0; i < numTopics; i++)
    {
        subscribe->prepareSubscribeData(
                    static_cast<Telemetry::TopicName>(10), 0);
    }
}

void Subscribe::pkg2UnpackCallback(Vehicle *vehiclePtr,
                                   RecvContainer recvContainer,
                                   UserData userData)
{
    // Receive GPS Position
    Subscribe *subscribe = (Subscribe*)userData;
    int numTopics = 1;
    for (int i = 0; i < numTopics; i++)
    {
        subscribe->prepareSubscribeData(
                    static_cast<Telemetry::TopicName>(14), 0);
    }
}

void Subscribe::pkg3UnpackCallback(Vehicle *vehiclePtr,
                                   RecvContainer recvContainer,
                                   UserData userData)
{
    // NOTHING YET
}

void Subscribe::pkg4UnpackCallback(Vehicle *vehiclePtr,
                                   RecvContainer recvContainer,
                                   UserData userData)
{
    // NOTHING YET
}

void Subscribe::startPkg0Requested()
{
    // QUATERNION
    int pkgIndex = 0;
    int freq = 0;
    bool enableTimestamp = false;

    //freq = freqEnum[3]; // 50 Hz
    freq = 50;
    Telemetry::TopicName topicList[Telemetry::TOTAL_TOPIC_NUMBER];
    pkg0Indices.push_back(0); // Quaternion
    pkg0Indices.push_back(10); // Height Fusion
    pkg0Indices.push_back(14); // GPS Position
    for (int i = 0; i < pkg0Indices.size(); i++)
        topicList[i] = static_cast<Telemetry::TopicName>(pkg0Indices[i]);

    //topicList[0] = static_cast<Telemetry::TopicName>(0); // Quaternion index

    bool pkgStatus = vehicle->subscribe->initPackageFromTopicList(
                pkgIndex, pkg0Indices.size(), topicList, enableTimestamp, freq);

    if (pkgStatus)
        vehicle->subscribe->startPackage(pkgIndex);
    QThread::msleep(100);
    vehicle->subscribe->registerUserPackageUnpackCallback(pkgIndex, pkg0UnpackCallback,
                                                          this);
}

void Subscribe::startPkg1Requested()
{

    // HEIGHT_FUSION
    int pkgIndex = 1;
    int freq = 0;
    bool enableTimestamp = false;

    freq = freqEnum[2]; // 10 Hz
    Telemetry::TopicName topicList[Telemetry::TOTAL_TOPIC_NUMBER];
    topicList[0] = static_cast<Telemetry::TopicName>(10); // Height Fusion index

    bool pkgStatus = vehicle->subscribe->initPackageFromTopicList(
                pkgIndex, 1, topicList, enableTimestamp, freq);

    if (pkgStatus)
        vehicle->subscribe->startPackage(pkgIndex);
    QThread::msleep(100);
    vehicle->subscribe->registerUserPackageUnpackCallback(pkgIndex, pkg1UnpackCallback,
                                                          this);
}

void Subscribe::startPkg2Requested()
{

    // GPS_POSITION
    int pkgIndex = 2;
    int freq = 0;
    bool enableTimestamp = false;

    freq = freqEnum[3]; // 50 Hz
    Telemetry::TopicName topicList[Telemetry::TOTAL_TOPIC_NUMBER];
    topicList[0] = static_cast<Telemetry::TopicName>(14); // GPS Position index

    bool pkgStatus = vehicle->subscribe->initPackageFromTopicList(
                pkgIndex, 1, topicList, enableTimestamp, freq);

    if (pkgStatus)
        vehicle->subscribe->startPackage(pkgIndex);
    QThread::msleep(100);
    vehicle->subscribe->registerUserPackageUnpackCallback(pkgIndex, pkg2UnpackCallback,
                                                          this);
}

void Subscribe::startPkg3Requested()
{
    // NOTHING YET
}

void Subscribe::startPkg4Requested()
{
    // NOTHING YET
}

void Subscribe::stopPkg0Requested()
{
    vehicle->subscribe->removePackage(0);
}

void Subscribe::stopPkg1Requested()
{
    vehicle->subscribe->removePackage(1);
}

void Subscribe::stopPkg2Requested()
{
    vehicle->subscribe->removePackage(2);
}

void Subscribe::stopPkg3Requested()
{
    // unnecessary
}

void Subscribe::stopPkg4Requested()
{
    // unnecessary
}
