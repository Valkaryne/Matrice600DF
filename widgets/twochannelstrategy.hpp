#ifndef TWOCHANNELSTRATEGY_HPP
#define TWOCHANNELSTRATEGY_HPP

#include "amplitudedisplaystrategy.hpp"

class TwoChannelStrategy : public AmplitudeDisplayStrategy
{
public:
    TwoChannelStrategy(AmplitudeSpectrumPlot *plot);
    TwoChannelStrategy(PolarPlot *polarPlot);
    ~TwoChannelStrategy() {}

    void update(const QVector<double> &samplesAm1, const QVector<double> &samplesAm2,
                const QVector<double> &samplesAmS);
    void update(const int &azHeading, const double &radAm1, const double &radAm2,
                const double &radAmS, const double &radPh);

    void setMaxHoldEnabled(bool enable) {
        if (enable)
            plot->curveMax->attach(plot);
        else
            plot->curveMax->detach();
    }
};

#endif // TWOCHANNELSTRATEGY_HPP
