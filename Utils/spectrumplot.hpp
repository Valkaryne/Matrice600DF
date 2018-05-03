#ifndef SPECTRUMPLOT_HPP
#define SPECTRUMPLOT_HPP

#include <qwt_plot.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_picker.h>
#include <qwt_picker_machine.h>
#include <qwt_scale_draw.h>
#include <qwt_scale_engine.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_marker.h>
#include <qwt_wheel.h>
#include <qwt_symbol.h>

#include "constset.hpp"

class SpectrumPlot : public QwtPlot
{
    Q_OBJECT

public:
    SpectrumPlot(QWidget *parent);

    /* Hints for resize QwtPlot */
    QSize minimumSizeHint() const { return QSize(600, 150); }

    /* Setters */
    virtual void setCentralFrequency(double cntrFrequency) = 0;

public slots:

protected:
    QwtPlotCanvas *canvas;
    double cntrFrequency;
};

#endif // SPECTRUMPLOT_HPP
