#ifndef AMPLITUDEDISPLAYSTRATEGY_HPP
#define AMPLITUDEDISPLAYSTRATEGY_HPP

#include <utils/constset.hpp>
#include "amplitudespectrumplot.hpp"

class AmplitudeSpectrumPlot;
class AmplitudeDisplayStrategy;

class AmplitudeDisplayStrategy
{
public:
    virtual ~AmplitudeDisplayStrategy() {}
    virtual void update(const QVector<double> &samplesAm1, const QVector<double> &samplesAm2,
                        const QVector<double> &samplesAmS) = 0;

protected:
    AmplitudeSpectrumPlot *plot;
};

#endif // AMPLITUDEDISPLAYSTRATEGY_HPP
