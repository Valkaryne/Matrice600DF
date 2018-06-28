#ifndef MAINVIEW_HPP
#define MAINVIEW_HPP

#include <QMainWindow>

#include <presenter/matricedfpresenter.hpp>
#include "mainiview.hpp"

#include <widgets/summarystrategy.hpp>
#include <widgets/twochannelstrategy.hpp>
#include <widgets/frequencyhoppingstrategy.hpp>
#include <widgets/spreadspectrumstrategy.hpp>

namespace Ui {
class MainView;
}

class MainView : public QMainWindow, public MainIView
{
    Q_OBJECT

public:
    explicit MainView(QWidget *parent = 0);
    ~MainView();

    void ctrlDjiStatusChanged(QString);
    void initDjiVehicleFinished(QString initStatus, bool initResult);
    void activateDjiVehicleFinished(QString activateStatus, bool activateResult);

private slots:
    void on_btn_apply_clicked();

    void on_btn_amMode_clicked(bool checked);

    void on_btn_testInit_clicked();

    void on_btn_testActivate_clicked();

    void on_btn_testControl_clicked();

    void on_btn_testReset_clicked();

    void on_cb_droneClassSelect_activated(const QString &arg1);

private:
    Ui::MainView        *ui;
    MatriceDFPresenter  *presenter;

};

#endif // MAINVIEW_HPP
