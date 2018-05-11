#include "amplitudespectrumplot.hpp"

AmplitudeSpectrumPlot::AmplitudeSpectrumPlot(QWidget *parent)
    : SpectrumPlot(parent),
      expCoeff(0.996)
{
    /* Axis */
    setAxisTitle(QwtPlot::yLeft, "Power");
    setAxisScale(QwtPlot::yLeft, 0 - calibrationI, 300 - calibrationI, 60); // WARNING: magic constants

    /* Curves */
    curve1 = new QwtPlotCurve();
    curve1->setStyle(QwtPlotCurve::Lines);
    curve1->setPen(Qt::yellow);
    curve1->setRenderHint(QwtPlotItem::RenderAntialiased, true);
    curve1->setPaintAttribute(QwtPlotCurve::ClipPolygons, false);
    curve1->attach(this);

    curve2 = new QwtPlotCurve();
    curve2->setStyle(QwtPlotCurve::Lines);
    curve2->setPen(Qt::cyan);
    curve2->setRenderHint(QwtPlotItem::RenderAntialiased, true);
    curve2->setPaintAttribute(QwtPlotCurve::ClipPolygons, false);
    curve2->attach(this);

    curveMax = new QwtPlotCurve();
    curveMax->setStyle(QwtPlotCurve::NoCurve);
    curveMax->setSymbol(new QwtSymbol(QwtSymbol::Ellipse,
                                      QBrush(Qt::gray), QPen(Qt::gray), QSize(2, 2)));
    curveMax->setRenderHint(QwtPlotItem::RenderAntialiased, true);
    curveMax->setPaintAttribute(QwtPlotCurve::ClipPolygons, false);
    curveMax->attach(this);

    /* Pickers */
    pickerMarkPr = new QwtPlotPicker(
                QwtPlot::xBottom, QwtPlot::yLeft,
                QwtPlotPicker::VLineRubberBand,
                QwtPicker::AlwaysOn,
                canvas);
    pickerMarkPr->setRubberBandPen(QColor(Qt::red));
    pickerMarkPr->setTrackerPen(QColor(Qt::white));
    pickerMarkPr->setStateMachine(new QwtPickerDragPointMachine());
    pickerMarkPr->setMousePattern(QwtPlotPicker::MouseSelect1, Qt::LeftButton);

    pickerMarkSec = new QwtPlotPicker(
                QwtPlot::xBottom, QwtPlot::yLeft,
                QwtPlotPicker::VLineRubberBand,
                QwtPicker::AlwaysOn,
                canvas);
    pickerMarkSec->setRubberBandPen(QColor(Qt::red));
    pickerMarkSec->setTrackerPen(QColor(Qt::white));
    pickerMarkSec->setStateMachine(new QwtPickerDragPointMachine());
    pickerMarkSec->setMousePattern(QwtPlotPicker::MouseSelect1, Qt::RightButton);

    pickerThrPr = new QwtPlotPicker(
                QwtPlot::xBottom, QwtPlot::yLeft,
                QwtPlotPicker::HLineRubberBand,
                QwtPicker::AlwaysOn,
                canvas);
    pickerThrPr->setRubberBandPen(QColor(Qt::green));
    pickerThrPr->setTrackerPen(QColor(Qt::white));
    pickerThrPr->setStateMachine(new QwtPickerDragPointMachine());
    pickerThrPr->setMousePattern(QwtPlotPicker::MouseSelect1, Qt::LeftButton);

    pickerThrSec = new QwtPlotPicker(
                QwtPlot::xBottom, QwtPlot::yLeft,
                QwtPlotPicker::HLineRubberBand,
                QwtPicker::AlwaysOn,
                canvas);
    pickerThrSec->setRubberBandPen(QColor(Qt::green));
    pickerThrSec->setTrackerPen(QColor(Qt::white));
    pickerThrSec->setStateMachine(new QwtPickerDragPointMachine());
    pickerThrSec->setMousePattern(QwtPlotPicker::MouseSelect1, Qt::RightButton);

    /* Thresholds */
    thresholdPr = new QwtPlotMarker();
    thresholdPr->setLineStyle(QwtPlotMarker::HLine);
    thresholdPr->setLinePen(QColor(Qt::white), 2, Qt::DashLine);
    thresholdPr->setValue(0, 0 - calibrationI); // WARNING: magic constant
    thresholdPr->attach(this);

    thresholdSec = new QwtPlotMarker();
    thresholdSec->setLineStyle(QwtPlotMarker::HLine);
    thresholdSec->setLinePen(QColor(Qt::white), 2, Qt::DashLine);
    thresholdSec->setValue(0, 300 - calibrationI); // WARNING: magic constant
    thresholdSec->attach(this);

    /* Colors */
    colors = {*new QColor(220, 20, 75), /* crimson */
             *new QColor(11, 218, 81), /* malachite */
             *new QColor(127,199,255), /* skiey */
             *new QColor(255, 153, 0), /* orange */
             *new QColor(255,170,204), /* rose */
             };

    /* Zoomer */
    zoomer = new QwtPlotZoomer(canvas);
    zoomer->setRubberBandPen(QColor(Qt::darkGreen));
    zoomer->setTrackerMode(QwtPlotPicker::AlwaysOn);
    zoomer->setMousePattern(QwtEventPattern::MouseSelect2,
                            Qt::RightButton, Qt::ControlModifier);

    /* Other settings */
    for (int i = 0; i < 4096; i++)
        maxSamples.append(0);

    for (int i = 1; i <= 5; i++)
        setMarker(i);

    this->markPairNmr = 1;

    connect(pickerMarkPr, SIGNAL(appended(const QPoint &)),
            this, SLOT(movePrimeMarker(const QPoint &)));
    connect(pickerMarkSec, SIGNAL(appended(const QPoint &)),
            this, SLOT(moveSecondMarker(const QPoint &)));
    connect(pickerThrPr, SIGNAL(appended(const QPoint &)),
            this, SLOT(movePrimeThreshold(const QPoint &)));
    connect(pickerThrSec, SIGNAL(appended(const QPoint &)),
            this, SLOT(moveSecondThreshold(const QPoint &)));
}

void AmplitudeSpectrumPlot::setMarker(int number)
{
    if (markerVector.size() < number * 2) {
        QwtPlotMarker *markerPr = new QwtPlotMarker();
        markerPr->setLineStyle(QwtPlotMarker::VLine);
        markerPr->setLinePen(colors.at((number - 1) % colors.size()), 2, Qt::SolidLine);
        markerPr->setValue(cntrFrequency - 15.45, 0); // WARNING: magic constant
        markerPr->attach(this);

        QwtPlotMarker *markerSec = new QwtPlotMarker();
        markerSec->setLineStyle(QwtPlotMarker::VLine);
        markerSec->setLinePen(colors.at((number - 1) % colors.size()), 2, Qt::SolidLine);
        markerSec->setValue(cntrFrequency + 15.27, 0); // WARNING: magic constant
        markerSec->attach(this);

        markerVector.append(markerPr);
        markerVector.append(markerSec);
    }
    markPairNmr = number * 2 - 1;
}

void AmplitudeSpectrumPlot::setPickers(bool enable)
{
    pickerMarkPr->setEnabled(enable);
    pickerMarkSec->setEnabled(enable);
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
        this->cntrFrequency = cntrFrequency;
        resetMarkers();
    }
    double xleft = cntrFrequency - LSHIFT;
    double xright = cntrFrequency + RSHIFT;
    setAxisScale(QwtPlot::xBottom, xleft, xright);
    setZoomBase(xleft, xright);
}

void AmplitudeSpectrumPlot::setZoomBase(double xleft, double xright)
{
    QStack<QRectF> stack = zoomer->zoomStack();
    QRectF base;
    base = QRectF(QPointF(xleft, 20 - calibrationI), QPointF(xright, 200 - calibrationI));
    stack.clear();
    stack.append(base);
    zoomer->setZoomStack(stack);
    zoomer->setZoomBase(base);
}

void AmplitudeSpectrumPlot::setMaxHold(bool holdOn)
{
    if (holdOn)
        curveMax->attach(this);
    else
        curveMax->detach();
}

void AmplitudeSpectrumPlot::setExpCoefficient(double expCoeff)
{
    if (this->expCoeff != expCoeff)
        this->expCoeff = expCoeff;
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
        bounds.append(2060 + (bound - cntrFrequency) / INCR); // WARNING: magic constant
        bound = markerVector.at(i)->xValue();
        bounds.append(2060 + (bound - cntrFrequency) / INCR); // WARNING: magic constant
        qSort(bounds.end() - 2, bounds.end());
        if ((i > 1) && (bounds.at(i - 1) < bounds.at(i - 3)) && (bounds.at(i) > bounds.at(i-2))) {
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
//    qDebug() << (int)(2060 + (markerVector.at(0)->xValue() - cntrFrequency) / INCR);
//    qDebug() << (int)(2060 + (markerVector.at(1)->xValue() - cntrFrequency) / INCR);
    if ((int)((2060 + (markerVector.at(0)->xValue() - cntrFrequency) / INCR)) == 0 && (int)((2060 + (markerVector.at(1)->xValue() - cntrFrequency) / INCR)) == 4095)
        bounds.append(10000);
    else
        bounds.append(thresholdSec->yValue());
    qSort(bounds);
    return bounds;
}

void AmplitudeSpectrumPlot::resetMarkers()
{
    for (int i = 1; i < markerVector.size(); i += 2) {
        markerVector.at(i - 1)->setValue(cntrFrequency - LSHIFT, 0);
        markerVector.at(i)->setValue(cntrFrequency + RSHIFT, 0);
    }
}

void AmplitudeSpectrumPlot::movePrimeMarker(const QPoint &pos)
{
    double position = invTransform(QwtPlot::xBottom, pos.x());
    markerVector.at(markPairNmr - 1)->setValue(position, 0);
}

void AmplitudeSpectrumPlot::moveSecondMarker(const QPoint &pos)
{
    double position = invTransform(QwtPlot::xBottom, pos.x());
    markerVector.at(markPairNmr)->setValue(position, 0);
}

void AmplitudeSpectrumPlot::movePrimeThreshold(const QPoint &pos)
{
    double position = invTransform(QwtPlot::yLeft, pos.y());
    thresholdPr->setValue(0, position);
}

void AmplitudeSpectrumPlot::moveSecondThreshold(const QPoint &pos)
{
    double position = invTransform(QwtPlot::yLeft, pos.y());
    thresholdSec->setValue(0, position);
}

void AmplitudeSpectrumPlot::updateCurve(const QVector<double> &samplesAm1, const QVector<double> &samplesAm2)
{
    int size = samplesAm1.size();
    QVector<double> frequency;

    for (double i = cntrFrequency - LSHIFT; i < size * INCR + (cntrFrequency - LSHIFT); i += INCR)
        frequency.append(i);

    for (int j = 0; j < samplesAm1.size(); j++) {
        if (samplesAm1.at(j) > maxSamples.at(j))
            maxSamples.replace(j, expCoeff * samplesAm1.at(j) + (1 - expCoeff) * maxSamples.at(j) + 5);
        else
            maxSamples.replace(j, (1 - expCoeff) * samplesAm1.at(j) + expCoeff * maxSamples.at(j));
    }

    curve1->setSamples(frequency, samplesAm1);
    curve2->setSamples(frequency, samplesAm2);
    curveMax->setSamples(frequency, maxSamples);

    replot();
}
