#ifndef QTOSDK_HPP
#define QTOSDK_HPP

#include <QFile>
#include <QSerialPortInfo>

#include <dji_vehicle.hpp>

#include "subscribe.hpp"
#include "../Utils/constset.hpp"

class QtOsdk : public QObject
{
    Q_OBJECT

public:
    explicit QtOsdk(QObject *parent = 0);

    void setVehicle(DJI::OSDK::Vehicle *vehiclePtr)
    {
        this->vehicle = vehiclePtr;
    }

    ~QtOsdk();

    void readAppIDKey();

    /* Callbacks */
    static void activateCallback(DJI::OSDK::Vehicle        *vehicle,
                                 DJI::OSDK::RecvContainer   recvFrame,
                                 DJI::OSDK::UserData        userData);
    static void setControlCallback(DJI::OSDK::Vehicle      *vehicle,
                                   DJI::OSDK::RecvContainer recvFrame,
                                   DJI::OSDK::UserData      userData);

    /* Getters */
    Subscribe* getSubscribeModule();

private:
    void initModules();
    void initVehicle();
    void activate();
    void obtainControl();
    //void initFinished(QString initStatus, bool initResult);
   // void activateFinished(QString activateStatus, bool activateResult);

private slots:
    void controlStatusChanged(bool obtain);
    void activationSuccessful();

private:
    DJI::OSDK::Vehicle  *vehicle;
    Subscribe           *subscribe;

    QString appID;
    QString appKey;
    bool    controlObtained;

signals:
    void changeControlStatus(bool obtain);
    void changeActivateStatus();
    void throwSubscribeData(const QVector<double>);
};

#endif // QTOSDK_HPP
