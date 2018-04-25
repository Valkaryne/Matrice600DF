#ifndef MATRICEDFVIEW_HPP
#define MATRICEDFVIEW_HPP

#include <QMainWindow>
#include <Utils/spectrumwaterfall.hpp>
#include <Utils/spectrumplot.hpp>
#include <Utils/amplitudespectrumplot.hpp>
#include <Utils/phasespectrumplot.hpp>
#include <Utils/polarplot.hpp>

namespace Ui {
class MatriceDFView;
}

class MatriceDFView : public QMainWindow
{
    Q_OBJECT

public:
    explicit MatriceDFView(QWidget *parent = 0);
    ~MatriceDFView();

private:
    Ui::MatriceDFView *ui;
    SpectrumWaterfall   *spectrumWaterfall;
    SpectrumPlot        *amplitudeSpectrumPlot;
    SpectrumPlot        *phaseSpectrumPlot;
    PolarPlot           *polarPlot;
};

#endif // MATRICEDFVIEW_HPP
