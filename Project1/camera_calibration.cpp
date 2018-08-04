#include "camera_calibration.h"

void MyCameraCalibrator::loadSettings(const char *s) {
	ifstream f(s);//Load setting file
	int h, w;
	f >> w >> h;
	patternSize= { w,h };//Set patternSize 
	cout << w << " " << h << endl;
	string tmps;
	while (!f.eof()) {
		f >> tmps;
		cout << tmps<<endl;
		if (!f.eof())imgFileName.push_back(tmps);//Get image file name
	}
	return;
}

void MyCameraCalibrator::solveCalibration() {
	vector<Point2f>tmpSrcCorners;
	vector<Point3f>tmpDstCorners;
	for (int i = 0; i < patternSize.height; i++)//height first!
		for (int j = 0; j < patternSize.width; j++)
			tmpDstCorners.push_back(Point3f(i,j,0.0f));
	Mat tmpImg;
	for (auto fname : imgFileName) {
		tmpImg = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		cout << fname << endl;
		findChessboardCorners(tmpImg, patternSize, tmpSrcCorners);
		TermCriteria criteria = TermCriteria(
			CV_TERMCRIT_ITER + CV_TERMCRIT_EPS,
			30,//max repeat times
			0.1//eps
		);
		cornerSubPix(tmpImg, tmpSrcCorners, Size(10,10)/*winSize*/, Size(-1,-1)/*zeroZone*/, criteria);//Find corners in subpix level
		/*DEBUG
		Mat copyImg = tmpImg.clone();
		for (auto i : tmpSrcCorners) {
			circle(copyImg, i, 5, Scalar(0, 0, 0), -1, 0, 0);
			cout << " [" << i.x << ',' << i.y << "] " << endl;
		}
		imshow("Detected", copyImg);
		waitKey(0);
		*/
		if (tmpSrcCorners.size() == patternSize.area()) {//All the corners are found
			SrcPoint.push_back(tmpSrcCorners);
			DstPoint.push_back(tmpDstCorners);
		}
	}
	imgSize = tmpImg.size();
	vector<Mat> rvecs, tvecs;
	calibrateCamera(DstPoint, SrcPoint, imgSize, cameraMatrix, distCoeffs, rvecs, tvecs);//get cameraMatrix
}

bool MyCameraCalibrator::calibrate(Mat *srcImg, Mat *dstImg) {
	if (srcImg->size() != imgSize) {
		printf("ERROR:Image sizes are not the same!");
		return 0;
	}
	Mat map1, map2;
	initUndistortRectifyMap(cameraMatrix, distCoeffs, Mat(), Mat(), imgSize, CV_32FC1, map1, map2);//get initU
	remap(*srcImg, *dstImg, map1, map2, INTER_LINEAR);
	return 1;
}

void MyCameraCalibrator::testCalibration(void) {
	for (auto fname : imgFileName) {
		Mat srcImg = imread(fname);
		Mat dstImg;
		calibrate(&srcImg, &dstImg);
		imshow("Distorted Image", srcImg);
		imshow("Undistorted Image", dstImg);
		waitKey(0);
	}
}