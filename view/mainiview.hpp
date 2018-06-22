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

    virtual void ctrlDjiStatusChanged(QString) = 0;
    virtual void initDjiVehicleFinished(QString initStatus, bool initResult) = 0;
    virtual void activateDjiVehicleFinished(QString activateStatus, bool activateResult) = 0;

protected:
    SpectrumPlot        *amplitudeSpectrumPlot;
    SpectrumPlot        *phaseSpectrumPlot;
};

#endif // MAINIVIEW_HPP
