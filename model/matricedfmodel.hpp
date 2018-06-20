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
    void setBand(int band) {
        if (this->band != band)
            this->band = band;
    }

    void setCurrentGain(double gain) {
        if (this->gain != gain)
            this->gain = gain;
    }

    /* Getters */
    UdpChannel* getUdpChannel() {
        return udpChannel;
    }

private slots:
    void samplesHandler(const QVector<double> samplesAm1, const QVector<double> samplesAm2,
                        const QVector<double> samplesPh);
    void polarSamplesHandler(const QVector<double> samplesAm1, const QVector<double> samplesAm2,
                             const QVector<double> samplesPh);
signals:
    void amplitudeSamplesReady(const QVector<double> samplesAm1, const QVector<double> samplesAm2, const QVector<double> samplesAmS);
    void phaseSamplesReady(const QVector<double> samplesPh);
    void polarSamplesReady(const int az, const double rado, const double radl, const double rads, const double phase);

private:
    int band;
    double gain;
    QVector<int> rangeBounds;

    UdpChannel *udpChannel;
};

#endif // MATRICEDFMODEL_HPP
