#ifndef MAINVIEW_HPP
#define MAINVIEW_HPP

#include <QMainWindow>

#include "mainiview.hpp"

namespace Ui {
class MainView;
}

class MainView : public QMainWindow, public MainIView
{
    Q_OBJECT

public:
    explicit MainView(QWidget *parent = 0);
    ~MainView();

private:
    Ui::MainView *ui;
};

#endif // MAINVIEW_HPP
