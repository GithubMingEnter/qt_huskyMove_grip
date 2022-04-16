#include "param.h"
#include "ui_param.h"

param::param(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::param)
{
    ui->setupUi(this);
}

param::~param()
{
    delete ui;
}
