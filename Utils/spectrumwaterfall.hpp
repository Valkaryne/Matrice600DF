#ifndef SPECTRUMWATERFALL_HPP
#define SPECTRUMWATERFALL_HPP

#include <QObject>
#include <qwt_plot.h>
#include <qwt_plot_spectrogram.h>
#include <qwt_plot_spectrocurve.h>
#include <qwt_matrix_raster_data.h>
#include <qwt_plot_canvas.h>
#include <qwt_scale_widget.h>
#include <qwt_color_map.h>

#include "constset.hpp"

class SpectrumWaterfall : public QwtPlot
{
    Q_OBJECT

public:
    SpectrumWaterfall(QWidget *parent);

    /* Hints for resize QwtPlot */
    //QSize sizeHint() const { return QSize(600, 150); }
    QSize minimumSizeHint() const { return QSize(600, 150); }

    /* Setters */
    void setCentralFrequency(double cntrFrequency);

public slots:
    void updateSpectrogram(const QVector<double> sampleAm1, const QVector<double> sampleAm2);

private:
    QwtPlotSpectrogram  *plot;
    QwtMatrixRasterData *plotData;
    QVector<double>     *vec;

    double cntrFrequency;
};

#endif // SPECTRUMWATERFALL_HPP
