#include "phasespectrumplot.hpp"

PhaseSpectrumPlot::PhaseSpectrumPlot(QWidget *parent) :
    SpectrumPlot(parent)
{
    /* Axis */
    setAxisTitle(QwtPlot::yLeft, "Phase");
    setAxisScale(QwtPlot::yLeft,0,360,90);

    /* Curves */
    curve = new QwtPlotCurve();
    curve->setStyle(QwtPlotCurve::Lines);
    curve->setPen(Qt::magenta);
    curve->setRenderHint(QwtPlotItem::RenderAntialiased, true);
    curve->setPaintAttribute(QwtPlotCurve::ClipPolygons, false);
    curve->attach(this);

    /* Wheel */
    wheel = new QwtWheel(canvas);
    wheel->setVisible(true);
    wheel->setOrientation(Qt::Vertical);
    wheel->setRange(-180, 180);
    wheel->setValue(0.0);
    wheel->setMass(1.0);
    wheel->setSingleStep(10.0);
    wheel->setTotalAngle(4 * 360.0);
    wheel->setGeometry(0, 30, 14, 44);
    wheel->setEnabled(true);

    connect(wheel,SIGNAL(valueChanged(double)),SIGNAL(phaseCorrector(double)));
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
    QVector<double> frequency;

    for (double i = cntrFrequency - LSHIFT; i < cntrFrequency + RSHIFT; i += INCR)
        frequency.append(i);

    curve->setSamples(frequency, samplesPh);
    replot();
}

void PhaseSpectrumPlot::equalZoom(const QRectF &rect)
{
    setAxisScale(QwtPlot::xBottom, rect.bottomLeft().x(), rect.bottomRight().x());
    replot();
}

void PhaseSpectrumPlot::scrollLeftAxis(double value)
{
    qDebug() << "Current phase correction: " << value;
}
