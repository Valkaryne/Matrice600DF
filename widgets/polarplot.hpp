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
class PolarCurveData;

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

    void changeAddCoefficient(int x);
    void changeProductCoefficient(double x);
    void resetScales();

private:
    void setAddScale(PolarCurveData *data, const double &x);
    void setProductScale(PolarCurveData *data, const double &x);

public slots:
    void getDirection(const QwtPointPolar &point);

signals:
    void setDirectionRequest(double direction);

public:
    QwtPolarCurve *curveAm1, *curveAm2, *curveAmS, *curvePh;

private:
    QwtPolarCurve *phaseMarker;
    QwtPolarCurve *allyDirection;

    QwtPolarPicker *polarPicker;

    AmplitudeDisplayStrategy *strategy;
};

class PolarCurveData : public QwtArraySeriesData<QwtPointPolar>
{
public:
    PolarCurveData() {
        add = 0;
        prod = 1;
    }

    void append(const QwtPointPolar &point)
    {
        QwtPointPolar& p = (QwtPointPolar&)point;
        p.setRadius((p.radius() + add) * prod);
        d_samples << p;
    }

    void setSamples(QVector<QwtPointPolar> samples)
    {
        d_samples.clear();
        d_boundingRect = QRectF();
        if (samples.count()) {
            QwtInterval xInterval = QwtInterval(samples.at(0).azimuth(), samples.at(0).azimuth());
            QwtInterval yInterval = QwtInterval(samples.at(0).radius(), samples.at(0).radius());
            foreach (QwtPointPolar sample, samples) {
                append(sample);
                //d_samples << sample;
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

public:
    int add;
    double prod = 1;
};

/*class PolarPhaseData : public QwtArraySeriesData<QwtPointPolar>
{
public:
    PolarPhaseData() {}
} */
#endif // POLARPLOT_HPP
