#include "spectrumplot.hpp"

SpectrumPlot::SpectrumPlot(QWidget *parent)
    : QwtPlot(parent),
      cntrFrequency(70)
{
    /* Canvas */
    canvas = new QwtPlotCanvas;
    canvas->setBorderRadius(10);
    setCanvas(canvas);
    setCanvasBackground(QColor(29, 100, 141));
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    /* Grid */
    QwtPlotGrid *grid = new QwtPlotGrid;
    grid->setMajorPen(Qt::white, 0, Qt::DotLine);
    grid->attach(this);

    /* Axis */
    setAxisTitle(QwtPlot::xBottom, "Frequency, MHz");
    setAxisScale(QwtPlot::xBottom, 54.55, 85.27, 3); // WARNING: magic constants
    setAxisAutoScale(QwtPlot::xBottom, false);
    setAxisAutoScale(QwtPlot::yLeft, false);
}

void SpectrumPlot::setCentralFrequency(double cntrFrequency)
{
    if (this->cntrFrequency != cntrFrequency)
        this->cntrFrequency = cntrFrequency;
}
