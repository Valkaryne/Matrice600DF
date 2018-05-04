#include "matricedfmodel.hpp"
#include <QDebug>

MatriceDFModel::MatriceDFModel()
    : am1N(8),
      am2N(8),
      amSN(16)
{
    this->gain = 60;
    qRegisterMetaType<QVector<double>>("QVector<double>");
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
    //int maxxa = vectorAm1.indexOf(*std::max_element(vectorAm1.begin(), vectorAm1.end()));

    if (setCalibration) {
        simpleAm1.append(*std::max_element(vectorAm1.begin(), vectorAm1.end()));
        simpleAm2.append(*std::max_element(vectorAm2.begin(), vectorAm2.end()));
    }

    QVector<double> ampl1range, ampl2range, amplsrange, phaserange;
    for (int i = 1; i < rangeBounds.size(); i += 2) {
        ampl1range.push_back(std::accumulate(vectorAm1.begin() + rangeBounds.at(i-1), vectorAm1.begin() + rangeBounds.at(i), .0) / (rangeBounds.at(i) - rangeBounds.at(i-1)));
        ampl2range.push_back(std::accumulate(vectorAm2.begin() + rangeBounds.at(i-1), vectorAm2.begin() + rangeBounds.at(i), .0) / (rangeBounds.at(i) - rangeBounds.at(i-1)));
        amplsrange.push_back(std::accumulate(vectorAmS.begin() + rangeBounds.at(i-1), vectorAmS.begin() + rangeBounds.at(i), .0) / (rangeBounds.at(i) - rangeBounds.at(i-1)));
        phaserange.push_back(std::accumulate(vectorPh.begin() + rangeBounds.at(i-1) - 14, vectorPh.begin() + rangeBounds.at(i) - 14, .0) / (rangeBounds.at(i) - rangeBounds.at(i-1)));
    }

    double rado = *std::max_element(ampl1range.begin(), ampl1range.end()) / am1N;
    double radl = *std::max_element(ampl2range.begin(), ampl2range.end()) / am2N;
    double rads = *std::max_element(amplsrange.begin(), amplsrange.end()) / amSN;
    double phase = std::accumulate(phaserange.begin(), phaserange.end(), .0) / phaserange.size();

    if (rado > 359)
        rado = 359;
    if (radl > 359)
        radl = 359;
    if (rads > 359)
        rads = 359;

    if (doubleChannel) {
        emit polarSamplesReady(angle, rado, radl, phase);
    } else {
        emit polarSamplesReady(angle, rads, phase);
    }
}


// TODO:
/*QVector<int> Plot::getMarkerBounds()
{
    QVector<int> bounds;
    for (int i = 1; i < markerVector.size();  i += 2) {
        double bound = markerVector.at(i - 1)->xValue();
        bounds.append(2060 + (bound - cntrFrequency) / INCR);

        double tmp_bound = bounds.at(i-1);

        if ((tmp_bound >= 0) && (tmp_bound < 2048))
            bounds.replace(i-1, tmp_bound += 2048);
        else if ((tmp_bound >= 2048) && (tmp_bound < 4096))
            bounds.replace(i-1, tmp_bound -= 2048);

        bound = markerVector.at(i)->xValue();
        bounds.append(2060 + (bound - cntrFrequency) / INCR);

        tmp_bound = bounds.at(i);

        if ((tmp_bound >= 0) && (tmp_bound < 2048))
            bounds.replace(i, tmp_bound += 2048);
        else if ((tmp_bound >= 2048) && (tmp_bound < 4096))
            bounds.replace(i, tmp_bound -= 2048);

        qSort(bounds.end() - 2, bsounds.end());
        if ((i > 1) && (bounds.at(i-1) < bounds.at(i-3)) && (bounds.at(i) > bounds.at(i-2))) {
            bounds.replace(i-1, bounds.at(i-3));
            bounds.replace(i, bounds.at(i-2));
        }
    }

    return bounds;
} */
