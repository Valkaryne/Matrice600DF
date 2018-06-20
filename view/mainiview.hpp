#ifndef MAINIVIEW_HPP
#define MAINIVIEW_HPP

#include <widgets/spectrumplot.hpp>
#include <widgets/amplitudespectrumplot.hpp>
#include <widgets/phasespectrumplot.hpp>

class MainIView
{
public:

    AmplitudeSpectrumPlot *getAmplitudeSpectrumPlot() {
        return (AmplitudeSpectrumPlot*) amplitudeSpectrumPlot;
    }

    PhaseSpectrumPlot *getPhaseSpectrumPlot() {
        return (PhaseSpectrumPlot*) phaseSpectrumPlot;
    }

protected:
    SpectrumPlot        *amplitudeSpectrumPlot;
    SpectrumPlot        *phaseSpectrumPlot;
};

#endif // MAINIVIEW_HPP
