#include "matricedfpresenter.hpp"

MatriceDFPresenter::MatriceDFPresenter(MainIView *view, QObject *parent) :
    QObject(parent)
{
    this->view = view;
    this->model = new MatriceDFModel();
    this->dji = new QtOsdk();

    model->moveToThread(model);
    model->start();

    //dji->moveToThread((QThread*)this);

    connect(model, SIGNAL(amplitudeSamplesReady(const QVector<double>,const QVector<double>,const QVector<double>)),
            SLOT(amplitudeSamplesPresenter(const QVector<double>,const QVector<double>,const QVector<double>)));
    connect(model, SIGNAL(phaseSamplesReady(const QVector<double>)),
            SLOT(phaseSamplesPresenter(const QVector<double>)));
    connect(model, SIGNAL(polarSamplesReady(const int,const double,const double,const double,const double)),
            SLOT(polarSamplesPresenter(const int,const double,const double, const double,const double)));
    connect(model, SIGNAL(polarScalesCorrector(const double &)),
            SLOT(polarScalesCorrectionPresenter(const double &)));

    connect(dji, SIGNAL(changeControlAuthorityStatus(QString)),
            SLOT(changeControlAuthorityStatus(QString)));
    connect(dji, SIGNAL(changeInitButton(QString,bool)),
            SLOT(changeInitButton(QString,bool)));
    connect(dji, SIGNAL(changeActivateButton(QString,bool)),
            SLOT(changeActivateButton(QString,bool)));
    connect(dji, SIGNAL(changeConnectionButtons()),
            SLOT(changeConnectionButtons()));
    connect(dji, SIGNAL(receiveTelemetryData(const QVector<double> &)),
            SLOT(receiveTelemetryData(const QVector<double> &)));

    connect(this, SIGNAL(startRotationRequest(int)), dji, SLOT(startRotationRequest(int)));
    connect(this, SIGNAL(stopRotationRequest()), dji, SIGNAL(stopRotationRequest()));
}

MatriceDFPresenter::~MatriceDFPresenter()
{
}

void MatriceDFPresenter::applyUsrpSettings(QVector<double> &settings)
{
    model->getUdpChannel()->sendDatagram(settings);
}

void MatriceDFPresenter::changeGainParameter(double gain)
{
    model->setCurrentGain(gain);
}

void MatriceDFPresenter::changeBandParameter(int band)
{
    model->setBand(band);
}

void MatriceDFPresenter::changeBoundsParameters(QVector<int> bounds)
{
    model->setRangeBounds(bounds);
}

void MatriceDFPresenter::changePhaseCorrection(const double &phaseCorrection)
{
    model->setPhaseCorrection(phaseCorrection);
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

void MatriceDFPresenter::sendFlightRunCommandRequest(int &commandIndex)
{
    dji->flightRunCommandRequest(commandIndex);
}

void MatriceDFPresenter::sendStartRotationRequest(int yawRate)
{
    emit startRotationRequest(yawRate);
    //dji->startRotationRequest(yawRate);
}

void MatriceDFPresenter::sendStopRotationRequest()
{
    emit stopRotationRequest();
    //dji->stopRotationRequest();
}

void MatriceDFPresenter::sendInitWaypointRequest(const QHash<QString, int> &settings)
{
    dji->initWaypointRequest(settings);
}

void MatriceDFPresenter::sendLoadWaypointRequest(const QHash<QString, int> &settings)
{
    dji->loadWaypointRequest(settings);
}

void MatriceDFPresenter::sendStartWaypointRequest()
{
    dji->startWaypointRequest();
}

void MatriceDFPresenter::sendAbortWaypointRequest()
{
    dji->abortWaypointRequest();
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
    view->getPolarPlot()->updateDiagram(az,rado,radl,rads,phase);
}

void MatriceDFPresenter::polarScalesCorrectionPresenter(const double &rad)
{
    view->correctPolarScales(rad);
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

void MatriceDFPresenter::changeConnectionButtons()
{
    view->connectionDjiVehicleResetted();
}

void MatriceDFPresenter::receiveTelemetryData(const QVector<double> &subscribeData)
{
    view->updateTelemetryData(subscribeData);
}

void MatriceDFPresenter::updateCurrentHeading(const int &heading)
{
    model->updateCurrentHeading(heading);
}
