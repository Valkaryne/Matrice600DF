#include "tcpchannel.hpp"
#include <QDebug>

TcpChannel::TcpChannel(QObject *parent)
    : QObject(parent)
{
    server = new QTcpServer(this);

    connect(server, SIGNAL(newConnection()),
            this, SLOT(newConnection()));

    if (!server->listen(QHostAddress::LocalHost, 8080))
    {
        qDebug() << "Server could not start";
    }
    else
    {
        qDebug() << "Server started!";
    }
}

void TcpChannel::newConnection()
{
    tcpSocket = server->nextPendingConnection();

    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

void TcpChannel::readyRead()
{
    mtelemetry::Telemetry telemetry;
    QByteArray buffer;

    while (tcpSocket->bytesAvailable() > 0)
    {
        buffer = tcpSocket->readAll();
        while (buffer.size() >= 22)
        {
            std::string data = buffer.mid(0, 22);
            buffer.remove(0, 22);
            telemetry.ParseFromString(data);
            if (telemetry.heading() != 0 || telemetry.latitude() != 0 || telemetry.longitude() != 0)
                emit telemetryReceived(telemetry);
        }
    }
    tcpSocket->flush();
}
