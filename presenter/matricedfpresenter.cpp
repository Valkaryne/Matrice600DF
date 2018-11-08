#include "matricedfpresenter.hpp"

MatriceDFPresenter::MatriceDFPresenter(MainIView *view, QObject *parent) :
    QObject(parent)
{
    this->view = view;
    this->model = new MatriceDFModel();
    this->dji = new QtOsdk();

    model->moveToThread(model);
    model->start();

    connect(model, SIGNAL(amplitudeSamplesReady(const QVector<double> &, const QVector<double> &, const QVector<double> &)),
            SLOT(amplitudeSamplesPresenter(const QVector<double> &, const QVector<double> &, const QVector<double> &)));
    connect(model, SIGNAL(phaseSamplesReady(const QVector<double> &)),
            SLOT(phaseSamplesPresenter(const QVector<double> &)));
    connect(model, SIGNAL(polarSamplesReady(const int &, const double &, const double &, const double &, const double &)),
            SLOT(polarSamplesPresenter(const int &, const double &, const double &, const double &, const double &)));
    connect(model, SIGNAL(phaseDeviationEstimated(const double &)),
            SLOT(phaseDeviationPresenter(const double &)));

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



    //connect(this->view->getPolarPlot(), SIGNAL(polarPlotReady()), this, SLOT(testCat()));
}

MatriceDFPresenter::~MatriceDFPresenter()
{
}

void MatriceDFPresenter::testCat()
{
    qDebug() << "I'm a cat";
}

void MatriceDFPresenter::applyUsrpSettings(QVector<double> &settings)
{
    MatriceDFModel::getUdpChannel()->sendDatagram(settings);
}

void MatriceDFPresenter::changeGainParameter(double gain)
{
    model->setCurrentGain(gain);
}

void MatriceDFPresenter::changeBoundsParameters(QVector<int> &bounds)
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

void MatriceDFPresenter::sendSetAutoHorizontalVelocityRequest(int velocity)
{
    dji->setAutoHorizontalVelocityRequest(velocity);
}

void MatriceDFPresenter::sendSetAutoYawRateRequest(int rate)
{
    dji->setAutoYawRateRequest(rate);
}

void MatriceDFPresenter::sendAutoPitchRequest(int direction)
{
    dji->autoPitchRequest(direction);
}

void MatriceDFPresenter::sendAutoRollRequest(int direction)
{
    dji->autoRollRequest(direction);
}

void MatriceDFPresenter::sendAutoYawRequest(int direction)
{
    dji->autoYawRequest(direction);
}

void MatriceDFPresenter::sendStableThrustRequest(int direction)
{
    dji->stableThrustRequest(direction);
}

void MatriceDFPresenter::sendStableYawRequest(int direction)
{
    dji->stableYawRequest(direction);
}

void MatriceDFPresenter::sendStablePitchRequest(int direction)
{
    dji->stablePitchRequest(direction);
}

void MatriceDFPresenter::sendStableRollRequest(int direction)
{
    dji->stableRollRequest(direction);
}

void MatriceDFPresenter::sendSlowThrustRequest(int direction)
{
    dji->slowThrustRequest(direction);
}

void MatriceDFPresenter::sendSlowYawRequest(int direction)
{
    dji->slowYawRequest(direction);
}

void MatriceDFPresenter::sendSlowPitchRequest(int direction)
{
    dji->slowPitchRequest(direction);
}

void MatriceDFPresenter::sendSlowRollRequest(int direction)
{
    dji->slowRollRequest(direction);
}

void MatriceDFPresenter::sendFastThrustRequest(int direction)
{
    dji->fastThrustRequest(direction);
}

void MatriceDFPresenter::sendFastYawRequest(int direction)
{
    dji->fastYawRequest(direction);
}

void MatriceDFPresenter::sendFastPitchRequest(int direction)
{
    dji->fastPitchRequest(direction);
}

void MatriceDFPresenter::sendFastRollRequest(int direction)
{
    dji->fastRollRequest(direction);
}

void MatriceDFPresenter::sendResetThrust()
{
    dji->resetThrust();
}

void MatriceDFPresenter::sendResetYaw()
{
    dji->resetYaw();
}

void MatriceDFPresenter::sendResetPitch()
{
    dji->resetPitch();
}

void MatriceDFPresenter::sendResetRoll()
{
    dji->resetRoll();
}

void MatriceDFPresenter::sendSetDefinedDirectionRequest(double &heading)
{
    if (heading > 180)
        heading -= 360;

    dji->setDefinedDirectionRequest(heading);
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

void MatriceDFPresenter::sendInitHotpointRequest(const QVector<double> &coordinates)
{
    dji->initHotpointRequest(coordinates);
}

void MatriceDFPresenter::sendStartHotpointRequest()
{
    dji->startHotpointRequest();
}

void MatriceDFPresenter::sendStopHotpointRequest()
{
    dji->stopHotpointRequest();
}

/* Slots */
void MatriceDFPresenter::amplitudeSamplesPresenter(const QVector<double> &samplesAm1, const QVector<double> &samplesAm2,
                                              const QVector<double> &samplesAmS)
{
    view->getAmplitudeSpectrumPlot()->updateCurve(samplesAm1, samplesAm2, samplesAmS);
}

void MatriceDFPresenter::phaseSamplesPresenter(const QVector<double> &samplesPh)
{
    view->getPhaseSpectrumPlot()->updateCurve(samplesPh);
}

void MatriceDFPresenter::polarSamplesPresenter(const int &az, const double &rado, const double &radl, const double &rads,
                                          const double &phase)
{
    view->getPolarPlot()->updateDiagram(az,rado,radl,rads,phase);
    model->getUdpChannel()->setSignalAllowed();
}

void MatriceDFPresenter::phaseDeviationPresenter(const double &phDev)
{
    view->displayPhaseDeviation(phDev);
    //view->automaticPathFinder(phDev);
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
