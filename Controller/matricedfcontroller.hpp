#ifndef MATRICEDFCONTROLLER_HPP
#define MATRICEDFCONTROLLER_HPP

#include <Model/matricedfmodel.hpp>
#include <View/matricedfview.hpp>

class MatriceDFController
{
public:
    MatriceDFController();

private:
    MatriceDFModel  *model;
    MatriceDFView   *view;
};

#endif // MATRICEDFCONTROLLER_HPP
