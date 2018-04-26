#ifndef CONTROLLERINTERFACE_HPP
#define CONTROLLERINTERFACE_HPP

#include <Model/modelinterface.hpp>
#include <View/matricedfview.hpp>

class ControllerInterface : public QObject
{
    Q_OBJECT
public:
    explicit ControllerInterface(ModelInterface *model = 0) {}

private:
    virtual void makeConnections(MatriceDFView *view) = 0;
};

#endif // CONTROLLERINTERFACE_HPP
