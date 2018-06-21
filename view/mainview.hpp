#ifndef MAINVIEW_HPP
#define MAINVIEW_HPP

#include <QMainWindow>

#include <presenter/matricedfpresenter.hpp>
#include "mainiview.hpp"

#include <widgets/summarystrategy.hpp>
#include <widgets/twochannelstrategy.hpp>

namespace Ui {
class MainView;
}

class MainView : public QMainWindow, public MainIView
{
    Q_OBJECT

public:
    explicit MainView(QWidget *parent = 0);
    ~MainView();

private slots:
    void on_btn_apply_clicked();

    void on_btn_amMode_clicked(bool checked);

private:
    Ui::MainView        *ui;
    MatriceDFPresenter  *presenter;
};

#endif // MAINVIEW_HPP
