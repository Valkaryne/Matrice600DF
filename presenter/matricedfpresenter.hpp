#ifndef MATRICEDFPRESENTER_HPP
#define MATRICEDFPRESENTER_HPP

#include "../model/matricedfmodel.hpp"
#include "../view/mainiview.hpp"

class MatriceDFPresenter : public QObject
{
    Q_OBJECT
public:
    explicit MatriceDFPresenter(MainIView *view, QObject *parent = nullptr);
    ~MatriceDFPresenter();

    void applyUsrpSettings(QVector<double> &settings);

private slots:
    void amplitudeSamplesPresenter(const QVector<double> samplesAm1, const QVector<double> samplesAm2,
                                   const QVector<double> samplesAmS);
    void phaseSamplesPresenter(const QVector<double> samplesPh);
    void polarSamplesPresenter(const int az, const double rado, const double radl, const double rads,
                               const double phase);

private:
    MainIView       *view;
    MatriceDFModel  *model;
};

#endif // MATRICEDFPRESENTER_HPP
