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

    /* Marker */
    QVector<QwtPointPolar> polarVector;
    for (int i = 0; i <= 360; i++)
        polarVector.append(QwtPointPolar(i, azimuthInterval.maxValue() / 2));

    PolarCurveData *dataMark = new PolarCurveData();
    dataMark->setSamples(polarVector);

    phaseMarker = new QwtPolarCurve();
    phaseMarker->setPen(QPen(Qt::red, 2));
    phaseMarker->setData(dataMark);
    phaseMarker->attach(this);

    /* Rotate */
    const double interval = 90.0;
    double origin = azimuthOrigin() / M_PI * 180;
    origin = qRound(origin / interval) * interval + interval;
    setAzimuthOrigin(origin / 180.0 * M_PI);

    /* Mirror */
    const double a1 = scaleDiv(QwtPolar::Azimuth)->upperBound();
    const double a2 = scaleDiv(QwtPolar::Azimuth)->lowerBound();
    setScale(QwtPolar::Azimuth, a1, a2, qAbs(a2 - a1) / 8.0);

    /* Curves */
    PolarCurveData *data1 = new PolarCurveData();
    curve1 = new QwtPolarCurve();
    curve1->setPen(QPen(Qt::yellow));
    curve1->setStyle(QwtPolarCurve::NoCurve);
    curve1->setSymbol(new QwtSymbol(QwtSymbol::Diamond,
                                    QBrush(Qt::yellow), QPen(Qt::yellow), QSize(4, 4)));
    curve1->setData(data1);
    curve1->attach(this);

    PolarCurveData *data2 = new PolarCurveData();
    curve2 = new QwtPolarCurve();
    curve2->setPen(QPen(Qt::cyan));
    curve2->setStyle(QwtPolarCurve::NoCurve);
    curve2->setSymbol(new QwtSymbol(QwtSymbol::Diamond,
                                    QBrush(Qt::cyan), QPen(Qt::cyan), QSize(4, 4)));
    curve2->setData(data2);
    curve2->attach(this);

    PolarCurveData *data3 = new PolarCurveData();
    curve3 = new QwtPolarCurve();
    curve3->setPen(QPen(Qt::magenta));
    curve3->setStyle(QwtPolarCurve::NoCurve);
    curve3->setSymbol(new QwtSymbol(QwtSymbol::Diamond,
                                    QBrush(Qt::magenta), QPen(Qt::magenta), QSize(4, 4)));
    curve3->setData(data3);
    curve3->attach(this);
}

void PolarPlot::clearDiagram()
{
    PolarCurveData *data1 = (PolarCurveData*)(curve1->data());
    data1->clear();
    PolarCurveData *data2 = (PolarCurveData*)(curve2->data());
    data2->clear();
    PolarCurveData *data3 = (PolarCurveData*)(curve3->data());
    data3->clear();
    replot();
}

void PolarPlot::updateDiagram(const int az, const double rado, const double phase)
{
    PolarCurveData *data1 = (PolarCurveData*)(curve1->data());
    data1->append(QwtPointPolar(az, rado));
    PolarCurveData *data3 = (PolarCurveData*)(curve3->data());
    data3->append(QwtPointPolar(az, phase));
    replot();
}

void PolarPlot::updateDiagram(const int az, const double rado, const double radl, const double phase)
{
    PolarCurveData *data1 = (PolarCurveData*)(curve1->data());
    data1->append(QwtPointPolar(az, rado));
    PolarCurveData *data2 = (PolarCurveData*)(curve2->data());
    data2->append(QwtPointPolar(az, radl));
    PolarCurveData *data3 = (PolarCurveData*)(curve3->data());
    data3->append(QwtPointPolar(az, phase));
    replot();
}
