#ifndef MATRICEDFMODEL_HPP
#define MATRICEDFMODEL_HPP

#include "modelinterface.hpp"
#include <Utils/constset.hpp>

class MatriceDFModel : public ModelInterface
{
    Q_OBJECT

public:
    MatriceDFModel();

    /* Setters */
    void setCalibrationMode(bool setCalibration);

    /* Getters */
    UdpChannel* getUDPChannel();
    TcpChannel* getTCPChannel();

    /* Other */
    void initializeModel();

private slots:
    void samplesHandler(const QVector<double> samplesAm1, const QVector<double> samplesAm2,
                        const QVector<double> samplesPh);
    void polarSamplesHandler(const QVector<double> samplesAm1, const QVector<double> samplesAm2,
                             const QVector<double> samplesPh);

signals:
    void amplitudeSamplesReady(const QVector<double> amplitudeOne, const QVector<double> amplitudeTwo);
    void phaseSamplesReady(const QVector<double> phaseDiff);
    void polarSamplesReady(const int az, const double rado, const double radl, const double phase);
    void polarSamplesReady(const int az, const double rado, const double phase);

private:
    double am1N, am2N, amSN;

    QVector<double> simpleAm1, simpleAm2, summAm;
};

#endif // MATRICEDFMODEL_HPP
