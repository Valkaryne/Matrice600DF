#include "matricedfcontroller.hpp"

MatriceDFController::MatriceDFController(ModelInterface *model)
    : model(model)
{
    MatriceDFView *view = new MatriceDFView(0, this, model);
    view->show();
    model->initializeModel();
}

void MatriceDFController::makeConnections()
{
    connect(model->getUDPChannel(), SIGNAL(samplesReceived(QVector<double>,QVector<double>,QVector<double>)),
            model, SLOT(samplesHandler(QVector<double>,QVector<double>,QVector<double>)));
}

