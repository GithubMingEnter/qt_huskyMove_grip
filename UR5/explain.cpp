#include "explain.h"
#include "ui_explain.h"

explain::explain(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::explain)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/new/prefix1/icons/explain.ico"));
    this->setWindowTitle("说明");
    this->resize(600,400);
    this->setMaximumSize(600,400);
}

explain::~explain()
{
    delete ui;
}
