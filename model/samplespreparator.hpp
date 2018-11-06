#ifndef SAMPLESPREPARATOR_HPP
#define SAMPLESPREPARATOR_HPP

#include <QThread>
#include <QVector>

class SamplesPreparator : public QThread
{
    Q_OBJECT
public:
    explicit SamplesPreparator();

private slots:
    void prepareLinearSamples(const QVector<double> &samplesAm1, const QVector<double> &samplesAm2,
                              const QVector<double> &samplesAmS, const QVector<double> &samplesPh);
    void preparePolarSamples(const QVector<double> &samplesAm1, const QVector<double> &samplesAm2,
                             const QVector<double> &samplesAmS, const QVector<double> &samplesPh);

signals:
    void amplitudeSamplesReady(const QVector<double> &samplesAm1, const QVector<double> &samplesAm2,
                                   const QVector<double> &samplesAmS);
    void phaseSamplesReady(const QVector<double> &samplesPh);
    void polarSamplesReady(const int &az, const double &rado, const double &radl, const double &rads, const double &phase);
    void phaseDeviationEstimated(const double &phDev);

};

#endif // SAMPLESPREPARATOR_HPP
