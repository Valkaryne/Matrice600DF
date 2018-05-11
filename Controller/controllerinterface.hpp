#ifndef CONTROLLERINTERFACE_HPP
#define CONTROLLERINTERFACE_HPP

#include <Model/modelinterface.hpp>

class ControllerInterface : public QObject
{
    Q_OBJECT
public:
    explicit ControllerInterface(ModelInterface *model = 0) {}

    virtual void changeGainParameter(double gain) = 0;
    virtual void changeAnalysisRange(QVector<int> range) = 0;
    virtual void enableCalibrationMode() = 0;
    virtual void disableCalibrationMode() = 0;

    virtual void setDoubleDrawingMode() = 0;
    virtual void setSummDrawingMode() = 0;
    virtual void sendRunCommandRequest(int commandIndex) = 0;

    virtual void sendInitWaypointRequest(QHash<QString, int> settings) = 0;
    virtual void sendLoadWaypointRequest(QHash<QString, int> settings) = 0;
    virtual void sendStartWaypointRequest() = 0;
    virtual void sendAbortWaypointRequest() = 0;

signals:
    void runCommandRequest(int);

    void initWaypointRequest(QHash<QString, int>);
    void loadWaypointRequest(QHash<QString, int>);
    void startWaypointRequest();
    void abortWaypointRequest();

private:
};

#endif // CONTROLLERINTERFACE_HPP
