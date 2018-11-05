#ifndef LINEARSAMPLESMODEL_H
#define LINEARSAMPLESMODEL_H

#include <QThread>


class LinearSamplesModel : public QThread
{
    Q_OBJECT
public:
    LinearSamplesModel();
};

#endif // LINEARSAMPLESMODEL_H
