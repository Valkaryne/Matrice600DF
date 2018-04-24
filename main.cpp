#include "matricedfview.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MatriceDFView w;
    w.show();

    return a.exec();
}
