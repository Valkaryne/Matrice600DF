#ifndef MATRICEDFCONTROLLER_HPP
#define MATRICEDFCONTROLLER_HPP

#include "controllerinterface.hpp"
#include <Model/modelinterface.hpp>
#include <Model/matricedfmodel.hpp>
#include <View/matricedfview.hpp>

#include <qtosdk.hpp>
#include <subscribe.hpp>
#include <QObject>


class MatriceDFController : public ControllerInterface
{

public:
    explicit MatriceDFController(ModelInterface *model = 0);

    void changeGainParameter(double gain);
    void changeAnalysisRange(QVector<int> range);
    void enableCalibrationMode();
    void disableCalibrationMode();

    void setDoubleDrawingMode();
    void setSummDrawingMode();
    void sendRunCommandRequest(int commandIndex);

    void sendInitWaypointRequest(QHash<QString, int>);
    void sendLoadWaypointRequest(QHash<QString, int>);
    void sendStartWaypointRequest();
    void sendAbortWaypointRequest();

private:

    //MatriceDFView   *view;
    ModelInterface  *model;
    QtOsdk  *sdk;

};

#endif // MATRICEDFCONTROLLER_HPP
