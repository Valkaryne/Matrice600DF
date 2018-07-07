#ifndef SUMMARYSTRATEGY_HPP
#define SUMMARYSTRATEGY_HPP

#include "amplitudedisplaystrategy.hpp"

class SummaryStrategy : public AmplitudeDisplayStrategy
{
public:
    SummaryStrategy(AmplitudeSpectrumPlot *plot);
    SummaryStrategy(PolarPlot *polarPlot);
    ~SummaryStrategy() {}

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

#endif // SUMMARYSTRATEGY_HPP
