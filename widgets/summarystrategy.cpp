#include "summarystrategy.hpp"

SummaryStrategy::SummaryStrategy(AmplitudeSpectrumPlot *plot)
{
    this->plot = plot;
    plot->curveAm1->detach();
    plot->curveAm2->detach();
    plot->curveAmS->attach(plot);
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
