#include "spectrumplot.hpp"

SpectrumPlot::SpectrumPlot(QWidget *parent) :
    QwtPlot(parent),
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
    setAxisScale(QwtPlot::xBottom, cntrFrequency - LSHIFT, cntrFrequency + RSHIFT, 3);
    setAxisAutoScale(QwtPlot::xBottom, false);
    setAxisAutoScale(QwtPlot::yLeft, false);
}
