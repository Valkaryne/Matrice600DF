#include "mainview.hpp"
#include "ui_mainview.h"

MainView::MainView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainView)
{
    ui->setupUi(this);
    presenter = new MatriceDFPresenter(this);

    /* spectrum plots */
    amplitudeSpectrumPlot = new AmplitudeSpectrumPlot(this);
    getAmplitudeSpectrumPlot()->setDisplayStrategy(new TwoChannelStrategy(getAmplitudeSpectrumPlot()));
    getAmplitudeSpectrumPlot()->setMarkerStrategy(new FrequencyHoppingStrategy(getAmplitudeSpectrumPlot()));
    getAmplitudeSpectrumPlot()->setPickers(false);
    getAmplitudeSpectrumPlot()->setZoomer(true);
    getAmplitudeSpectrumPlot()->setThresholdPickers(false);
    ui->spectrumPlotLayout->addWidget(amplitudeSpectrumPlot);

    phaseSpectrumPlot = new PhaseSpectrumPlot(this);
    ui->spectrumPlotLayout->addWidget(phaseSpectrumPlot);
    connect(getAmplitudeSpectrumPlot()->getZoomer(), SIGNAL(zoomed(const QRectF &)),
            getPhaseSpectrumPlot(), SLOT(equalZoom(const QRectF &)));
    connect(getPhaseSpectrumPlot(), SIGNAL(phaseCorrector(double)), SLOT(phaseCorrectionChanged(double)));

    /* polar plot */
    polarPlot = new PolarPlot(this);
    polarPlot->setDisplayStrategy(new TwoChannelStrategy(polarPlot));
    ui->polarPlotLayout->addWidget(polarPlot, 0, 0, 2, 2);
    ui->polarPlotLayout->addWidget(ui->slider_add, 2, 0, 1, 1);
    ui->polarPlotLayout->addWidget(ui->slider_product, 0, 3, 1, 1);
    connect(polarPlot, SIGNAL(setDirectionRequest(const double &)), SLOT(makeDirection(const double &)));

    /* map */
    map = new QQuickView;
    map->setSource(QUrl(QStringLiteral("qrc:/mapview.qml")));

    QWidget *map_wrapper = QWidget::createWindowContainer(map, this);
    ui->mapLayout->addWidget(map_wrapper);
    QObject *object = map->rootObject();
    QMetaObject::invokeMethod(object, "initializeMapItems");

    QQmlContext *ctx = map->rootContext();
    ctx->setContextProperty("mainview", this);

    /* menus and actions */
    switchProviderAct = new QAction(tr("&Switch Provider"), this);
    connect(switchProviderAct, &QAction::triggered, this, &MainView::switchMapProvider);

    savePresetAct = new QAction(tr("&Save Preset"), this);
    connect(savePresetAct, &QAction::triggered, this, &MainView::savePreset);

    loadPresetAct = new QAction(tr("&Load Preset"), this);
    connect(loadPresetAct, &QAction::triggered, this, &MainView::loadPreset);

    mapMenu = menuBar()->addMenu(tr("&Map"));
    mapMenu->addAction(switchProviderAct);

    settingsMenu = menuBar()->addMenu(tr("&Settings"));
    settingsMenu->addAction(savePresetAct);
    settingsMenu->addAction(loadPresetAct);
}

MainView::~MainView()
{
    delete ui;
}

void MainView::keyPressEvent(QKeyEvent *event)
{
    bool controlModifier = event->modifiers().testFlag(Qt::ControlModifier);
    bool shiftModifier = event->modifiers().testFlag(Qt::ShiftModifier);

    switch (event->key()) {
    case Qt::Key_W:
        if (shiftModifier) presenter->sendFastThrustRequest(1);
        else if (controlModifier) presenter->sendSlowThrustRequest(1);
        else presenter->sendStableThrustRequest(1);
        break;
    case Qt::Key_A:
        if (shiftModifier) presenter->sendFastYawRequest(-1);
        else if (controlModifier) presenter->sendSlowYawRequest(-1);
        else presenter->sendStableYawRequest(-1);
        break;
    case Qt::Key_S:
        if (shiftModifier) presenter->sendFastThrustRequest(-1);
        else if (controlModifier) presenter->sendSlowThrustRequest(-1);
        else presenter->sendStableThrustRequest(-1);
        break;
    case Qt::Key_D:
        if (shiftModifier) presenter->sendFastYawRequest(1);
        else if (controlModifier) presenter->sendSlowYawRequest(1);
        else presenter->sendStableYawRequest(1);
        break;
    case Qt::Key_Q:
        if (shiftModifier) presenter->sendFastYawRequest(-1);
        else if (controlModifier) presenter->sendSlowYawRequest(-1);
        else presenter->sendStableYawRequest(-1);
        break;
    case Qt::Key_E:
        if (shiftModifier) presenter->sendFastYawRequest(1);
        else if (controlModifier) presenter->sendSlowYawRequest(1);
        else presenter->sendStableYawRequest(1);
        break;
    case Qt::Key_I:
        if (shiftModifier) presenter->sendFastPitchRequest(1);
        else if (controlModifier) presenter->sendSlowPitchRequest(1);
        else presenter->sendStablePitchRequest(1);
        break;
    case Qt::Key_J:
        if (shiftModifier) presenter->sendFastRollRequest(-1);
        else if (controlModifier) presenter->sendSlowRollRequest(-1);
        else presenter->sendStableRollRequest(-1);
        break;
    case Qt::Key_K:
        if (shiftModifier) presenter->sendFastPitchRequest(-1);
        else if (controlModifier) presenter->sendSlowPitchRequest(-1);
        else presenter->sendStablePitchRequest(-1);
        break;
    case Qt::Key_L:
        if (shiftModifier) presenter->sendFastRollRequest(1);
        else if (controlModifier) presenter->sendSlowRollRequest(1);
        else presenter->sendStableRollRequest(1);
        break;
    default:
        break;
    }
}

void MainView::keyReleaseEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat())
        return;

    switch (event->key()) {
    case Qt::Key_W: case Qt::Key_S:
        presenter->sendResetThrust();
        break;
    case Qt::Key_A: case Qt::Key_D:
        presenter->sendResetYaw();
        break;
    case Qt::Key_K:
        presenter->sendResetPitch();
        break;
    case Qt::Key_J: case Qt::Key_L:
        presenter->sendResetRoll();
        break;
    }
}

void MainView::on_btn_apply_clicked()
{
    QVector<double> settings;
    qDebug() << ui->sb_frequency->value();
    settings.append(ui->sb_frequency->value() * 1000000);
    settings.append(ui->sb_gain->value());
    settings.append(ui->sb_temp_prod->value()); // Temp value: product
    settings.append(ui->sb_temp_add->value()); // temp value: add

    QVector<int> thresholds = getAmplitudeSpectrumPlot()->getThresholdBounds();
    for (int thr : thresholds)
        settings.append(thr + ui->sb_gain->value() + CALIBRATION);

    QVector<int> bounds = getAmplitudeSpectrumPlot()->getMarkerBounds();
    if (thresholds.at(1) > 9999) {
        for (int bnd : bounds)
            settings.append(bnd);
    } else {
        for (int bnd : bounds) {
            if ((bnd >= 0) && (bnd < 2048)) bnd += 2048;
            else if ((bnd >= 2048) && (bnd < 4096)) bnd -= 2048;
            settings.append(bnd);
        }
    }

    presenter->applyUsrpSettings(settings);

    getAmplitudeSpectrumPlot()->setCentralFrequency(ui->sb_frequency->value());
    getPhaseSpectrumPlot()->setCentralFrequency(ui->sb_frequency->value());
    presenter->changeGainParameter(ui->sb_gain->value());
    presenter->changeBandParameter((int)ui->sb_frequency->value() / 1000);
    presenter->changeBoundsParameters(bounds);
}

void MainView::on_btn_amMode_clicked(bool checked)
{
    AmplitudeSpectrumPlot *plot = getAmplitudeSpectrumPlot();
    if (checked){
        plot->setDisplayStrategy(new SummaryStrategy(plot));
        polarPlot->setDisplayStrategy(new SummaryStrategy(polarPlot));
        ui->btn_amMode->setText("Two-Channel");
    } else {
        plot->setDisplayStrategy(new TwoChannelStrategy(plot));
        polarPlot->setDisplayStrategy(new TwoChannelStrategy(polarPlot));
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

void MainView::connectionDjiVehicleResetted()
{
    ui->btn_testInit->setText("Initialize");
    ui->btn_testInit->setStyleSheet("standard");

    ui->btn_testActivate->setText("Activate");
    ui->btn_testActivate->setStyleSheet("standard");
}

void MainView::updateTelemetryData(const QVector<double> &subscribeData){
    double latitude = subscribeData.at(3) / 10000000.0;
    double longitude = subscribeData.at(2) / 10000000.0;
    double altitude = subscribeData.at(1);
    double heading = subscribeData.at(0);

    ui->le_latitude->setText(QString::number(latitude,'f',6));
    ui->le_longitude->setText(QString::number(longitude,'f',6));
    ui->le_altitude->setText(QString::number(altitude,'f',2));
    ui->le_heading->setText(QString::number(heading,'f',2));

    QObject *object = map->rootObject();
    QMetaObject::invokeMethod(object, "updateDroneLocation",
                              Q_ARG(QVariant, latitude),
                              Q_ARG(QVariant, longitude),
                              Q_ARG(QVariant, heading));

    presenter->updateCurrentHeading(heading);
}

void MainView::setHomePoint(QString azimuth) {
    polarPlot->updateAllyDirection(azimuth.toDouble());
}

void MainView::setPointOnMap(QString lat, QString lng)
{
    markerLatitude = lat.toDouble();
    markerLongitude = lng.toDouble();

    QVector<double> coordinates;
    coordinates.append(markerLatitude);
    coordinates.append(markerLongitude);
    coordinates.append(ui->sb_wp_altitude->value());
    presenter->sendInitHotpointRequest(coordinates);
}

void MainView::makeDirection(const double &direction)
{
    QObject *object = map->rootObject();
    QMetaObject::invokeMethod(object, "makeBeam",
                              Q_ARG(QVariant, direction));
}

void MainView::phaseCorrectionChanged(double phaseCorrection)
{
    // 5760: -130
    // 2430: +130
    // 915: -180

    presenter->changePhaseCorrection(phaseCorrection);
    if (phaseCorrection <= 0)
        ui->lbltest_phase->setText("Phase: " + QString::number(phaseCorrection));
    else
        ui->lbltest_phase->setText("Phase: +" + QString::number(phaseCorrection));
}

void MainView::on_cb_droneClassSelect_activated(const QString &arg1)
{
    AmplitudeSpectrumPlot *plot = getAmplitudeSpectrumPlot();
    if (arg1 == "Matrice/Phantom") {
        ui->sb_temp_prod->setValue(70);
        plot->setMarkerStrategy(new FrequencyHoppingStrategy(plot));
    } else if (arg1 == "Mavic/Spark") {
        ui->sb_temp_prod->setValue(70);
        plot->setMarkerStrategy(new SpreadSpectrumStrategy(plot));
    } else if (arg1 == "Planer") {
        ui->sb_temp_prod->setValue(35);
        plot->setMarkerStrategy(new DenseHoppingStrategy(plot));
    }
}

void MainView::on_bgr_markers_buttonClicked(QAbstractButton *button)
{
    int number = button->text().toInt();
    getAmplitudeSpectrumPlot()->setMarker(number);
}

void MainView::on_bgr_modes_buttonClicked(QAbstractButton *button)
{
    AmplitudeSpectrumPlot *plot = getAmplitudeSpectrumPlot();
    QString name = button->text();
    if (name == "Markers") {
        plot->setPickers(true);
        plot->setZoomer(false);
        plot->setThresholdPickers(false);
    } else if (name == "Zoom") {
        plot->setZoomer(true);
        plot->setThresholdPickers(false);
        plot->setPickers(false);
    } else if (name == "Thresholds") {
        plot->setThresholdPickers(true);
        plot->setPickers(false);
        plot->setZoomer(false);
    }
}

void MainView::on_btn_maxHold_clicked(bool checked)
{
    AmplitudeSpectrumPlot *plot = getAmplitudeSpectrumPlot();
    plot->setMaxHold(checked);
    if (ui->btn_maxHold->text() == "Disable MaxHold") {
        plot->setExpCoefficient(0);
        ui->btn_maxHold->setText("Enable MaxHold");
    }
    else if (ui->btn_maxHold->text() == "Enable MaxHold") {
        plot->setExpCoefficient(ui->sb_expCoeff->value());
        ui->btn_maxHold->setText("Disable MaxHold");
    }
}

void MainView::on_sb_expCoeff_valueChanged(double arg1)
{
    getAmplitudeSpectrumPlot()->setExpCoefficient(arg1);
}

void MainView::on_slider_add_valueChanged(int position)
{
    polarPlot->changeSharpCoefficient(position);
}

void MainView::on_slider_product_valueChanged(int position)
{

}

void MainView::on_btn_resetScales_clicked()
{
    polarPlot->resetScales();
}

void MainView::on_btn_refresh_clicked()
{
    polarPlot->clearDiagram();
    QObject *object = map->rootObject();
    QMetaObject::invokeMethod(object, "updateHomeDirection");
}

void MainView::switchMapProvider()
{
    QObject *object = map->rootObject();
    QMetaObject::invokeMethod(object, "switchProvider");
}

void MainView::savePreset()
{
    QString fileName = QFileDialog::getSaveFileName(this);
    if (!fileName.isEmpty())
        writeToFile(fileName);
}

void MainView::loadPreset()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty())
        readFromFile(fileName);
}

void MainView::on_btn_clearMap_clicked()
{
    QObject *object = map->rootObject();
    QMetaObject::invokeMethod(object, "clearMap");
}

void MainView::on_btn_wp_init_clicked()
{
    QHash<QString, int> initSettings;
    initSettings.insert("latitude", ui->le_latitude->text().toDouble() * 1000000);
    initSettings.insert("longitude", ui->le_longitude->text().toDouble() * 1000000);
    initSettings.insert("altitude", ui->le_altitude->text().toInt());
    initSettings.insert("velocity", ui->sb_wp_velocity->value());
    initSettings.insert("yaw logic", ui->cb_wp_yawLogic->currentIndex());
    initSettings.insert("on rc lost", ui->cb_wp_rcLost->currentIndex());

    presenter->sendInitWaypointRequest(initSettings);
}

void MainView::on_btn_wp_load_clicked()
{
    QHash<QString, int> loadSettings;
    loadSettings.insert("latitude", markerLatitude * 1000000);
    loadSettings.insert("longitude", markerLongitude * 1000000);
    loadSettings.insert("altitude", ui->sb_wp_altitude->value());

    presenter->sendLoadWaypointRequest(loadSettings);
}

void MainView::on_btn_wp_start_clicked()
{
    presenter->sendStartWaypointRequest();
}

void MainView::on_btn_wp_abort_clicked()
{
    presenter->sendAbortWaypointRequest();
}

void MainView::on_btn_runCommand_clicked()
{
    int commandIndex = ui->cb_command->currentIndex();
    presenter->sendFlightRunCommandRequest(commandIndex);
}

void MainView::on_btn_hp_start_clicked(bool checked)
{
    if (checked) {
        int yawRate = ui->sb_yawRate->value();
        presenter->sendStartHotpointRequest(yawRate);
        ui->btn_hp_start->setText("Stop");
    } else {
        presenter->sendStopHotpointRequest();
        ui->btn_hp_start->setText("Rotate");
    }
}

void MainView::writeToFile(const QString &fileName)
{
    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::information(this, tr("Unable to open file"), file.errorString());
        return;
    }

    QVector<QVector<int>> preset;
    QVector<int> bounds = getAmplitudeSpectrumPlot()->getMarkerBounds();
    QVector<int> thresholds = getAmplitudeSpectrumPlot()->getThresholdBounds();
    QVector<int> settings = getSettingsArray();
    QVector<int> mapSettings = getMapSettingsArray();
    preset.append(bounds);
    preset.append(thresholds);
    preset.append(settings);
    preset.append(mapSettings);
    QDataStream out(&file);
    out << preset;
}

void MainView::readFromFile(const QString &fileName)
{
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(this, tr("Unable to open file"), file.errorString());
        return;
    }

    QVector<QVector<int>> preset;
    QDataStream in(&file);
    in >> preset;
    getAmplitudeSpectrumPlot()->setMarkerBounds(preset.at(0));
    getAmplitudeSpectrumPlot()->setThresholdBounds(preset.at(1));
    setSettingsArray(preset.at(2));
    setMapSettingsArray(preset.at(3));
}

QVector<int> MainView::getSettingsArray()
{
    QVector<int> settings;
    settings.append(ui->sb_frequency->value());
    settings.append(ui->sb_gain->value());
    settings.append(ui->sb_temp_prod->value());
    settings.append(ui->sb_temp_add->value());
    settings.append(ui->lbltest_phase->text().remove(0,7).toInt());

    return settings;
}

QVector<int> MainView::getMapSettingsArray()
{
    QVector<int> mapSettings;
    QVariant varProvider, varCenter, varZoomLevel, varHomePoint;

    QObject *object = map->rootObject();

    QMetaObject::invokeMethod(object, "getMapProvider",
                              Q_RETURN_ARG(QVariant, varProvider));
    QMetaObject::invokeMethod(object, "getMapCenter",
                              Q_RETURN_ARG(QVariant, varCenter));
    QMetaObject::invokeMethod(object, "getMapZoomLevel",
                              Q_RETURN_ARG(QVariant, varZoomLevel));
    QMetaObject::invokeMethod(object, "getMapHomePoint",
                              Q_RETURN_ARG(QVariant, varHomePoint));

    int provider = varProvider.value<int>();
    QVector2D center = varCenter.value<QVector2D>();
    int zoomLevel = varZoomLevel.value<double>() * 100;
    QVector2D homePoint = varHomePoint.value<QVector2D>();

    mapSettings.append(provider);
    mapSettings.append(center.x() * 1000000);
    mapSettings.append(center.y() * 1000000);
    mapSettings.append(zoomLevel);
    mapSettings.append(homePoint.x() * 1000000);
    mapSettings.append(homePoint.y() * 1000000);

    return  mapSettings;
}

void MainView::setSettingsArray(QVector<int> settings)
{
    ui->sb_frequency->setValue(settings.at(0));
    ui->sb_gain->setValue(settings.at(1));
    ui->sb_temp_prod->setValue(settings.at(2));
    ui->sb_temp_add->setValue(settings.at(3));

    getPhaseSpectrumPlot()->setPhaseCorrection(settings.at(4));
}

void MainView::setMapSettingsArray(QVector<int> mapSettings)
{
    QObject *object = map->rootObject();

    QMetaObject::invokeMethod(object, "setMapProvider",
                              Q_ARG(QVariant, mapSettings.at(0)));
    QMetaObject::invokeMethod(object, "setMapCenter",
                              Q_ARG(QVariant, mapSettings.at(1) / 1000000.0),
                              Q_ARG(QVariant, mapSettings.at(2) / 1000000.0));
    QMetaObject::invokeMethod(object, "setMapZoomLevel",
                              Q_ARG(QVariant, mapSettings.at(3) / 100.0));
    QMetaObject::invokeMethod(object, "setMapHomePoint",
                              Q_ARG(QVariant, mapSettings.at(4) / 1000000.0),
                              Q_ARG(QVariant, mapSettings.at(5) / 1000000.0));
}
