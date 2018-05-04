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

private:
};

#endif // CONTROLLERINTERFACE_HPP
