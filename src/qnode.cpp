/**
 * @file /src/qnode.cpp
 *
 * @brief Ros communication central!
 *
 * @date February 2011
 **/

/*****************************************************************************
** Includes
*****************************************************************************/

#include <ros/ros.h>
#include <ros/network.h>
#include <string>
#include <std_msgs/String.h>
#include <sstream>
#include "../include/class1_ros_qt_demo/qnode.hpp"
#include "../include/class1_ros_qt_demo/main_window.hpp"


/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace class1_ros_qt_demo {

/*****************************************************************************
** Implementation
*****************************************************************************/
float t = 2.0;
QNode::QNode(int argc, char** argv ) :
    init_argc(argc),
    init_argv(argv)
    {

}

QNode::~QNode() {
    if(ros::isStarted()) {
      ros::shutdown(); // explicitly needed since we use ros::start();
      ros::waitForShutdown();
    }
    wait();
}

bool QNode::init() {
    ros::init(init_argc,init_argv,"class1_ros_qt_demo");
    if ( ! ros::master::check() ) {
        return false;
    }
    ros::start(); // explicitly needed since our nodehandle is going out of scope.
    ros::NodeHandle n;

    pub=n.advertise<class1_ros_qt_demo::Robotiq2FGripper_robot_output>
            ("Robotiq2FGripperRobotOutput", 1000);
    class1_ros_qt_demo::Robotiq2FGripper_robot_output command;
        command.rACT = 0;
        command.rGTO = 0;
        command.rATR = 0;
        command.rPR = 0;
        command.rSP = 0;
        command.rFR = 0;

        pub.publish(command);
        ros::Duration(1).sleep();

        command.rACT = 1;
        command.rGTO = 1;
        command.rATR = 0;
        command.rPR = 0;
        command.rSP = 255;
        command.rFR = 150;
        pub.publish(command);
        ros::Duration(t).sleep();

    // Add your ros communications here.
    chatter_publisher = n.advertise<std_msgs::String>("chatter", 1000);
   // chatter_sub=n.subscribe("chatter",1000,&QNode::chatter_callback,this);//change
    //second initial function also edits
    cmd_vel_pub=n.advertise<geometry_msgs::Twist>("cmd_vel",1000);

    odom_sub=n.subscribe("/husky_velocity_controller/odom",1000,&QNode::odom_callback,this);
    ///husky_velocity_controller/odom need to change according topic
    ros::NodeHandle pnh("~");
    pnh.param("map_frame_id",mapFrameId,mapFrameId);
    pnh.param("object_prefix",objFramePrefix,objFramePrefix);

        //ros::NodeHandle nh;
    sub=n.subscribe("objectsStamped",1000,&QNode::objectsDetectedCallback1,this);

    start();
    return true;
}

bool QNode::init(const std::string &master_url, const std::string &host_url) {
    std::map<std::string,std::string> remappings;
    remappings["__master"] = master_url;
    remappings["__hostname"] = host_url;
    ros::init(remappings,"class1_ros_qt_demo");
    if ( ! ros::master::check() ) {
        return false;
    }
    ros::start(); // explicitly needed since our nodehandle is going out of scope.
    ros::NodeHandle n;

    pub=n.advertise<class1_ros_qt_demo::Robotiq2FGripper_robot_output>
            ("Robotiq2FGripperRobotOutput", 1000);
    class1_ros_qt_demo::Robotiq2FGripper_robot_output command;
        command.rACT = 0;
        command.rGTO = 0;
        command.rATR = 0;
        command.rPR = 0;
        command.rSP = 0;
        command.rFR = 0;

        pub.publish(command);
        ros::Duration(1).sleep();

        command.rACT = 1;
        command.rGTO = 1;
        command.rATR = 0;
        command.rPR = 0;
        command.rSP = 255;
        command.rFR = 150;
        pub.publish(command);
        ros::Duration(t).sleep();

    ros::NodeHandle pnh("~");
    pnh.param("map_frame_id",mapFrameId,mapFrameId);
    pnh.param("object_prefix",objFramePrefix,objFramePrefix);

        //ros::NodeHandle nh;
    sub=n.subscribe("objectsStamped",1000,&QNode::objectsDetectedCallback1,this);
    // Add your ros communications here.
    chatter_publisher = n.advertise<std_msgs::String>("chatter", 1000);
   // chatter_sub=n.subscribe("chatter",1000,&QNode::chatter_callback,this);//change
    //topic name, length, callback_function,
    cmd_vel_pub=n.advertise<geometry_msgs::Twist>("cmd_vel",1000);

    odom_sub=n.subscribe("/husky_velocity_controller/odom",1000,&QNode::odom_callback,this);
   //raw_odom according situation
    start();
    return true;
}
void QNode::odom_callback(const nav_msgs::Odometry &msg)
{
    //emit info to mainwidow for speed
    emit speed_vel(msg.twist.twist.linear.x,msg.twist.twist.linear.y);
}


void  QNode::gripChange(int v)
{
    ros::NodeHandle n;
    pub=n.advertise<class1_ros_qt_demo::Robotiq2FGripper_robot_output>("Robotiq2FGripperRobotOutput", 1000);
    class1_ros_qt_demo::Robotiq2FGripper_robot_output command;

    command.rACT = 1;
    command.rGTO = 1;
    command.rATR = 0;
    command.rPR = v;
    command.rSP = 255;
    command.rFR = 150;

    pub.publish(command);
}
void QNode::gripP()
{
    ros::NodeHandle n;
    int v=0;
    v+=5;
    pub=n.advertise<class1_ros_qt_demo::Robotiq2FGripper_robot_output>
            ("Robotiq2FGripperRobotOutput", 1000);
    class1_ros_qt_demo::Robotiq2FGripper_robot_output command;

        command.rACT = 1;
        command.rGTO = 1;
        command.rATR = 0;
        command.rPR = v;
        command.rSP = 255;
        command.rFR = 150;

        pub.publish(command);
       // ros::Duration(1).sleep();
}
void QNode::gripS()
{
    ros::NodeHandle n;
    pub=n.advertise<class1_ros_qt_demo::Robotiq2FGripper_robot_output>
            ("Robotiq2FGripperRobotOutput", 1000);
    class1_ros_qt_demo::Robotiq2FGripper_robot_output command;

        command.rACT = 1;
        command.rGTO = 1;
        command.rATR = 0;
        command.rPR = 50;
        command.rSP = 255;
        command.rFR = 150;

        pub.publish(command);
       // ros::Duration(1).sleep();
}

void QNode::subImage(QString topicName,int id)
{

    ros::NodeHandle nh;
    image_transport::ImageTransport it_(nh);
    image_sub=it_.subscribe(topicName.toStdString(),1000,&QNode::image_callback,this);
   // ros::spinOnce();

}
void QNode::subImage2(QString topicName2,int id)
{

    ros::NodeHandle nh;
    image_transport::ImageTransport it_(nh);
    image_sub2=it_.subscribe(topicName2.toStdString(),1000,&QNode::image_callback2,this);
    //ros::spinOnce();  don't add ,it will lock thread and loop

}
void QNode::subImage3(QString topicName2,int id)
{

    ros::NodeHandle nh;
    image_transport::ImageTransport it_(nh);
    image_sub3=it_.subscribe(topicName2.toStdString(),1000,&QNode::image_callback3,this);
    //ros::spinOnce();  don't add ,it will lock thread and loop

}
void QNode::subImage4(QString topicName2,int id)
{

    ros::NodeHandle nh;
    image_transport::ImageTransport it_(nh);
    image_sub4=it_.subscribe(topicName2.toStdString(),1000,&QNode::myCallback_depth,this);
    //ros::spinOnce();  don't add ,it will lock thread and loop

}
void QNode::image_callback(const sensor_msgs::ImageConstPtr &msg)
{
    cv_bridge::CvImagePtr cv_ptr;
    try
    {
        cv_ptr=cv_bridge::toCvCopy(msg,msg->encoding);
        QImage im=Mat2QImage(cv_ptr->image);
        emit image_vel(0,im);
    }//change
    catch (cv_bridge::Exception& e)
    {

      log(Error,("video frame0 exception: "+QString(e.what())).toStdString());
      return;
    }
}
void QNode::image_callback2(const sensor_msgs::ImageConstPtr &msg)
{
    cv_bridge::CvImagePtr cv_ptr;
    try
    {
        cv_ptr=cv_bridge::toCvCopy(msg,msg->encoding);
        QImage im=Mat2QImage(cv_ptr->image);
        emit image_vel2(0,im);
    }//change
    catch (cv_bridge::Exception& e)
    {

      log(Error,("video frame2 exception: "+QString(e.what())).toStdString());
      return;
    }
}
void QNode::image_callback3(const sensor_msgs::ImageConstPtr &msg)
{
    cv_bridge::CvImagePtr cv_ptr;
    try
    {
        cv_ptr=cv_bridge::toCvCopy(msg,msg->encoding);
        QImage im=Mat2QImage(cv_ptr->image);
        emit image_vel3(0,im);
    }//change
    catch (cv_bridge::Exception& e)
    {

      log(Error,("video frame3 exception: "+QString(e.what())).toStdString());
      return;
    }
}
//void QNode::image_callback4(const sensor_msgs::ImageConstPtr &msg)
//{
//    cv_bridge::CvImagePtr cv_ptr;
//    try
//    {
//        cv_ptr=cv_bridge::toCvCopy(msg,msg->encoding);
//        QImage im=Mat2QImage(cv_ptr->image);
//        emit image_vel4(0,im);
//        //sleep(100);
//        //cv::waitKey(100);
//    }//change
//    catch (cv_bridge::Exception& e)
//    {

//      log(Error,("video frame4 exception: "+QString(e.what())).toStdString());
//      return;
//    }
//}
QImage QNode::Mat2QImage(cv::Mat const& src)
{
  QImage dest(src.cols, src.rows, QImage::Format_ARGB32);

  const float scale = 255.0;

  if (src.depth() == CV_8U) {
    if (src.channels() == 1) {
      for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
          int level = src.at<quint8>(i, j);
          dest.setPixel(j, i, qRgb(level, level, level));
        }
      }
    } else if (src.channels() == 3) {
      for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
          cv::Vec3b bgr = src.at<cv::Vec3b>(i, j);
          dest.setPixel(j, i, qRgb(bgr[2], bgr[1], bgr[0]));
        }
      }
    }
  } else if (src.depth() == CV_32F) {
    if (src.channels() == 1) {
      for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
          int level = scale * src.at<float>(i, j);
          dest.setPixel(j, i, qRgb(level, level, level));
        }
      }
    } else if (src.channels() == 3) {
      for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
          cv::Vec3f bgr = scale * src.at<cv::Vec3f>(i, j);
          dest.setPixel(j, i, qRgb(bgr[2], bgr[1], bgr[0]));
        }
      }
    }
  }

  return dest;
}
void QNode::sleep(unsigned int msec)  //msec为毫秒
{
    //currnentTime 返回当前时间 用当前时间加上我们要延时的时间msec得到一个新的时刻
        QTime reachTime = QTime::currentTime().addMSecs(msec);
        //用while循环不断比对当前时间与我们设定的时间
        while(QTime::currentTime()<reachTime){
        //如果当前的系统时间尚未达到我们设定的时刻，就让Qt的应用程序类执行默认的处理，
        //以使程序仍处于响应状态。一旦到达了我们设定的时刻，就跳出该循环，继续执行后面的语句。
            QApplication::processEvents(QEventLoop::AllEvents,100);
        }
}


void QNode::myCallback_depth(const sensor_msgs::ImageConstPtr &msg)
{
//    if(grasp_state)
//    {
        cv_bridge::CvImageConstPtr cv_ptrDEPTH;
        try
        {
            cv_ptrDEPTH = cv_bridge::toCvCopy(msg);
            dph = cv_ptrDEPTH->image;

    //        depthnearestFiltering(dph);
           unsigned short max_depth;

           uchar p_depth_argb[424*512*4];
           int idx = 0;
            //find depth max
           for (int x = 0; x < dph.rows; x++) {
               for (int y = 0; y < dph.cols; y++) {
                    if(dph.at<unsigned short>(x,y) > max_depth)
                        max_depth = dph.at<unsigned short>(x,y);
               }
           }

           for (int x = 0; x < dph.rows; x++) {
               for (int y = 0; y < dph.cols; y++) {
           //一定要使用1.0f相乘，转换成float类型，否则该工程的结果会有错误,因为这个要么是0，要么是1，0的概率要大很多
                       float fscale = 1.0f*(dph.at<unsigned short>(x,y))/max_depth;
                       if(dph.at<unsigned short>(x,y) != 0) {
                           p_depth_argb[idx++] = 255*(1-fscale);    //蓝色分量
                           p_depth_argb[idx++] = 0; //绿色分量
                           p_depth_argb[idx++] = 255*fscale;   //红色分量，越远越红
                           p_depth_argb[idx++] = 255*(1-fscale); //距离越近，越不透明
                       }
                       else {
                           p_depth_argb[idx++] = 0;
                           p_depth_argb[idx++] = 0;
                           p_depth_argb[idx++] = 0;
                           p_depth_argb[idx++] = 255;
                       }
                }
           }
            depth = QImage(p_depth_argb, dph.cols , dph.rows,  QImage::Format_ARGB32);//change  to QImage format
            emit image_vel4(0,depth);
            //cv::waitKey(100);
        }
        catch (cv_bridge::Exception& e)
        {
            ROS_ERROR("cv_bridge exception: %s", e.what());
            return;
        }


}
void QNode::set_cmd_vel(char key,float linear,float angular)
{
    std::map<char, std::vector<float>> moveBindings
    {
      {'i', {1, 0, 0, 0}},
      {'o', {1, 0, 0, -1}},
      {'j', {0, 0, 0, 1}},
      {'l', {0, 0, 0, -1}},
      {'u', {1, 0, 0, 1}},
      {',', {-1, 0, 0, 0}},
      {'.', {-1, 0, 0, 1}},
      {'m', {-1, 0, 0, -1}},
      {'O', {1, -1, 0, 0}},
      {'I', {1, 0, 0, 0}},
      {'J', {0, 1, 0, 0}},
      {'L', {0, -1, 0, 0}},
      {'U', {1, 1, 0, 0}},
      {'<', {-1, 0, 0, 0}},
      {'>', {-1, -1, 0, 0}},
      {'M', {-1, 1, 0, 0}},
      {'t', {0, 0, 1, 0}},
      {'b', {0, 0, -1, 0}},
      {'k', {0, 0, 0, 0}},
      {'K', {0, 0, 0, 0}}
    };
    char keyN=key;
    // Grab the direction data
    int x = moveBindings[keyN][0];
    int y = moveBindings[keyN][1];
    int z = moveBindings[keyN][2];
    int th = moveBindings[keyN][3];

    geometry_msgs::Twist twist;
    twist.linear.x=x*linear;
    twist.linear.y=y*linear;
    twist.linear.z=z*linear;

    twist.angular.x=0;
    twist.angular.y=0;
    twist.angular.z=th*angular;

    cmd_vel_pub.publish(twist);
}
void QNode::chatter_callback(const std_msgs::String &msg)// transform const type
{
    log(Info,"I recive"+msg.data);
}


void QNode::subFindTf(QString topicName)
{




}

void QNode::objectsDetectedCallback1(const find_object_2d::ObjectsStampedConstPtr & msg)
{
    tf::TransformListener tfListener;
    ros::Duration(0.4).sleep();
    tf::StampedTransform pose;
    tf::StampedTransform poseCam;
    double roll,pitch,yaw;
    if(msg->objects.data.size())
    {


        for(unsigned int i=0;i<msg->objects.data.size();i+=12)
        {
            //ros::Duration(10.0).sleep();

            //获取数据
            int id = (int)msg->objects.data[i];
            // "object_1", "object_2"
            std::string objectFrameId=QString("%1_%2").arg(objFramePrefix.c_str()).arg(id).toStdString();

//            try{
//              ros::Time now = ros::Time::now();
//              listener.waitForTransform("/turtle2", "/turtle1",
//                                        now, ros::Duration(1.0));
//              listener.lookupTransform("/turtle2", "/turtle1",
//                                       now, transform);
            try
            {
                ros::Time now = ros::Time::now();
//                tfListener.waitForTransform(mapFrameId,objectFrameId,ros::Time(0),ros::Duration(0.4));
                tfListener.waitForTransform(mapFrameId,objectFrameId,ros::Time(0),ros::Duration(0.4));//优化 "object_16"
                tfListener.lookupTransform(mapFrameId,objectFrameId,ros::Time(0),pose);//msg->header.stamp
                tfListener.waitForTransform(msg->header.frame_id,objectFrameId,ros::Time(0),ros::Duration(0.4));
                tfListener.lookupTransform(msg->header.frame_id,objectFrameId,ros::Time(0),poseCam);
            }
            catch(tf::TransformException & ex)
            {
                ROS_WARN("%s",ex.what());
                continue;
            }
            //NOTE 这是在 /map frame下
            ROS_INFO("Object_%d [x,y,z] [x,y,z,w] in \"%s\" frame: [%f,%f,%f] [%f,%f,%f,%f]",
                    id, mapFrameId.c_str(),
                    pose.getOrigin().x(), pose.getOrigin().y(), pose.getOrigin().z(),
                    pose.getRotation().x(), pose.getRotation().y(), pose.getRotation().z(), pose.getRotation().w());
            ROS_INFO("Object_%d [x,y,z] [x,y,z,w] in \"%s\" frame: [%f,%f,%f] [%f,%f,%f,%f]",
                    id, msg->header.frame_id.c_str(),
                    poseCam.getOrigin().x(), poseCam.getOrigin().y(), poseCam.getOrigin().z(),
                    poseCam.getRotation().x(), poseCam.getRotation().y(), poseCam.getRotation().z(), poseCam.getRotation().w());

           log(Info,"tf");
           log(Info,mapFrameId.c_str());
//           tf::Quaternion quat;
//           tf::quaternionMsgToTF(,quat);
//           tf::Matrix3x3(quat).getRPY(roll,pitch,yaw);
           float x=pose.getRotation().x();
           float y=pose.getRotation().y();
           float z=pose.getRotation().z();
           float w=pose.getRotation().w();
           roll=atan2(2*(w*x+y*z),1-2*(x*x+y*y));
           pitch=asin(2*(w*y-z*x));
           yaw=atan2(2*(w*z+x*y),1-2*(y*y+z*z));

           emit mapTf(pose.getOrigin().x(),pose.getOrigin().y(),pose.getOrigin().z(),roll,pitch,yaw);
           //ui.label_15->setNum((double)pose.getOrigin().x());
           //ui.label_16->setText(objectFrameId.c_str());
            //qDebug()<<objectFrameId.c_str();
        }
    }

}

void QNode::run() {


    ros::AsyncSpinner s(4);
    s.start();
    //tf::TransformListener find;
    //ros::Duration(10.0).sleep();   //可设置等待10s缓冲
    ros::Rate loop_rate(10);
    int count = 0;

    while ( ros::ok() ) {

        std_msgs::String msg;
        std::stringstream ss;
//        tf::StampedTransform trans;
//        try{
//            find.lookupTransform("/map","/odom",ros::Time(0),trans);
//        }
//        catch (tf::TransformException &ex){
//            ROS_ERROR("%s",ex.what());
//            ros::Duration(1.0).sleep();
//            continue;
//        }

        ss << "hello world " << count;
        msg.data = ss.str();
        if(count<3)
        {
            chatter_publisher.publish(msg);
            log(Info,std::string("I sent: ")+msg.data);
        }
        //ros::MultiThreadedSpinner s(2);//multi thread
        ros::spinOnce();
        loop_rate.sleep();
        ++count;
    }
    std::cout << "Ros shutdown, proceeding to close the gui." << std::endl;
    Q_EMIT rosShutdown(); // used to signal the gui for a shutdown (useful to roslaunch)
}


void QNode::log( const LogLevel &level, const std::string &msg) {
    logging_model.insertRows(logging_model.rowCount(),1);
    std::stringstream logging_model_msg;
    switch ( level ) {
        case(Debug) : {
                ROS_DEBUG_STREAM(msg);
                logging_model_msg << "[DEBUG] [" << ros::Time::now() << "]: " << msg;
                break;
        }
        case(Info) : {
                ROS_INFO_STREAM(msg);
                logging_model_msg << "[INFO] [" << ros::Time::now() << "]: " << msg;
                break;
        }
        case(Warn) : {
                ROS_WARN_STREAM(msg);
                logging_model_msg << "[INFO] [" << ros::Time::now() << "]: " << msg;
                break;
        }
        case(Error) : {
                ROS_ERROR_STREAM(msg);
                logging_model_msg << "[ERROR] [" << ros::Time::now() << "]: " << msg;
                break;
        }
        case(Fatal) : {
                ROS_FATAL_STREAM(msg);
                logging_model_msg << "[FATAL] [" << ros::Time::now() << "]: " << msg;
                break;
        }
    }
    QVariant new_row(QString(logging_model_msg.str().c_str()));
    logging_model.setData(logging_model.index(logging_model.rowCount()-1),new_row);
    Q_EMIT loggingUpdated(); // used to readjust the scrollbar
}

}  // namespace class1_ros_qt_demo
