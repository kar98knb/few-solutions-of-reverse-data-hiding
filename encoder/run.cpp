#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/imgproc.hpp"
#include <iostream>
#include<fstream>
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

int returnPureCapacity(pair<int, int> thisPair, Mat hist) {
	int maxIndex = thisPair.first, minIndex = thisPair.second;
	return hist.at<float>(maxIndex) - hist.at<float>(minIndex)*(9+9);
}

pair<int, int> find_Nth_pair(additonalInfo addInfo, Mat hist) {

	int minIndex1, minIndex2 , maxIndex = 0;
	for (int i = 0; i < 256; i++)
	{
		if ((hist.at<float>(i) > hist.at<float>(maxIndex))&&addInfo.mask[i]==0) {
			maxIndex = i;
		}
	}
	minIndex1 = maxIndex;
	minIndex2 = maxIndex;
	for (int i = maxIndex; i < 256; i++)
	{
		if (hist.at<float>(i) == 0) {
			minIndex1 = i;
			break;
		}
		if ((hist.at<float>(i) < hist.at<float>(minIndex1)) && addInfo.mask[i] == 0) {
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
		if ((hist.at<float>(i) < hist.at<float>(minIndex2)) && addInfo.mask[i] == 0) {
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
	string toEmbed = argv[2];
	if (image.empty())
	{
		cout << "Could not open or find the image" << endl;
		return -1;
	}
	int histSize = 256;
	float range[] = { 0, 256 }; //the upper boundary is exclusive
	const float* histRange[] = { range };
	bool uniform = true, accumulate = false;
	Mat hist;
	calcHist(&image, 1, 0, Mat(), hist, 1, &histSize, histRange, uniform, accumulate);

	assert(!histIsHorizontal(hist));



	additonalInfo addInfo(toEmbed.length());
	int toEmbedIndex = 0;

	while (toEmbedIndex <= toEmbed.length()) {
		pair<int, int> thisPair = find_Nth_pair(addInfo, hist);
		int thisCapacity = returnPureCapacity(thisPair, hist);
		if (thisCapacity<=0) {
			cout << "fail to embed: info is too long" << endl;
			return -1;
		}
		toEmbedIndex += thisCapacity;
		//峰值点在零值点右侧
		if (thisPair.first>thisPair.second) {
			addInfo.mask[thisPair.second] = -2;
			for (int i = thisPair.second+1; i < thisPair.first; i++) {
				addInfo.mask[i] = -1;
			}
			addInfo.mask[thisPair.first] = -3;
		}
		//峰值点在零值点左侧
		else {
			addInfo.mask[thisPair.first] = 3;
			for (int i = thisPair.first+1; i < thisPair.second; i++) {
				addInfo.mask[i] = 1;
			}
			addInfo.mask[thisPair.second] = 2;
		}
	}

	int embedIndex = 0;
	for (int i = 0; i < image.rows; i++){
		for (int j = 0; j < image.cols; j++){
			if (addInfo.mask[image.at<uchar>(i, j)] == 1) {
				image.at<uchar>(i, j)++;
			}
			else if (addInfo.mask[image.at<uchar>(i, j)] == -1) {
				image.at<uchar>(i, j)--;
			}
		}
	}

	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j++) {
			if (addInfo.mask[image.at<uchar>(i, j)] == 3) {
				if (toEmbed[embedIndex] == '1') {
					image.at<uchar>(i, j)++;
				}
				
			}
			else if (addInfo.mask[image.at<uchar>(i, j)] == -3) {
				if (toEmbed[embedIndex] == '1') {
					image.at<uchar>(i, j)--;
				}
			}
			else {
				continue;
			}
			embedIndex++;
			if (embedIndex == toEmbed.length()) {
				goto here;
			}
		}
	}

here:
	imwrite("embed.bmp", image);
	ofstream OutFile("addinfo.txt"); //利用构造函数创建txt文本，并且打开该文本
	OutFile << "length:" <<addInfo.toembedLength<<endl ; //把字符串内容"This is a Test!"，写入Test.txt文件
	OutFile << "mask:" ;
	for (int i = 0; i < 256; i++) {
		OutFile << addInfo.mask[i];
	}
	OutFile.close(); //关闭Test.txt文件

	cout << "success!" << endl;

	return 0;
}