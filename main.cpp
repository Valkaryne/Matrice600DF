#include "view/mainview.hpp"
//#include "presenter/matricedfpresenter.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainView w;

    //MatriceDFPresenter *presenter = new MatriceDFPresenter(&w);

    w.show();

    return a.exec();
}
