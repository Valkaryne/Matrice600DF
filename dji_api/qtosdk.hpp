#ifndef QTOSDK_HPP
#define QTOSDK_HPP

#include <utils/constset.hpp>
#include <QFile>
#include <QSerialPortInfo>
#include <dji_vehicle.hpp>

class QtOsdk : public QObject
{
    Q_OBJECT

public:
    explicit QtOsdk(QObject *parent = 0);
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

signals:
    void changeControlAuthorityStatus(QString textToDisplay);
    void changeInitButton(QString textToDisplay, bool success);
    void changeActivateButton(QString textToDisplay, bool success);

private:
    DJI::OSDK::Vehicle* vehicle;

    QString appIDInput;
    QString keyInput;
};

#endif // QTOSDK_HPP
