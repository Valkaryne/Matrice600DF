#ifndef AMPLITUDEDISPLAYSTRATEGY_HPP
#define AMPLITUDEDISPLAYSTRATEGY_HPP

#include <utils/constset.hpp>
#include "amplitudespectrumplot.hpp"
#include "polarplot.hpp"

class AmplitudeSpectrumPlot;
class PolarPlot;

class AmplitudeDisplayStrategy
{
public:
    virtual ~AmplitudeDisplayStrategy() {}
    virtual void update(const QVector<double> &samplesAm1, const QVector<double> &samplesAm2,
                        const QVector<double> &samplesAmS, const int &number) = 0;
    virtual void update(const int &azHeading, const double &radAm1, const double &radAm2,
                        const double &radAmS, const double &radPh) = 0;

    virtual void setMaxHoldEnabled(bool enable) = 0;

    virtual double getPowerMaximum() = 0;

    void setExpCoefficient(double expCoeff) {
        if (this->expCoeff != expCoeff)
            this->expCoeff = expCoeff;
    }

protected:
    AmplitudeSpectrumPlot *plot;
    PolarPlot *polarPlot;

    double expCoeff;
};

#endif // AMPLITUDEDISPLAYSTRATEGY_HPP
