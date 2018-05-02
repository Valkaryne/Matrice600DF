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

public slots:
    virtual void samplesHandler(const QVector<double> samplesAm1, const QVector<double> samplesAm2,
                                const QVector<double> samplesPh) = 0;

signals:

protected:
    UdpChannel  *udpChannel;
    TcpChannel  *tcpChannel;
};

#endif // MODELINTERFACE_HPP
