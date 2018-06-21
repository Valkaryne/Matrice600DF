#ifndef TWOCHANNELSTRATEGY_HPP
#define TWOCHANNELSTRATEGY_HPP

#include "amplitudedisplaystrategy.hpp"

class TwoChannelStrategy : public AmplitudeDisplayStrategy
{
public:
    TwoChannelStrategy(AmplitudeSpectrumPlot *plot);
    ~TwoChannelStrategy() {}

    void update(const QVector<double> &samplesAm1, const QVector<double> &samplesAm2,
                const QVector<double> &samplesAmS);

private:
};

#endif // TWOCHANNELSTRATEGY_HPP
