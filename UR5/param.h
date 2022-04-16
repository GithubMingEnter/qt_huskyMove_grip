#ifndef PARAM_H
#define PARAM_H

#include <QDialog>

namespace Ui {
class param;
}

class param : public QDialog
{
    Q_OBJECT

public:
    explicit param(QWidget *parent = 0);
    ~param();

private:
    Ui::param *ui;
};

#endif // PARAM_H
