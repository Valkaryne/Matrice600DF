#include "amplitudespectrumplot.hpp"

AmplitudeSpectrumPlot::AmplitudeSpectrumPlot(QWidget *parent) :
    SpectrumPlot(parent),
    droneClass(0),
    expCoeff(0.999)
{
    /* Axis */
    setAxisTitle(QwtPlot::yLeft, "Power");
    setAxisScale(QwtPlot::yLeft, 40 - CALIBRATION, 120 - CALIBRATION, 20);

    /* Curves */
    curve1 = new QwtPlotCurve;
    curve1->setStyle(QwtPlotCurve::Lines);
    curve1->setPen(Qt::yellow);
    curve1->setRenderHint(QwtPlotItem::RenderAntialiased, true);
    curve1->setPaintAttribute(QwtPlotCurve::ClipPolygons, false);
    curve1->attach(this);

    curve2 = new QwtPlotCurve;
    curve2->setStyle(QwtPlotCurve::Lines);
    curve2->setPen(Qt::cyan);
    curve2->setRenderHint(QwtPlotItem::RenderAntialiased, true);
    curve2->setPaintAttribute(QwtPlotCurve::ClipPolygons, false);
    curve2->attach(this);

    curveMax = new QwtPlotCurve;
    curveMax->setStyle(QwtPlotCurve::NoCurve);
    curveMax->setSymbol(new QwtSymbol(QwtSymbol::Ellipse,
                                      QBrush(Qt::gray), QPen(Qt::gray), QSize(2, 2)));
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
    markerPicker->setStateMachine(new QwtPickerDragPointMachine);
    markerPicker->setMousePattern(QwtPlotPicker::MouseSelect1, Qt::LeftButton);

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

    connect(markerPicker,SIGNAL(appended(const QPoint &)),
            SLOT(moveMarkers(const QPoint &)));
    connect(pickerThrPr,SIGNAL(appended(const QPoint &)),
            SLOT(movePrimeThreshold(const QPoint &)));
    connect(pickerThrSec,SIGNAL(appended(const QPoint &)),
            SLOT(moveSecondThreshold(const QPoint &)));
}

void AmplitudeSpectrumPlot::setMarker(int number)
{

}

void AmplitudeSpectrumPlot::setPickers(bool enable)
{

}

void AmplitudeSpectrumPlot::setThresholdPickers(bool enable)
{

}

void AmplitudeSpectrumPlot::setZoomer(bool enable)
{

}

void AmplitudeSpectrumPlot::setCentralFrequency(double cntrFrequency)
{

}

void AmplitudeSpectrumPlot::setZoomBase(double xleft, double xright)
{

}

void AmplitudeSpectrumPlot::setMaxHold(bool holdOn)
{

}

void AmplitudeSpectrumPlot::setDroneClass(int droneClass)
{

}

void AmplitudeSpectrumPlot::setExpCoefficient(double expCoeff)
{

}

QwtPlotZoomer* AmplitudeSpectrumPlot::getZoomer()
{
    return zoomer;
}

QVector<int> AmplitudeSpectrumPlot::getMarkerBounds()
{
    QVector<int> bounds;
    return bounds;
}

QVector<int> AmplitudeSpectrumPlot::getThresholdBounds()
{
    QVector<int> bounds;
    return bounds;
}

void AmplitudeSpectrumPlot::resetMarkers()
{

}

void AmplitudeSpectrumPlot::updateCurve(const QVector<double> &samplesAm1, const QVector<double> &samplesAm2,
                                        const QVector<double> &samplesAmS)
{
    int size = samplesAm1.size();
    QVector<double> frequency;

    for (double i = cntrFrequency - LSHIFT; i < size * INCR + (cntrFrequency - LSHIFT); i += INCR)
        frequency.append(i);

    curve1->setSamples(frequency, samplesAm1);
    curve2->setSamples(frequency, samplesAm2);

    replot();
}

void AmplitudeSpectrumPlot::moveMarkers(const QPoint &pos)
{

}

void AmplitudeSpectrumPlot::movePrimeThreshold(const QPoint &pos)
{

}

void AmplitudeSpectrumPlot::moveSecondThreshold(const QPoint &pos)
{

}

void AmplitudeSpectrumPlot::scrollLeftAxis(double value)
{
    setAxisScale(yLeft, value, value + 100.0);
    replot();
}
