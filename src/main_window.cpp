/**
 * @file /src/main_window.cpp
 *
 * @brief Implementation for the qt gui.
 *
 * @date February 2011
 **/
/*****************************************************************************
** Includes
*****************************************************************************/

#include <QtGui>
#include <QMessageBox>
#include <iostream>
#include "../include/class1_ros_qt_demo/main_window.hpp"

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace class1_ros_qt_demo {

using namespace Qt;

/*****************************************************************************
** Implementation [MainWindow]
*****************************************************************************/

MainWindow::MainWindow(int argc, char** argv, QWidget *parent)
	: QMainWindow(parent)
	, qnode(argc,argv)
{
	ui.setupUi(this); // Calling this incidentally connects all ui's triggers to on_...() callbacks in this class.
    QObject::connect(ui.actionAbout_Qt, SIGNAL(triggered(bool)), qApp, SLOT(aboutQt())); // qApp is a global variable for the application

    ReadSettings();
	setWindowIcon(QIcon(":/images/icon.png"));
	ui.tab_manager->setCurrentIndex(0); // ensure the first tab is showing - qt-designer should have this already hardwired, but often loses it (settings?).
    QObject::connect(&qnode, SIGNAL(rosShutdown()), this, SLOT(close()));

	/*********************
	** Logging
	**********************/
	ui.view_logging->setModel(qnode.loggingModel());
    QObject::connect(&qnode, SIGNAL(loggingUpdated()), this, SLOT(updateLoggingView()));

    /*********************
    ** Auto Start
    **********************/
    if ( ui.checkbox_remember_settings->isChecked() ) {
        on_button_connect_clicked(true);
    }
    connect(ui.horizontalSlider_linera,SIGNAL(valueChanged(int)),this,SLOT(slot_linear_value_change(int)));
    connect(ui.horizontalSlider_raw,SIGNAL(valueChanged(int)),this,SLOT(slot_raw_value_change(int)));
    connect(ui.keyI,SIGNAL(clicked()),this,SLOT(slot_pushbtn_click()));
    connect(ui.keyO,SIGNAL(clicked()),this,SLOT(slot_pushbtn_click()));
    connect(ui.keyL,SIGNAL(clicked()),this,SLOT(slot_pushbtn_click()));
    connect(ui.keyDot,SIGNAL(clicked()),this,SLOT(slot_pushbtn_click()));
    connect(ui.keyDotc,SIGNAL(clicked()),this,SLOT(slot_pushbtn_click()));
    connect(ui.keyM,SIGNAL(clicked()),this,SLOT(slot_pushbtn_click()));
    connect(ui.keyJ,SIGNAL(clicked()),this,SLOT(slot_pushbtn_click()));
    connect(ui.keyU,SIGNAL(clicked()),this,SLOT(slot_pushbtn_click()));

    //terminal
    connect(ui.Enter_btn,SIGNAL(clicked()),this,SLOT(slot_quick_cmd_clicked()));

    //image
    connect(&qnode,SIGNAL(image_vel(QImage)),this,SLOT(slot_update_image(QImage)));
    connect(ui.displayBtn,SIGNAL(clicked()),this,SLOT(solt_subImage()));

    //init ui
    speedX_dashBoard=new CCtrlDashBoard(ui.widget_speedX);
    speedY_dashBoard=new CCtrlDashBoard(ui.widget_speedY);

    speedX_dashBoard->setGeometry(ui.widget->rect());
    speedY_dashBoard->setGeometry(ui.widget->rect());

    speedX_dashBoard->setValue(0);
    speedY_dashBoard->setValue(0);

    ui.horizontalSlider_linera->setValue(30);
    ui.horizontalSlider_raw->setValue(30);
    //connect
    connect(&qnode,SIGNAL(speed_vel(float,float)),this,SLOT(slot_updat_dashboard(float,float)));

}
void MainWindow::slot_updat_dashboard(float x,float y)
{
    ui.label_dirx->setText(x>0?"+":"-");
    ui.label_diry->setText(y>0?"+":"-");
    speedX_dashBoard->setValue(abs(x*100));
    speedY_dashBoard->setValue(abs(y*100));

}

void MainWindow::slot_update_image(QImage im)
{
    ui.labelImage->setPixmap(QPixmap::fromImage(im));
}
void MainWindow::solt_subImage()
{
    qnode.subImage(ui.lineEdit_image_topic->text());
}

void MainWindow::slot_quick_cmd_clicked()
{
    cmd=new QProcess;
    cmd->start("bash");
    cmd->write(ui.textEditCmd->toPlainText().toLocal8Bit()+'\n');
    connect(cmd,SIGNAL(readyReadStandardError()),this,SLOT(slot_quick_cmd_output()));
    connect(cmd,SIGNAL(readyReadStandardOutput()),this,SLOT(slot_quick_cmd_output()));
}
void MainWindow::slot_quick_cmd_output()
{
    ui.textEditInfo->append("<font color=\"#FF0000\">"+cmd->readAllStandardError()+"</font>");
    ui.textEditInfo->append("<font color=\"#FFFFFF\">"+cmd->readAllStandardOutput()+"</font>");

}

void MainWindow::slot_pushbtn_click()
{
    QPushButton* btn=qobject_cast<QPushButton*>(sender());
    qDebug()<<btn->text();
    char key=btn->text().toStdString()[0];//transform data type
    bool is_all=ui.AnyCheck->isChecked();
    float linear=ui.label_linera->text().toFloat()*0.01;
    float angular=ui.label_raw->text().toFloat()*0.01;
    switch(key){
    case 'i':
        qnode.set_cmd_vel(is_all?'I':'i',linear,angular);
        break;
    case 'u':
        qnode.set_cmd_vel(is_all?'U':'u',linear,angular);
        break;
      case 'o':
        qnode.set_cmd_vel(is_all?'O':'o',linear,angular);
        break;
      case 'j':
        qnode.set_cmd_vel(is_all?'J':'j',linear,angular);
        break;
      case 'l':
        qnode.set_cmd_vel(is_all?'L':'l',linear,angular);
        break;
      case 'm':
        qnode.set_cmd_vel(is_all?'M':'m',linear,angular);
        break;
      case ',':
        qnode.set_cmd_vel(is_all?'<':',',linear,angular);
        break;
      case '.':
        qnode.set_cmd_vel(is_all?'>':'.',linear,angular);
        break;


    }
}

void MainWindow::slot_linear_value_change(int value)
{
    ui.label_linera->setText(QString::number(value));
}
void MainWindow::slot_raw_value_change(int value)
{
    ui.label_raw->setText(QString::number(value));
}
MainWindow::~MainWindow() {}

/*****************************************************************************
** Implementation [Slots]
*****************************************************************************/

void MainWindow::showNoMasterMessage() {
	QMessageBox msgBox;
	msgBox.setText("Couldn't find the ros master.");
	msgBox.exec();
    close();
}

/*
 * These triggers whenever the button is clicked, regardless of whether it
 * is already checked or not.
 */

void MainWindow::on_button_connect_clicked(bool check ) {
	if ( ui.checkbox_use_environment->isChecked() ) {
		if ( !qnode.init() ) {
			showNoMasterMessage();
		} else {
			ui.button_connect->setEnabled(false);
		}
	} else {
		if ( ! qnode.init(ui.line_edit_master->text().toStdString(),
				   ui.line_edit_host->text().toStdString()) ) {
			showNoMasterMessage();
		} else {
			ui.button_connect->setEnabled(false);
			ui.line_edit_master->setReadOnly(true);
			ui.line_edit_host->setReadOnly(true);
			ui.line_edit_topic->setReadOnly(true);
		}
	}
}


void MainWindow::on_checkbox_use_environment_stateChanged(int state) {
	bool enabled;
	if ( state == 0 ) {
		enabled = true;
	} else {
		enabled = false;
	}
	ui.line_edit_master->setEnabled(enabled);
	ui.line_edit_host->setEnabled(enabled);
	//ui.line_edit_topic->setEnabled(enabled);
}

/*****************************************************************************
** Implemenation [Slots][manually connected]
*****************************************************************************/

/**
 * This function is signalled by the underlying model. When the model changes,
 * this will drop the cursor down to the last line in the QListview to ensure
 * the user can always see the latest log message.
 */
void MainWindow::updateLoggingView() {
        ui.view_logging->scrollToBottom();
}

/*****************************************************************************
** Implementation [Menu]
*****************************************************************************/

void MainWindow::on_actionAbout_triggered() {
    QMessageBox::about(this, tr("About ..."),tr("<h2>PACKAGE_NAME Test Program 0.10</h2><p>Copyright Yujin Robot</p><p>This package needs an about description.</p>"));
}

/*****************************************************************************
** Implementation [Configuration]
*****************************************************************************/

void MainWindow::ReadSettings() {
    QSettings settings("Qt-Ros Package", "class1_ros_qt_demo");
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());
    QString master_url = settings.value("master_url",QString("http://192.168.1.2:11311/")).toString();
    QString host_url = settings.value("host_url", QString("192.168.1.3")).toString();
    //QString topic_name = settings.value("topic_name", QString("/chatter")).toString();
    ui.line_edit_master->setText(master_url);
    ui.line_edit_host->setText(host_url);
    //ui.line_edit_topic->setText(topic_name);
    bool remember = settings.value("remember_settings", false).toBool();
    ui.checkbox_remember_settings->setChecked(remember);
    bool checked = settings.value("use_environment_variables", false).toBool();
    ui.checkbox_use_environment->setChecked(checked);
    if ( checked ) {
    	ui.line_edit_master->setEnabled(false);
    	ui.line_edit_host->setEnabled(false);
    	//ui.line_edit_topic->setEnabled(false);
    }
}

void MainWindow::WriteSettings() {
    QSettings settings("Qt-Ros Package", "class1_ros_qt_demo");
    settings.setValue("master_url",ui.line_edit_master->text());
    settings.setValue("host_url",ui.line_edit_host->text());
    //settings.setValue("topic_name",ui.line_edit_topic->text());
    settings.setValue("use_environment_variables",QVariant(ui.checkbox_use_environment->isChecked()));
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
    settings.setValue("remember_settings",QVariant(ui.checkbox_remember_settings->isChecked()));

}

void MainWindow::closeEvent(QCloseEvent *event)
{
	WriteSettings();
	QMainWindow::closeEvent(event);
}

}  // namespace class1_ros_qt_demo

