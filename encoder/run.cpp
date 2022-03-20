#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <cassert>
#include"encoder.h"
using namespace cv;
using namespace std;

int q = 2000;

bool histIsHorizontal(Mat hist) {
	int test = hist.at<float>(0);
	for (int i = 0; i < 256; i++) {
		if (hist.at<float>(i) != test) {
			return false;
		}
	}
	return true;
}

int returnPureCapacity(additonalInfo addInfo, Mat hist, int n) {
	int maxIndex = addInfo.pairs[n].first, minIndex = addInfo.pairs[n].second;
	return hist.at<float>(maxIndex) - hist.at<float>(minIndex);
}

pair<int, int> find_Nth_pair(additonalInfo addInfo, Mat hist) {

	int minIndex1, minIndex2 , maxIndex = 0;
	for (int i = 0; i < 256; i++)
	{
		if ((hist.at<float>(i) > hist.at<float>(maxIndex))&&addInfo.mask[i]!=1) {
			maxIndex = i;
		}
	}
	for (int i = maxIndex; i < 256; i++)
	{
		if (hist.at<float>(i) == 0) {
			minIndex1 = i;
			break;
		}
		if ((hist.at<float>(i) < hist.at<float>(minIndex1)) && addInfo.mask[i] != 1) {
			minIndex1 = i;
			continue;
		}
		if (addInfo.mask[i] == 1) {
			break;
		}
	}

	for (int i = maxIndex; i >= 0; i--)
	{
		if (hist.at<float>(i) == 0) {
			minIndex2 = i;
			break;
		}
		if ((hist.at<float>(i) < hist.at<float>(minIndex2)) && addInfo.mask[i] != 1) {
			minIndex2 = i;
			continue;
		}
		if (addInfo.mask[i] == 1) {
			break;
		}
	}
	

	if (hist.at<float>(minIndex2) < hist.at<float>(minIndex1)) {
		return make_pair(maxIndex, minIndex2);
	}
	else {
		return make_pair(maxIndex, minIndex1);
	}
}

int main(int argc, char** argv)
{
	Mat image;
	image = imread(argv[1], IMREAD_GRAYSCALE); // Read the file
	if (image.empty())
	{
		cout << "Could not open or find the image" << endl;
		exit(-1);
	}
	vector<Mat> bgr_planes;
	split(image, bgr_planes);
	int histSize = 256;
	float range[] = { 0, 256 }; //the upper boundary is exclusive
	const float* histRange[] = { range };
	bool uniform = true, accumulate = false;
	Mat hist;
	calcHist(&bgr_planes[0], 1, 0, Mat(), hist, 1, &histSize, histRange, uniform, accumulate);

	assert(!histIsHorizontal(hist));



	additonalInfo* addInfo = new additonalInfo();

	

	return 0;
}