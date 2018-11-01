#ifndef UDPCHANNEL_HPP
#define UDPCHANNEL_HPP

#include <QObject>
#include <QUdpSocket>
#include <QNetworkDatagram>

constexpr int PACKAGE_LENGTH = 256;
constexpr int PACKAGE_NUMBER = 16;
constexpr int FULL_LENGTH = PACKAGE_LENGTH * PACKAGE_NUMBER;

class UdpChannel : public QObject
{
    Q_OBJECT
public:
    explicit UdpChannel(const QHostAddress &addressSrv, quint16 portSrv,
                        const QHostAddress &addressClt, quint16 portClt,
                        QObject *parent = nullptr);
    void sendDatagram(const QVector<double> settings);

signals:
    void samplesReceived(const QVector<double> &samplesAm1, const QVector<double> &samplesAm2,
                         const QVector<double> &samplesAmS, const QVector<double> &samplesPh);

public slots:
    void readPendingDatagram();

private:
    int getPackageNumber(QVector<unsigned char> &binaryVector);
    void putSamples(const QVector<double> &srcSamples, QVector<double> &dstSamples, const int &number);

private:
    QUdpSocket      *socket;
    QHostAddress     addressSendTo;
    quint16          portSendTo;

    QVector<double> samplesAm1, samplesAm2, samplesAmS, samplesPh;
};

#endif // UDPCHANNEL_HPP
