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
	for (int i = 0; i < patternSize.width; i++)
		for (int j = 0; j < patternSize.height; j++)
			tmpDstCorners.push_back({ 1.0F*i,1.0F*j,0.0f });
	Mat tmpImg;
	for (auto fname : imgFileName) {
		tmpImg = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		cout << fname << endl;
		if (!findChessboardCorners(tmpImg, patternSize, tmpSrcCorners))
			findChessboardCorners(tmpImg, Size(patternSize.height, patternSize.width),tmpSrcCorners);
		TermCriteria criteria = TermCriteria(
			CV_TERMCRIT_ITER + CV_TERMCRIT_EPS,
			30,//max repeat times
			0.1//eps
		);
		cornerSubPix(tmpImg, tmpSrcCorners, Size(10,10)/*winSize*/, Size(-1,-1)/*zeroZone*/, criteria);//Find corners in subpix level
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
	initUndistortRectifyMap(cameraMatrix, distCoeffs, Mat(), Mat(), imgSize, CV_32F, map1, map2);//get initU
	remap(*srcImg, *dstImg, map1, map2, INTER_LINEAR);
	return 1;
}