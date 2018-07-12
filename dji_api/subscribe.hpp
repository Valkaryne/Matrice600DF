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

    static void pkgUnpackCallback(DJI::OSDK::Vehicle       *vehiclePtr,
                                  DJI::OSDK::RecvContainer  recvContainer,
                                  DJI::OSDK::UserData       userData);
    void stopPkgRequested();

private slots:
    void startPkgRequested();


signals:
    void subscribeDataReady(const QVector<double> &subscribeData);

private:
    QVector<double> subscribeData;
    QVector<int> pkgIndices;
    DJI::OSDK::Vehicle  *vehicle;
};

#endif // SUBSCRIBE_HPP
