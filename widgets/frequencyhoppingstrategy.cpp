#include "frequencyhoppingstrategy.hpp"

FrequencyHoppingStrategy::FrequencyHoppingStrategy(AmplitudeSpectrumPlot *plot)
{
    this->plot = plot;
}

void FrequencyHoppingStrategy::moveMarker(const double &position, const int number)
{
    plot->markerVector.at(number - 1)->setValue(position - 31 * INCR, 0);
    plot->markerVector.at(number)->setValue(position + 32 * INCR, 0);
    plot->replot();
}
