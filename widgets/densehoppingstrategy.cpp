#include "densehoppingstrategy.hpp"

DenseHoppingStrategy::DenseHoppingStrategy(AmplitudeSpectrumPlot *plot)
{
    this->plot = plot;
}

void DenseHoppingStrategy::moveMarker(const double &position, const int number)
{
    plot->markerVector.at(number - 1)->setValue(position - 15 * INCR, 0);
    plot->markerVector.at(number)->setValue(position + 16 * INCR, 0);
    plot->replot();
}
