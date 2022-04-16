#ifndef EXPLAIN_H
#define EXPLAIN_H

#include <QDialog>

namespace Ui {
class explain;
}

class explain : public QDialog
{
    Q_OBJECT

public:
    explicit explain(QWidget *parent = 0);
    ~explain();

private:
    Ui::explain *ui;
};

#endif // EXPLAIN_H
