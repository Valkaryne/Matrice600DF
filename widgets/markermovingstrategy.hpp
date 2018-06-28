#ifndef MARKERMOVINGSTRATEGY_HPP
#define MARKERMOVINGSTRATEGY_HPP

#include <utils/constset.hpp>
#include "amplitudespectrumplot.hpp"

class AmplitudeSpectrumPlot;

class MarkerMovingStrategy
{
public:
    virtual ~MarkerMovingStrategy() {}
    virtual void moveMarker(const double &position, const int number) = 0;

protected:
    AmplitudeSpectrumPlot *plot;
};

#endif // MARKERSETTINGSTRATEGY_HPP
