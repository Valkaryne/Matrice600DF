#ifndef MATRICEDFMODEL_HPP
#define MATRICEDFMODEL_HPP

#include <QObject>
#include <QThread>

#include "../utils/constset.hpp"
#include "udpchannel.hpp"
#include "samplespreparator.hpp"

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

    static void setRangeBounds(QVector<int> &rangeBounds) {
        MatriceDFModel::rangeBounds = rangeBounds;
    }

    static void updateCurrentHeading(const int &heading) {
        MatriceDFModel::heading = heading;
    }

    void setPhaseCorrection(const double &phaseCorrection) {
        this->phaseCorrection = phaseCorrection;
    }

    /* Getters */
    static UdpChannel* getUdpChannel() {
        return &UdpChannel::getInstance();
    }

    static QVector<int> getRangeBounds() {
        return rangeBounds;
    }

    static int getCurrentHeading() {
        return heading;
    }

private:
    double phaseCorrectionHandler(const double phase);

private slots:
    void samplesHandler(const QVector<double> &samplesAm1, const QVector<double> &samplesAm2,
                        const QVector<double> &samplesAmS, const QVector<double> &samplesPh);

signals:
    void prepareSamples(const QVector<double> &samplesAm1, const QVector<double> &samplesAm2,
                        const QVector<double> &samplesAmS, const QVector<double> &samplesPh);
    void amplitudeSamplesReady(const QVector<double> &samplesAm1, const QVector<double> &samplesAm2,
                               const QVector<double> &samplesAmS);
    void phaseSamplesReady(const QVector<double> &samplesPh);
    void polarSamplesReady(const int &az, const double &rado, const double &radl, const double &rads, const double &phase);
    void phaseDeviationEstimated(const double &phDev);

private:
    double gain;
    static QVector<int> rangeBounds;

    double phaseCorrection;
    static int heading;

    UdpChannel *udpChannel;
};

#endif // MATRICEDFMODEL_HPP
