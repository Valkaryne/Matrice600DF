#ifndef AMPLITUDESPECTRUMPLOT_HPP
#define AMPLITUDESPECTRUMPLOT_HPP

#include "spectrumplot.hpp"
#include "amplitudedisplaystrategy.hpp"
#include "markermovingstrategy.hpp"

//class AmplitudeSpectrumPlot;
class AmplitudeDisplayStrategy;
class MarkerMovingStrategy;

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

    void setDisplayStrategy(AmplitudeDisplayStrategy *strategy);
    void setMarkerStrategy(MarkerMovingStrategy *strategy);

    void setMarkerBounds(QVector<int> bounds);
    void setThresholdBounds(QVector<int> bounds);

    /* Getters */
    QwtPlotZoomer* getZoomer();
    QVector<int> getMarkerBounds();
    QVector<int> getThresholdBounds();

    /* Other */
    void resetMarkers(double dF);
    void updateCurve(const QVector<double> &samplesAm1, const QVector<double> &samplesAm2,
                     const QVector<double> &samplesAmS);

    QwtPlotCurve    *curveAm1, *curveAm2, *curveAmS;
    QwtPlotCurve    *curveMax;
    QVector<QwtPlotMarker*> markerVector;
    QVector<double> maxSamples;

private slots:
    void moveMarkers(const QPoint &pos);
    void clearMarkers(const QPoint &pos);
    void movePrimeThreshold(const QPoint &pos);
    void moveSecondThreshold(const QPoint &pos);
    void scrollLeftAxis(double);

private:
    QwtPlotPicker   *markerPicker;
    QwtPlotPicker   *clearPicker;
    QwtPlotPicker   *pickerThrPr;
    QwtPlotPicker   *pickerThrSec;
    QwtPlotMarker   *thresholdPr;
    QwtPlotMarker   *thresholdSec;
    QwtPlotZoomer   *zoomer;

    AmplitudeDisplayStrategy *strategy;
    MarkerMovingStrategy *markerStrategy;

    QVector<QColor> colors;

    int markPairNum;
    double expCoeff;
};

#endif // AMPLITUDESPECTRUMPLOT_HPP
