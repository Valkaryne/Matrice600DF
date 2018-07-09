#include "polarplot.hpp"

const QwtInterval radialInterval (0.0, 360.0);
const QwtInterval azimuthInterval(0.0, 360.0);

PolarPlot::PolarPlot(QWidget *parent) :
    QwtPolarPlot(parent)
{
    setPlotBackground(QColor(29,100,141));

    /* Scales */
    setScale(QwtPolar::Azimuth,
             azimuthInterval.minValue(),azimuthInterval.maxValue(),
             azimuthInterval.width() / 12);
    setScaleMaxMinor(QwtPolar::Azimuth, 2);
    setScale(QwtPolar::Radius,
                     radialInterval.minValue(),radialInterval.maxValue());

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

    /* Picker */
    polarPicker = new QwtPolarPicker(canvas());
    polarPicker->setStateMachine(new QwtPickerDragPointMachine);
    polarPicker->setMousePattern(QwtPolarPicker::MouseSelect1, Qt::LeftButton);

    connect(polarPicker,SIGNAL(appended(const QwtPointPolar &)),
            SLOT(getDirection(const QwtPointPolar &)));

    /* Marker */

    PolarCurveData *dataMark = new PolarCurveData;
    QVector<QwtPointPolar> polarVector;
    for (int i = 0; i <= 360; i++)
        polarVector.append(QwtPointPolar(i, 180.0));
    dataMark->setSamples(polarVector);

    phaseMarker = new QwtPolarCurve;
    phaseMarker->setPen(QPen(Qt::red,2));
    phaseMarker->setData(dataMark);
    phaseMarker->attach(this);

    /* Rotate */
    const double interval = 90.0;
    double origin = azimuthOrigin() / M_PI * 180.0;
    origin = qRound(origin / interval) * interval + interval;
    setAzimuthOrigin(origin / 180.0 * M_PI);

    /* Mirror */
    const double a1 = scaleDiv(QwtPolar::Azimuth)->upperBound();
    const double a2 = scaleDiv(QwtPolar::Azimuth)->lowerBound();
    setScale(QwtPolar::Azimuth, a1, a2, qAbs(a2 - a1) / 8.0);

    /* Curves */
    PolarCurveData *dataAm1 = new PolarCurveData;
    curveAm1 = new QwtPolarCurve;
    curveAm1->setPen(QPen(Qt::yellow));
    curveAm1->setStyle(QwtPolarCurve::NoCurve);
    curveAm1->setSymbol(new QwtSymbol(QwtSymbol::Diamond,
                                      QBrush(Qt::yellow), QPen(Qt::yellow), QSize(3, 3)));
    curveAm1->setData(dataAm1);
    curveAm1->attach(this);

    PolarCurveData *dataAm2 = new PolarCurveData;
    curveAm2 = new QwtPolarCurve;
    curveAm2->setPen(QPen(Qt::cyan));
    curveAm2->setStyle(QwtPolarCurve::NoCurve);
    curveAm2->setSymbol(new QwtSymbol(QwtSymbol::Diamond,
                                      QBrush(Qt::cyan), QPen(Qt::cyan), QSize(3, 3)));
    curveAm2->setData(dataAm2);
    curveAm2->attach(this);

    PolarCurveData *dataAmS = new PolarCurveData;
    curveAmS = new QwtPolarCurve;
    curveAmS->setPen(QPen(Qt::green));
    curveAmS->setStyle(QwtPolarCurve::NoCurve);
    curveAmS->setSymbol(new QwtSymbol(QwtSymbol::Diamond,
                                      QBrush(Qt::green), QPen(Qt::green), QSize(3, 3)));
    curveAmS->setData(dataAmS);
    curveAmS->attach(this);

    PolarCurveData *dataPh = new PolarCurveData;
    curvePh = new QwtPolarCurve;
    curvePh->setPen(QPen(Qt::magenta));
    curvePh->setStyle(QwtPolarCurve::NoCurve);
    curvePh->setSymbol(new QwtSymbol(QwtSymbol::Diamond,
                                      QBrush(Qt::magenta), QPen(Qt::magenta), QSize(3, 3)));
    curvePh->setData(dataPh);
    curvePh->attach(this);

    PolarCurveData *dataAlly = new PolarCurveData;
    allyDirection = new QwtPolarCurve;
    allyDirection->setPen(QPen(Qt::white));
    allyDirection->setData(dataAlly);
    allyDirection->attach(this);
}

void PolarPlot::setDisplayStrategy(AmplitudeDisplayStrategy *strategy)
{
    this->strategy = strategy;
}

void PolarPlot::clearDiagram()
{
    PolarCurveData *dataAm1 = (PolarCurveData*)(curveAm1->data());
    dataAm1->clear();
    PolarCurveData *dataAm2 = (PolarCurveData*)(curveAm2->data());
    dataAm2->clear();
    PolarCurveData *dataAmS = (PolarCurveData*)(curveAmS->data());
    dataAmS->clear();
    PolarCurveData *dataPh = (PolarCurveData*)(curvePh->data());
    dataPh->clear();
}

void PolarPlot::updateDiagram(const int &azHeading, const double &radAm1, const double &radAm2,
                              const double &radAmS, const double &radPh)
{
    strategy->update(azHeading, radAm1, radAm2, radAmS, radPh);
}

void PolarPlot::updateAllyDirection(const int az)
{
    PolarCurveData *dataAlly = (PolarCurveData*)(allyDirection->data());
    dataAlly->clear();
    dataAlly->append(QwtPointPolar(0, 0));
    dataAlly->append(QwtPointPolar(az, 360.0));
    replot();
}

void PolarPlot::changeAddCoefficient(int x)
{
    PolarCurveData *dataAm1 = (PolarCurveData*)(curveAm1->data());
    PolarCurveData *dataAm2 = (PolarCurveData*)(curveAm2->data());
    PolarCurveData *dataAmS = (PolarCurveData*)(curveAmS->data());

    setAddScale(dataAm1, x);
    setAddScale(dataAm2, x);
    setAddScale(dataAmS, x);

    replot();
}

void PolarPlot::changeProductCoefficient(double x)
{
    PolarCurveData *dataAm1 = (PolarCurveData*)(curveAm1->data());
    PolarCurveData *dataAm2 = (PolarCurveData*)(curveAm2->data());
    PolarCurveData *dataAmS = (PolarCurveData*)(curveAmS->data());

    setProductScale(dataAm1, x);
    setProductScale(dataAm2, x);
    setProductScale(dataAmS, x);

    replot();
}

void PolarPlot::resetScales()
{
    PolarCurveData *dataAm1 = (PolarCurveData*)(curveAm1->data());
    PolarCurveData *dataAm2 = (PolarCurveData*)(curveAm2->data());
    PolarCurveData *dataAmS = (PolarCurveData*)(curveAmS->data());

    int x_add = -dataAm1->add;
    double x_product = 1 / (dataAm1->prod - 1);

    setAddScale(dataAm1, x_add);
    setAddScale(dataAm2, x_add);
    setAddScale(dataAmS, x_add);

    setProductScale(dataAm1, x_product);
    setProductScale(dataAm2, x_product);
    setProductScale(dataAmS, x_product);

    replot();
}

void PolarPlot::setAddScale(PolarCurveData *data, const double &x)
{
    for (int i = 0; i < data->size(); i++) {
        QwtPointPolar &s = (QwtPointPolar&)data->samples().at(i);
        s.setRadius(s.radius() + x);
    }

    data->add += x;
}

void PolarPlot::setProductScale(PolarCurveData *data, const double &x)
{
    for (int i = 0; i < data->size(); i++) {
        QwtPointPolar &s = (QwtPointPolar&)data->samples().at(i);
        s.setRadius(s.radius() * x);
    }

    if (x > 1)
        data->prod += 1/x;
    else if (x < 1)
        data->prod -= 1/x;
}

void PolarPlot::getDirection(const QwtPointPolar &point)
{
    emit setDirectionRequest(point.azimuth());
}
