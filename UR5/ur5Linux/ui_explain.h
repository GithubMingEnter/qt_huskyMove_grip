/********************************************************************************
** Form generated from reading UI file 'explain.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EXPLAIN_H
#define UI_EXPLAIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>

QT_BEGIN_NAMESPACE

class Ui_explain
{
public:
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *label_10;
    QLabel *label_11;
    QLabel *label_12;
    QLabel *label_13;

    void setupUi(QDialog *explain)
    {
        if (explain->objectName().isEmpty())
            explain->setObjectName(QStringLiteral("explain"));
        explain->resize(557, 318);
        explain->setStyleSheet(QStringLiteral("background-color: rgb(183, 255, 179);"));
        label = new QLabel(explain);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(0, 10, 131, 31));
        label_2 = new QLabel(explain);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(20, 60, 221, 16));
        label_3 = new QLabel(explain);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(20, 80, 161, 16));
        label_4 = new QLabel(explain);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(20, 100, 171, 16));
        label_5 = new QLabel(explain);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(10, 120, 54, 12));
        label_6 = new QLabel(explain);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(10, 140, 141, 16));
        label_7 = new QLabel(explain);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(10, 160, 371, 16));
        label_8 = new QLabel(explain);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(20, 180, 81, 16));
        label_9 = new QLabel(explain);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(10, 180, 481, 16));
        label_10 = new QLabel(explain);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(20, 200, 81, 16));
        label_11 = new QLabel(explain);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(20, 270, 91, 31));
        label_12 = new QLabel(explain);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setGeometry(QRect(230, 280, 111, 16));
        label_13 = new QLabel(explain);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setGeometry(QRect(10, 200, 481, 16));

        retranslateUi(explain);

        QMetaObject::connectSlotsByName(explain);
    } // setupUi

    void retranslateUi(QDialog *explain)
    {
        explain->setWindowTitle(QApplication::translate("explain", "Dialog", 0));
        label->setText(QApplication::translate("explain", "\346\234\272\346\242\260\350\207\202\347\250\213\345\272\217\347\253\257\345\217\243\350\257\264\346\230\216\357\274\232", 0));
        label_2->setText(QApplication::translate("explain", "29999\357\274\232DashBoard\345\221\275\344\273\244\347\232\204\346\224\266\345\217\221", 0));
        label_3->setText(QApplication::translate("explain", "30001\357\274\232\346\214\211\351\224\256\346\216\247\345\210\266\345\221\275\344\273\244\345\217\221\351\200\201", 0));
        label_4->setText(QApplication::translate("explain", "30003\357\274\232\346\234\272\346\242\260\350\207\202\345\217\215\351\246\210\346\225\260\346\215\256\347\232\204\346\216\245\346\224\266", 0));
        label_5->setText(QApplication::translate("explain", "\346\263\250\346\204\217\357\274\232", 0));
        label_6->setText(QApplication::translate("explain", "1.\346\234\272\346\242\260\350\207\202\347\211\210\346\234\254\344\270\272\357\274\232UR5 3.7.0", 0));
        label_7->setText(QApplication::translate("explain", "2.\347\273\217\350\277\207\345\256\236\346\265\213\345\217\257\344\273\245\346\255\243\345\270\270\346\216\247\345\210\266UR5\345\256\236\344\275\223\346\234\272\346\242\260\350\207\202\357\274\214\344\271\237\345\217\257\344\273\245\346\216\247\345\210\266UR5\344\273\277\347\234\237\345\231\250URSim", 0));
        label_8->setText(QString());
        label_9->setText(QApplication::translate("explain", "3.\351\200\211\344\270\255\342\200\234Hex\346\216\245\346\224\266\342\200\235\345\217\257\344\273\245\350\216\267\345\217\226\346\234\272\346\242\260\350\207\20230003\347\253\257\345\217\243\345\256\236\346\227\266\346\225\260\346\215\256\357\274\214\344\270\215\351\200\211\344\270\255\351\273\230\350\256\244\350\216\267\345\217\22629999\347\253\257\345\217\243\346\225\260\346\215\256", 0));
        label_10->setText(QString());
        label_11->setText(QApplication::translate("explain", "\345\274\200\345\217\221\345\221\230\357\274\232*\345\261\261", 0));
        label_12->setText(QApplication::translate("explain", "QQ\357\274\2322284108152", 0));
        label_13->setText(QApplication::translate("explain", "4.\351\200\211\344\270\255\"DashBoard\345\221\275\344\273\244\345\217\221\351\200\201\"\345\217\257\351\200\232\350\277\20729999\347\253\257\345\217\243\345\217\221\346\225\260\346\215\256\357\274\214\344\270\215\351\200\211\344\270\255\351\273\230\350\256\24430001\347\253\257\345\217\243\345\217\221\346\225\260\346\215\256", 0));
    } // retranslateUi

};

namespace Ui {
    class explain: public Ui_explain {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EXPLAIN_H
