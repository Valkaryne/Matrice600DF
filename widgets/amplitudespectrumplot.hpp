#ifndef AMPLITUDESPECTRUMPLOT_HPP
#define AMPLITUDESPECTRUMPLOT_HPP

#include "spectrumplot.hpp"
#include <qwt_wheel.h>

class AmplitudeSpectrumPlot : public SpectrumPlot
{
    Q_OBJECT
public:
    AmplitudeSpectrumPlot(QWidget *parent);

    /* Setters */
    void setMarker(int number);
    void setPickers(bool enable);
    void setThresholdPickers(bool enable);
    void setZoomer(bool enable);
    void setCentralFrequency(double cntrFrequency);
    void setZoomBase(double xleft, double xright);
    void setMaxHold(bool holdOn);
    void setExpCoefficient(double expCoeff);

    void setDroneClass(int droneClass);

    /* Getters */
    QwtPlotZoomer* getZoomer();
    QVector<int> getMarkerBounds();
    QVector<int> getThresholdBounds();

    /* Other */
    void resetMarkers();
    void updateCurve(const QVector<double> &samplesAm1, const QVector<double> &samplesAm2,
                     const QVector<double> &samplesAmS);

private slots:
    void moveMarkers(const QPoint &pos);
    void movePrimeThreshold(const QPoint &pos);
    void moveSecondThreshold(const QPoint &pos);
    void scrollLeftAxis(double);

private:
    QwtPlotCurve    *curve1;
    QwtPlotCurve    *curve2;
    QwtPlotCurve    *curveMax;
    QwtPlotPicker   *markerPicker;
    QwtPlotPicker   *pickerThrPr;
    QwtPlotPicker   *pickerThrSec;
    QwtPlotMarker   *thresholdPr;
    QwtPlotMarker   *thresholdSec;
    QwtPlotZoomer   *zoomer;
    QwtWheel        *wheel;

    QVector<QwtPlotMarker*> markerVector;
    QVector<QColor> colors;
    QVector<double> maxSamples;

    int droneClass;
    int markPairNum;
    double expCoeff;
};

#endif // AMPLITUDESPECTRUMPLOT_HPP