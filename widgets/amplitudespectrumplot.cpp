#include "amplitudespectrumplot.hpp"

AmplitudeSpectrumPlot::AmplitudeSpectrumPlot(QWidget *parent) :
    SpectrumPlot(parent),
    expCoeff(0.999)
{
    /* Axis */
    setAxisTitle(QwtPlot::yLeft, "Power");
    setAxisScale(QwtPlot::yLeft, 40 - CALIBRATION, 120 - CALIBRATION, 20);

    /* Curves */
    curveAm1 = new QwtPlotCurve;
    curveAm1->setStyle(QwtPlotCurve::Lines);
    curveAm1->setPen(Qt::yellow);
    curveAm1->setRenderHint(QwtPlotItem::RenderAntialiased, true);
    curveAm1->setPaintAttribute(QwtPlotCurve::ClipPolygons, false);
    curveAm1->attach(this);

    curveAm2 = new QwtPlotCurve;
    curveAm2->setStyle(QwtPlotCurve::Lines);
    curveAm2->setPen(Qt::cyan);
    curveAm2->setRenderHint(QwtPlotItem::RenderAntialiased, true);
    curveAm2->setPaintAttribute(QwtPlotCurve::ClipPolygons, false);
    curveAm2->attach(this);

    curveAmS = new QwtPlotCurve;
    curveAmS->setStyle(QwtPlotCurve::Lines);
    curveAmS->setPen(Qt::green);
    curveAmS->setRenderHint(QwtPlotItem::RenderAntialiased, true);
    curveAmS->setPaintAttribute(QwtPlotCurve::ClipPolygons, false);
    curveAmS->attach(this);

    curveMax = new QwtPlotCurve;
    curveMax->setStyle(QwtPlotCurve::NoCurve);
    curveMax->setSymbol(new QwtSymbol(QwtSymbol::Ellipse,
                                      QBrush(Qt::gray), QPen(Qt::gray), QSize(1, 1)));
    curveMax->setRenderHint(QwtPlotItem::RenderAntialiased, true);
    curveMax->setPaintAttribute(QwtPlotCurve::ClipPolygons, false);
    curveMax->attach(this);

    /* Pickers */
    markerPicker = new QwtPlotPicker(
                QwtPlot::xBottom, QwtPlot::yLeft,
                QwtPlotPicker::VLineRubberBand,
                QwtPicker::AlwaysOn,
                canvas);
    markerPicker->setRubberBandPen(QColor(Qt::red));
    markerPicker->setTrackerPen(QColor(Qt::white));
    markerPicker->setStateMachine(new QwtPickerDragPointMachine());
    markerPicker->setMousePattern(QwtPlotPicker::MouseSelect1, Qt::LeftButton);

    clearPicker = new QwtPlotPicker(
                QwtPlot::xBottom, QwtPlot::yLeft,
                QwtPlotPicker::VLineRubberBand,
                QwtPicker::AlwaysOn,
                canvas);
    clearPicker->setRubberBandPen(QColor(Qt::red));
    clearPicker->setTrackerPen(QColor(Qt::white));
    clearPicker->setStateMachine(new QwtPickerDragPointMachine);
    clearPicker->setMousePattern(QwtPlotPicker::MouseSelect1, Qt::RightButton);

    pickerThrPr = new QwtPlotPicker(
                QwtPlot::xBottom, QwtPlot::yLeft,
                QwtPlotPicker::VLineRubberBand,
                QwtPicker::AlwaysOn,
                canvas);
    pickerThrPr->setRubberBandPen(QColor(Qt::green));
    pickerThrPr->setTrackerPen(QColor(Qt::white));
    pickerThrPr->setStateMachine(new QwtPickerDragPointMachine);
    pickerThrPr->setMousePattern(QwtPlotPicker::MouseSelect1, Qt::LeftButton);

    pickerThrSec = new QwtPlotPicker(
                QwtPlot::xBottom, QwtPlot::yLeft,
                QwtPlotPicker::VLineRubberBand,
                QwtPicker::AlwaysOn,
                canvas);
    pickerThrSec->setRubberBandPen(QColor(Qt::green));
    pickerThrSec->setTrackerPen(QColor(Qt::white));
    pickerThrSec->setStateMachine(new QwtPickerDragPointMachine);
    pickerThrSec->setMousePattern(QwtPlotPicker::MouseSelect1, Qt::RightButton);

    /* Thresholds */
    thresholdPr = new QwtPlotMarker;
    thresholdPr->setLineStyle(QwtPlotMarker::HLine);
    thresholdPr->setLinePen(QColor(Qt::white),2,Qt::DashLine);
    thresholdPr->setValue(0, 0 - CALIBRATION);
    thresholdPr->attach(this);

    thresholdSec = new QwtPlotMarker;
    thresholdSec->setLineStyle(QwtPlotMarker::HLine);
    thresholdSec->setLinePen(QColor(Qt::white),2,Qt::DashLine);
    thresholdSec->setValue(0, 300 - CALIBRATION);
    thresholdSec->attach(this);

    /* Color */
    colors = {*new QColor(220,20,75), /* crimson */
             *new QColor(11,218,81), /* malachite */
             *new QColor(127,199,255), /* skiey */
             *new QColor(255,153,0), /* orange */
             *new QColor(255,170,204), /* rose */
             };

    /* Zoomer */
    zoomer = new QwtPlotZoomer(canvas);
    zoomer->setRubberBandPen(QColor(Qt::darkGreen));
    zoomer->setTrackerMode(QwtPlotPicker::AlwaysOn);
    zoomer->setMousePattern(QwtEventPattern::MouseSelect2,
                            Qt::RightButton, Qt::ControlModifier);

    /* Wheel */
    wheel = new QwtWheel(canvas);
    wheel->setVisible(true);
    wheel->setOrientation(Qt::Vertical);
    wheel->setRange(-10 - CALIBRATION, 90 - CALIBRATION);
    wheel->setValue(0.0);
    wheel->setMass(1.0);
    wheel->setSingleStep(10.0);
    wheel->setTotalAngle(4 * 360.0);
    wheel->setGeometry(0, 30, 14, 44);
    wheel->setEnabled(true);

    connect(wheel,SIGNAL(valueChanged(double)),SLOT(scrollLeftAxis(double)));

    /* Other settings */
    for (int i = 0; i < 4096; i++)
        maxSamples.append(0);

    for (int i = 1; i <= 5; i++)
        setMarker(i);

    this->markPairNum = 1;

    connect(markerPicker, SIGNAL(appended(const QPoint &)),
            SLOT(moveMarkers(const QPoint &)));
    connect(clearPicker, SIGNAL(appended(const QPoint &)),
            SLOT(clearMarkers(const QPoint &)));
    connect(pickerThrPr, SIGNAL(appended(const QPoint &)),
            SLOT(movePrimeThreshold(const QPoint &)));
    connect(pickerThrSec, SIGNAL(appended(const QPoint &)),
            SLOT(moveSecondThreshold(const QPoint &)));
}

void AmplitudeSpectrumPlot::setMarker(int number)
{
    if (markerVector.size() < number * 2) {
        QwtPlotMarker *markerPr = new QwtPlotMarker();
        markerPr->setLineStyle(QwtPlotMarker::VLine);
        markerPr->setLinePen(colors.at((number - 1) % colors.size()), 2, Qt::SolidLine);
        markerPr->setValue(cntrFrequency - LSHIFT, 0);
        markerPr->attach(this);

        QwtPlotMarker *markerSec = new QwtPlotMarker();
        markerSec->setLineStyle(QwtPlotMarker::VLine);
        markerSec->setLinePen(colors.at((number - 1) % colors.size()), 2, Qt::SolidLine);
        markerSec->setValue(cntrFrequency + RSHIFT, 0);
        markerSec->attach(this);

        markerVector.append(markerPr);
        markerVector.append(markerSec);
    }
    markPairNum = number * 2 - 1;
}

void AmplitudeSpectrumPlot::setPickers(bool enable)
{
    markerPicker->setEnabled(enable);
    clearPicker->setEnabled(enable);
}

void AmplitudeSpectrumPlot::setThresholdPickers(bool enable)
{
    pickerThrPr->setEnabled(enable);
    pickerThrSec->setEnabled(enable);
}

void AmplitudeSpectrumPlot::setZoomer(bool enable)
{
    zoomer->setEnabled(enable);
}

void AmplitudeSpectrumPlot::setCentralFrequency(double cntrFrequency)
{
    if (this->cntrFrequency != cntrFrequency) {
        resetMarkers(cntrFrequency - this->cntrFrequency);
        this->cntrFrequency = cntrFrequency;
    }
    double xleft = cntrFrequency - LSHIFT;
    double xright = cntrFrequency + RSHIFT;
    setAxisScale(QwtPlot::xBottom, xleft, xright);
    setZoomBase(xleft, xright);
}

void AmplitudeSpectrumPlot::setZoomBase(double xleft, double xright)
{
    QStack<QRectF> stack = zoomer->zoomStack();
    QRectF base = QRectF(QPointF(xleft, 40 - CALIBRATION), QPointF(xright, 120 - CALIBRATION));
    stack.clear();
    stack.append(base);
    zoomer->setZoomStack(stack);
    zoomer->setZoomBase(base);
}

void AmplitudeSpectrumPlot::setMaxHold(bool holdOn)
{
    strategy->setMaxHoldEnabled(holdOn);
}

void AmplitudeSpectrumPlot::setExpCoefficient(double expCoeff)
{
    strategy->setExpCoefficient(0.99 + 0.01*expCoeff);
}

void AmplitudeSpectrumPlot::setDisplayStrategy(AmplitudeDisplayStrategy *strategy)
{
    this->strategy = strategy;
}

void AmplitudeSpectrumPlot::setMarkerStrategy(MarkerMovingStrategy *strategy)
{
    this->markerStrategy = strategy;
}

QwtPlotZoomer* AmplitudeSpectrumPlot::getZoomer()
{
    return zoomer;
}

QVector<int> AmplitudeSpectrumPlot::getMarkerBounds()
{
    QVector<int> bounds;
    for (int i = 1; i < markerVector.size(); i += 2) {
        double bound = markerVector.at(i - 1)->xValue();
        bounds.append(CENTER + (bound - cntrFrequency) / INCR);
        bound = markerVector.at(i)->xValue();
        bounds.append(CENTER + (bound - cntrFrequency) / INCR);
        qSort(bounds.end() - 2, bounds.end());
        if ((i > 1) &&
                (bounds.at(i - 1) < bounds.at(i - 3)) &&
                (bounds.at(i) > bounds.at(i - 2))) {
            bounds.replace(i - 1, bounds.at(i - 3));
            bounds.replace(i, bounds.at(i - 2));
        }
    }
    return bounds;
}

QVector<int> AmplitudeSpectrumPlot::getThresholdBounds()
{
    QVector<int> bounds;
    bounds.append(thresholdPr->yValue());

    if ((int)((CENTER + (markerVector.at(0)->xValue() - cntrFrequency) / INCR)) <= 0 &&
            (int)((CENTER + (markerVector.at(1)->xValue() - cntrFrequency) / INCR)) >= 4095)
        bounds.append(10000);
    else
        bounds.append(thresholdSec->yValue());

    qSort(bounds);
    return bounds;
}

void AmplitudeSpectrumPlot::resetMarkers(double dF)
{
    for (int i = 0; i < markerVector.size(); i++) {
        QwtPlotMarker *marker = markerVector.at(i);
        marker->setValue(marker->xValue() + dF, 0);
    }
    replot();
}

void AmplitudeSpectrumPlot::updateCurve(const QVector<double> &samplesAm1, const QVector<double> &samplesAm2,
                                        const QVector<double> &samplesAmS)
{
    strategy->update(samplesAm1, samplesAm2, samplesAmS);
}

void AmplitudeSpectrumPlot::moveMarkers(const QPoint &pos)
{
    double position = invTransform(QwtPlot::xBottom, pos.x());
    markerStrategy->moveMarker(position, markPairNum);
}

void AmplitudeSpectrumPlot::clearMarkers(const QPoint &pos)
{
    Q_UNUSED(pos)
    markerVector.at(markPairNum - 1)->setValue(cntrFrequency - LSHIFT, 0);
    markerVector.at(markPairNum)->setValue(cntrFrequency + RSHIFT, 0);
    replot();
}

void AmplitudeSpectrumPlot::movePrimeThreshold(const QPoint &pos)
{
    double position = invTransform(QwtPlot::yLeft, pos.y());
    thresholdPr->setValue(0, position);
    replot();
}

void AmplitudeSpectrumPlot::moveSecondThreshold(const QPoint &pos)
{
    double position = invTransform(QwtPlot::yLeft, pos.y());
    thresholdSec->setValue(0, position);
    replot();
}

void AmplitudeSpectrumPlot::scrollLeftAxis(double value)
{
    setAxisScale(yLeft, value, value + 100.0);
    replot();
}
