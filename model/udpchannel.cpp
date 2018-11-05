#include "udpchannel.hpp"

UdpChannel *UdpChannel::channelInstance = 0;
UdpChannelDestroyer UdpChannel::channelDestroyer;

UdpChannelDestroyer::~UdpChannelDestroyer()
{
    delete channelInstance;
}

void UdpChannelDestroyer::initialize(UdpChannel *channel)
{
    channelInstance = channel;
}

UdpChannel& UdpChannel::getInstance(const QHostAddress &addressSrv, quint16 portSrv,
                                    const QHostAddress &addressClt, quint16 portClt,
                                    QObject *parent)
{
    if (!channelInstance) {
        channelInstance = new UdpChannel(parent);
        channelDestroyer.initialize(channelInstance);

        channelInstance->establishUdpConnection(addressSrv, portSrv, addressClt, portClt);
    }
    return *channelInstance;
}

void UdpChannel::establishUdpConnection(const QHostAddress &addressSrv, quint16 portSrv,
                                        const QHostAddress &addressClt, quint16 portClt)
{
    socket = new QUdpSocket(channelInstance);
    socket->bind(addressSrv, portSrv);

    addressSendTo = addressClt;
    portSendTo = portClt;

    connect(socket, SIGNAL(readyRead()), SLOT(readPendingDatagram()));

    for (int i = 0; i < FULL_LENGTH; i++)
    {
        samplesAm1.append(0);
        samplesAm2.append(0);
        samplesAmS.append(0);
        samplesPh.append(0);
    }
}

void UdpChannel::readPendingDatagram()
{
    while (socket->hasPendingDatagrams())
    {
        QNetworkDatagram datagram = socket->receiveDatagram();

        QByteArray tmp = datagram.data();
        QVector<double> samplesAm1, samplesAm2, samplesAmS, samplesPh;
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
            samplesAmS.append(static_cast<double>((val_am1 + val_am2) / 2));
            samplesPh.append(static_cast<double>(val_ph));
            numbers.append(val_num);
        }

        int number = getPackageNumber(numbers);
        putSamples(samplesAm1, this->samplesAm1, number);
        putSamples(samplesAm2, this->samplesAm2, number);
        putSamples(samplesAmS, this->samplesAmS, number);
        putSamples(samplesPh, this->samplesPh, number);

        emit samplesReceived(this->samplesAm1, this->samplesAm2, this->samplesAmS, this->samplesPh);
    }
}

void UdpChannel::sendDatagram(const QVector<double> settings)
{
    QByteArray toSend = QByteArray::fromRawData(
                reinterpret_cast<const char*>(settings.constData()), sizeof(double) * settings.size());
    qDebug() << "Settings Array: " << settings;
    socket->writeDatagram(toSend, addressSendTo, portSendTo);
}

int UdpChannel::getPackageNumber(QVector<unsigned char> &binaryVector)
{
    int number = 0;

    binaryVector.resize(32);
    std::reverse(binaryVector.begin(), binaryVector.end());

    for (int i = 0; i < binaryVector.size(); i++)
    {
        number = number + (binaryVector.at(i) << i);
    }

    // swap spectrum parts
    number += PACKAGE_NUMBER / 2;
    if (number > PACKAGE_NUMBER) number -= PACKAGE_NUMBER;

    return number;
}

void UdpChannel::putSamples(const QVector<double> &srcSamples, QVector<double> &dstSamples, const int &number)
{
    for (int i = PACKAGE_LENGTH * (number - 1), j = 0; i < PACKAGE_LENGTH * number; i++, j++)
    {
        dstSamples.replace(i, srcSamples.at(j));
    }
}
