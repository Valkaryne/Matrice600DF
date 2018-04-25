#include "spectrumplot.hpp"

SpectrumPlot::SpectrumPlot(QWidget *parent)
    : QwtPlot(parent)
{
    /* Canvas */
    QwtPlotCanvas *canvas = new QwtPlotCanvas;
    canvas->setBorderRadius(10);
    setCanvas(canvas);
    setCanvasBackground(Qt::darkBlue);
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
