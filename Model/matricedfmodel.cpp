#include "matricedfmodel.hpp"
#include <QDebug>

MatriceDFModel::MatriceDFModel()
    : gain(60)
{
}

void MatriceDFModel::initializeModel()
{
    udpChannel = new UdpChannel(QHostAddress(SERVER_IP), SERVER_PORT, QHostAddress(CLIENT_IP), CLIENT_PORT);
    qDebug() << "UDP Channel created";
    tcpChannel = new TcpChannel();
}

UdpChannel* MatriceDFModel::getUDPChannel()
{
    return udpChannel;
}

TcpChannel* MatriceDFModel::getTCPChannel()
{
    return tcpChannel;
}

void MatriceDFModel::samplesHandler(const QVector<double> samplesAm1, const QVector<double> samplesAm2,
                                    const QVector<double> samplesPh)
{
    QVector<double> ampl1Mod, ampl2Mod, phMod;

    for (int j = samplesAm1.size() / 2; j < samplesAm1.size(); j++)
    {
        ampl1Mod.push_back(samplesAm1[j] / 16.0 - 97.0 - gain);
        ampl2Mod.push_back(samplesAm2[j] / 16.0 - 97.0 - gain);
        phMod.push_back(samplesPh[j]);
    }

    for (int i = 0; i < samplesAm1.size() / 2; i++)
    {
        ampl1Mod.push_back(samplesAm1[i] / 16.0 - 97.0 - gain);
        ampl2Mod.push_back(samplesAm2[i] / 16.0 - 97.0 - gain);
        phMod.push_back(samplesPh[i]);
    }

    emit(amplitudeSamplesReady(ampl1Mod, ampl2Mod));
    emit(phaseSamplesReady(phMod));
}
