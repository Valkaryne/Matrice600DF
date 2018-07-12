#include "view/mainview.hpp"
//#include "presenter/matricedfpresenter.hpp"
#include <QApplication>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainView w;

    qRegisterMetaType<QHash<QString, int> >("QHash<QString, int>");

    //MatriceDFPresenter *presenter = new MatriceDFPresenter(&w);
    w.move(QApplication::desktop()->availableGeometry(&w).topLeft() + QPoint(20, 20));
    w.show();

    return a.exec();
}
