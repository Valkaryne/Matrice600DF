#include "twochannelstrategy.hpp"

TwoChannelStrategy::TwoChannelStrategy(AmplitudeSpectrumPlot *plot)
{
    this->plot = plot;

    /* Curves */
    curve1 = new QwtPlotCurve;
    curve1->setStyle(QwtPlotCurve::Lines);
    curve1->setPen(Qt::yellow);
    curve1->setRenderHint(QwtPlotItem::RenderAntialiased, true);
    curve1->setPaintAttribute(QwtPlotCurve::ClipPolygons, false);
    curve1->attach(plot);

    curve2 = new QwtPlotCurve;
    curve2->setStyle(QwtPlotCurve::Lines);
    curve2->setPen(Qt::cyan);
    curve2->setRenderHint(QwtPlotItem::RenderAntialiased, true);
    curve2->setPaintAttribute(QwtPlotCurve::ClipPolygons, false);
    curve2->attach(plot);
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

    curve1->setSamples(frequency, samplesAm1);
    curve2->setSamples(frequency, samplesAm2);

    plot->replot();
}
