#include "amplitudespectrumplot.hpp"

AmplitudeSpectrumPlot::AmplitudeSpectrumPlot(QWidget *parent)
    : SpectrumPlot(parent)
{
    /* Axis */
    setAxisTitle(QwtPlot::yLeft, "Power");
    setAxisScale(QwtPlot::yLeft, 0 - 157, 140 - 157, 20); // WARNING: magic constants
}
