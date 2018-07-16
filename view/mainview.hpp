#ifndef MAINVIEW_HPP
#define MAINVIEW_HPP

#include <QMainWindow>
#include <QAbstractButton>

#include <presenter/matricedfpresenter.hpp>
#include "mainiview.hpp"

#include <widgets/summarystrategy.hpp>
#include <widgets/twochannelstrategy.hpp>

#include <widgets/frequencyhoppingstrategy.hpp>
#include <widgets/spreadspectrumstrategy.hpp>
#include <widgets/densehoppingstrategy.hpp>

#include <QQuickView>
#include <QQuickItem>
#include <QQmlContext>
#include <QtPositioning>
#include <QVariant>

namespace Ui {
class MainView;
}

class QAction;
class QMenu;

class MainView : public QMainWindow, public MainIView
{
    Q_OBJECT

public:
    explicit MainView(QWidget *parent = 0);
    ~MainView();

    void ctrlDjiStatusChanged(QString);
    void initDjiVehicleFinished(QString initStatus, bool initResult);
    void activateDjiVehicleFinished(QString activateStatus, bool activateResult);
    void connectionDjiVehicleResetted();
    void updateTelemetryData(const QVector<double> &subscribeData);

    void correctPolarScales(const double &rad);

    Q_INVOKABLE void setHomePoint(QString azimuth);
    Q_INVOKABLE void setPointOnMap(QString lat, QString lng);

private slots:
    void on_btn_apply_clicked();

    void on_btn_amMode_clicked(bool checked);

    void on_btn_testInit_clicked();

    void on_btn_testActivate_clicked();

    void on_btn_testControl_clicked();

    void on_btn_testReset_clicked();

    void on_cb_droneClassSelect_activated(const QString &arg1);

    void on_bgr_markers_buttonClicked(QAbstractButton *button);
    void on_bgr_modes_buttonClicked(QAbstractButton *button);

    void on_btn_maxHold_clicked(bool checked);

    void on_sb_expCoeff_valueChanged(double arg1);

    void on_slider_add_valueChanged(int position);

    void on_slider_product_valueChanged(int position);

    void on_btn_resetScales_clicked();

    void on_btn_refresh_clicked();

    void switchMapProvider();
    void setAutoScaleMode();
    void makeDirection(const double &direction);
    void phaseCorrectionChanged(double phaseCorrection);

    void on_btn_clearMap_clicked();

    void on_btn_wp_init_clicked();

    void on_btn_wp_load_clicked();

    void on_btn_wp_start_clicked();

    void on_btn_wp_abort_clicked();

    void on_btn_runCommand_clicked();

    void on_btn_startYaw_clicked(bool checked);

    void on_sb_radTest_valueChanged(int arg1);

private:
    Ui::MainView        *ui;
    MatriceDFPresenter  *presenter;

    QQuickView          *map;

    QMenu   *mapMenu;
    QMenu   *settingsMenu;
    QAction *switchProviderAct;
    QAction *autoScaleAct;

    bool autoScaleEnabled;
    int slider_add_prev;
    double slider_product_prev;
    double markerLatitude, markerLongitude;

};

#endif // MAINVIEW_HPP
