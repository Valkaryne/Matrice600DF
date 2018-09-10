#ifndef MATRICEDFPRESENTER_HPP
#define MATRICEDFPRESENTER_HPP

#include "../model/matricedfmodel.hpp"
#include "../view/mainiview.hpp"
#include "../dji_api/qtosdk.hpp"

#include <QHash>

class MatriceDFPresenter : public QObject
{
    Q_OBJECT
public:
    explicit MatriceDFPresenter(MainIView *view, QObject *parent = nullptr);
    ~MatriceDFPresenter();

    void applyUsrpSettings(QVector<double> &settings);

    void changeGainParameter(double gain);
    void changeBandParameter(int band);
    void changeBoundsParameters(QVector<int> bounds);
    void updateCurrentHeading(const int &heading);
    void changePhaseCorrection(const double &phaseCorrection);

    void initDjiVehicle();
    void activateDjiVehicle();
    void obtainDjiControl(QString ctrlOperation);
    void resetDjiConnection();

    void sendFlightRunCommandRequest(int &commandIndex);

    void sendSetAutoHorizontalVelocityRequest(int velocity);
    void sendSetAutoYawRateRequest(int rate);

    void sendAutoYawRequest(int direction);
    void sendAutoPitchRequest(int direction);
    void sendAutoRollRequest(int direction);

    void sendStableThrustRequest(int direction);
    void sendStableYawRequest(int direction);
    void sendStablePitchRequest(int direction);
    void sendStableRollRequest(int direction);

    void sendSlowThrustRequest(int direction);
    void sendSlowYawRequest(int direction);
    void sendSlowPitchRequest(int direction);
    void sendSlowRollRequest(int direction);

    void sendFastThrustRequest(int direction);
    void sendFastYawRequest(int direction);
    void sendFastPitchRequest(int direction);
    void sendFastRollRequest(int direction);

    void sendResetThrust();
    void sendResetYaw();
    void sendResetPitch();
    void sendResetRoll();

    void sendSetDefinedDirectionRequest(double &heading);

    void sendInitWaypointRequest(const QHash<QString, int> &settings);
    void sendLoadWaypointRequest(const QHash<QString, int> &settings);
    void sendStartWaypointRequest();
    void sendAbortWaypointRequest();

    void sendInitHotpointRequest(const QVector<double> &coordinates);
    void sendStartHotpointRequest(int yawRate);
    void sendStopHotpointRequest();

private slots:
    void amplitudeSamplesPresenter(const QVector<double> samplesAm1, const QVector<double> samplesAm2,
                                   const QVector<double> samplesAmS);
    void phaseSamplesPresenter(const QVector<double> samplesPh);
    void polarSamplesPresenter(const int az, const double rado, const double radl, const double rads,
                               const double phase);
    void phaseDeviationPresenter(const double phDev);

    void changeControlAuthorityStatus(QString textToDisplay);
    void changeInitButton(QString textToDisplay, bool success);
    void changeActivateButton(QString textToDisplay, bool success);
    void changeConnectionButtons();
    void receiveTelemetryData(const QVector<double> &subscribeData);

private:
    MainIView       *view;
    MatriceDFModel  *model;
    QtOsdk          *dji;
};

#endif // MATRICEDFPRESENTER_HPP
