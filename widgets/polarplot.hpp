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

#include <QTimer>

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

    void changeSharpCoefficient(int x);
    void resetScales();

    // WARNING: DELETE AFTER TEST
private slots:
    void testDiagram();
    void getDirectionAuto(const QwtPointPolar &point);
    void getDirectionManual(const QwtPointPolar &point);

signals:
    void setDirectionRequest(double &direction);

private:
    double estimateZeroPhase(const double azimuth);
    void enlightSector(const double azimuth);

public:
    QwtPolarCurve *curveAm1, *curveAm2, *curveAmS, *curvePh;

private:
    QwtPolarCurve *phaseMarker;
    QwtPolarCurve *allyDirection;
    QwtPolarCurve *dfVector;
    QwtPolarCurve *dfSector;

    QwtPolarPicker *polarPicker;
    QwtPolarPicker *polarPickerManual;

    AmplitudeDisplayStrategy *strategy;

    // WARNING: DELETE AFTER TEST
    QTimer  *testTimer;
    double counter;
};

class PolarCurveData : public QwtArraySeriesData<QwtPointPolar>
{
public:
    PolarCurveData() {
        c_add = 0.0;
        c_prod = 1.0;
        c_min = 1000;
        c_max = -1000;
        this->autoscaleEnabled = false;
        this->sharpness = -1;
    }

    void append(const QwtPointPolar &point)
    {
        QwtPointPolar& p = (QwtPointPolar&)point;
        double rad = point.radius();

        if (autoscaleEnabled) {
            if (rad < c_min) {
                c_min = rad;
                updateCoefficients();
            }
            if (rad > c_max) {
                c_max = rad;
                updateCoefficients();
            }

            p.setRadius((p.radius() + c_add) * c_prod);
            if (p.radius() > 360.0)
                p.setRadius(360.0);
        }

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
                xInterval |= sample.azimuth();
                yInterval |= sample.radius();
            }
        }
    }

    QVector<QwtPointPolar> getSamplesWithinSector(const double azimuth)
    {
        QVector<QwtPointPolar> samples;
        foreach (QwtPointPolar sample, d_samples) {
            if ((sample.azimuth() >= azimuth - 20) && (sample.azimuth() <= azimuth + 20)) {
                if ((sample.radius() >= 0) && (sample.radius() <= 360))
                    samples.append(sample);
            }
        }
        return samples;
    }

    double getPowerMaximum() {
        double maximum = -1000;
        double powerAzimuth = 0;
        foreach (QwtPointPolar sample, d_samples) {
            if (sample.radius() > maximum) {
                maximum = sample.radius();
                powerAzimuth = sample.azimuth();
            }
        }
        return powerAzimuth;
    }

    void setAutoscale(bool autoscaleEnabled) {
        if (this->autoscaleEnabled != autoscaleEnabled)
            this->autoscaleEnabled = autoscaleEnabled;
    }

    void setSharpness(int sharpness) {
        if (this->sharpness != sharpness)
            this->sharpness = sharpness;
        updateCoefficients();
    }

    void resetScales()
    {
        c_max = -1000;
        c_min = 1000;
        resetSamples();
        c_add = 0;
        c_prod = 1;
        updateSamples();
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

private:
    void resetSamples() {
        for (int i = 0; i < size(); i++) {
            QwtPointPolar &s = (QwtPointPolar&)samples().at(i);
                s.setRadius(s.radius() / c_prod - c_add);
        }
    }

    void updateSamples() {
        for (int i = 0; i < size(); i++) {
            QwtPointPolar &s = (QwtPointPolar&)samples().at(i);
                s.setRadius((s.radius() + c_add) * c_prod);
        }
    }

    void updateCoefficients() {
        resetSamples();
        c_add = qAbs(c_min) - sharpness;
        c_prod = 0.95 * 360.0 / (c_max + c_add);
        updateSamples();
    }

private:
    bool autoscaleEnabled;
    double c_add;
    double c_prod;
    double c_min;
    double c_max;

    int sharpness;
};

#endif // POLARPLOT_HPP
