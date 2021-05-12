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
#include <QStringListModel>
#include <std_msgs/String.h>
#include<geometry_msgs/Twist.h>
#include<map>
#include<std_msgs/Float32.h>
#include<image_transport/image_transport.h>
#include<cv_bridge/cv_bridge.h>
#include<sensor_msgs/image_encodings.h>//encode type
#include<QImage>

#include<nav_msgs/Odometry.h>
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

        void subImage(QString topicName);
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

	QStringListModel* loggingModel() { return &logging_model; }
	void log( const LogLevel &level, const std::string &msg);

Q_SIGNALS:
	void loggingUpdated();
    void rosShutdown();
    void image_vel(QImage);
    void speed_vel(float,float);
    //by rosmsg show nav_msgs/Odometry
private:
	int init_argc;
	char** init_argv;
	ros::Publisher chatter_publisher;
        ros::Publisher cmd_vel_pub;
        image_transport::Subscriber image_sub;

        ros::Subscriber odom_sub;//note no Publisher
    QStringListModel logging_model;
    ros::Subscriber chatter_sub;//change
    void chatter_callback(const std_msgs::String &msg);// transform const type
    //note include head file
    void image_callback(const sensor_msgs::ImageConstPtr &msg);
    QImage Mat2QImage(cv::Mat const& src);

    void odom_callback(const nav_msgs::Odometry &msg);

};

}  // namespace class1_ros_qt_demo

#endif /* class1_ros_qt_demo_QNODE_HPP_ */
