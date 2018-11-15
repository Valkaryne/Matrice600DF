#include "mainpresenter.hpp"
#include "../view/mainview.hpp"

MainPresenter::MainPresenter(MainIView *view, QObject *parent) :
    QObject (parent)
{
    this->view = view;
    this->model = new MainModel();

    if (instanceof<MainView>(view)) {
        dynamic_cast<MainView*>(view)->sayCat();
    }
}

MainPresenter::~MainPresenter()
{}
