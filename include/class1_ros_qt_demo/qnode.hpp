/**
 * @file /include/class1_ros_qt_demo/qnode.hpp
 *
 * @brief Communications central!
 *
 * @date February 2011
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef class1_ros_qt_demo_QNODE_HPP_
#define class1_ros_qt_demo_QNODE_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/

// To workaround boost/qt4 problems that won't be bugfixed. Refer to
//    https://bugreports.qt.io/browse/QTBUG-22829
#ifndef Q_MOC_RUN
#include <ros/ros.h>
#endif
#include <string>
#include <QThread>
#include <QApplication>
#include <QEventLoop>
#include<QTime>
#include<QStringListModel>
#include<std_msgs/String.h>
#include<geometry_msgs/Twist.h>
#include<map>
#include<std_msgs/Float32.h>
#include<image_transport/image_transport.h>
#include<cv_bridge/cv_bridge.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include<sensor_msgs/image_encodings.h>//encode type
#include<QImage>
//#include"main_window.hpp"
#include<nav_msgs/Odometry.h>
#include<tf/transform_listener.h>
#include<message_filters/subscriber.h>
#include<message_filters/time_synchronizer.h>
#include<opencv2/opencv.hpp>
#include<QTransform>
#include<image_transport/image_transport.h>
#include<image_transport/subscriber_filter.h>
#include<find_object_2d/ObjectsStamped.h>
#include <QtCore/QString>
#include<QDebug>
#include <QtWidgets/QMainWindow>
#include "ui_main_window.h"

#include "Robotiq2FGripper_robot_output.h"
#include "Robotiq2FGripper_robot_input.h"
/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace class1_ros_qt_demo {

/*****************************************************************************
** Class
*****************************************************************************/

class QNode : public QThread {
    Q_OBJECT
public:
        QNode(int argc, char** argv );
        virtual ~QNode();
        bool init();
        bool init(const std::string &master_url, const std::string &host_url);
        void run();
        void set_cmd_vel(char key,float linear,float angular);

        void subImage(QString topicName,int id);
        void subImage2(QString topicName2,int id);
        void subImage3(QString topicName,int id);
        void subImage4(QString topicName,int id);

        void subFindTf(QString topicName);
        void sleep(unsigned int  msec);
        void gripP();
        void gripS();
        void gripChange(int v);
        /*********************
        ** Logging
        **********************/
        enum LogLevel {
                 Debug,
                 Info,
                 Warn,
                 Error,
                 Fatal
         };
        QImage depth;
        QStringListModel* loggingModel() { return &logging_model; }
        void log( const LogLevel &level, const std::string &msg);

Q_SIGNALS:
    void loggingUpdated();
    void rosShutdown();
    void image_vel(int,QImage);
    void image_vel2(int,QImage);
    void image_vel3(int,QImage);
    void image_vel4(int,QImage);

    void speed_vel(float,float);
    void mapTf(float,float,float,double,double,double);
    //by rosmsg show nav_msgs/Odometry
private:
        Ui::MainWindowDesign ui;
        int init_argc;
        char** init_argv;
        cv::Mat dph;

        ros::Publisher chatter_publisher;
        ros::Publisher cmd_vel_pub;

        ros::Subscriber sub;
        std::string mapFrameId="/map";
        std::string objFramePrefix="object";
//        mapFrameId
//        objFramePrefix
        image_transport::Subscriber image_sub;
        image_transport::Subscriber image_sub2;
        image_transport::Subscriber image_sub3;
        image_transport::Subscriber image_sub4;

        ros::Publisher pub;
        ros::Subscriber odom_sub;//note no Publisher
    QStringListModel logging_model;
    ros::Subscriber chatter_sub;//change
    void chatter_callback(const std_msgs::String &msg);// transform const type
    //note include head file
    void image_callback(const sensor_msgs::ImageConstPtr &msg);
    void image_callback_compressed(const sensor_msgs::CompressedImageConstPtr &msg);
    void image_callback2(const sensor_msgs::ImageConstPtr &msg);
    void image_callback3(const sensor_msgs::ImageConstPtr &msg);
    void image_callback4(const sensor_msgs::ImageConstPtr &msg);
    void myCallback_depth(const sensor_msgs::ImageConstPtr &msg);
    void objectsDetectedCallback1(const find_object_2d::ObjectsStampedConstPtr & msg);

    QImage Mat2QImage(cv::Mat const& src);

    void odom_callback(const nav_msgs::Odometry &msg);

};

}  // namespace class1_ros_qt_demo

#endif /* class1_ros_qt_demo_QNODE_HPP_ */
