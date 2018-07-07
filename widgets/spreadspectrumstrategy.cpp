#include "spreadspectrumstrategy.hpp"

SpreadSpectrumStrategy::SpreadSpectrumStrategy(AmplitudeSpectrumPlot *plot)
{
    this->plot = plot;
}

void SpreadSpectrumStrategy::moveMarker(const double &position, const int number)
{
    plot->markerVector.at(number - 1 + 0)->setValue(position - 31 * INCR, 0);
    plot->markerVector.at(number - 1 + 1)->setValue(position + 32 * INCR, 0);
    plot->markerVector.at(number - 1 + 2)->setValue(position + (32 + 107) * INCR, 0);
    plot->markerVector.at(number - 1 + 3)->setValue(position + (32*3 + 107) * INCR, 0);
    plot->markerVector.at(number - 1 + 4)->setValue(position + (32*3 + 107*2) * INCR, 0);
    plot->markerVector.at(number - 1 + 5)->setValue(position + (32*5 + 107*2) * INCR, 0);
    plot->replot();
}
