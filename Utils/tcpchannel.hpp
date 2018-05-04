#ifndef TCPCHANNEL_HPP
#define TCPCHANNEL_HPP

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include "MatriceTelemetry.pb.h"

class TcpChannel : public QObject
{
    Q_OBJECT
public:
    explicit TcpChannel(QObject *parent = 0);

signals:
    void telemetryReceived(const mtelemetry::Telemetry &telemetry);

public slots:
    void newConnection();
    void readyRead();

private:
    QTcpServer *server;
    QTcpSocket *tcpSocket;
};

#endif // TCPCHANNEL_HPP
