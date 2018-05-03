#ifndef AMPLITUDESPECTRUMPLOT_HPP
#define AMPLITUDESPECTRUMPLOT_HPP

#include "spectrumplot.hpp"

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

    /* Getters */
    QwtPlotPicker* getMarkerPicker(bool prime);
    QwtPlotPicker* getThreshPicker(bool prime);
    QwtPlotZoomer* getZoomer();
    QVector<int> getMarkerBounds();
    QVector<int> getThresholdBounds();

    /* Other */
    void resetMarkers();
    void moveMarker(double position, bool prime);
    void moveThreshold(double position, bool prime);

public slots:
    void updateCurve(const QVector<double> &samplesAm1, const QVector<double> &samplesAm2);

private:
    QwtPlotCurve    *curve1;
    QwtPlotCurve    *curve2;
    QwtPlotCurve    *curveMax;
    QwtPlotPicker   *pickerMarkPr;
    QwtPlotPicker   *pickerMarkSec;
    QwtPlotPicker   *pickerThrPr;
    QwtPlotPicker   *pickerThrSec;
    QwtPlotMarker   *thresholdPr;
    QwtPlotMarker   *thresholdSec;
    QwtPlotZoomer   *zoomer;

    QVector<QwtPlotMarker*> markerVector;
    QVector<QColor> colors;
    QVector<double> maxSamples;

    int markPairNmr;
    double expCoeff;
};

#endif // AMPLITUDESPECTRUMPLOT_HPP
