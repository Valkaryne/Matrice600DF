#include "summarystrategy.hpp"

SummaryStrategy::SummaryStrategy(AmplitudeSpectrumPlot *plot)
{
    this->plot = plot;
    plot->curveAm1->detach();
    plot->curveAm2->detach();
    plot->curveAmS->attach(plot);
}

SummaryStrategy::SummaryStrategy(PolarPlot *polarPlot)
{
    this->polarPlot = polarPlot;
    polarPlot->curveAm1->detach();
    polarPlot->curveAm2->detach();
    polarPlot->curveAmS->attach(polarPlot);
}

void SummaryStrategy::update(const QVector<double> &samplesAm1, const QVector<double> &samplesAm2,
                             const QVector<double> &samplesAmS)
{
    Q_UNUSED(samplesAm1)
    Q_UNUSED(samplesAm2)
    int size = samplesAmS.size();
    QVector<double> frequency;
    double cntrFrequency = plot->getCentralFrequency();

    for (double i = cntrFrequency - LSHIFT; i < size * INCR + (cntrFrequency - LSHIFT); i += INCR)
        frequency.append(i);

    plot->curveAmS->setSamples(frequency, samplesAmS);
    plot->replot();
}

void SummaryStrategy::update(const int &azHeading, const double &radAm1, const double &radAm2,
                             const double &radAmS, const double &radPh)
{
    Q_UNUSED(radAm1)
    Q_UNUSED(radAm2)
    PolarCurveData *dataAmS = (PolarCurveData*)(polarPlot->curveAmS->data());
    dataAmS->append(QwtPointPolar(azHeading, radAmS));
    PolarCurveData *dataPh = (PolarCurveData*)(polarPlot->curvePh->data());
    dataPh->append(QwtPointPolar(azHeading, radPh));
    polarPlot->replot();
}
