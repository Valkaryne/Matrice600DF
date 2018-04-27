//#include "View/matricedfview.hpp"
#include "Model/matricedfmodel.hpp"
#include "Controller/matricedfcontroller.hpp"
#include <QApplication>

#include <QtMath>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ModelInterface *model = new MatriceDFModel();
    ControllerInterface *controller = new MatriceDFController(model);
    //MatriceDFView w;
    //w.show();

    return a.exec();
}
