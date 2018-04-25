#ifndef UDPCHANNEL_HPP
#define UDPCHANNEL_HPP

#include <QObject>
#include <QUdpSocket>
#include <QNetworkDatagram>

class UdpChannel
{
    Q_OBJECT
public:
    explicit UdpChannel(const QHostAddress &addressSrv, quint16 portSrv,
                        const QHostAddress &addressClt, quint16 portClt,
                        QObject *parent = 0);

signals:
    void samplesReceived(const QVector<double> samplesAm1, const QVector<double> samplesAm2,
                         const QVector<double> samplesPh);

public slots:
    void readPendingDatagram();
    void sendDatagram(const QVector<double> settings);

private:
    QUdpSocket     *socket;
    QHostAddress    addressSendTo;
    quint16         portSendTo;
};

#endif // UDPCHANNEL_HPP
