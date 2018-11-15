#include <QApplication>
#include <QDesktopWidget>

#include "view/mainview.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainIView *view = new MainView();
    view->show();

    return a.exec();
}
