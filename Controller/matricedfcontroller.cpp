#include "matricedfcontroller.hpp"

MatriceDFController::MatriceDFController(ModelInterface *model)
    : model(model)
{
    connect(this, SIGNAL(destroyed()), model, SLOT(quit()));

    MatriceDFView *view = new MatriceDFView(0, this, model);
    view->show();
    model->initializeModel();


    connect(model->getUDPChannel(), SIGNAL(samplesReceived(QVector<double>,QVector<double>,QVector<double>)),
            model, SLOT(samplesHandler(QVector<double>,QVector<double>,QVector<double>)));
    connect(model->getUDPChannel(), SIGNAL(samplesReceived(QVector<double>,QVector<double>,QVector<double>)),
            model, SLOT(polarSamplesHandler(QVector<double>,QVector<double>,QVector<double>)));

    //connect(model->getTCPChannel(), SIGNAL(telemetryReceived(mtelemetry::Telemetry)),
    //        view, SLOT(updateTelemetryData(mtelemetry::Telemetry)));
    connect(view, SIGNAL(headingChanged(int)), model, SLOT(updateHeading(int)));

    connect(model, SIGNAL(amplitudeSamplesReady(QVector<double>,QVector<double>)),
            view->getAmplitudeSpectrumPlot(), SLOT(updateCurve(QVector<double>,QVector<double>)));
    connect(model, SIGNAL(amplitudeSamplesReady(QVector<double>,QVector<double>)),
            view->getSpectrumWaterfall(), SLOT(updateSpectrogram(QVector<double>,QVector<double>)));
    connect(model, SIGNAL(phaseSamplesReady(QVector<double>)),
            view->getPhaseSpectrumPlot(), SLOT(updateCurve(QVector<double>)));
    connect(model, SIGNAL(polarSamplesReady(int,double,double)),
            view->getPolarPlot(), SLOT(updateDiagram(int,double,double)));
    connect(model, SIGNAL(polarSamplesReady(int,double,double,double)),
            view->getPolarPlot(), SLOT(updateDiagram(int,double,double,double)));

    connect(view->getAmplitudeSpectrumPlot()->getZoomer(), SIGNAL(zoomed(const QRectF &)),
            view->getPhaseSpectrumPlot(), SLOT(equalZoom(const QRectF &)));

    connect(view, SIGNAL(settingsReady(QVector<double>)), model->getUDPChannel(), SLOT(sendDatagram(QVector<double>)));

    model->moveToThread(model);
    model->start();

    sdk = new QtOsdk();
    connect(sdk,SIGNAL(throwSubscribeData(const QVector<double>)),
            view,SLOT(updateTelemetryData(const QVector<double>)));
}

void MatriceDFController::changeGainParameter(double gain)
{
    model->setCurrentGain(gain);
}

void MatriceDFController::changeAnalysisRange(QVector<int> range)
{
    model->setRangeBounds(range);
}

void MatriceDFController::enableCalibrationMode()
{
    model->setCalibrationMode(true);
}

void MatriceDFController::disableCalibrationMode()
{
    model->setCalibrationMode(false);
}

void MatriceDFController::setDoubleDrawingMode()
{
    model->setDrawingMode(true);
}

void MatriceDFController::setSummDrawingMode()
{
    model->setDrawingMode(false);
}
