#include "matricedfcontroller.hpp"

MatriceDFController::MatriceDFController(ModelInterface *model)
    : model(model)
{
    MatriceDFView *view = new MatriceDFView(0, this, model);
    view->show();
    model->initializeModel();

    connect(model->getTCPChannel(), SIGNAL(telemetryReceived(mtelemetry::Telemetry)),
            view, SLOT(updateTelemetryData(mtelemetry::Telemetry)));
}

void MatriceDFController::makeConnections()
{
    connect(model->getUDPChannel(), SIGNAL(samplesReceived(QVector<double>,QVector<double>,QVector<double>)),
            model, SLOT(samplesHandler(QVector<double>,QVector<double>,QVector<double>)));
}

