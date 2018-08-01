#ifndef MAINIVIEW_HPP
#define MAINIVIEW_HPP

#include <widgets/spectrumplot.hpp>
#include <widgets/amplitudespectrumplot.hpp>
#include <widgets/phasespectrumplot.hpp>
#include <widgets/polarplot.hpp>
#include <qwt_thermo.h>

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

public:
    int freqEnum[31] = { 895, 915, 2400, 2420, 2440, 2460, 2480, 2500, 5180, 5200, 5220, 5240, 5260, 5280,
                       5300, 5320, 5500, 5520, 5540, 5560, 5580, 5600, 5620, 5640, 5660, 5680, 5700, 5745,
                       5765, 5785, 5805};

protected:
    SpectrumPlot        *amplitudeSpectrumPlot;
    SpectrumPlot        *phaseSpectrumPlot;
    PolarPlot           *polarPlot;
    QwtThermo           *deviationIndicator;

    double current_phDev;
};

#endif // MAINIVIEW_HPP
