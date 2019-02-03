/**
@file videocapture_basic.cpp
@brief A very basic sample for using VideoCapture and VideoWriter
@author PkLab.net
@date Aug 24, 2016
*/

#include <opencv2/core.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <fstream>
#include <stdio.h>

using namespace cv;
using namespace std;

void detectAndDisplay(Mat frame);

CascadeClassifier hand_cascade;

int main(int, char**)
{

	Mat frame;
	//--- INITIALIZE VIDEOCAPTURE
	VideoCapture cap;
	// open the default camera using default API
	cap.open(0);
	// OR advance usage: select any API backend
	int deviceID = 0;             // 0 = open default camera
	int apiID = cv::CAP_ANY;      // 0 = autodetect default API
								  // open selected camera using selected API
	cap.open(deviceID + apiID);
	// check if we succeeded
	if (!cap.isOpened()) {
		cerr << "ERROR! Unable to open camera\n";
		return -1;
	}

	//-- 1. Load the cascades
	if (!hand_cascade.load("C:\\Users\\ofir\\Desktop\\temp\\palm_v4.xml"))
	{
		cout << "--(!)Error loading face cascade\n";
		return -1;
	};

	//--- GRAB AND WRITE LOOP
	cout << "Start grabbing" << endl
		<< "Press any key to terminate" << endl;
	for (;;)
	{
		// wait for a new frame from camera and store it into 'frame'
		cap.read(frame);
		// check if we succeeded
		if (frame.empty()) {
			cerr << "ERROR! blank frame grabbed\n";
			break;
		}

		//-- 3. Apply the classifier to the frame
		detectAndDisplay(frame);
		// show live and wait for a key with timeout long enough to show images
		//imshow("Live", frame);
		if (waitKey(5) >= 0)
			break;
	}
	// the camera will be deinitialized automatically in VideoCapture destructor
	return 0;
}

Mat rotate_image(Mat frame_gray, double angle)
{
	// get rotation matrix for rotating the image around its center in pixel coordinates
	cv::Point2f center((frame_gray.cols - 1) / 2.0, (frame_gray.rows - 1) / 2.0);
	cv::Mat rot = cv::getRotationMatrix2D(center, angle, 1.0);
	// determine bounding rectangle, center not relevant
	cv::Rect2f bbox = cv::RotatedRect(cv::Point2f(), frame_gray.size(), angle).boundingRect2f();
	// adjust transformation matrix
	rot.at<double>(0, 2) += bbox.width / 2.0 - frame_gray.cols / 2.0;
	rot.at<double>(1, 2) += bbox.height / 2.0 - frame_gray.rows / 2.0;

	cv::Mat dst;
	cv::warpAffine(frame_gray, dst, rot, bbox.size());
	return dst;
}

void detectAndDisplay(Mat frame)
{
	Mat frame_gray,rotated;
	cv::cvtColor(frame, frame_gray, CV_BGR2GRAY);
	cv::equalizeHist(frame_gray, frame_gray);

	rotated = rotate_image(frame_gray, 90);

	//-- Detect faces
	std::vector<Rect> hands;
	hand_cascade.detectMultiScale(rotated, hands);
	for (size_t i = 0; i < hands.size(); i++)
	{
		//Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);
		rectangle(rotated, hands[i], Scalar(255, 0, 255), 4);
	}
	//-- Show what you got
	imshow("Capture - Hand detection", rotated);
}
