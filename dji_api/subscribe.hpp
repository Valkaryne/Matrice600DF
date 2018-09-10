#ifndef SUBSCRIBE_HPP
#define SUBSCRIBE_HPP

#include <QObject>
#include <QVector>
#include <dji_vehicle.hpp>

#include <utils/constset.hpp>

class Subscribe : public QObject
{
    Q_OBJECT

public:
    explicit Subscribe(DJI::OSDK::Vehicle *vehiclePtr = 0,
                       QObject *parent = 0);
    ~Subscribe();

    void prepareSubscribeData(DJI::OSDK::Telemetry::TopicName topicName, uint32_t id);

    static void pkg50UnpackCallback(DJI::OSDK::Vehicle       *vehiclePtr,
                                  DJI::OSDK::RecvContainer  recvContainer,
                                  DJI::OSDK::UserData       userData);
    static void pkg10UnpackCallback(DJI::OSDK::Vehicle       *vehiclePtr,
                                    DJI::OSDK::RecvContainer  recvContainter,
                                    DJI::OSDK::UserData       userData);

    void stopPkg50Requested();
    void stopPkg10Requested();

private slots:
    void startPkg50Requested();
    void startPkg10Requested();


signals:
    void subscribeDataReady(const QVector<double> &subscribeData);

private:
    QVector<double> subscribeData;
    QVector<int> pkg50Indices;
    QVector<int> pkg10Indices;
    DJI::OSDK::Vehicle  *vehicle;
};

#endif // SUBSCRIBE_HPP
