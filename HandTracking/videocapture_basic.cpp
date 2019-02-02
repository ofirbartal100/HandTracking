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

CascadeClassifier face_cascade;

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
	if (!face_cascade.load("C:\\Users\\ofir\\Desktop\\temp\\palm.xml"))
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

void detectAndDisplay(Mat frame)
{
	Mat frame_gray;
	cv::cvtColor(frame, frame_gray, CV_BGR2GRAY);
	cv::equalizeHist(frame_gray, frame_gray);
	//-- Detect faces
	std::vector<Rect> faces;
	face_cascade.detectMultiScale(frame_gray, faces);
	for (size_t i = 0; i < faces.size(); i++)
	{
		//Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);
		rectangle(frame, faces[i], Scalar(255, 0, 255), 4);
		//ellipse(frame, center, Size(faces[i].width / 2, faces[i].height / 2), 0, 0, 360, Scalar(255, 0, 255), 4);
		//Mat faceROI = frame_gray(faces[i]);
		//-- In each face, detect eyes
		/*std::vector<Rect> eyes;
		eyes_cascade.detectMultiScale(faceROI, eyes);
		for (size_t j = 0; j < eyes.size(); j++)
		{
		Point eye_center(faces[i].x + eyes[j].x + eyes[j].width / 2, faces[i].y + eyes[j].y + eyes[j].height / 2);
		int radius = cvRound((eyes[j].width + eyes[j].height)*0.25);
		circle(frame, eye_center, radius, Scalar(255, 0, 0), 4);
		}*/
	}
	//-- Show what you got
	imshow("Capture - Face detection", frame);
}
