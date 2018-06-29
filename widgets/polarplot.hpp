#ifndef POLARPLOT_HPP
#define POLARPLOT_HPP

#include <qwt_polar_plot.h>
#include <qwt_polar_curve.h>
#include <qwt_polar_grid.h>
#include <qwt_polar_picker.h>
#include <qwt_picker_machine.h>
#include <qwt_scale_div.h>
#include <qwt_symbol.h>

#include "amplitudedisplaystrategy.hpp"

class AmplitudeDisplayStrategy;

class PolarPlot : public QwtPolarPlot
{
    Q_OBJECT
public:
    PolarPlot(QWidget *parent = 0);

    QSize minimumSizeHint() const { return QSize(400, 400); }

    void setDisplayStrategy(AmplitudeDisplayStrategy *strategy);

    void clearDiagram();
    void updateDiagram(const int &azHeading, const double &radAm1, const double &radAm2,
                       const double &radAmS, const double &radPh);
    void updateAllyDirection(const int az);

    QwtPolarCurve *curveAm1, *curveAm2, *curveAmS, *curvePh;

public slots:
    void getDirection(const QwtPointPolar &point);

signals:
    void setDirectionRequest(double direction);

private:
    QwtPolarCurve *phaseMarker;
    QwtPolarCurve *allyDirection;

    QwtPolarPicker *polarPicker;

    AmplitudeDisplayStrategy *strategy;
};

class PolarCurveData : public QwtArraySeriesData<QwtPointPolar>
{
public:
    PolarCurveData() {}

    void append(const QwtPointPolar &point)
    {
        d_samples << point;
    }

    void setSamples(QVector<QwtPointPolar> samples)
    {
        d_samples.clear();
        d_boundingRect = QRectF();
        if (samples.count()) {
            QwtInterval xInterval = QwtInterval(samples.at(0).azimuth(), samples.at(0).azimuth());
            QwtInterval yInterval = QwtInterval(samples.at(0).radius(), samples.at(0).radius());
            foreach (QwtPointPolar sample, samples) {
                d_samples << sample;
                xInterval |= sample.azimuth();
                yInterval |= sample.radius();
            }
        }
    }

    void clear()
    {
        d_samples.clear();
        d_samples.squeeze();
        d_boundingRect = QRectF(0.0, 0.0, -1.0, -1.0);
    }

    QRectF boundingRect() const
    {
        return d_boundingRect;
    }
};

#endif // POLARPLOT_HPP
