#ifndef MAINMODEL_HPP
#define MAINMODEL_HPP

#include <QThread>

class MainModel : public QThread
{
    Q_OBJECT
public:
    explicit MainModel();
};

#endif // MAINMODEL_HPP
