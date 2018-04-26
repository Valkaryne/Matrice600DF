#include "amplitudespectrumplot.hpp"

AmplitudeSpectrumPlot::AmplitudeSpectrumPlot(QWidget *parent)
    : SpectrumPlot(parent),
      expCoeff(0.996)
{
    /* Axis */
    setAxisTitle(QwtPlot::yLeft, "Power");
    setAxisScale(QwtPlot::yLeft, 0 - 157, 140 - 157, 20); // WARNING: magic constants

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

    /* Other settings */
    for (int i = 0; i < 4096; i++)
        maxSamples.append(0);
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
