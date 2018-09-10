#include "subscribe.hpp"
#include <QDebug>

using namespace DJI::OSDK;

Subscribe::Subscribe(Vehicle *vehiclePtr, QObject *parent)
    : QObject(parent)
{
    vehicle = vehiclePtr;

    int numTopics = 4;
    for (int i = 0; i <= numTopics; i++)
        subscribeData.append(0);

    qDebug() << "Subscribe init";
    vehicle->subscribe->verify();

    startPkg50Requested();
    startPkg10Requested();
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
    case Telemetry::TOPIC_ALTITUDE_FUSIONED:
    {
        float32_t h =
                vehicle->subscribe->getValue<Telemetry::TOPIC_ALTITUDE_FUSIONED>();
        subscribeData.replace(1, (double)h);
        break;
    }
    case Telemetry::TOPIC_ALTITUDE_BAROMETER:
    {
        float32_t h =
                vehicle->subscribe->getValue<Telemetry::TOPIC_ALTITUDE_BAROMETER>();
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
    };
    case Telemetry::TOPIC_GPS_FUSED:
    {
        Telemetry::TypeMap<Telemetry::TOPIC_GPS_FUSED>::type gpsPos =
                vehicle->subscribe->getValue<Telemetry::TOPIC_GPS_FUSED>();
        subscribeData.replace(1, gpsPos.altitude);
        subscribeData.replace(2, gpsPos.longitude / DEG2RAD);
        subscribeData.replace(3, gpsPos.latitude / DEG2RAD);
        break;
    }
    case Telemetry::TOPIC_BATTERY_INFO:
    {
        Telemetry::TypeMap<Telemetry::TOPIC_BATTERY_INFO>::type batt =
                vehicle->subscribe->getValue<Telemetry::TOPIC_BATTERY_INFO>();
        subscribeData.replace(4, batt.percentage);
        break;
    }
    default:
        break;
    }
    emit subscribeDataReady(subscribeData);
}

void Subscribe::pkg50UnpackCallback(Vehicle *vehiclePtr,
                                  RecvContainer recvContainer,
                                  UserData userData)
{
    Subscribe *subscribe = (Subscribe*)userData;
    int numTopics = subscribe->pkg50Indices.size();
    for (int i = 0; i < numTopics; i++) {
        subscribe->prepareSubscribeData(
                    static_cast<Telemetry::TopicName>(subscribe->pkg50Indices[i]), 0);
    }
}

void Subscribe::pkg10UnpackCallback(Vehicle *vehiclePtr,
                                    RecvContainer recvContainer,
                                    UserData userData)
{
    Subscribe *subscribe = (Subscribe*)userData;
    int numTopics = subscribe->pkg10Indices.size();
    for (int i = 0; i < numTopics; i++) {
        subscribe->prepareSubscribeData(
                    static_cast<Telemetry::TopicName>(subscribe->pkg10Indices[i]), 0);
    }
}

void Subscribe::startPkg50Requested()
{
    int pkgIndex = 0;
    int freq = 0;
    bool enableTimestamp = false;

    freq = 50; // Hz
    Telemetry::TopicName topicList[Telemetry::TOTAL_TOPIC_NUMBER];
    pkg50Indices.push_back(0); // Quaternion
    //pkgIndices.push_back(7); // Altitude fusioned
    //pkgIndices.push_back(8); // Altitude barometer
    //pkg50Indices.push_back(10); // Height fusion
    pkg50Indices.push_back(11); // GPS Position
    for (int i = 0; i < pkg50Indices.size(); i++)
        topicList[i] = static_cast<Telemetry::TopicName>(pkg50Indices[i]);

    bool pkgStaus = vehicle->subscribe->initPackageFromTopicList(
                pkgIndex, pkg50Indices.size(), topicList, enableTimestamp, freq);

    if (pkgStaus)
        vehicle->subscribe->startPackage(pkgIndex);
    QThread::msleep(100);
    vehicle->subscribe->registerUserPackageUnpackCallback(pkgIndex, pkg50UnpackCallback,
                                                          this);
}

void Subscribe::startPkg10Requested()
{
    int pkgIndex = 1;
    int freq = 0;
    bool enableTimestamp = false;

    freq = 10; // Hz
    Telemetry::TopicName topicList[Telemetry::TOTAL_TOPIC_NUMBER];
    pkg10Indices.push_back(30);
    for (int i = 0; i < pkg10Indices.size(); i++)
        topicList[i] = static_cast<Telemetry::TopicName>(pkg10Indices[i]);

    bool pkgStatus = vehicle->subscribe->initPackageFromTopicList(
                pkgIndex, pkg10Indices.size(), topicList, enableTimestamp, freq);

    if (pkgStatus)
        vehicle->subscribe->startPackage(pkgIndex);
    QThread::msleep(100);
    vehicle->subscribe->registerUserPackageUnpackCallback(pkgIndex, pkg10UnpackCallback,
                                                          this);
}

void Subscribe::stopPkg50Requested()
{
    vehicle->subscribe->removePackage(0);
}

void Subscribe::stopPkg10Requested()
{
    vehicle->subscribe->removePackage(1);
}
