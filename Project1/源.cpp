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
	calibrator.testCalibration();
}