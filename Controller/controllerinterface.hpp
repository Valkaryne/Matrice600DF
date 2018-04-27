#ifndef CONTROLLERINTERFACE_HPP
#define CONTROLLERINTERFACE_HPP

#include <Model/modelinterface.hpp>

class ControllerInterface : public QObject
{
    Q_OBJECT
public:
    explicit ControllerInterface(ModelInterface *model = 0) {}

private:
    virtual void makeConnections() = 0;
};

#endif // CONTROLLERINTERFACE_HPP
