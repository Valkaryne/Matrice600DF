#include "twochannelstrategy.hpp"

TwoChannelStrategy::TwoChannelStrategy(AmplitudeSpectrumPlot *plot)
{
    this->plot = plot;
    plot->curveAm1->attach(plot);
    plot->curveAm2->attach(plot);
    plot->curveAmS->detach();
}

TwoChannelStrategy::TwoChannelStrategy(PolarPlot *polarPlot)
{
    this->polarPlot = polarPlot;
    polarPlot->curveAm1->attach(polarPlot);
    polarPlot->curveAm2->attach(polarPlot);
    polarPlot->curveAmS->detach();
}

void TwoChannelStrategy::update(const QVector<double> &samplesAm1, const QVector<double> &samplesAm2,
                                 const QVector<double> &samplesAmS)
{
    Q_UNUSED(samplesAmS)
    int size = samplesAm1.size();
    QVector<double> frequency;
    double cntrFrequency = plot->getCentralFrequency();

    for (double i = cntrFrequency - LSHIFT; i < size * INCR + (cntrFrequency - LSHIFT); i += INCR)
        frequency.append(i);

    plot->curveAm1->setSamples(frequency, samplesAm1);
    plot->curveAm2->setSamples(frequency, samplesAm2);

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

