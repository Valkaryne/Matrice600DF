#include "mainview.hpp"
#include "ui_mainview.h"

MainView::MainView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainView)
{
    ui->setupUi(this);
    presenter = new MatriceDFPresenter(this);

    amplitudeSpectrumPlot = new AmplitudeSpectrumPlot(this);
    getAmplitudeSpectrumPlot()->setDisplayStrategy(new TwoChannelStrategy(getAmplitudeSpectrumPlot()));
    getAmplitudeSpectrumPlot()->setMarkerStrategy(new FrequencyHoppingStrategy(getAmplitudeSpectrumPlot()));
    ui->spectrumPlotLayout->addWidget(amplitudeSpectrumPlot);

    phaseSpectrumPlot = new PhaseSpectrumPlot(this);
    ui->spectrumPlotLayout->addWidget(phaseSpectrumPlot);
}

MainView::~MainView()
{
    delete ui;
}

void MainView::on_btn_apply_clicked()
{
    QVector<double> settings;
    settings.append(ui->sb_frequency->value() * 1000000.0);
    settings.append(ui->sb_gain->value());
    settings.append(140); // Temp value: product
    settings.append(50); // temp value: add

    presenter->applyUsrpSettings(settings);
}

void MainView::on_btn_amMode_clicked(bool checked)
{
    AmplitudeSpectrumPlot *plot = getAmplitudeSpectrumPlot();
    if (checked){
        plot->setDisplayStrategy(new SummaryStrategy(plot));
        ui->btn_amMode->setText("Two-Channel");
    } else {
        plot->setDisplayStrategy(new TwoChannelStrategy(plot));
        ui->btn_amMode->setText("Summary");
    }
}

void MainView::on_btn_testInit_clicked()
{
    presenter->initDjiVehicle();
}

void MainView::on_btn_testActivate_clicked()
{
    presenter->activateDjiVehicle();
}

void MainView::on_btn_testControl_clicked()
{
    presenter->obtainDjiControl(ui->btn_testControl->text());
}

void MainView::on_btn_testReset_clicked()
{
    presenter->resetDjiConnection();
}

void MainView::ctrlDjiStatusChanged(QString valueToDisplay)
{
    qDebug() << valueToDisplay;
    if (valueToDisplay == "Obtained Control")
        ui->btn_testControl->setText("Release Control");
    else if (valueToDisplay == "Released Control")
        ui->btn_testControl->setText("Obtain Control");
}

void MainView::initDjiVehicleFinished(QString initStatus, bool initResult)
{
    if (initResult) {
        ui->btn_testInit->setStyleSheet("background-color: qlineargradient(x1: 0, "
                                        "y1: 0, x2: 0, y2: 1, stop: 0 #44a8f2, "
                                        "stop: 1 #44a8f2); color: white");
        ui->btn_testInit->setText(initStatus);
    } else {
        ui->btn_testInit->setStyleSheet("background-color: qlineargradient(x1: 0, "
                                        "y1: 0, x2: 0, y2: 1, stop: 0 red, stop: 1 "
                                        "red); color: white");
        ui->btn_testInit->setText(initStatus);
    }
}

void MainView::activateDjiVehicleFinished(QString activateStatus, bool activateResult)
{
    if (!activateResult) {
        ui->btn_testActivate->setText(activateStatus);
        ui->btn_testActivate->setStyleSheet("background-color: qlineargradient(x1: "
                                            "0, y1: 0, x2: 0, y2: 1, stop: 0 "
                                            "#B4B4B4, stop: 1 #B4B4B4); color: white");
    } else {
        ui->btn_testActivate->setText(activateStatus);
        ui->btn_testActivate->setStyleSheet("background-color: qlineargradient(x1: 0, "
                                            "y1: 0, x2: 0, y2: 1, stop: 0 #44a8f2, "
                                            "stop: 1 #44a8f2); color: white");
        // display versions
        // init components
    }
}

void MainView::on_cb_droneClassSelect_activated(const QString &arg1)
{
    AmplitudeSpectrumPlot *plot = getAmplitudeSpectrumPlot();
    if (arg1 == "Matrice/Phantom")
        plot->setMarkerStrategy(new FrequencyHoppingStrategy(plot));
    else if (arg1 == "Mavic/Spark")
        plot->setMarkerStrategy(new SpreadSpectrumStrategy(plot));
}
