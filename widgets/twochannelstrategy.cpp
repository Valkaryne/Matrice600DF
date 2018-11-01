#include "twochannelstrategy.hpp"

TwoChannelStrategy::TwoChannelStrategy(AmplitudeSpectrumPlot *plot)
{
    this->plot = plot;
    this->expCoeff = 0.999;
    plot->curveAm1->attach(plot);
    plot->curveAm2->attach(plot);
    plot->curveAmS->detach();
    plot->replot();
}

TwoChannelStrategy::TwoChannelStrategy(PolarPlot *polarPlot)
{
    this->polarPlot = polarPlot;
    polarPlot->curveAm1->attach(polarPlot);
    polarPlot->curveAm2->attach(polarPlot);
    polarPlot->curveAmS->detach();
    polarPlot->replot();
}

void TwoChannelStrategy::update(const QVector<double> &samplesAm1, const QVector<double> &samplesAm2,
                                 const QVector<double> &samplesAmS)
{
    Q_UNUSED(samplesAmS)
    QVector<double> frequency;
    double cntrFrequency = plot->getCentralFrequency();

    for (double i = cntrFrequency - LSHIFT; i < cntrFrequency + RSHIFT; i += INCR)
        frequency.append(i);

    for (int j = 0; j < samplesAm1.size(); j++) {
        if (samplesAm1.at(j) > plot->maxSamples.at(j))
            plot->maxSamples.replace(j, expCoeff * samplesAm1.at(j) + (1 - expCoeff) * plot->maxSamples.at(j) + 5);
        else
            plot->maxSamples.replace(j, (1 - expCoeff) * samplesAm1.at(j) + expCoeff * plot->maxSamples.at(j));
    }

    plot->curveAm1->setSamples(frequency, samplesAm1);
    plot->curveAm2->setSamples(frequency, samplesAm2);
    plot->curveMax->setSamples(frequency, plot->maxSamples);
    plot->replot();
}

void TwoChannelStrategy::update(const int &azHeading, const double &radAm1,
                                const double &radAm2, const double &radAmS, const double &radPh)
{
    Q_UNUSED(radAmS)
    PolarCurveData *dataAm1 = (PolarCurveData*)(polarPlot->curveAm1->data());
    dataAm1->append(QwtPointPolar(azHeading, radAm1));
    PolarCurveData *dataAm2 = (PolarCurveData*)(polarPlot->curveAm2->data());
    dataAm2->append(QwtPointPolar(azHeading, radAm2));
    PolarCurveData *dataPh = (PolarCurveData*)(polarPlot->curvePh->data());
    dataPh->append(QwtPointPolar(azHeading, radPh));
    polarPlot->replot();
}

double TwoChannelStrategy::getPowerMaximum()
{
    double maximumAz1 = 0;
    double maximumAz2 = 0;
    double maximumAz = 0;
    PolarCurveData *data1 = (PolarCurveData*)(polarPlot->curveAm1->data());
    maximumAz1 = data1->getPowerMaximum();
    PolarCurveData *data2 = (PolarCurveData*)(polarPlot->curveAm2->data());
    maximumAz2 = data2->getPowerMaximum();

    bool zeroCondition = ((maximumAz1 > 315) && (maximumAz2 < 45))
            || ((maximumAz2 > 315) && (maximumAz1 < 45));

    if (zeroCondition)
        maximumAz = (maximumAz1 + maximumAz2 - 360) / 2;
    else
        maximumAz = (maximumAz1 + maximumAz2) / 2;

    if (maximumAz < 0) maximumAz += 360;

    return maximumAz;
}

