#ifndef MATRICEDFMODEL_HPP
#define MATRICEDFMODEL_HPP

#include "modelinterface.hpp"

class MatriceDFModel : public ModelInterface
{
    Q_OBJECT

public:
    MatriceDFModel();

    void initializeModel();
    UdpChannel* getUDPChannel();
    TcpChannel* getTCPChannel();

private slots:
    void samplesHandler(const QVector<double> samplesAm1, const QVector<double> samplesAm2,
                        const QVector<double> samplesPh);

signals:
    void amplitudeSamplesReady(const QVector<double> amplitudeOne, const QVector<double> amplitudeTwo);
    void phaseSamplesReady(const QVector<double> phaseDiff);

private:
    double gain;
};

#endif // MATRICEDFMODEL_HPP
