#ifndef MAINVIEW_HPP
#define MAINVIEW_HPP

#include "mainiview.hpp"

#include <QDebug>

class MainView : public MainIView
{
    Q_OBJECT
public:
    explicit MainView(QWidget *parent = nullptr);
    ~MainView();

    void sayCat() {
        qDebug() << "Cat";
    }
};

#endif // MAINVIEW_HPP
