#ifndef MATRICEDFVIEW_HPP
#define MATRICEDFVIEW_HPP

#include <QMainWindow>

#include <Utils/spectrumwaterfall.hpp>
#include <Utils/spectrumplot.hpp>
#include <Utils/amplitudespectrumplot.hpp>
#include <Utils/phasespectrumplot.hpp>
#include <Utils/polarplot.hpp>

#include <Model/modelinterface.hpp>
#include <Model/matricedfmodel.hpp>
#include <Controller/controllerinterface.hpp>
#include <Controller/matricedfcontroller.hpp>

// FIXIT: Create composite object from webview
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

    // FIXIT: Create composite object from webview
    QWebEngineView  *webview;

private slots:
    void on_decrAngleButton_clicked();

    void on_incrAngleButton_clicked();

    void on_makeDirButton_clicked();

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
