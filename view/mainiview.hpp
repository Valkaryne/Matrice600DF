#ifndef MAINIVIEW_HPP
#define MAINIVIEW_HPP

#include <widgets/spectrumplot.hpp>
#include <widgets/amplitudespectrumplot.hpp>
#include <widgets/phasespectrumplot.hpp>
#include <widgets/polarplot.hpp>

class MainIView
{
public:

    AmplitudeSpectrumPlot *getAmplitudeSpectrumPlot() {
        return (AmplitudeSpectrumPlot*) amplitudeSpectrumPlot;
    }

    PhaseSpectrumPlot *getPhaseSpectrumPlot() {
        return (PhaseSpectrumPlot*) phaseSpectrumPlot;
    }

    PolarPlot *getPolarPlot() {
        return polarPlot;
    }

    virtual void ctrlDjiStatusChanged(QString) = 0;
    virtual void initDjiVehicleFinished(QString initStatus, bool initResult) = 0;
    virtual void activateDjiVehicleFinished(QString activateStatus, bool activateResult) = 0;
    virtual void connectionDjiVehicleResetted() = 0;
    virtual void updateTelemetryData(const QVector<double> &subscribeData);
    virtual void displayPhaseDeviation(const double &phDev) = 0;

protected:
    SpectrumPlot        *amplitudeSpectrumPlot;
    SpectrumPlot        *phaseSpectrumPlot;
    PolarPlot           *polarPlot;
};

#endif // MAINIVIEW_HPP
