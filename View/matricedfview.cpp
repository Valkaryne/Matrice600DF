#include <QLayout>

#include "matricedfview.hpp"
#include "ui_matricedfview.h"

MatriceDFView::MatriceDFView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MatriceDFView)
{
    ui->setupUi(this);

    /* Create spectrum plot and waterfall objects */
    QHBoxLayout *spLayout = new QHBoxLayout();
    QHBoxLayout *phLayout = new QHBoxLayout();

    spectrumWaterfall = new SpectrumWaterfall(this);
    ui->analysisLayout->addWidget(spectrumWaterfall);

    amplitudeSpectrumPlot = new AmplitudeSpectrumPlot(this);
    ui->analysisLayout->addSpacing(10);
    spLayout->addSpacing(12);
    spLayout->addWidget(amplitudeSpectrumPlot);
    ui->analysisLayout->addLayout(spLayout);

    phaseSpectrumPlot = new PhaseSpectrumPlot(this);
    phLayout->addSpacing(12);
    phLayout->addWidget(phaseSpectrumPlot);
    ui->analysisLayout->addSpacing(10);
    ui->analysisLayout->addLayout(phLayout);

    polarPlot = new PolarPlot(this);
    ui->polarLayout->addWidget(polarPlot);
}

MatriceDFView::~MatriceDFView()
{
    delete ui;
}
