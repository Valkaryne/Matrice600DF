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


private:
    void initModules();
    void initVehicle();
    void activate();
    void obtainControl();
    void controlStatusChanged(QString);
    //void initFinished(QString initStatus, bool initResult);
   // void activateFinished(QString activateStatus, bool activateResult);
    static void activationSuccessful();

private:
    DJI::OSDK::Vehicle  *vehicle;
    Subscribe           *subscribe;

    QString appID;
    QString appKey;
    static bool    controlObtained;
};

#endif // QTOSDK_HPP
