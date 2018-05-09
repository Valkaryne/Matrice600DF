#ifndef MATRICEDFVIEW_HPP
#define MATRICEDFVIEW_HPP

#include <QMainWindow>
#include <QAbstractButton>

#include <Utils/spectrumwaterfall.hpp>
#include <Utils/spectrumplot.hpp>
#include <Utils/amplitudespectrumplot.hpp>
#include <Utils/phasespectrumplot.hpp>
#include <Utils/polarplot.hpp>
//#include <Utils/MatriceTelemetry.pb.h>

#include <Model/modelinterface.hpp>
#include <Model/matricedfmodel.hpp>
#include <Controller/controllerinterface.hpp>
#include <Controller/matricedfcontroller.hpp>

// FIXME: Create composite object from webview
#include <QtWebEngineWidgets/QWebEngineView>
#include <QtWebEngineWidgets/QWebEnginePage>
#include <QtWebEngineWidgets/QWebEngineSettings>
#include <QtWebChannel/QWebChannel>

namespace Ui {
class MatriceDFView;
}

class MatriceDFView : public QMainWindow
{
    Q_OBJECT

public:
    explicit MatriceDFView(QWidget *parent = 0, ControllerInterface *controller = 0,
                           ModelInterface *model = 0);
    ~MatriceDFView();

    /* Getters */
    AmplitudeSpectrumPlot* getAmplitudeSpectrumPlot();
    PhaseSpectrumPlot* getPhaseSpectrumPlot();
    SpectrumWaterfall* getSpectrumWaterfall();
    PolarPlot* getPolarPlot();

    // FIXME: Create composite object from webview
    QWebEngineView  *webview;
    //Q_INVOKABLE void setPointOnMap(QString lat, QString lng);
    double markerLatitude, markerLongitude;

private slots:
    //void updateTelemetryData(const mtelemetry::Telemetry &telemetry);
    void updateTelemetryData(const QVector<double> subscribeData);

    void on_makeDirectionButton_clicked();
    void on_clearMapButton_clicked();

    void on_applyButton_clicked();
    void on_calibrateButton_clicked(bool checked);
    void on_clearDiagramButton_clicked();
    void on_setRangeButton_clicked();

    void on_markersButtonGroup_buttonClicked(QAbstractButton *button);
    void on_modeButtonGroup_buttonClicked(QAbstractButton *button);

    void on_maxHoldCheckBox_stateChanged(int arg1);
    void on_expSpinBox_valueChanged(double arg1);
    void on_amModeCheckBox_stateChanged(int arg1);

    void on_runCommandButton_clicked();

    void on_wpInitButton_clicked();
    void on_wpLoadStartButton_clicked(bool checked);
    void on_wpAbortButton_clicked();

signals:
    void settingsReady(const QVector<double> settings);
    void headingChanged(const int heading);

private:
    Ui::MatriceDFView   *ui;

    ModelInterface      *model;
    ControllerInterface *controller;

    SpectrumWaterfall   *spectrumWaterfall;
    SpectrumPlot        *amplitudeSpectrumPlot;
    SpectrumPlot        *phaseSpectrumPlot;
    PolarPlot           *polarPlot;
};

#endif // MATRICEDFVIEW_HPP
