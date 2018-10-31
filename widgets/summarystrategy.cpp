#include "summarystrategy.hpp"

SummaryStrategy::SummaryStrategy(AmplitudeSpectrumPlot *plot)
{
    this->plot = plot;
    this->expCoeff = 0.999;
    plot->curveAm1->detach();
    plot->curveAm2->detach();
    plot->curveAmS->attach(plot);
    plot->replot();
}

SummaryStrategy::SummaryStrategy(PolarPlot *polarPlot)
{
    this->polarPlot = polarPlot;
    polarPlot->curveAm1->detach();
    polarPlot->curveAm2->detach();
    polarPlot->curveAmS->attach(polarPlot);
    polarPlot->replot();
}

void SummaryStrategy::update(const QVector<double> &samplesAm1, const QVector<double> &samplesAm2,
                             const QVector<double> &samplesAmS, const int &number)
{
    Q_UNUSED(samplesAm1)
    Q_UNUSED(samplesAm2)
    int size = plot->samplesAmS.size();
    QVector<double> frequency;
    double cntrFrequency = plot->getCentralFrequency();

    for (double i = cntrFrequency - LSHIFT; i < size * INCR + (cntrFrequency - LSHIFT); i += INCR)
        frequency.append(i);

    for (int i = 256 * (number - 1), j = 0; i < 256 * number; i++, j++)
    {
        plot->samplesAmS.replace(i, samplesAmS.at(j));
    }

    for (int j = 0; j < plot->samplesAmS.size(); j++) {
        if (plot->samplesAmS.at(j) > plot->maxSamples.at(j))
            plot->maxSamples.replace(j, expCoeff * plot->samplesAmS.at(j) + (1 - expCoeff) * plot->maxSamples.at(j) + 5);
        else
            plot->maxSamples.replace(j, (1 - expCoeff) * plot->samplesAmS.at(j) + expCoeff * plot->maxSamples.at(j));
    }

    if (number != -1)
    {
        plot->curveAmS->setSamples(frequency, plot->samplesAmS);
        plot->curveMax->setSamples(frequency, plot->maxSamples);
        plot->replot();
    }
}

void SummaryStrategy::update(const int &azHeading, const double &radAm1, const double &radAm2,
                             const double &radAmS, const double &radPh)
{
    Q_UNUSED(radAm1)
    Q_UNUSED(radAm2)
    PolarCurveData *dataAmS = (PolarCurveData*)(polarPlot->curveAmS->data());
    dataAmS->append(QwtPointPolar(azHeading, radAmS));
    PolarCurveData *dataPh = (PolarCurveData*)(polarPlot->curvePh->data());
    dataPh->append(QwtPointPolar(azHeading, radPh));
    polarPlot->replot();
}

double SummaryStrategy::getPowerMaximum()
{
    double maximumAz = 0;
    PolarCurveData *data = (PolarCurveData*)(polarPlot->curveAmS->data());
    maximumAz = data->getPowerMaximum();
    qDebug() << maximumAz;
    return maximumAz;
}
