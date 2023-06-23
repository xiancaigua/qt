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
#include "../include/rosqt/qnode.hpp"



/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace rosqt {

/*****************************************************************************
** Implementation
*****************************************************************************/

QNode::QNode(int argc, char** argv ) :
	init_argc(argc),
	init_argv(argv)
	{}

QNode::~QNode() {
    if(ros::isStarted()) {
      ros::shutdown(); // explicitly needed since we use ros::start();
      ros::waitForShutdown();
    }
	wait();
}

bool QNode::init() {
	ros::init(init_argc,init_argv,"rosqt");
	if ( ! ros::master::check() ) {
		return false;
	}
	ros::start(); // explicitly needed since our nodehandle is going out of scope.
	ros::NodeHandle n;
	// Add your ros communications here.
	chatter_publisher = n.advertise<std_msgs::String>("chatter", 1000);
    ros::Publisher test_pub=n.advertise<std_msgs::String>("test",100);
    goal_pub = n.advertise<geometry_msgs::PoseStamped>("move_base_simple/goal",10);
    qDebug()<<"goal_pub";
    locate_sub = n.subscribe("/qingzhou_locate",1,&QNode::LocateCallBack, this);
    odom_sub = n.subscribe("amcl_pose",2,&QNode::OdomCallBack,this);
    // camera
    image_transport::ImageTransport it(n);
        /*订阅者的实现，也算订阅者的初始化
        第一个参数："/XUHONGBO" 表示的话题名
        第二个参数：1 表示的是队列长度
        第三个参数：&QNode::myCallback_img是订阅者的回调函数
        第四个参数：this指针，表面是在当前线程里订阅
        */
    image_sub = it.subscribe("/Img",10,&QNode::myCallback_img,this);
    //battery
    battery = n.subscribe("/battery", 10, &QNode::battery_callback, this);
	start();
	return true;
}

bool QNode::init(const std::string &master_url, const std::string &host_url) {
	std::map<std::string,std::string> remappings;
	remappings["__master"] = master_url;
	remappings["__hostname"] = host_url;
	ros::init(remappings,"rosqt");
	if ( ! ros::master::check() ) {
		return false;
	}
	ros::start(); // explicitly needed since our nodehandle is going out of scope.
	ros::NodeHandle n;
	// Add your ros communications here.
	chatter_publisher = n.advertise<std_msgs::String>("chatter", 1000);
    ros::Publisher test_pub=n.advertise<std_msgs::String>("test",100);
    goal_pub = n.advertise<geometry_msgs::PoseStamped>("move_base_simple/goal",10);
    qDebug()<<"goal_pub";
    locate_sub = n.subscribe("/qingzhou_locate",1,&QNode::LocateCallBack, this);
    odom_sub = n.subscribe("amcl_pose",2,&QNode::OdomCallBack,this);
    // camera
    image_transport::ImageTransport it(n);
        /*订阅者的实现，也算订阅者的初始化
        第一个参数："/XUHONGBO" 表示的话题名
        第二个参数：1 表示的是队列长度
        第三个参数：&QNode::myCallback_img是订阅者的回调函数
        第四个参数：this指针，表面是在当前线程里订阅
        */
    image_sub = it.subscribe("/Img",10,&QNode::myCallback_img,this);
    //battery
    battery = n.subscribe("/battery", 10, &QNode::battery_callback, this);
    start();
	return true;
}

// 依据车的真是位置再次发布目标点至UNLOAD
void QNode::OdomCallBack(const geometry_msgs::PoseWithCovarianceStamped &msgs)
{
    odom = msgs;
//    qDebug()<<"In odom callback";
//    qDebug()<<odom.pose.pose.position.y;
//    if(odom.pose.pose.position.y<-4.5 && robotlocation == 8)
//    {
//        set_goal_from_buttom(2);
//        qDebug()<<"set goal from auto";
//    }
//    else if(odom.pose.pose.position.y<-4.5 && robotlocation == 2)
//    {
//        set_goal_from_buttom(1);
//    };
    //datax = QString("%1").arg(odom.pose.pose.position.x);
    //datay = QString("%1").arg(odom.pose.pose.position.y);
    //dataz = QString("%1").arg(odom.pose.pose.position.z);
}

void QNode::LocateCallBack(const std_msgs::Int32& data)
{
    robotlocation = data.data;
    qDebug()<<robotlocation;
    if (int(robotlocation) == 5 )
    {
        qDebug()<<"go back to the start";
        set_goal_from_buttom(4);
    }
    /*
    if (int(robotlocation) == 1 )
    {
        qDebug()<<"go to traffic ";
        set_goal_from_buttom(1);
    }

    if (int(robotlocation) == 3 )
    {
        qDebug()<<"go to traffic ";
        set_goal_from_buttom(1);
    }*/

}


// 和槽函数的接口
void QNode::set_goal_from_buttom(int goal_index)
{
    // load traffic  unload start
//    qDebug()<<"set_goal_from_buttom";//2.51575  0.000
    float pdX[]={2.13135,2.02135,-2.407611,0.89579,-0.10};//RoadLine 0.53579
    float pdY[]={-3.40674,-6.05,-5.550,-3.6004388,0.10};//RoadLine -3.954388
    float pdth[]={-1.6707963,-1.57500963,1.5707963,1.5707963,0.00};
    float w[]={0,0,0};
    int count = 0;
    int a[5];
    a[0] = 5;
    a[1] = 5;
    a[2] = 10;
    a[3] = 5;
    a[4] = 5;
    ros::Rate r(1);
    while(count<=a[goal_index])
    {
    if (goal_index==0
            || goal_index==3
            || goal_index==1
            || goal_index==2
            ||goal_index==4){
        w[0]=pdX[goal_index];
        w[1]=pdY[goal_index];
        w[2]=pdth[goal_index];
        set_position(w);
        k=goal_index;
    };
    r.sleep();
//    qDebug()<<"I am counting";
    count++;
    };
}

// 最终发布到话题
void QNode::set_position(float p[])
{
    geometry_msgs::PoseStamped goal;
    goal.header.frame_id = "map";
    goal.header.stamp = ros::Time::now();
    goal.pose.position.x = p[0];
    goal.pose.position.y = p[1] ;
    tf2::Quaternion q;
    q.setRPY(0, 0, p[2]);
    tf2::convert(q, goal.pose.orientation);
    goal.pose.orientation.x = q[0];
    goal.pose.orientation.y = q[1];
    goal.pose.orientation.z = q[2];
    goal.pose.orientation.w = q[3];
    goal_pub.publish(goal);
}

void QNode::run() {
//    ros::Rate loop_rate(1);
	int count = 0;
	while ( ros::ok() ) {

		std_msgs::String msg;
		std::stringstream ss;
		ss << "hello world " << count;
		msg.data = ss.str();
		chatter_publisher.publish(msg);
		ros::spinOnce();
//		loop_rate.sleep();
        if (odom.pose.pose.position.y<-5.5 && robotlocation == 8)
        {
            qDebug()<<"the second destination is published";
            set_goal_from_buttom(2);
        }
        else if(odom.pose.pose.position.y<-5.5 && robotlocation == 1)
            {
                set_goal_from_buttom(1);
            };
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

// camera callback
void QNode::myCallback_img(const sensor_msgs::ImageConstPtr &msg)
{
  qDebug()<<"Qt gets the camera message";
  cv_bridge::CvImagePtr cv_ptr;
  try
  {   /*change to CVImage*/
  cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);//将ros::sensor_msgs转为opencv格式
  img = cv_ptr->image;//转为opencv图像格式
  if(img.empty())
  {
    qDebug()<<"img is empty";
  }

  Q_EMIT imageSignal(img) ; //将图像发出去，由于是循环执行回调函数，所以会一直发送图像
  }
  catch (cv_bridge::Exception& e)
  {
    ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
  }
}

// battery callback
void QNode::battery_callback(const std_msgs::Float32 &msg) {
    Q_EMIT batterySignal(msg.data);
}

}  // namespace rosqt
