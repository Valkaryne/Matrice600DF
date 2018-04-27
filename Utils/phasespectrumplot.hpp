#ifndef PHASESPECTRUMPLOT_HPP
#define PHASESPECTRUMPLOT_HPP

#include "spectrumplot.hpp"

class PhaseSpectrumPlot : public SpectrumPlot
{
    Q_OBJECT

public:
    PhaseSpectrumPlot(QWidget *parent);

public slots:
    void updateCurve(const QVector<double> &samplesPh);

private:
    QwtPlotCurve    *curve;
};

#endif // PHASESPECTRUMPLOT_HPP
