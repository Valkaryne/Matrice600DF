#ifndef AMPLITUDEDISPLAYSTRATEGY_HPP
#define AMPLITUDEDISPLAYSTRATEGY_HPP

#include <utils/constset.hpp>
#include <widgets/amplitudespectrumplot.hpp>

class AmplitudeDisplayStrategy
{
    Q_OBJECT
public:
    AmplitudeDisplayStrategy(AmplitudeSpectrumPlot *plot) {}
    virtual ~AmplitudeDisplayStrategy() {}
    virtual void update(const QVector<double> &samplesAm1, const QVector<double> &samplesAm2,
                const QVector<double> &samplesAmS) = 0;

    AmplitudeSpectrumPlot *plot;
};

#endif // AMPLITUDEDISPLAYSTRATEGY_HPP
