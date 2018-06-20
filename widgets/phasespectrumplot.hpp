#ifndef PHASESPECTRUMPLOT_HPP
#define PHASESPECTRUMPLOT_HPP

#include "spectrumplot.hpp"

class PhaseSpectrumPlot : public SpectrumPlot
{
    Q_OBJECT
public:
    PhaseSpectrumPlot(QWidget *parent);
    void setCentralFrequency(double cntrFrequency);

    void updateCurve(const QVector<double> &samplesPh);
    void equalZoom(const QRectF &rect);

private:
    QwtPlotCurve    *curve;
};

#endif // PHASESPECTRUMPLOT_HPP
