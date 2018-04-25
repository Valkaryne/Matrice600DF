#include "spectrumwaterfall.hpp"

class ColorMap : public QwtLinearColorMap
{
public:
    ColorMap():
        QwtLinearColorMap(Qt::blue, Qt::red)
    {
        addColorStop(0.5, Qt::green);
    }
};

SpectrumWaterfall::SpectrumWaterfall(QWidget *parent) :
    QwtPlot(parent)
{
    /* Canvas */
    QwtPlotCanvas *canvas = new QwtPlotCanvas;
    canvas->setBorderRadius(10);
    setCanvas(canvas);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    /* Axis */
    enableAxis(QwtPlot::yLeft, false);
    setAxisAutoScale(QwtPlot::yLeft, true);
    setAxisAutoScale(QwtPlot::xBottom, true);
    setAxisScale(QwtPlot::yLeft, 100 - 157, 0 - 157, 20); //WARNING: magic constants
    setAxisScale(QwtPlot::xBottom, 54.55, 85.27, 3); // WARNING: magic constants

    /* Plot */
    plot = new QwtPlotSpectrogram();
    plot->setRenderThreadCount(0);
    plot->setColorMap(new ColorMap());

    /* Plot Data */
    plotData = new QwtMatrixRasterData();
    plotData->setResampleMode(QwtMatrixRasterData::ResampleMode::BilinearInterpolation);
    plotData->setInterval(Qt::XAxis, QwtInterval(0, 4095, QwtInterval::ExcludeMaximum)); // WARNING: magic constants
    plotData->setInterval( Qt::YAxis, QwtInterval(0 - 157, 100 - 157, QwtInterval::ExcludeMaximum)); // WARNING: magic constants
    plotData->setInterval(Qt::ZAxis, QwtInterval(0 - 157, 100 - 157)); // WARNING: magic constants

    const QwtInterval interval(0 - 157, 100 - 157); // WARNING: magic constants

    /* Color bar */
    QwtScaleWidget *leftAxis = axisWidget(QwtPlot::yLeft);
    leftAxis->setTitle("Power");
    leftAxis->setColorBarEnabled(true);

    setAxisScale(QwtPlot::yLeft, 0 - 157, 100 - 157); // WARNING: magic constants
    enableAxis(QwtPlot::yLeft);

    leftAxis->setColorMap(interval, new ColorMap());

    vec = new QVector<double>(102400);

    for (int i = 0; i < 102400; i++)
        vec->append(interval.minValue());

    plotData->setValueMatrix(*vec, 1024);
    plot->setData(plotData);
    plot->attach(this);
}
