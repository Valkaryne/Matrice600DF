#ifndef SPREADSPECTRUMSTRATEGY_HPP
#define SPREADSPECTRUMSTRATEGY_HPP

#include "markermovingstrategy.hpp"

class SpreadSpectrumStrategy : public MarkerMovingStrategy
{
public:
    SpreadSpectrumStrategy(AmplitudeSpectrumPlot *plot = nullptr);
    ~SpreadSpectrumStrategy() {}

    void moveMarker(const double &position, const int number);
};

#endif // SPREADSPECTRUMSTRATEGY_HPP
