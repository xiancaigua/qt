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
    start();
	return true;
}

// 依据车的真是位置再次发布目标点至UNLOAD
void QNode::OdomCallBack(const geometry_msgs::PoseWithCovarianceStamped &msgs)
{
    odom = msgs;
    if(odom.pose.pose.position.y<=-3.0&& k==1)
    {
        set_goal_from_buttom(2);
    }
    //datax = QString("%1").arg(odom.pose.pose.position.x);
    //datay = QString("%1").arg(odom.pose.pose.position.y);
    //dataz = QString("%1").arg(odom.pose.pose.position.z);
}

void QNode::LocateCallBack(const std_msgs::Int32& data)
{


}


// 和槽函数的接口
void QNode::set_goal_from_buttom(int goal_index)
{
    // load traffic  unload start
    qDebug()<<"set_goal_from_buttom";
    float pdX[]={1.52668,1.4483,-2.45729,0.0826};
    float pdY[]={-2.8000,-4.43526,-5.05238,0.00876};
    float pdth[]={-1.584,-1.584,1.613,0};
    float w[]={0,0,0};
    if (goal_index==0
            || goal_index==3
            || goal_index==1
            || goal_index==2){
        w[0]=pdX[goal_index];
        w[1]=pdY[goal_index];
        w[2]=pdth[goal_index];
        set_position(w);
        k=goal_index;
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
	ros::Rate loop_rate(1);
	int count = 0;
	while ( ros::ok() ) {

		std_msgs::String msg;
		std::stringstream ss;
		ss << "hello world " << count;
		msg.data = ss.str();
		chatter_publisher.publish(msg);
		log(Info,std::string("I sent: ")+msg.data);
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

}  // namespace rosqt
