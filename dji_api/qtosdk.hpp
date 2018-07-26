#ifndef QTOSDK_HPP
#define QTOSDK_HPP

#include <utils/constset.hpp>
#include <QFile>
#include <QSerialPortInfo>
#include <dji_vehicle.hpp>
#include <QTimer>

#include <subscribe.hpp>
#include <flightcontroller.hpp>
#include <waypoint.hpp>
#include <hotpoint.hpp>

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
    void flightControlRequest(QChar control);

    void initWaypointRequest(const QHash<QString, int> &settings);
    void loadWaypointRequest(const QHash<QString, int> &settings);
    void startWaypointRequest();
    void abortWaypointRequest();

    void initHotpointRequest(const QVector<double> &coordinates);
    void startHotpointRequest(int yawRate);
    void stopHotpointRequest();

signals:
    void changeControlAuthorityStatus(QString textToDisplay);
    void changeInitButton(QString textToDisplay, bool success);
    void changeActivateButton(QString textToDisplay, bool success);
    void changeConnectionButtons();
    void receiveTelemetryData(const QVector<double> &subscribeData);

private slots:
    void testTimer();

private:
    DJI::OSDK::Vehicle  *vehicle;
    Subscribe           *subscribe;
    FlightController    *flightController;
    Waypoint            *waypoint;
    Hotpoint            *hotpoint;

    QString appIDInput;
    QString keyInput;

    QTimer *autoSend;
};

#endif // QTOSDK_HPP
