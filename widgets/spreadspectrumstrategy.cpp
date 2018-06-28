#include "spreadspectrumstrategy.hpp"

SpreadSpectrumStrategy::SpreadSpectrumStrategy(AmplitudeSpectrumPlot *plot)
{
    this->plot = plot;
}

void SpreadSpectrumStrategy::moveMarker(const double &position, const int number)
{
    plot->markerVector.at(number - 1 + 0)->setValue(position + (31*1 + 32*(-2) + 107*(-1)) * INCR, 0);
    plot->markerVector.at(number - 1 + 1)->setValue(position + (31*1 + 32*0 + 107*(-1)) * INCR, 0);
    plot->markerVector.at(number - 1 + 2)->setValue(position + (31*1 + 32*0 + 107*0) * INCR, 0);
    plot->markerVector.at(number - 1 + 3)->setValue(position + (31*0 + 32*1 + 107*0) * INCR, 0);
    plot->markerVector.at(number - 1 + 4)->setValue(position + (31*0 + 32*1 + 107*1) * INCR, 0);
    plot->markerVector.at(number - 1 + 5)->setValue(position + (31*0 + 32*3 + 107*1) * INCR, 0);
    plot->replot();
    qDebug() << plot->markerVector.at(number - 1)->value().x() - plot->markerVector.at(number)->value().x();
    qDebug() << plot->markerVector.at(number - 3)->value().x() - plot->markerVector.at(number - 2)->value().x();
    qDebug() << plot->markerVector.at(number - 5)->value().x() - plot->markerVector.at(number - 4)->value().x();
}
