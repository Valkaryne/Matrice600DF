#include "udpchannel.hpp"

UdpChannel::UdpChannel(const QHostAddress &addressSrv, quint16 portSrv,
                       const QHostAddress &addressClt, quint16 portClt,
                       QObject *parent) :
    QObject(parent)
{
    socket = new QUdpSocket(this);
    socket->bind(addressSrv, portSrv);

    addressSendTo = addressClt;
    portSendTo = portClt;

    connect(socket, SIGNAL(readyRead()), SLOT(readPendingDatagram()));
}

void UdpChannel::readPendingDatagram()
{
    while (socket->hasPendingDatagrams())
    {
        QNetworkDatagram datagram = socket->receiveDatagram();

        QByteArray tmp = datagram.data();
        QVector<double> samplesAm1, samplesAm2, samplesPh;

        for (int i = 0; i < tmp.size() / 4; i++)
        {
            /* some low level magic to convert byte array to double array */
            unsigned char val1 = static_cast<unsigned char>(tmp[4 * i + 3]);
            unsigned char val2 = static_cast<unsigned char>(tmp[4 * i + 2]);
            unsigned char val3 = static_cast<unsigned char>(tmp[4 * i + 1]);
            unsigned char val4 = static_cast<unsigned char>(tmp[4 * i]);

            unsigned short val_am1 = (static_cast<unsigned short>(val1) << 8) |
                    (static_cast<unsigned short>(val2));
            val_am1 >>= 5;

            unsigned short val_am2 = (static_cast<unsigned short>(val2) << 8) |
                    (static_cast<unsigned short>(val3));
            val_am2 <<= 3;
            val_am2 >>= 5;

            unsigned short val_ph = (static_cast<unsigned short>(val3) << 8) |
                    (static_cast<unsigned short>(val4));
            val_ph <<= 6;
            val_ph >>= 6;

            samplesAm1.push_back(static_cast<double>(val_am1));
            samplesAm2.push_back(static_cast<double>(val_am2));
            samplesPh.push_back(static_cast<double>(val_ph));
        }
        emit samplesReceived(samplesAm1, samplesAm2, samplesPh);
    }
}

void UdpChannel::sendDatagram(const QVector<double> settings)
{
    QByteArray toSend = QByteArray::fromRawData(
                reinterpret_cast<const char*>(settings.constData()), sizeof(double) * settings.size());
    qDebug() << "Settings Array: " << settings;
    socket->writeDatagram(toSend, addressSendTo, portSendTo);
}
