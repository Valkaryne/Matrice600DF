#ifndef FREQUENCYHOPPINGSTRATEGY_HPP
#define FREQUENCYHOPPINGSTRATEGY_HPP

#include "markermovingstrategy.hpp"

class FrequencyHoppingStrategy : public MarkerMovingStrategy
{
public:
    FrequencyHoppingStrategy(AmplitudeSpectrumPlot *plot = nullptr);
    ~FrequencyHoppingStrategy() {}

    void moveMarker(const double &position, const int number);
};

#endif // FREQUENCYHOPPINGSTRATEGY_HPP
