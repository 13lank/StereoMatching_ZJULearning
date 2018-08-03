#include<opencv2/opencv.hpp>
#include<iostream>
#include<stdio.h>
#include"camera_calibration.h"
using namespace std;
using namespace cv;

int main() {
	MyCameraCalibrator calibrator;
	calibrator.loadSettings("../ImageData/FileList.txt");
	calibrator.solveCalibration();
	Mat srcImg = imread("../ImageData/left/left01.jpg");
	Mat dstImg;
	calibrator.calibrate(&srcImg, &dstImg);
	imshow("Distorted Image", srcImg);
	imshow("Undistorted Image", dstImg);
	waitKey(0);
}