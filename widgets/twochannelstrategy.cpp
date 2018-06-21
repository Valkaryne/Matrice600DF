#include "twochannelstrategy.hpp"

TwoChannelStrategy::TwoChannelStrategy(AmplitudeSpectrumPlot *plot)
{
    this->plot = plot;
    plot->curveAm1->attach(plot);
    plot->curveAm2->attach(plot);
    plot->curveAmS->detach();
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

