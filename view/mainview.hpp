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

#include <QFileDialog>
#include <QMessageBox>

#include <QKeyEvent>

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

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    void ctrlDjiStatusChanged(QString);
    void initDjiVehicleFinished(QString initStatus, bool initResult);
    void activateDjiVehicleFinished(QString activateStatus, bool activateResult);
    void connectionDjiVehicleResetted();
    void updateTelemetryData(const QVector<double> &subscribeData);
    void displayPhaseDeviation(const double &phDev);

    //void correctPolarScales(const double &rad);

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
    void savePreset();
    void loadPreset();

    void makeDirection(const double &direction);
    void phaseCorrectionChanged(double phaseCorrection);

    void on_btn_clearMap_clicked();

    void on_btn_wp_init_clicked();

    void on_btn_wp_load_clicked();

    void on_btn_wp_start_clicked();

    void on_btn_wp_abort_clicked();

    void on_btn_runCommand_clicked();

    void on_btn_hp_start_clicked(bool checked);

private:
    void writeToFile(const QString &fileName);
    void readFromFile(const QString &fileName);

    QVector<int> getSettingsArray();
    QVector<int> getMapSettingsArray();
    void setSettingsArray(QVector<int> settings);
    void setMapSettingsArray(QVector<int> mapSettings);

private:
    Ui::MainView        *ui;
    MatriceDFPresenter  *presenter;

    QQuickView          *map;

    QMenu   *mapMenu;
    QMenu   *settingsMenu;
    QAction *switchProviderAct;
    QAction *savePresetAct;
    QAction *loadPresetAct;

    double markerLatitude, markerLongitude;
};

#endif // MAINVIEW_HPP
