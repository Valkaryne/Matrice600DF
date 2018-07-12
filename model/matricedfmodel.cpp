#include "matricedfmodel.hpp"

MatriceDFModel::MatriceDFModel()
{
    udpChannel = new UdpChannel(QHostAddress(SERVER_IP), SERVER_PORT, QHostAddress(CLIENT_IP), CLIENT_PORT);
    qRegisterMetaType<QVector<double>>("QVector<double>");
    connect(udpChannel,SIGNAL(samplesReceived(QVector<double>,QVector<double>,QVector<double>)),SLOT(samplesHandler(QVector<double>,QVector<double>,QVector<double>)));
    connect(udpChannel,SIGNAL(samplesReceived(QVector<double>,QVector<double>,QVector<double>)),SLOT(polarSamplesHandler(QVector<double>,QVector<double>,QVector<double>)));

    this->gain = 60;
}

void MatriceDFModel::samplesHandler(const QVector<double> samplesAm1, const QVector<double> samplesAm2,
                                    const QVector<double> samplesPh)
{
    QVector<double> ampl1Mod, ampl2Mod, amplSum, phMod;

    for (int j = samplesAm1.size() / 2; j < samplesAm1.size(); j++)
    {
        double tmp = samplesAm1.at(j) - CALIBRATION - gain;
        ampl1Mod.append(tmp);
        double tmp2 = samplesAm2.at(j) - CALIBRATION - gain;
        ampl2Mod.append(tmp2);
        amplSum.append((tmp + tmp2) / 2);
        double tmp3 = samplesPh.at(j);
        if (band == 2) {
            tmp3 += PH2;
            if (tmp3 > 360) tmp3 -= 360;
        } else if (band == 5) {
            tmp3 += PH5;
            if (tmp3 > 360) tmp3 -= 360;
        } else {
            tmp += PH0;
            if (tmp3 > 360) tmp3 -= 360;
        }
        phMod.append(tmp3);
    }

    for (int i = 0; i < samplesAm1.size() / 2; i++)
    {
        double tmp = samplesAm1.at(i) - CALIBRATION - gain;
        ampl1Mod.append(tmp);
        double tmp2 = samplesAm2.at(i) - CALIBRATION - gain;
        ampl2Mod.append(tmp2);
        amplSum.append((tmp + tmp2) / 2);
        double tmp3 = samplesPh.at(i);
        if (band == 2) {
            tmp3 += PH2;
            if (tmp3 > 360) tmp3 -= 360;
        } else if (band == 5) {
            tmp3 += PH5;
            if (tmp3 > 360) tmp3 -= 360;
        }
        phMod.append(tmp3);
    }

    emit amplitudeSamplesReady(ampl1Mod, ampl2Mod, amplSum);
    emit phaseSamplesReady(phMod);
    //TODO: draw plot for summary and two-channel
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
        double tmp3 = samplesPh.at(j);
        if (band == 2) {
            tmp3 += PH2;
            if (tmp3 > 360) tmp3 -= 360;
        } else if (band == 5) {
            tmp3 += PH5;
            if (tmp3 > 360) tmp3 -= 360;
        } else {
            tmp3 += PH0;
            if (tmp3 > 360) tmp3 -= 360;
        }
        phMod.append(tmp3);
    }

    for (int i = 0; i < samplesAm1.size() / 2; i++)
    {
        double tmp = samplesAm1.at(i) - CALIBRATION - gain;
        ampl1Mod.append(tmp);
        double tmp2 = samplesAm2.at(i) - CALIBRATION - gain;
        ampl2Mod.append(tmp2);
        amplSum.append((tmp + tmp2) / 2);
        double tmp3 = samplesPh.at(i);
        if (band == 2) {
            tmp3 += PH2;
            if (tmp3 > 360) tmp3 -= 360;
        } else if (band == 5) {
            tmp3 += PH5;
            if (tmp3 > 360) tmp3 -= 360;
        } else {
            tmp3 += PH0;
            if (tmp3 > 360) tmp3 -= 360;
        }
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
    //TODO: draw polarplot for summary and two-channel
}
