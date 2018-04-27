#ifndef MODELINTERFACE_HPP
#define MODELINTERFACE_HPP

#include <QObject>
#include <QThread>
#include <Utils/udpchannel.hpp>
#include <Utils/constset.hpp>

class ModelInterface : public QThread
{
    Q_OBJECT

public:
    explicit ModelInterface() {}

    virtual void initializeModel() = 0;
    virtual UdpChannel* getUDPChannel() = 0;

public slots:
    virtual void samplesHandler(const QVector<double> samplesAm1, const QVector<double> samplesAm2,
                                const QVector<double> samplesPh) = 0;

signals:
    virtual void amplitudeSamplesReady(const QVector<double> amplitudeOne, const QVector<double> amplitudeTwo) = 0;
    virtual void phaseSamplesReady(const QVector<double> phaseDiff) = 0;

protected:
    UdpChannel  *udpChannel;
};

#endif // MODELINTERFACE_HPP
