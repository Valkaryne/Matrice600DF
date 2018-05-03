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

    // FIXME: Create composite object from webview
    webview = new QWebEngineView();
    ui->mapLayout->addWidget(webview);

    QWebChannel *webChannel = new QWebChannel();
    webChannel->registerObject("demoWindow", this);
    webview->page()->setWebChannel(webChannel);
    webview->page()->load(QUrl("qrc:/map.html"));
    webview->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

MatriceDFView::~MatriceDFView()
{
    delete ui;
}

void MatriceDFView::updateTelemetryData(const mtelemetry::Telemetry &telemetry)
{
    float latitude = telemetry.latitude();
    float longitude = telemetry.longitude();
    float altitude = telemetry.altitude();
    float heading = telemetry.heading();

    ui->mapLatitudeLine->setText(
                QString::number(latitude,'f',6));
    ui->mapLongitudeLine->setText(
                QString::number(longitude,'f',6));
    ui->mapAltitudeLine->setText(
                QString::number(altitude,'f',2));
    ui->mapYawLine->setText(
                QString::number(heading,'f',2));

    QString updateScript = QString("updateDroneLocation(%1, %2, %3);")
            .arg(QString::number(latitude,'f',6))
            .arg(QString::number(longitude,'f',6))
            .arg(QString::number(heading,'f',2));
    webview->page()->runJavaScript(updateScript);
}

void MatriceDFView::on_makeDirectionButton_clicked()
{
    webview->page()->runJavaScript("makeBeam();");
}

void MatriceDFView::on_clearMapButton_clicked()
{
    webview->page()->runJavaScript("clearMap();");
}

AmplitudeSpectrumPlot* MatriceDFView::getAmplitudeSpectrumPlot()
{
    return (AmplitudeSpectrumPlot*) amplitudeSpectrumPlot;
}

PhaseSpectrumPlot* MatriceDFView::getPhaseSpectrumPlot()
{
    return (PhaseSpectrumPlot*) phaseSpectrumPlot;
}
