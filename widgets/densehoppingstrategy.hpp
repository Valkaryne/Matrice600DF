#ifndef DENSEHOPPINGSTRATEGY_HPP
#define DENSEHOPPINGSTRATEGY_HPP

#include "markermovingstrategy.hpp"

class DenseHoppingStrategy : public MarkerMovingStrategy
{
public:
    DenseHoppingStrategy(AmplitudeSpectrumPlot *plot = nullptr);
    ~DenseHoppingStrategy() {}

    void moveMarker(const double &position, const int number);
};

#endif // DENSEHOPPINGSTRATEGY_HPP
