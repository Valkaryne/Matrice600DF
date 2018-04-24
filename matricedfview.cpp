#include "matricedfview.hpp"
#include "ui_matricedfview.h"

MatriceDFView::MatriceDFView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MatriceDFView)
{
    ui->setupUi(this);
}

MatriceDFView::~MatriceDFView()
{
    delete ui;
}
