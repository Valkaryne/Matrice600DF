#include "matricedfmodel.hpp"

MatriceDFModel::MatriceDFModel()
{
    udpChannel = new UdpChannel(QHostAddress(SERVER_IP), SERVER_PORT, QHostAddress(CLIENT_IP), CLIENT_PORT);
    qRegisterMetaType<QVector<double>>("QVector<double>");
    connect(udpChannel,SIGNAL(samplesReceived(QVector<double>,QVector<double>,QVector<double>,int)),SLOT(samplesHandler(QVector<double>,QVector<double>,QVector<double>,int)));
    connect(udpChannel,SIGNAL(samplesReceived(QVector<double>,QVector<double>,QVector<double>)),SLOT(polarSamplesHandler(QVector<double>,QVector<double>,QVector<double>)));

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

void MatriceDFModel::samplesHandler(const QVector<double> samplesAm1, const QVector<double> samplesAm2,
                                    const QVector<double> samplesPh, const int number)
{
    QVector<double> ampl1Mod, ampl2Mod, amplSum, phMod;

    /* for (int j = samplesAm1.size() / 2; j < samplesAm1.size(); j++)
    {
        double tmp = samplesAm1.at(j) - CALIBRATION - gain;
        ampl1Mod.append(tmp);
        double tmp2 = samplesAm2.at(j) - CALIBRATION - gain;
        ampl2Mod.append(tmp2);
        amplSum.append((tmp + tmp2) / 2);
        double tmp3 = phaseCorrectionHandler(samplesPh.at(j));
        phMod.append(tmp3);
    }

    for (int i = 0; i < samplesAm1.size() / 2; i++)
    {
        double tmp = samplesAm1.at(i) - CALIBRATION - gain;
        ampl1Mod.append(tmp);
        double tmp2 = samplesAm2.at(i) - CALIBRATION - gain;
        ampl2Mod.append(tmp2);
        amplSum.append((tmp + tmp2) / 2);
        double tmp3 = phaseCorrectionHandler(samplesPh.at(i));
        phMod.append(tmp3);
    } */

    for (int i = 0; i < samplesAm1.size(); i++)
    {
        double tmp = samplesAm1.at(i) - CALIBRATION - gain;
        double tmp2 = samplesAm2.at(i) - CALIBRATION - gain;
        double tmp3 = phaseCorrectionHandler(samplesPh.at(i));

        ampl1Mod.append(tmp);
        ampl2Mod.append(tmp2);
        amplSum.append((tmp + tmp2) / 2);
        phMod.append(tmp3);
    }

    int num = number + 8;
    if (num > 16) num -= 16;

    emit amplitudeSamplesReady(ampl1Mod, ampl2Mod, amplSum, num);
    emit phaseSamplesReady(phMod, num);
}

void MatriceDFModel::polarSamplesHandler(const QVector<double> samplesAm1, const QVector<double> samplesAm2,
                                         const QVector<double> samplesPh)
{
    /* Method of averages */

    QVector<double> ampl1Mod, ampl2Mod, amplSum, phMod;

    for (int j = samplesAm1.size() / 2; j < samplesAm1.size(); j++)
    {
        double tmp = samplesAm1.at(j) - CALIBRATION - gain;
        ampl1Mod.append(tmp);
        double tmp2 = samplesAm2.at(j) - CALIBRATION - gain;
        ampl2Mod.append(tmp2);
        amplSum.append((tmp + tmp2) / 2);
        double tmp3 = phaseCorrectionHandler(samplesPh.at(j));
        phMod.append(tmp3);
    }

    for (int i = 0; i < samplesAm1.size() / 2; i++)
    {
        double tmp = samplesAm1.at(i) - CALIBRATION - gain;
        ampl1Mod.append(tmp);
        double tmp2 = samplesAm2.at(i) - CALIBRATION - gain;
        ampl2Mod.append(tmp2);
        amplSum.append((tmp + tmp2) / 2);
        double tmp3 = phaseCorrectionHandler(samplesPh.at(i));
        phMod.append(tmp3);
    }

    QVector<double> ampl1range, ampl2range, amplsrange, phaserange;
    for (int i = 1; i < rangeBounds.size(); i += 2) {
        ampl1range.append(std::accumulate(ampl1Mod.begin() + rangeBounds.at(i-1),
                                          ampl1Mod.begin() + rangeBounds.at(i), .0) / (rangeBounds.at(i) - rangeBounds.at(i-1)));
        ampl2range.append(std::accumulate(ampl2Mod.begin() + rangeBounds.at(i-1),
                                          ampl2Mod.begin() + rangeBounds.at(i), .0) / (rangeBounds.at(i) - rangeBounds.at(i-1)));
        amplsrange.append(std::accumulate(amplSum.begin() + rangeBounds.at(i-1),
                                          amplSum.begin() + rangeBounds.at(i), .0) / (rangeBounds.at(i) - rangeBounds.at(i-1)));
        phaserange.append(std::accumulate(phMod.begin() + rangeBounds.at(i-1),
                                          phMod.begin() + rangeBounds.at(i), .0) / (rangeBounds.at(i) - rangeBounds.at(i-1)));
    }

    double rado = (*std::max_element(ampl1range.begin(), ampl1range.end()));
    double radl = (*std::max_element(ampl2range.begin(), ampl2range.end()));
    double rads = (*std::max_element(amplsrange.begin(), amplsrange.end()));
    int maxxa = ampl1range.indexOf(rado);
    double phase = phaserange.at(maxxa);

    emit polarSamplesReady(heading, rado, radl, rads, phase);
    emit phaseDeviationEstimated(phase - 180.0);
}
