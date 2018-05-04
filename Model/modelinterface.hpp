#ifndef MODELINTERFACE_HPP
#define MODELINTERFACE_HPP

#include <QObject>
#include <QThread>
#include <Utils/udpchannel.hpp>
#include <Utils/tcpchannel.hpp>
#include <Utils/constset.hpp>

class ModelInterface : public QThread
{
    Q_OBJECT

public:
    explicit ModelInterface() {}

    virtual void initializeModel() = 0;
    virtual UdpChannel* getUDPChannel() = 0;
    virtual TcpChannel* getTCPChannel() = 0;

    /* Setters */
    virtual void setRangeBounds(QVector<int> rangeBounds) {
        if (this->rangeBounds != rangeBounds)
            this->rangeBounds = rangeBounds;
    }

    virtual void setCalibrationMode(bool setCalibration) {
        if (this->setCalibration != setCalibration)
            this->setCalibration = setCalibration;
    }

    virtual void setDrawingMode(bool doubleChannel) {
        if (this->doubleChannel != doubleChannel)
            this->doubleChannel = doubleChannel;
    } // NOTE: is it possible to use a strategy-pattern?

    virtual void setCurrentGain(double gain) {
        if (this->gain != gain)
            this->gain = gain;
    }


public slots:
    virtual void samplesHandler(const QVector<double> samplesAm1, const QVector<double> samplesAm2,
                                const QVector<double> samplesPh) = 0;
    virtual void polarSamplesHandler(const QVector<double> samplesAm1, const QVector<double> samplesAm2,
                                     const QVector<double> samplesPh) = 0;
    virtual void updateHeading(const int heading) {
        if (this->heading != heading)
            this->heading = heading;
    }

signals:
    virtual void amplitudeSamplesReady(const QVector<double> amplitudeOne, const QVector<double> amplitudeTwo) = 0;
    virtual void phaseSamplesReady(const QVector<double> phaseDiff) = 0;
    virtual void polarSamplesReady(const int az, const double rado, const double phase) = 0;
    virtual void polarSamplesReady(const int az, const double rado, const double radl, const double phase) = 0;

protected:
    UdpChannel  *udpChannel;
    TcpChannel  *tcpChannel;

    int heading;
    double gain;
    bool setCalibration;
    bool doubleChannel;

    QVector<int> rangeBounds;
};

#endif // MODELINTERFACE_HPP
