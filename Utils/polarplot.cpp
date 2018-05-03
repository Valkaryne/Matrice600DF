#include "polarplot.hpp"

const QwtInterval radialInterval (0.0, 360.0);
const QwtInterval azimuthInterval (0.0, 360.0);

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

PolarPlot::PolarPlot(QWidget *parent)
    : QwtPolarPlot(parent)
{
    setPlotBackground(QColor(29, 100, 141));

    /* Scales */
    setScale(QwtPolar::Azimuth,
             azimuthInterval.minValue(), azimuthInterval.maxValue(),
             azimuthInterval.width() / 12);
    setScaleMaxMinor(QwtPolar::Azimuth, 2);
    setScale(QwtPolar::Radius,
             radialInterval.minValue(), radialInterval.maxValue());

    /* Grids, Axis */
    QwtPolarGrid *grid = new QwtPolarGrid();
    grid->setPen(QPen(Qt::white));

    for (int scaleId = 0; scaleId < QwtPolar::ScaleCount; scaleId++)
    {
        grid->showGrid(scaleId);
        grid->showMinorGrid(scaleId);
        QPen minorPen(Qt::gray);
#if 0
        minorPen.setStyle(Qt::DotLine);
#endif
        grid->setMinorGridPen(scaleId, minorPen);
    }

    grid->setAxisPen(QwtPolar::AxisAzimuth, QPen(Qt::black));
    grid->showAxis(QwtPolar::AxisAzimuth, true);
    grid->showAxis(QwtPolar::AxisLeft, false);
    grid->showAxis(QwtPolar::AxisRight, true);
    grid->showAxis(QwtPolar::AxisTop, true);
    grid->showAxis(QwtPolar::AxisBottom, false);
    grid->showGrid(QwtPolar::Azimuth, true);
    grid->showGrid(QwtPolar::Radius, true);
    grid->attach(this);

    /* Rotate */
    const double interval = 90.0;
    double origin = azimuthOrigin() / M_PI * 180;
    origin = qRound(origin / interval) * interval + interval;
    setAzimuthOrigin(origin / 180.0 * M_PI);

    /* Mirror */
    const double a1 = scaleDiv(QwtPolar::Azimuth)->upperBound();
    const double a2 = scaleDiv(QwtPolar::Azimuth)->lowerBound();
    setScale(QwtPolar::Azimuth, a1, a2, qAbs(a2 - a1) / 8.0);
}
