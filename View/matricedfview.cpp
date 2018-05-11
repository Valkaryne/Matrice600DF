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
    getAmplitudeSpectrumPlot()->setPickers(false);
    getAmplitudeSpectrumPlot()->setZoomer(true);
    getAmplitudeSpectrumPlot()->setThresholdPickers(false);

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

AmplitudeSpectrumPlot* MatriceDFView::getAmplitudeSpectrumPlot()
{
    return (AmplitudeSpectrumPlot*) amplitudeSpectrumPlot;
}

PhaseSpectrumPlot* MatriceDFView::getPhaseSpectrumPlot()
{
    return (PhaseSpectrumPlot*) phaseSpectrumPlot;
}

SpectrumWaterfall* MatriceDFView::getSpectrumWaterfall()
{
    return spectrumWaterfall;
}

PolarPlot* MatriceDFView::getPolarPlot()
{
    return polarPlot;
}

/*void MatriceDFView::updateTelemetryData(const mtelemetry::Telemetry &telemetry)
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

    emit headingChanged((int)heading);
} */

void MatriceDFView::updateTelemetryData(const QVector<double> subscribeData)
{
    double latitude = subscribeData.at(3) / 10000000.0;
    double longitude = subscribeData.at(2) / 10000000.0;
    double altitude = subscribeData.at(1);
    double heading = subscribeData.at(0);

    //double latitude = 53.952373;
    //double longitude = 27.667157;

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

    emit headingChanged((int)heading);
}

void MatriceDFView::on_makeDirectionButton_clicked()
{
    webview->page()->runJavaScript("makeBeam();");
}

void MatriceDFView::on_clearMapButton_clicked()
{
    webview->page()->runJavaScript("clearMap();");
}

void MatriceDFView::on_applyButton_clicked()
{
    amplitudeSpectrumPlot->setCentralFrequency(ui->frequencySpinBox->value());
    phaseSpectrumPlot->setCentralFrequency(ui->frequencySpinBox->value());
    spectrumWaterfall->setCentralFrequency(ui->frequencySpinBox->value());
    controller->changeGainParameter(ui->gainSpinBox->value());

    QVector<double> settings;
    settings.append(ui->frequencySpinBox->value() * 1000000.0);
    settings.append(ui->gainSpinBox->value());
    settings.append(ui->productSpinBox->value());
    settings.append(ui->addSpinxBox->value());

    QVector<int> thresholds = getAmplitudeSpectrumPlot()->getThresholdBounds();
    for (int thr : thresholds)
        settings.append((thr + ui->gainSpinBox->value() + calibrationI) /*ui->gainSpinBox->value())*/);

    QVector<int> bounds = getAmplitudeSpectrumPlot()->getMarkerBounds();
    if (thresholds.at(1) > 9999)
    {
        for (int bnd : bounds)
            settings.append(bnd);
    }
    else
    {
        for (int bnd : bounds)
        {
            if ((bnd >= 0) && (bnd < 2048))
                bnd += 2048;
            else if ((bnd >= 2048) && (bnd < 4096))
                bnd -= 2048;
            settings.append(bnd);
        }
    }

    emit settingsReady(settings);
}

void MatriceDFView::on_calibrateButton_clicked(bool checked)
{
    if (checked) {
        ui->calibrateButton->setText("Stop");
        controller->enableCalibrationMode();
    } else {
        ui->calibrateButton->setText("Calibrate");
        controller->disableCalibrationMode();
    }
}

void MatriceDFView::on_clearDiagramButton_clicked()
{
    polarPlot->clearDiagram();
}

void MatriceDFView::on_setRangeButton_clicked()
{
    QVector<int> bounds = getAmplitudeSpectrumPlot()->getMarkerBounds();
    controller->changeAnalysisRange(bounds);
}

void MatriceDFView::on_markersButtonGroup_buttonClicked(QAbstractButton *button)
{
    int number = button->text().toInt();
    getAmplitudeSpectrumPlot()->setMarker(number);
}

void MatriceDFView::on_modeButtonGroup_buttonClicked(QAbstractButton *button)
{
    QString name = button->text();
    if (name == "Markers") {
        getAmplitudeSpectrumPlot()->setPickers(true);
        getAmplitudeSpectrumPlot()->setZoomer(false);
        getAmplitudeSpectrumPlot()->setThresholdPickers(false);
    } else if (name == "Zoom") {
        getAmplitudeSpectrumPlot()->setZoomer(true);
        getAmplitudeSpectrumPlot()->setThresholdPickers(false);
        getAmplitudeSpectrumPlot()->setPickers(false);
    } else if (name == "Threshold") {
        getAmplitudeSpectrumPlot()->setThresholdPickers(true);
        getAmplitudeSpectrumPlot()->setPickers(false);
        getAmplitudeSpectrumPlot()->setZoomer(false);
    }
}

void MatriceDFView::on_maxHoldCheckBox_stateChanged(int arg1)
{
    if (arg1 != 0)
        getAmplitudeSpectrumPlot()->setMaxHold(true);
    else
        getAmplitudeSpectrumPlot()->setMaxHold(false);
}

void MatriceDFView::on_expSpinBox_valueChanged(double arg1)
{
    getAmplitudeSpectrumPlot()->setExpCoefficient(arg1);
}

void MatriceDFView::on_amModeCheckBox_stateChanged(int arg1)
{
    polarPlot->clearDiagram();
    if (arg1 == 0)
        controller->setDoubleDrawingMode();
    else
        controller->setSummDrawingMode();
}

void MatriceDFView::on_runCommandButton_clicked()
{
    int commandIndex = ui->commandComboBox->currentIndex();
    controller->sendRunCommandRequest(commandIndex);
}

void MatriceDFView::on_wpInitButton_clicked()
{
    QHash<QString, int> initSettings;
    initSettings.insert("latitude", ui->mapLatitudeLine->text().toDouble() * 1000000);
    initSettings.insert("longitude", ui->mapLongitudeLine->text().toDouble() * 1000000);
    initSettings.insert("altitude", ui->mapAltitudeLine->text().toInt());
    initSettings.insert("velocity", ui->wpVelocitySpinBox->value());
    initSettings.insert("yaw logic", ui->wpYawComboBox->currentIndex());
    initSettings.insert("on rc lost", ui->wpRCLostComboBox->currentIndex());

    controller->sendInitWaypointRequest(initSettings);
}

void MatriceDFView::on_wpLoadStartButton_clicked(bool checked)
{
    if (!checked)
    {
        QHash<QString, int> loadSettings;
        loadSettings.insert("latitude", markerLatitude * 1000000);
        loadSettings.insert("longitude", markerLongitude * 1000000);
        loadSettings.insert("altitude", ui->wpAltitudeSpinBox->value());

        controller->sendLoadWaypointRequest(loadSettings);
        ui->wpLoadStartButton->setText("Start");
    }
    else
    {
        controller->sendStartWaypointRequest();
        ui->wpLoadStartButton->setText("Load");
    }
}

void MatriceDFView::on_wpAbortButton_clicked()
{
    controller->sendAbortWaypointRequest();
}

void MatriceDFView::setPointOnMap(QString lat, QString lng)
{
    markerLatitude = lat.toDouble();
    markerLongitude = lng.toDouble();
    qDebug() << markerLatitude;
    qDebug() << markerLongitude;
}

