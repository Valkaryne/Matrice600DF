#ifndef MAINVIEW_HPP
#define MAINVIEW_HPP

#include "mainiview.hpp"

class MainView : public MainIView
{
    Q_OBJECT
public:
    explicit MainView(QWidget *parent = nullptr);
    ~MainView();
};

#endif // MAINVIEW_HPP
