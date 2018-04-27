#include "matricedfcontroller.hpp"

MatriceDFController::MatriceDFController(ModelInterface *model)
    : model(model)
{
    MatriceDFView *view = new MatriceDFView(0, this, model);
    view->show();
    //view = new MatriceDFView(0, this, model);
    model->initializeModel();
    makeConnections();
    connect(model, SIGNAL(amplitudeSamplesReady(QVector<double>,QVector<double>)),
            view->getAmplitudeSpectrumPlot(), SLOT(updateCurve(QVector<double>,QVector<double>)));
    connect(model, SIGNAL(phaseSamplesReady(QVector<double>)),
            view->getPhaseSpectrumPlot(), SLOT(updateCurve(QVector<double>)));
}

void MatriceDFController::makeConnections()
{
    connect(model->getUDPChannel(), SIGNAL(samplesReceived(QVector<double>,QVector<double>,QVector<double>)),
            model, SLOT(samplesHandler(QVector<double>,QVector<double>,QVector<double>)));
}

