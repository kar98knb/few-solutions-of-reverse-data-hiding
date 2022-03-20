#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <cassert>
using namespace cv;
using namespace std;


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


	int hist_w = 512, hist_h = 400;
	//int bin_w = cvRound((double)hist_w / 256);
	//Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));
	//normalize(hist, hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());

	int minIndex = 0, maxIndex = 0;
	int count = 0;
	for (int i = 0; i < 256; i++)
	{
		/*line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(hist.at<float>(i - 1))),
			Point(bin_w * (i), hist_h - cvRound(hist.at<float>(i))),
			Scalar(255, 0, 0), 2, 8, 0);*/
		/*line(histImage, Point(bin_w * (i), 0),
			Point(bin_w * (i), hist_h - cvRound(hist.at<float>(i))),
			Scalar(255, 255, 255), 2, 8, 0);*/
		//cout << i << ' ' << cvRound(hist.at<float>(i)) << endl;
		if (hist.at<float>(i) == 0) {
			minIndex = i;
			break;
		}
	}
	for (int i = 0; i < 256; i++)
	{
		if (hist.at<float>(i) > hist.at<float>(maxIndex)) {
			maxIndex = i;
		}
	}
	assert(hist.at<float>(maxIndex) != hist.at<float>(minIndex));

	vector<pair<int, int>> pairs;
	pairs.push_back(make_pair(maxIndex, minIndex));

	if (maxIndex > minIndex) {
		
	}
	else {

	}
	/*imshow("calcHist Demo", histImage);
	waitKey(0);*/

	return 0;
}