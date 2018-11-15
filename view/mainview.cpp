#include "mainview.hpp"


#include <QDebug>

MainView::MainView(QWidget *parent) :
    MainIView (parent)
{
    ui->setupUi(this);
    presenter = new MainPresenter(this);
}

MainView::~MainView()
{}
