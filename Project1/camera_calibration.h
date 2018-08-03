#pragma 
#include<opencv2/opencv.hpp>
#include<iostream>
#include<vector>
#include<string>

using namespace cv;
using namespace std;

class MyCameraCalibrator {
private:
	Size patternSize,imgSize;
	vector<string> imgFileName;
	vector<vector<Point2f> >SrcPoint;
	vector<vector<Point3f> >DstPoint;
	Mat cameraMatrix, distCoeffs;
public:
	void loadSettings(const char *settingFile);
	void solveCalibration(void);
	bool calibrate(Mat *srcImg, Mat *dstImg);
};
