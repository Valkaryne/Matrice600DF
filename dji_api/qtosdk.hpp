#ifndef QTOSDK_HPP
#define QTOSDK_HPP

#include <utils/constset.hpp>
#include <QFile>
#include <QSerialPortInfo>
#include <dji_vehicle.hpp>

#include <subscribe.hpp>
#include <flightcontroller.hpp>
#include <waypoint.hpp>

class QtOsdk : public QObject
{
    Q_OBJECT

public:
    explicit QtOsdk(QObject *parent = nullptr);
    void setVehicle(DJI::OSDK::Vehicle* vehiclePtr)
    {
        this->vehicle = vehiclePtr;
    }
    ~QtOsdk();

    void readAppIDKey();

private:
    void refreshPort();
    void initComponents();

    // Callbacks
public:
    static void activateCallback(DJI::OSDK::Vehicle        *vehicle,
                                 DJI::OSDK::RecvContainer   recvFrame,
                                 DJI::OSDK::UserData        userData);
    static void setControlCallback(DJI::OSDK::Vehicle      *vehicle,
                                   DJI::OSDK::RecvContainer recvFrame,
                                   DJI::OSDK::UserData      userData);

public:
    void initVehicle();
    void activate();
    void obtainCtrl(QString ctrlOperation);
    void resetConnection();

    void flightRunCommandRequest(int &commandIndex);


    void initWaypointRequest(const QHash<QString, int> &settings);
    void loadWaypointRequest(const QHash<QString, int> &settings);
    void startWaypointRequest();
    void abortWaypointRequest();

signals:
    void changeControlAuthorityStatus(QString textToDisplay);
    void changeInitButton(QString textToDisplay, bool success);
    void changeActivateButton(QString textToDisplay, bool success);
    void changeConnectionButtons();
    void receiveTelemetryData(const QVector<double> &subscribeData);
    void stopRotationRequest();

public slots:
    void startRotationRequest(int yawRate);

private:
    DJI::OSDK::Vehicle  *vehicle;
    Subscribe           *subscribe;
    FlightController    *flightController;
    Waypoint            *waypoint;

    QString appIDInput;
    QString keyInput;
};

#endif // QTOSDK_HPP
