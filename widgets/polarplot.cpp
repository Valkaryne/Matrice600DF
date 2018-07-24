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
    connect(polarPicker, SIGNAL(appended(const QwtPointPolar &)),
            SLOT(enlightSector(const QwtPointPolar &)));

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

    dataAm1->setAutoscale(true);
    dataAm2->setAutoscale(true);
    dataAmS->setAutoscale(true);

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
    allyDirection->setPen(QPen(QBrush(Qt::white), 3));
    allyDirection->setData(dataAlly);
    allyDirection->attach(this);

    PolarCurveData *dataVector = new PolarCurveData;
    dfVector = new QwtPolarCurve;
    dfVector->setPen(QPen(QBrush(Qt::black), 2));
    dfVector->setData(dataVector);
    dfVector->attach(this);

    PolarCurveData *dataSector = new PolarCurveData;
    dfSector = new QwtPolarCurve;
    dfSector->setPen(QPen(Qt::darkRed));
    dfSector->setStyle(QwtPolarCurve::NoCurve);
    dfSector->setSymbol(new QwtSymbol(QwtSymbol::Diamond,
                                      QBrush(Qt::darkRed), QPen(Qt::darkRed), QSize(1, 1)));
    dfSector->setData(dataSector);
    dfSector->attach(this);

    testTimer = new QTimer(this);
    testTimer->setInterval(25);
    connect(testTimer, SIGNAL(timeout()), this, SLOT(testDiagram()));
    testTimer->start();
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

void PolarPlot::changeSharpCoefficient(int x)
{
    PolarCurveData *dataAm1 = (PolarCurveData*)(curveAm1->data());
    PolarCurveData *dataAm2 = (PolarCurveData*)(curveAm2->data());
    PolarCurveData *dataAmS = (PolarCurveData*)(curveAmS->data());

    dataAm1->setSharpness(x);
    dataAm2->setSharpness(x);
    dataAmS->setSharpness(x);

    replot();
}

void PolarPlot::resetScales()
{
    PolarCurveData *dataAm1 = (PolarCurveData*)(curveAm1->data());
    PolarCurveData *dataAm2 = (PolarCurveData*)(curveAm2->data());
    PolarCurveData *dataAmS = (PolarCurveData*)(curveAmS->data());

    dataAm1->resetScales();
    dataAm2->resetScales();
    dataAmS->resetScales();

    replot();
}

void PolarPlot::enlightSector(const QwtPointPolar &point)
{
    double azimuth = point.azimuth();
    double radius = point.radius();

    PolarCurveData *data = (PolarCurveData*)(dfSector->data());
    QVector<QwtPointPolar> samples;
    for (double i = 0; i < 361; i++) {
        samples.append(QwtPointPolar(azimuth - 20, i));
    }
    for (double i = 0; i < 361; i++) {
        samples.append(QwtPointPolar(azimuth + 20, i));
    }
    for (double i = azimuth - 20; i < azimuth + 20; i++) {
        samples.append(QwtPointPolar(i, 0));
    }
    for (double i = azimuth - 20; i < azimuth + 20; i++) {
        samples.append(QwtPointPolar(i, 360));
    }
    data->setSamples(samples);

    replot();
}

void PolarPlot::getDirection(const QwtPointPolar &point)
{
    double azimuth = point.azimuth();
    double radius = point.radius();

    double zeroPhase = estimateZeroPhase(azimuth, radius);
    qDebug() << zeroPhase;

    if (zeroPhase < 0.1) {
        qDebug() << "Nope";
        return;
    } else if ((zeroPhase < azimuth - 20) || (zeroPhase > azimuth + 20)) {
        qDebug() << "Nope";
        return;
    }


    PolarCurveData *data = (PolarCurveData*)(dfVector->data());
    data->clear();
    data->append(QwtPointPolar(0, 0));
    data->append(QwtPointPolar(zeroPhase, 360.0));

    replot();

    emit setDirectionRequest(zeroPhase);
}

double PolarPlot::estimateZeroPhase(const double azimuth, const double radius)
{
    double zeroPhase;
    PolarCurveData *data = (PolarCurveData*)(curvePh->data());

    QVector<QwtPointPolar> samplesWithinSector = data->getSamplesWithinSector(azimuth, radius);
    QVector<double> azs, rads;
    QVector<double> azrad, sqaz;

    foreach (QwtPointPolar sample, samplesWithinSector) {
        azs.append(sample.azimuth());
        rads.append(sample.radius());
        azrad.append(sample.azimuth() * sample.radius());
        sqaz.append(sample.azimuth() * sample.azimuth());
    }

    double azs_sum = std::accumulate(azs.begin(), azs.end(), 0);
    double rads_sum = std::accumulate(rads.begin(), rads.end(), 0);
    double azrad_sum = std::accumulate(azrad.begin(), azrad.end(), 0);
    double sqaz_sum = std::accumulate(sqaz.begin(), sqaz.end(), 0);
    int n = samplesWithinSector.size();

    double a = (n*azrad_sum - azs_sum*rads_sum)/(n*sqaz_sum-azs_sum*azs_sum);
    double b = (rads_sum - a*azs_sum)/n;

    QVector<QwtPointPolar> zeroSamples;
    for (int az = azimuth - 20; az <= azimuth + 20; az++) {
        int rad = a*az+b;
        zeroSamples.append(QwtPointPolar(az, rad));
    }

     foreach (QwtPointPolar sample, zeroSamples) {
        if ((sample.radius() <= 181) && (sample.radius() >= 179))
            zeroPhase = sample.azimuth();
    }

    /* PolarCurveData *dataV = (PolarCurveData*)(dfVector->data());
    dataV->setSamples(zeroSamples);
    replot(); */

    //qDebug() << "Size: " << samplesWithinSector.size();
    //qDebug() << "Samples: " << samplesWithinSector;
    return zeroPhase;
}

// WARNING: DELETE AFTER TEST
void PolarPlot::testDiagram()
{
    counter+=1;

    double d = 45;
    double kd = 2.7831 / d;
    double h = 360./(361-1);
    double fi = counter * h;

    double fi01 = 45;
    double x1 = fi - fi01;
    if (x1 >= 180) x1 -= 360;
    double a1 = (qrand() % int (qPow(10,2) + 1)) / qPow(10,2);
    double y1 = (0.93 + a1/10);
    if (x1 != 0) y1 = (0.93*qAbs(sin(kd*x1)/(kd*x1)) + a1/10);
    y1 = 10*qLn(y1) * M_LOG10E - 60;

    double fi02 = 75;
    double x2 = fi - fi02;
    if (x2 >= 180) x2 -= 360;
    double a2 = (qrand() % int (qPow(10,2) + 1)) / qPow(10,2);
    double y2 = (0.93 + a2/10);
    if (x2 != 0) y2 = (0.93*qAbs(sin(kd*x2)/(kd*x2)) + a2/10);
    y2 = 10*qLn(y2) * M_LOG10E - 60;

    double fi0s = 60;
    double xs = fi - fi0s;
    if (xs >= 180) xs -= 360;
    double as = (qrand() % int (qPow(10,2) + 1)) / qPow(10,2);
    double ys = (0.93 + as/10);
    if (xs != 0) ys = (0.93*qAbs(sin(kd*xs)/(kd*xs)) + as/10);
    ys = 10*qLn(ys) * M_LOG10E - 60;

    double fi0p = 60;
    double xp = counter;
    double step = 1/(2*fi0p);
    double ap = (qrand() % int (qPow(10,2) + 1)) / qPow(10,2);
    double yp = 360*step*xp + 16*ap;

    while (yp > 360)
        yp -= 360;

    updateDiagram(counter, y1, y2, ys, yp);

    if (counter > 360)
        counter -= 360;
}
