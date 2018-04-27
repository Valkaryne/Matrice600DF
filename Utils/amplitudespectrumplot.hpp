#ifndef AMPLITUDESPECTRUMPLOT_HPP
#define AMPLITUDESPECTRUMPLOT_HPP

#include "spectrumplot.hpp"

class AmplitudeSpectrumPlot : public SpectrumPlot
{
    Q_OBJECT

public:
    AmplitudeSpectrumPlot(QWidget *parent);

public slots:
    void updateCurve(const QVector<double> &samplesAm1, const QVector<double> &samplesAm2);

private:
    QwtPlotCurve    *curve1;
    QwtPlotCurve    *curve2;
    QwtPlotCurve    *curveMax;

    QVector<double> maxSamples;

    double expCoeff;
};

#endif // AMPLITUDESPECTRUMPLOT_HPP
