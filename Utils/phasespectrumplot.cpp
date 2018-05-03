#include "phasespectrumplot.hpp"

PhaseSpectrumPlot::PhaseSpectrumPlot(QWidget *parent)
    : SpectrumPlot(parent)
{
    /* Axis */
    setAxisTitle(QwtPlot::yLeft, "Phase");
    setAxisScale(QwtPlot::yLeft, 0, 360, 90);

    /* Curve */
    curve = new QwtPlotCurve();
    curve->setStyle(QwtPlotCurve::Lines);
    curve->setPen(Qt::magenta);
    curve->setRenderHint(QwtPlotItem::RenderAntialiased, true);
    curve->setPaintAttribute(QwtPlotCurve::ClipPolygons, false);
    curve->attach(this);
}

void PhaseSpectrumPlot::setCentralFrequency(double cntrFrequency)
{
    if (this->cntrFrequency != cntrFrequency)
        this->cntrFrequency = cntrFrequency;
    double xleft = cntrFrequency - LSHIFT;
    double xright = cntrFrequency + RSHIFT;
    setAxisScale(QwtPlot::xBottom, xleft, xright);
}

void PhaseSpectrumPlot::updateCurve(const QVector<double> &samplesPh)
{
    int size = samplesPh.size();
    QVector<double> frequency;

    for (double i = cntrFrequency - LSHIFT; i < size * INCR + (cntrFrequency - LSHIFT); i += INCR)
        frequency.append(i);

    curve->setSamples(frequency, samplesPh);
    replot();
}

void PhaseSpectrumPlot::equalZoom(const QRectF &rect)
{
    setAxisScale(QwtPlot::xBottom, rect.bottomLeft().x(), rect.bottomRight().x());
}
