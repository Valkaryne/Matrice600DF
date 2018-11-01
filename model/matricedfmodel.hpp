#ifndef MATRICEDFMODEL_HPP
#define MATRICEDFMODEL_HPP

#include <QObject>
#include <QThread>

#include "../utils/constset.hpp"
#include "udpchannel.hpp"

class MatriceDFModel : public QThread
{
    Q_OBJECT

public:
    explicit MatriceDFModel();

    /* Setters */

    void setCurrentGain(double &gain) {
        if (this->gain != gain)
            this->gain = gain;
    }

    void setRangeBounds(QVector<int> &rangeBounds) {
        this->rangeBounds = rangeBounds;
    }

    void updateCurrentHeading(const int &heading) {
        this->heading = heading;
    }

    void setPhaseCorrection(const double &phaseCorrection) {
        this->phaseCorrection = phaseCorrection;
    }

    /* Getters */
    UdpChannel* getUdpChannel() {
        return udpChannel;
    }

private:
    double phaseCorrectionHandler(const double phase);
    void prepareLinearSamples(const QVector<double> &samplesAm1, const QVector<double> &samplesAm2,
                              const QVector<double> &samplesAmS, const QVector<double> &samplesPh);
    void preparePolarSamples(const QVector<double> &samplesAm1, const QVector<double> &samplesAm2,
                              const QVector<double> &samplesAmS, const QVector<double> &samplesPh);

private slots:
    void samplesHandler(const QVector<double> &samplesAm1, const QVector<double> &samplesAm2,
                        const QVector<double> &samplesAmS, const QVector<double> &samplesPh);

signals:
    void amplitudeSamplesReady(const QVector<double> &samplesAm1, const QVector<double> &samplesAm2,
                               const QVector<double> &samplesAmS);
    void phaseSamplesReady(const QVector<double> &samplesPh);
    void polarSamplesReady(const int &az, const double &rado, const double &radl, const double &rads, const double &phase);
    void phaseDeviationEstimated(const double &phDev);

private:
    double gain;
    QVector<int> rangeBounds;

    double phaseCorrection;
    int heading;

    UdpChannel *udpChannel;
};

#endif // MATRICEDFMODEL_HPP
