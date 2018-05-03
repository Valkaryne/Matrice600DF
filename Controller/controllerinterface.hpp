#ifndef CONTROLLERINTERFACE_HPP
#define CONTROLLERINTERFACE_HPP

#include <Model/modelinterface.hpp>

class ControllerInterface : public QObject
{
    Q_OBJECT
public:
    explicit ControllerInterface(ModelInterface *model = 0) {}

private:
};

#endif // CONTROLLERINTERFACE_HPP
