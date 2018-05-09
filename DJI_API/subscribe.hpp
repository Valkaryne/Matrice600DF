#ifndef SUBSCRIBE_HPP
#define SUBSCRIBE_HPP

#include <QObject>
#include <QVector>
#include <dji_vehicle.hpp>

class Subscribe : public QObject
{
    Q_OBJECT

public:
    int freqEnum[7] = { 0, 1, 10, 50, 100, 400 };

public:
    explicit Subscribe(DJI::OSDK::Vehicle *vehiclePtr = 0,
                       QObject *parent = 0);
    ~Subscribe();

    void prepareSubscribeData(DJI::OSDK::Telemetry::TopicName topicName, uint32_t id);

    static void pkg0UnpackCallback(DJI::OSDK::Vehicle   *vehiclePtr,
                                   DJI::OSDK::RecvContainer recvContainer,
                                   DJI::OSDK::UserData      userData);
    static void pkg1UnpackCallback(DJI::OSDK::Vehicle   *vehiclePtr,
                                   DJI::OSDK::RecvContainer recvContainer,
                                   DJI::OSDK::UserData      userData);
    static void pkg2UnpackCallback(DJI::OSDK::Vehicle   *vehiclePtr,
                                   DJI::OSDK::RecvContainer recvContainer,
                                   DJI::OSDK::UserData      userData);
    static void pkg3UnpackCallback(DJI::OSDK::Vehicle   *vehiclePtr,
                                   DJI::OSDK::RecvContainer recvContainer,
                                   DJI::OSDK::UserData      userData);
    static void pkg4UnpackCallback(DJI::OSDK::Vehicle   *vehiclePtr,
                                   DJI::OSDK::RecvContainer recvContainer,
                                   DJI::OSDK::UserData      userData);

private slots:
    void startPkg0Requested();
    void startPkg1Requested();
    void startPkg2Requested();
    void startPkg3Requested();
    void startPkg4Requested();

    void stopPkg0Requested();
    void stopPkg1Requested();
    void stopPkg2Requested();
    void stopPkg3Requested();
    void stopPkg4Requested();

signals:
    void subscribeDataReady(const QVector<double> subscribeData);

private:
    QVector<double> subscribeData;

    DJI::OSDK::Vehicle  *vehicle;
};

#endif // SUBSCRIBE_HPP
