#include "View/matricedfview.hpp"
#include <QApplication>

#include <QtMath>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MatriceDFView w;
    w.show();

    return a.exec();
}
