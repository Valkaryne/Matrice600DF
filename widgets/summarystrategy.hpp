#ifndef SUMMARYSTRATEGY_HPP
#define SUMMARYSTRATEGY_HPP

#include "amplitudedisplaystrategy.hpp"

class SummaryStrategy : public AmplitudeDisplayStrategy
{
public:
    SummaryStrategy(AmplitudeSpectrumPlot *plot = nullptr);
    ~SummaryStrategy() {}

    void update(const QVector<double> &samplesAm1, const QVector<double> &samplesAm2,
                const QVector<double> &samplesAmS);

private:
};

#endif // SUMMARYSTRATEGY_HPP
