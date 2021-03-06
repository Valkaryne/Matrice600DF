#ifndef PHASESPECTRUMPLOT_HPP
#define PHASESPECTRUMPLOT_HPP

#include "spectrumplot.hpp"

class PhaseSpectrumPlot : public SpectrumPlot
{
    Q_OBJECT
public:
    PhaseSpectrumPlot(QWidget *parent);
    void setCentralFrequency(double cntrFrequency);
    void setZoomBase(double xleft, double xright) {
        Q_UNUSED(xleft)
        Q_UNUSED(xright)
    }
    void setPhaseCorrection(int phaseCorrection);

    void updateCurve(const QVector<double> &samplesPh);

private slots:
    void equalZoom(const QRectF &rect);
    void scrollLeftAxis(double);

signals:
    void phaseCorrector(double);

private:
    QwtPlotCurve    *curve;
};

#endif // PHASESPECTRUMPLOT_HPP
