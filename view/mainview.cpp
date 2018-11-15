#include "mainview.hpp"

MainView::MainView(QWidget *parent) :
    MainIView (parent)
{
    ui->setupUi(this);
    presenter = new MainPresenter(this);
}

MainView::~MainView()
{}
