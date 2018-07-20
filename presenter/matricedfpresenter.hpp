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
