#ifndef MAINIVIEW_HPP
#define MAINIVIEW_HPP

#include <QMainWindow>

#include <presenter/mainpresenter.hpp>

#include "ui_mainview.h"

class MainIView : public QMainWindow
{
public:
    explicit MainIView(QWidget *parent = nullptr) :
        QMainWindow(parent),  ui(new Ui::MainView) {}
    ~MainIView() {}

protected:
    Ui::MainView    *ui;
    MainPresenter   *presenter;
};

#endif // MAINIVIEW_HPP
