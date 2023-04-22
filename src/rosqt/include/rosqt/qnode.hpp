/**
 * @file /include/rosqt/qnode.hpp
 *
 * @brief Communications central!
 *
 * @date February 2011
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef rosqt_QNODE_HPP_
#define rosqt_QNODE_HPP_

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
#include <QDebug>

#include <std_msgs/Int32.h>
//#include <tf/tf.h>
#include <tf2/utils.h>
#include "geometry_msgs/PoseWithCovarianceStamped.h"
#include "geometry_msgs/PoseArray.h"
#include "geometry_msgs/Pose.h"
#include "geometry_msgs/PoseStamped.h"
#include <nav_msgs/Odometry.h>

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace rosqt {

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

    void LocateCallBack(const std_msgs::Int32& data);
    void set_goal_from_buttom(int goal_index);
    void set_position(float p[]);
    void OdomCallBack(const geometry_msgs::PoseWithCovarianceStamped &msgs);

    ros::Publisher goal_pub;
    ros::Subscriber goal_sub;
    ros::Subscriber locate_sub;
    ros::Subscriber odom_sub;
    int k;
    geometry_msgs::PoseWithCovarianceStamped odom;

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

private:
	int init_argc;
    char** init_argv;
	ros::Publisher chatter_publisher;
    QStringListModel logging_model;
};

}  // namespace rosqt

#endif /* rosqt_QNODE_HPP_ */
