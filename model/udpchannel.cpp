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
        QVector<unsigned char> numbers;

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
            val_ph <<= 7;
            val_ph >>= 7;

            unsigned char val_num = (val3 << 6);
            val_num >>= 7;

            samplesAm1.append(static_cast<double>(val_am1));
            samplesAm2.append(static_cast<double>(val_am2));
            samplesPh.append(static_cast<double>(val_ph));
            numbers.append(val_num);
        }
        numbers.resize(32);
        std::reverse(numbers.begin(), numbers.end());
        int number = 0;
        for (int i = 0; i < numbers.size(); i++)
        {
            number = number + (numbers.at(i) << i);
        }

        emit samplesReceived(samplesAm1, samplesAm2, samplesPh, number);
    }
}

void UdpChannel::sendDatagram(const QVector<double> settings)
{
    QByteArray toSend = QByteArray::fromRawData(
                reinterpret_cast<const char*>(settings.constData()), sizeof(double) * settings.size());
    qDebug() << "Settings Array: " << settings;
    socket->writeDatagram(toSend, addressSendTo, portSendTo);
}
