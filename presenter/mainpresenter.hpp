#ifndef MAINPRESENTER_HPP
#define MAINPRESENTER_HPP

#include <QObject>

#include "../model/mainmodel.hpp"
#include "../utils/template_functions.hpp"

class MainIView;

class MainPresenter : public QObject
{
    Q_OBJECT
public:
    explicit MainPresenter(MainIView *view, QObject *parent = nullptr);
    ~MainPresenter();

private:
    MainIView *view;
    MainModel *model;
};

#endif // MAINPRESENTER_HPP
