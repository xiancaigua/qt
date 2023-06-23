/**
 * @file /include/rosqt/main_window.hpp
 *
 * @brief Qt based gui for rosqt.
 *
 * @date November 2010
 **/
#ifndef rosqt_MAIN_WINDOW_H
#define rosqt_MAIN_WINDOW_H

/*****************************************************************************
** Includes
*****************************************************************************/

#include <QtGui/QMainWindow>
#include "ui_main_window.h"
#include "qnode.hpp"


#include "geometry_msgs/PoseWithCovarianceStamped.h"
#include "geometry_msgs/PoseArray.h"
#include "geometry_msgs/Pose.h"
#include "geometry_msgs/PoseStamped.h"

#include<opencv2/opencv.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include "sensor_msgs/image_encodings.h"
#include<image_transport/subscriber.h>
#include<sensor_msgs/Image.h>
#include<cv_bridge/cv_bridge.h>

#include <QDebug>

/*****************************************************************************
** Namespace
*****************************************************************************/

namespace rosqt {

/*****************************************************************************
** Interface [MainWindow]
*****************************************************************************/
/**
 * @brief Qt central, all operations relating to the view part here.
 */
class MainWindow : public QMainWindow {
Q_OBJECT

public:
	MainWindow(int argc, char** argv, QWidget *parent = 0);
	~MainWindow();

	void ReadSettings(); // Load up qt program settings at startup
	void WriteSettings(); // Save qt program settings when closing

	void closeEvent(QCloseEvent *event); // Overloaded function
	void showNoMasterMessage();

public Q_SLOTS:
	/******************************************
	** Auto-connections (connectSlotsByName())
	*******************************************/
	void on_actionAbout_triggered();
	void on_button_connect_clicked(bool check );
	void on_checkbox_use_environment_stateChanged(int state);

    /******************************************
    ** Manual connections
    *******************************************/
    void updateLoggingView(); // no idea why this can't connect automatically
    void loadslot();
    void unloadslot();
    void startslot();

    /******************************************
    ** Camera messages
    *******************************************/
    void displayMat(cv::Mat image);//显示视频流的显示槽函数

    void batteryslot(float);

private:
	Ui::MainWindowDesign ui;
	QNode qnode;
};

}  // namespace rosqt

#endif // rosqt_MAIN_WINDOW_H
