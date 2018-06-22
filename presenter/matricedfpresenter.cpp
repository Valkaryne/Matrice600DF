#include "matricedfpresenter.hpp"

MatriceDFPresenter::MatriceDFPresenter(MainIView *view, QObject *parent) :
    QObject(parent)
{
    this->view = view;
    this->model = new MatriceDFModel();
    this->dji = new QtOsdk();

    connect(model,SIGNAL(amplitudeSamplesReady(const QVector<double>,const QVector<double>,const QVector<double>)),
            SLOT(amplitudeSamplesPresenter(const QVector<double>,const QVector<double>,const QVector<double>)));
    connect(model,SIGNAL(phaseSamplesReady(const QVector<double>)),
            SLOT(phaseSamplesPresenter(const QVector<double>)));
    connect(model,SIGNAL(polarSamplesReady(const int,const double,const double,const double,const double)),
            SLOT(polarSamplesPresenter(const int,const double,const double, const double,const double)));

    connect(dji,SIGNAL(changeControlAuthorityStatus(QString)),
            SLOT(changeControlAuthorityStatus(QString)));
    connect(dji,SIGNAL(changeInitButton(QString,bool)),
            SLOT(changeInitButton(QString,bool)));
    connect(dji,SIGNAL(changeActivateButton(QString,bool)),
            SLOT(changeActivateButton(QString,bool)));
}

MatriceDFPresenter::~MatriceDFPresenter()
{
}

void MatriceDFPresenter::applyUsrpSettings(QVector<double> &settings)
{
    model->getUdpChannel()->sendDatagram(settings);
}

void MatriceDFPresenter::initDjiVehicle()
{
    dji->initVehicle();
}

void MatriceDFPresenter::activateDjiVehicle()
{
    dji->activate();
}

void MatriceDFPresenter::obtainDjiControl(QString ctrlOperation)
{
    dji->obtainCtrl(ctrlOperation);
}

void MatriceDFPresenter::resetDjiConnection()
{
    dji->resetConnection();
}

/* Slots */
void MatriceDFPresenter::amplitudeSamplesPresenter(const QVector<double> samplesAm1, const QVector<double> samplesAm2,
                                              const QVector<double> samplesAmS)
{
    view->getAmplitudeSpectrumPlot()->updateCurve(samplesAm1, samplesAm2, samplesAmS);
}

void MatriceDFPresenter::phaseSamplesPresenter(const QVector<double> samplesPh)
{
    view->getPhaseSpectrumPlot()->updateCurve(samplesPh);
}

void MatriceDFPresenter::polarSamplesPresenter(const int az, const double rado, const double radl, const double rads,
                                          const double phase)
{

}

void MatriceDFPresenter::changeControlAuthorityStatus(QString textToDisplay)
{
    view->ctrlDjiStatusChanged(textToDisplay);
}

void MatriceDFPresenter::changeInitButton(QString textToDisplay, bool success)
{
    view->initDjiVehicleFinished(textToDisplay, success);
}

void MatriceDFPresenter::changeActivateButton(QString textToDisplay, bool success)
{
    view->activateDjiVehicleFinished(textToDisplay, success);
}
