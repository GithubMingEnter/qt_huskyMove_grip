/**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
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
int frame_id=0;

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
    //需要连接硬件
    myCom = new QSerialPort();//这个在.h中定义了 QSerialPort *myCom;//声明对象
    myCom->setPortName("/dev/ttyUSB0");//串口的名称
    myCom->open(QIODevice::ReadWrite);//读写模式
    myCom->setBaudRate(QSerialPort::Baud115200);//波特率
    myCom->setBaudRate(QSerialPort::Data8);//8位数据
    myCom->setParity(QSerialPort::NoParity);//无校验位
    myCom->setStopBits(QSerialPort::OneStop);//1位停止
    myCom->setFlowControl(QSerialPort::NoFlowControl);//无数据流控制
    connect(myCom,SIGNAL(readyRead()),this,SLOT(slot_getSdata()));//连接接收的槽函数

    connect(ui.horizontalSlider_linerac,SIGNAL(valueChanged(int)),this,SLOT(slot_linear_value_change(int)));
    connect(ui.horizontalSlider_rawc,SIGNAL(valueChanged(int)),this,SLOT(slot_raw_value_change(int)));
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

    //image1
    ui.lineEdit_image_topic->setText("/usb_cam/image_raw");
    ui.lineEdit_image_topic_2->setText("/image_raw");
    ui.lineEdit_image_topic_4->setText("/kinect2/sd/image_depth");///kinect2/sd/image_depth   /kinect2/qhd/image_mono
    ui.lineEdit_image_topic_3->setText("/kinect2/qhd/image_color");
    connect(ui.displayBtn,SIGNAL(clicked()),this,SLOT(solt_subImage()));
    connect(ui.displayBtn_2,SIGNAL(clicked()),this,SLOT(solt_subImage2()));



    //init ui dashBoard
    speedX_dashBoard=new CCtrlDashBoard(ui.widget_speedX);
    speedY_dashBoard=new CCtrlDashBoard(ui.widget_speedY);

    speedX_dashBoard->setGeometry(ui.widget_speedX->rect());
    speedY_dashBoard->setGeometry(ui.widget_speedY->rect());

    speedX_dashBoard->setValue(0);
    speedY_dashBoard->setValue(0);

    ui.horizontalSlider_linerac->setValue(30);
    ui.horizontalSlider_rawc->setValue(30);

//    ui.checkGrisp->setC
    //connect
    connect(&qnode,SIGNAL(speed_vel(float,float)),this,SLOT(slot_updat_dashboard(float,float)));
    connect(&qnode,SIGNAL(mapTf(float,float,float,double,double,double)),this,SLOT(slot_update_tf(float,float,float,double,double,double)));
    //grisp
    connect(ui.grip_btnP,SIGNAL(pressed()),this,SLOT(slot_gripP()));
    connect(ui.grip_btnS,SIGNAL(clicked()),this,SLOT(slot_gripS()));
    connect(ui.horizontalScrollBar_grip,SIGNAL(valueChanged(int)),this,SLOT(slot_grip(int)));
    connect(ui.checkGrisp,SIGNAL(clicked(bool)),this,SLOT(slot_triggerGrisp()));
    //ur5
    connect(ui.btn_UR5,SIGNAL(clicked()),this,SLOT(slot_UR5()));
}
void MainWindow::slot_triggerGrisp()
{
//    if(ui.checkGrisp->isChecked())
//    {
//        ui.horizontalScrollBar_grip->setValue(glovec.thumb);

}

void MainWindow::slot_getSdata()
{
    QByteArray buf_data;            //接收数据，
    QString strGripD;

    buf_data = myCom->readAll();    //接收数据
//    ui.serialRead->setText(buf_data);
    qDebug()<<buf_data.size();
    qDebug()<<buf_data<<endl;
    if(!buf_data.isEmpty())
    {
//        if(buf_data.size()==12)
        QString strTemp=QString(buf_data);
        QString strGrip=strTemp.mid(strTemp.indexOf("\r\n"),8);
        strGripD=strGrip.mid(2,4);
//        qDebug()<<strGripD;
    }
    glovec.thumb=strGripD.toInt(&ok);
    glovec.thumb=glovec.thumb*0.22-66;//transform to scale 0-100
    if(ui.checkGrisp->isChecked())
    {
        ui.label_grip->setText(QString::number(glovec.thumb));
        qnode.gripChange(glovec.thumb*2.55);
    }
    qDebug()<<glovec.thumb;

   QThread::sleep(0.01);

//    qWarning()<<buf_data.at(3);
    //ui->ReceiveMsg_Lie->setText("");
}
void MainWindow::slot_gripP()
{
    qnode.gripP();
}
void MainWindow::slot_UR5()
{
    Ur5=new QProcess;
    QString prog="~/qt_ros/src/UR5/ur5Linux/UR5";//absoluted path
    Ur5->start(prog);
}

void MainWindow::slot_gripS()
{
    qnode.gripS();
}
void MainWindow::slot_grip(int v)
{
    ui.label_grip->setText(QString::number(v));
    qDebug()<<v*2.55;
    qnode.gripChange(v*2.55);
}

void MainWindow::slot_updat_dashboard(float x,float y)
{
    ui.label_dirx->setText(x>0?"+f":"-");
    ui.label_diry->setText(y>0?"+":"-");
    speedX_dashBoard->setValue(abs(x*100));
    speedY_dashBoard->setValue(abs(y*100));

}
void MainWindow::slot_update_tf(float x,float y,float z,double pitch,double roll,double yaw)
{
    ui.label_15->setNum(x);
    ui.label_16->setNum(y);
    ui.label_17->setNum(z);
    ui.label_18->setNum(pitch);
    ui.label_19->setNum(roll);
    ui.label_20->setNum(yaw);
}

void MainWindow::slot_update_image(int id,QImage im)
{
    qimage_mutex_.lock();
    qDebug()<<"show image1";
//    switch(id)
//    {
//     case 0:
//        ui.labelImage->setPixmap(QPixmap::fromImage(im).scaled(ui.labelImage->width(),ui.labelImage->height()));
//        break;
//     case 1:
//        ui.labelImage_2->setPixmap(QPixmap::fromImage(im).scaled(ui.labelImage_2->width(),ui.labelImage_2->height()));
//        break;
//    }
   // ui.labelImage->setPixmap(QPixmap:: fromImage(im).scaled(ui.labelImage->width(),ui.labelImage->height()));
//ui.labelImage_2->setPixmap(QPixmap::fromImage(im).scaled(ui.labelImage_2->width(),ui.labelImage_2->height()));
    ui.labelImage->setPixmap(QPixmap::fromImage(im).scaled(ui.labelImage->width(),ui.labelImage->height()));
    qimage_mutex_.unlock();


}
void MainWindow::slot_update_image2(int id,QImage im)
{
    qimage_mutex_2.lock();
    qDebug()<<"show image2";
     ui.labelImage_2->setPixmap(QPixmap::fromImage(im).scaled(ui.labelImage_2->width(),ui.labelImage_2->height()));
    qimage_mutex_2.unlock();
}
void MainWindow::slot_update_image3(int id,QImage im)
{
    qimage_mutex_3.lock();
    //qDebug()<<"show image3";
     ui.labelImage_3->setPixmap(QPixmap::fromImage(im).scaled(ui.labelImage_3->width(),ui.labelImage_3->height()));
    qimage_mutex_3.unlock();
}
void MainWindow::slot_update_image4(int id,QImage im)
{
    qdepth_mutex_.lock();
    //qDebug()<<"show image4";
     ui.labelImage_4->setPixmap(QPixmap::fromImage(im).scaled(ui.labelImage_4->width(),ui.labelImage_4->height()));
    qdepth_mutex_.unlock();
}
void MainWindow::initVideos()
{
//    if(ui.lineEdit_image_topic->text()!="")
//    if(ui.lineEdit_image_topic_2->text()!="")
//    qnode.subImage2(ui.lineEdit_image_topic->text(),0);
   // qnode.subImage(ui.lineEdit_image_topic_2->text(),2);
    //qnode.subImage2(ui.lineEdit_image_topic->text(),0);
    qnode.subImage3(ui.lineEdit_image_topic_3->text(),1);
    qnode.subImage4(ui.lineEdit_image_topic_4->text(),3);//no 3
    qnode.subFindTf("objectsStamped");
   // connect(&qnode,SIGNAL(image_vel(int,QImage)),this,SLOT(slot_update_image(int,QImage)));
   // connect(&qnode,SIGNAL(image_vel2(int,QImage)),this,SLOT(slot_update_image2(int,QImage)));
    connect(&qnode,SIGNAL(image_vel3(int,QImage)),this,SLOT(slot_update_image3(int,QImage)));
    connect(&qnode,SIGNAL(image_vel4(int,QImage)),this,SLOT(slot_update_image4(int,QImage)));

}

void MainWindow::solt_subImage()
{
    qnode.subImage(ui.lineEdit_image_topic->text(),0);
}
void MainWindow::solt_subImage2()
{
    frame_id=1;
    qnode.subImage2(ui.lineEdit_image_topic_2->text(),2);

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
    float linear=ui.label_linerac->text().toFloat()*0.01;
    float angular=ui.label_rawc->text().toFloat()*0.01;
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
    ui.label_linerac->setText(QString::number(value));
}
void MainWindow::slot_raw_value_change(int value)
{
    ui.label_rawc->setText(QString::number(value));
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
    if ( ui.checkbox_use_environment->isChecked() ) {//use enviroment
        if ( !qnode.init() ) {
            showNoMasterMessage();
        } else {
            ui.button_connect->setEnabled(false);
                initVideos();
                qDebug()<<"tr2";
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
            initVideos();
            qDebug()<<"tr1";
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
//    initVideos();
//    qDebug()<<"tr2";
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

