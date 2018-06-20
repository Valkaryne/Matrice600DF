#include "matricedfpresenter.hpp"

MatriceDFPresenter::MatriceDFPresenter(MainIView *view, QObject *parent) :
    QObject(parent)
{
    this->view = view;
    this->model = new MatriceDFModel();

    connect(model,SIGNAL(amplitudeSamplesReady(const QVector<double>,const QVector<double>,const QVector<double>)),
            SLOT(amplitudeSamplesPresenter(const QVector<double>,const QVector<double>,const QVector<double>)));
    connect(model,SIGNAL(phaseSamplesReady(const QVector<double>)),
            SLOT(phaseSamplesPresenter(const QVector<double>)));
    connect(model,SIGNAL(polarSamplesReady(const int,const double,const double,const double,const double)),
            SLOT(polarSamplesPresenter(const int,const double,const double, const double,const double)));
}

MatriceDFPresenter::~MatriceDFPresenter()
{
}

void MatriceDFPresenter::applyUsrpSettings(QVector<double> &settings)
{
    model->getUdpChannel()->sendDatagram(settings);
}

/* Slots */
void MatriceDFPresenter::amplitudeSamplesPresenter(const QVector<double> samplesAm1, const QVector<double> samplesAm2,
                                              const QVector<double> samplesAmS)
{
    view->getAmplitudeSpectrumPlot()->updateCurve(samplesAm1, samplesAm2, samplesAmS);
}

void MatriceDFPresenter::phaseSamplesPresenter(const QVector<double> samplesPh)
{
    view->getPhaseSpectrumPlot()->updateCurve(samplesPh);
}

void MatriceDFPresenter::polarSamplesPresenter(const int az, const double rado, const double radl, const double rads,
                                          const double phase)
{

}
