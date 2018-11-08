#ifndef UDPCHANNEL_HPP
#define UDPCHANNEL_HPP

#include <QObject>
#include <QUdpSocket>
#include <QNetworkDatagram>

constexpr int PACKAGE_LENGTH = 256;
constexpr int PACKAGE_NUMBER = 16;
constexpr int FULL_LENGTH = PACKAGE_LENGTH * PACKAGE_NUMBER;

class UdpChannel;

class UdpChannelDestroyer
{
private:
    UdpChannel *channelInstance;
public:
    ~UdpChannelDestroyer();
    void initialize(UdpChannel* channel);
};

class UdpChannel : public QObject
{
    Q_OBJECT

private:
    static UdpChannel *channelInstance;
    static UdpChannelDestroyer channelDestroyer;
protected:
    UdpChannel(QObject *parent = nullptr) : QObject(parent) { }
    UdpChannel(const UdpChannel& );
    UdpChannel& operator=(UdpChannel&);
    ~UdpChannel() { }
    friend class UdpChannelDestroyer;

public:
    static UdpChannel& getInstance(const QHostAddress &addressSrv, quint16 portSrv,
                                   const QHostAddress &addressClt, quint16 portClt,
                                   QObject *parent = nullptr);
    static UdpChannel& getInstance();
    void sendDatagram(const QVector<double> settings);

signals:
    void samplesReceived(const QVector<double> &samplesAm1, const QVector<double> &samplesAm2,
                         const QVector<double> &samplesAmS, const QVector<double> &samplesPh);

public slots:
    void readPendingDatagram();
    void setSignalAllowed();

private:
    void establishUdpConnection(const QHostAddress &addressSrv, quint16 portSrv,
                                const QHostAddress &addressClt, quint16 portClt);
    int getPackageNumber(QVector<unsigned char> &binaryVector);
    void putSamples(const QVector<double> &srcSamples, QVector<double> &dstSamples, const int &number);

private:
    QUdpSocket      *socket;
    QHostAddress     addressSendTo;
    quint16          portSendTo;

    int counter;
    bool signalAllowed;

    QVector<double> samplesAm1, samplesAm2, samplesAmS, samplesPh;
};

#endif // UDPCHANNEL_HPP
