#ifndef POLARPLOT_HPP
#define POLARPLOT_HPP

#include <qwt_polar_plot.h>
#include <qwt_polar_curve.h>
#include <qwt_polar_grid.h>
#include <qwt_scale_div.h>
#include <qwt_symbol.h>

class PolarPlot : public QwtPolarPlot
{
    Q_OBJECT

public:
    PolarPlot(QWidget *parent = 0);

    QSize minimumSizeHint() const { return QSize(400, 400); }

    void clearDiagram();

public slots:
    void updateDiagram(const int az, const double rado, const double phase);
    void updateDiagram(const int az, const double rado, const double radl, const double phase);

private:
    QwtPolarCurve *phaseMarker;
    QwtPolarCurve *curve1;
    QwtPolarCurve *curve2;
    QwtPolarCurve *curve3;
};

#endif // POLARPLOT_HPP
