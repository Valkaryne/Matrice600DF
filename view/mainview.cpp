#include "mainview.hpp"
#include "ui_mainview.h"

MainView::MainView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainView)
{
    ui->setupUi(this);
    presenter = new MatriceDFPresenter(this);

    amplitudeSpectrumPlot = new AmplitudeSpectrumPlot(this);
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

    presenter->applyUsrpSettings(settings);
}
