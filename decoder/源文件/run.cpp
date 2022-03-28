#define _CRT_SECURE_NO_WARNINGS

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/imgproc.hpp"
#include <iostream>
#include<fstream>
#include <cassert>
#include<json/json.h>
#include "../../encoder/encoder.h"
using namespace cv;
using namespace std;


int main(int argc, char** argv) {
	if (argc != 2) {
		cout << "请输入图片路径" << endl;
		return -1;
	}

	Mat image;
	image = imread(argv[1], IMREAD_GRAYSCALE); // Read the file

	additonalInfo addinfo;
	pair<int, int> firstpair = make_pair(image.at<uchar>(0, 0), image.at<uchar>(0, 1));
	image.at<uchar>(0, 0) = 0; image.at<uchar>(0, 1) = 0;
	if (image.empty())
	{
		cout << "Could not open or find the image" << endl;
		return -1;
	}


	string embedInfo;
	

	if (firstpair.first > firstpair.second) {

		for (int j = 2; j < image.cols; j++) {
			if (image.at<uchar>(0, j) == firstpair.first) {
				embedInfo += '0';
			}
			else if (image.at<uchar>(0, j) == firstpair.first - 1) {
				embedInfo += '1';
			}
		}
		for (int i = 1; i < image.rows; i++) {
			for (int j = 0; j < image.cols; j++) {
				if (image.at<uchar>(i, j) == firstpair.first) {
					embedInfo += '0';
				}
				else if (image.at<uchar>(i, j) == firstpair.first - 1) {
					embedInfo += '1';
				}
			}
		}

	}
	else {

		for (int j = 2; j < image.cols; j++) {
			if (image.at<uchar>(0, j) == firstpair.first) {
				embedInfo += '0';
			}
			else if (image.at<uchar>(0, j) == firstpair.first + 1) {
				embedInfo += '1';
			}
		}
		for (int i = 1; i < image.rows; i++) {
			for (int j = 0; j < image.cols; j++) {
				if (image.at<uchar>(i, j) == firstpair.first) {
					embedInfo += '0';
				}
				else if (image.at<uchar>(i, j) == firstpair.first + 1) {
					embedInfo += '1';
				}
			}
		}

	}


	if (embedInfo.size() < 72) {
		cout << "这个算法不应该出现这样的结果，请在github的issue中提交你的用例";
		return -1;
	}
	string totalAddLength = embedInfo.substr(0, 16);
	string toembedLength = embedInfo.substr(32, 16);
	for (int i = 15,mask = 1; i >= 0;i--) {
		if (totalAddLength[i] == '1') {
			addinfo.totalAddLength += mask;
		}
		if (toembedLength[i] == '1') {
			addinfo.toembedLength += mask;
		}
		mask <<= 1;
	}

	string imageGrayscaleAt_0_0 = embedInfo.substr(16, 8);
	string imageGrayscaleAt_0_1 = embedInfo.substr(24, 8);
	string lengthForBitmapMinLength = embedInfo.substr(48,8);
	for (int i = 8, mask = 1; i >= 0; i--) {
		if (imageGrayscaleAt_0_0[i] == '1') {
			addinfo.imageGrayscaleAt_0_0 += mask;
		}
		if (lengthForBitmapMinLength[i] == '1') {
			addinfo.lengthForBitmapMinLength += mask;
		}
		mask <<= 1;
	}

	pair<int, int> thispair = firstpair;
	while (1) {
		
	}


here:
	imwrite("output.bmp", image);


	return 0;
}