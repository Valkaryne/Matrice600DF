/*
 * TODO: make udpchannel as a singletone with static instance
 */

#include "matricedfmodel.hpp"

MatriceDFModel::MatriceDFModel()
{
    //udpChannel = new UdpChannel(QHostAddress(SERVER_IP), SERVER_PORT, QHostAddress(CLIENT_IP), CLIENT_PORT);
    udpChannel = &UdpChannel::getInstance(QHostAddress(SERVER_IP), SERVER_PORT, QHostAddress(CLIENT_IP), CLIENT_PORT);
    qRegisterMetaType<QVector<double>>("QVector<double>");
    connect(udpChannel,SIGNAL(samplesReceived(const QVector<double> &, const QVector<double> &, const QVector<double> &, const QVector<double> &)),
            SLOT(samplesHandler(const QVector<double> &, const QVector<double> &, const QVector<double> &, const QVector<double> &)));

    this->gain = 60;
}

double MatriceDFModel::phaseCorrectionHandler(const double phase)
{
    double corrPhase = 0;
    corrPhase = phase + phaseCorrection;
    if (corrPhase > 360)
        corrPhase -= 360;
    else if (corrPhase < 0)
        corrPhase += 360;

    return corrPhase;
}

void MatriceDFModel::prepareLinearSamples(const QVector<double> &samplesAm1, const QVector<double> &samplesAm2,
                                          const QVector<double> &samplesAmS, const QVector<double> &samplesPh)
{
    emit amplitudeSamplesReady(samplesAm1, samplesAm2, samplesAmS);
    emit phaseSamplesReady(samplesPh);
}

void MatriceDFModel::preparePolarSamples(const QVector<double> &samplesAm1, const QVector<double> &samplesAm2,
                                         const QVector<double> &samplesAmS, const QVector<double> &samplesPh)
{
    QVector<double> ampl1range, ampl2range, amplsrange, phaserange;
    for (int i = 1; i < rangeBounds.size(); i += 2) {
        ampl1range.append(std::accumulate(samplesAm1.begin() + rangeBounds.at(i-1),
                                          samplesAm1.begin() + rangeBounds.at(i), .0) / (rangeBounds.at(i) - rangeBounds.at(i-1)));
        ampl2range.append(std::accumulate(samplesAm2.begin() + rangeBounds.at(i-1),
                                          samplesAm2.begin() + rangeBounds.at(i), .0) / (rangeBounds.at(i) - rangeBounds.at(i-1)));
        amplsrange.append(std::accumulate(samplesAmS.begin() + rangeBounds.at(i-1),
                                          samplesAmS.begin() + rangeBounds.at(i), .0) / (rangeBounds.at(i) - rangeBounds.at(i-1)));
        phaserange.append(std::accumulate(samplesPh.begin() + rangeBounds.at(i-1),
                                          samplesPh.begin() + rangeBounds.at(i), .0) / (rangeBounds.at(i) - rangeBounds.at(i-1)));
    }

    double rado = (*std::max_element(ampl1range.begin(), ampl1range.end()));
    double radl = (*std::max_element(ampl2range.begin(), ampl2range.end()));
    double rads = (*std::max_element(amplsrange.begin(), amplsrange.end()));
    int maxxa = ampl1range.indexOf(rado);
    double phase = phaserange.at(maxxa);

    emit polarSamplesReady(heading, rado, radl, rads, phase);
    emit phaseDeviationEstimated(phase - 180.0);
}

void MatriceDFModel::samplesHandler(const QVector<double> &samplesAm1, const QVector<double> &samplesAm2,
                                    const QVector<double> &samplesAmS, const QVector<double> &samplesPh)
{
    QVector<double> ampl1Mod, ampl2Mod, amplSum, phMod;

    for (int i = 0; i < samplesAm1.size(); i++)
    {
        double tmp1 = samplesAm1.at(i) - CALIBRATION - gain;
        double tmp2 = samplesAm2.at(i) - CALIBRATION - gain;
        double tmpS = samplesAmS.at(i) - CALIBRATION - gain;
        double tmpP = phaseCorrectionHandler(samplesPh.at(i));

        ampl1Mod.append(tmp1);
        ampl2Mod.append(tmp2);
        amplSum.append(tmpS);
        phMod.append(tmpP);
    }

    prepareLinearSamples(ampl1Mod, ampl2Mod, amplSum, phMod);
    preparePolarSamples(ampl1Mod, ampl2Mod, amplSum, phMod);
}
