/*
 * TODO: make udpchannel as a singletone with static instance
 */

#include "matricedfmodel.hpp"

MatriceDFModel::MatriceDFModel()
{
    udpChannel = &UdpChannel::getInstance(QHostAddress(SERVER_IP), SERVER_PORT, QHostAddress(CLIENT_IP), CLIENT_PORT);
    qRegisterMetaType<QVector<double>>("QVector<double>");
    connect(udpChannel,SIGNAL(samplesReceived(const QVector<double> &, const QVector<double> &, const QVector<double> &, const QVector<double> &)),
            SLOT(samplesHandler(const QVector<double> &, const QVector<double> &, const QVector<double> &, const QVector<double> &)));

    SamplesPreparator *linearPreparator = new SamplesPreparator;
    SamplesPreparator *polarPreparator = new SamplesPreparator;

    linearPreparator->moveToThread(linearPreparator);
    linearPreparator->start();

    polarPreparator->moveToThread(polarPreparator);
    polarPreparator->start();

    connect(this, SIGNAL(prepareSamples(const QVector<double> &, const QVector<double> &, const QVector<double> &, const QVector<double> &)),
            linearPreparator, SLOT(prepareLinearSamples(const QVector<double> &, const QVector<double> &, const QVector<double> &, const QVector<double> &)));
    connect(this, SIGNAL(prepareSamples(const QVector<double> &, const QVector<double> &, const QVector<double> &, const QVector<double> &)),
            polarPreparator, SLOT(preparePolarSamples(const QVector<double> &, const QVector<double> &, const QVector<double> &, const QVector<double> &)));

    connect(linearPreparator, SIGNAL(amplitudeSamplesReady(const QVector<double> &, const QVector<double> &, const QVector<double> &)),
            this, SIGNAL(amplitudeSamplesReady(const QVector<double> &, const QVector<double> &, const QVector<double> &)));
    connect(linearPreparator, SIGNAL(phaseSamplesReady(const QVector<double> &)),
            this, SIGNAL(phaseSamplesReady(const QVector<double> &)));
    connect(polarPreparator, SIGNAL(polarSamplesReady(const int &, const double &, const double &, const double &, const double &)),
            this, SIGNAL(polarSamplesReady(const int &, const double &, const double &, const double &, const double &)));
    connect(polarPreparator, SIGNAL(phaseDeviationEstimated(const double &)),
            this, SIGNAL(phaseDeviationEstimated(const double &)));

    this->gain = 60;
    this->phaseCorrection = 0;
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

    emit prepareSamples(ampl1Mod, ampl2Mod, amplSum, phMod);
}
