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
    QString displayStrategy = ui->btn_amMode->text();
    if (checked){
        getAmplitudeSpectrumPlot()->setDisplayStrategy(new SummaryStrategy(getAmplitudeSpectrumPlot()));
        ui->btn_amMode->setText("Two-Channel");
    } else {
        getAmplitudeSpectrumPlot()->setDisplayStrategy(new TwoChannelStrategy(getAmplitudeSpectrumPlot()));
        ui->btn_amMode->setText("Summary");
    }
}
