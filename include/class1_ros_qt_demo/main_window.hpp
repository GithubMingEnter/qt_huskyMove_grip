/**
 * @file /include/class1_ros_qt_demo/main_window.hpp
 *
 * @brief Qt based gui for class1_ros_qt_demo.
 *
 * @date November 2010
 **/
#ifndef class1_ros_qt_demo_MAIN_WINDOW_H
#define class1_ros_qt_demo_MAIN_WINDOW_H

/*****************************************************************************
** Includes
*****************************************************************************/

#include <QtWidgets/QMainWindow>
#include "ui_main_window.h"
#include "qnode.hpp"
#include "CCtrlDashBoard.h"
#include<QProcess>
#include "Robotiq2FGripper_robot_output.h"
#include "Robotiq2FGripper_robot_input.h"

#include <QStandardItemModel>
#include <QTreeWidgetItem>
#include <QSoundEffect>
#include <QComboBox>
#include <QSpinBox>
#include <QVariant>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTimer>
#include <QQueue>
#include <map>
#include <QSettings>
#include <QMutex>//added
#include <QSerialPort>        //提供访问串口的功能
#include <QSerialPortInfo>    //提供系统中存在的串口的信息
#include <QDebug>
#include <QThread>
/*****************************************************************************
** Namespace
*****************************************************************************/

namespace class1_ros_qt_demo {

/*****************************************************************************
** Interface [MainWindow]
*****************************************************************************/
/**
 * @brief Qt central, all operations relating to the view part here.
 */
typedef  struct{
        int p;
        int t;
        int thumb;
        int fore_finger;
        int middle;
        int ring;
        int  little;

}glove;
class MainWindow : public QMainWindow {
Q_OBJECT

public:
        MainWindow(int argc, char** argv, QWidget *parent = 0);
        ~MainWindow();

        void ReadSettings(); // Load up qt program settings at startup
        void WriteSettings(); // Save qt program settings when closing

        void closeEvent(QCloseEvent *event); // Overloaded function
        void showNoMasterMessage();
        void initVideos();
public Q_SLOTS:
        /******************************************
        ** Auto-connections (connectSlotsByName())
        *******************************************/
        void on_actionAbout_triggered();
        void on_button_connect_clicked(bool check );
        void on_checkbox_use_environment_stateChanged(int state);
        void slot_linear_value_change(int value);
        void slot_raw_value_change(int value);
        void slot_pushbtn_click();
        void slot_quick_cmd_clicked();
        void slot_quick_cmd_output();
        void slot_update_image(int,QImage);
        void slot_update_image2(int,QImage);
        void slot_update_image3(int,QImage);
        void slot_update_image4(int,QImage);

        void solt_subImage();
        void solt_subImage2();
        void slot_update_tf(float x,float y,float z,double pitch,double roll,double yaw);
        void slot_updat_dashboard(float x,float y);

        void slot_gripP();
        void slot_gripS();

        void slot_grip(int v);

        void slot_UR5();

         void slot_getSdata();//get serial port data
         void slot_triggerGrisp();
        /******************************************
    ** Manual connections
    *******************************************/
    void updateLoggingView(); // no idea why this can't connect automatically

private:
        Ui::MainWindowDesign ui;
        QNode qnode;
        QProcess *cmd;
        QProcess *Ur5;
        CCtrlDashBoard* speedX_dashBoard;
        CCtrlDashBoard* speedY_dashBoard;
        mutable QMutex qimage_mutex_;//added
        mutable QMutex qimage_mutex_2;//added
        mutable QMutex qimage_mutex_3;//added


        QImage qimage_;//added
        QImage qimage_2;//added


        mutable QMutex qdepth_mutex_;
       QSerialPort *myCom;

        glove glovec;
        bool ok;//judge if convert 
};

}  // namespace class1_ros_qt_demo

#endif // class1_ros_qt_demo_MAIN_WINDOW_H
