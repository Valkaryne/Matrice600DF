#ifndef MATRICEDFCONTROLLER_HPP
#define MATRICEDFCONTROLLER_HPP

#include "controllerinterface.hpp"
#include <Model/modelinterface.hpp>
#include <Model/matricedfmodel.hpp>
#include <View/matricedfview.hpp>


class MatriceDFController : public ControllerInterface
{

public:
    explicit MatriceDFController(ModelInterface *model = 0);

private slots:


signals:

private:

    //MatriceDFView   *view;
    ModelInterface  *model;

};

#endif // MATRICEDFCONTROLLER_HPP
