#include "summarystrategy.hpp"

SummaryStrategy::SummaryStrategy(AmplitudeSpectrumPlot *plot)
{
    this->plot = plot;

    curve = new QwtPlotCurve;
    curve->setStyle(QwtPlotCurve::Lines);
    curve->setPen(Qt::green);
    curve->setRenderHint(QwtPlotItem::RenderAntialiased, true);
    curve->setPaintAttribute(QwtPlotCurve::ClipPolygons, false);
    curve->attach(plot);
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

    curve->setSamples(frequency, samplesAmS);

    plot->replot();
}
