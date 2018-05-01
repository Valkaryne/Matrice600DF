#include <QLayout>

#include "matricedfview.hpp"
#include "ui_matricedfview.h"

MatriceDFView::MatriceDFView(QWidget *parent, ControllerInterface *controller, ModelInterface *model) :
    QMainWindow(parent),
    controller(controller),
    model(model),
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

    // FIXIT: Create composite object from webview
    webview = new QWebEngineView();
    ui->mapLayout->addWidget(webview);

    QWebChannel *webChannel = new QWebChannel();
    webChannel->registerObject("demoWindow", this);
    webview->page()->setWebChannel(webChannel);
    webview->page()->load(QUrl("qrc:/map.html"));
}

MatriceDFView::~MatriceDFView()
{
    delete ui;
}

void MatriceDFView::on_decrAngleButton_clicked()
{
    webview->page()->runJavaScript("decrAngle();");
}

void MatriceDFView::on_incrAngleButton_clicked()
{
    webview->page()->runJavaScript("incrAngle();");
}

void MatriceDFView::on_makeDirButton_clicked()
{
    webview->page()->runJavaScript("makeBeam();");
}
