#ifndef MATRICEDFPRESENTER_HPP
#define MATRICEDFPRESENTER_HPP

#include "../model/matricedfmodel.hpp"
#include "../view/mainiview.hpp"
#include "../dji_api/qtosdk.hpp"

class MatriceDFPresenter : public QObject
{
    Q_OBJECT
public:
    explicit MatriceDFPresenter(MainIView *view, QObject *parent = nullptr);
    ~MatriceDFPresenter();

    void applyUsrpSettings(QVector<double> &settings);

    void initDjiVehicle();
    void activateDjiVehicle();
    void obtainDjiControl(QString ctrlOperation);
    void resetDjiConnection();

private slots:
    void amplitudeSamplesPresenter(const QVector<double> samplesAm1, const QVector<double> samplesAm2,
                                   const QVector<double> samplesAmS);
    void phaseSamplesPresenter(const QVector<double> samplesPh);
    void polarSamplesPresenter(const int az, const double rado, const double radl, const double rads,
                               const double phase);

    void changeControlAuthorityStatus(QString textToDisplay);
    void changeInitButton(QString textToDisplay, bool success);
    void changeActivateButton(QString textToDisplay, bool success);

private:
    MainIView       *view;
    MatriceDFModel  *model;
    QtOsdk          *dji;
};

#endif // MATRICEDFPRESENTER_HPP
