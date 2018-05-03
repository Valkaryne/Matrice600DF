#include "matricedfmodel.hpp"
#include <QDebug>

MatriceDFModel::MatriceDFModel()
    : am1N(8),
      am2N(8),
      amSN(16)
{
    this->gain = 60;
}

void MatriceDFModel::setCalibrationMode(bool setCalibration)
{
    ModelInterface::setCalibrationMode(setCalibration);

    if (!setCalibration) {
        am1N = (*std::max_element(simpleAm1.begin(), simpleAm1.end())) * 1.2 / 360.0;
        am2N = (*std::max_element(simpleAm2.begin(), simpleAm2.end())) * 1.2 / 360.0;
        amSN = am1N + am2N;
        simpleAm1.clear();
        simpleAm2.clear();
    }
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

void MatriceDFModel::polarSamplesHandler(const QVector<double> samplesAm1, const QVector<double> samplesAm2,
                                         const QVector<double> samplesPh)
{
    QVector<double> vectorAm1, vectorAm2, vectorAmS, vectorPh;

    for (int j = samplesAm1.size() / 2; j < samplesAm1.size(); j++) {
        double tmp = pow(7, (samplesAm1.at(j) / 160)) / 1000;
        vectorAm1.append(tmp);
        double tmp2 = pow(7, (samplesAm2.at(j) / 160)) / 1000;
        vectorAm2.append(tmp);
        vectorAmS.append(tmp + tmp2);
        vectorPh.append(samplesPh.at(j));
    }
    for (int i = 0; i < samplesAm1.size() / 2; i++) {
        double tmp = pow(7, (samplesAm1.at(i) / 160)) / 1000;
        vectorAm1.append(tmp);
        double tmp2 = pow(7, (samplesAm2.at(i) / 160)) / 1000;
        vectorAm2.append(tmp2);
        vectorAmS.append(tmp + tmp2);
        vectorPh.append(samplesPh.at(i));
    }

    int angle = 0; // TODO: getCurrentAngle()
    if (angle < 0)
        angle += 360;

    // TODO: detect maximum on range
    int maxxa = vectorAm1.indexOf(*std::max_element(vectorAm1.begin(), vectorAm1.end()));

    if (setCalibration) {
        simpleAm1.append(*std::max_element(vectorAm1.begin(), vectorAm1.end()));
        simpleAm2.append(*std::max_element(vectorAm2.begin(), vectorAm2.end()));
    }

    double rado = *std::max_element(vectorAm1.begin(), vectorAm1.end()) / am1N;
    double radl = *std::max_element(vectorAm2.begin(), vectorAm2.end()) / am2N;
    double rads = *std::max_element(vectorAmS.begin(), vectorAmS.end()) / amSN;

    if (rado > 359)
        rado = 359;
    if (radl > 359)
        radl = 359;
    if (rads > 359)
        rads = 359;

    if (doubleChannel) {
        emit polarSamplesReady(angle, rado, radl, vectorPh.at(maxxa - 14));
    } else {
        emit polarSamplesReady(angle, rads, vectorPh.at(maxxa - 14));
    }
}
