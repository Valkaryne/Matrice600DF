#ifndef MATRICEDFVIEW_HPP
#define MATRICEDFVIEW_HPP

#include <QMainWindow>

namespace Ui {
class MatriceDFView;
}

class MatriceDFView : public QMainWindow
{
    Q_OBJECT

public:
    explicit MatriceDFView(QWidget *parent = 0);
    ~MatriceDFView();

private:
    Ui::MatriceDFView *ui;
};

#endif // MATRICEDFVIEW_HPP
