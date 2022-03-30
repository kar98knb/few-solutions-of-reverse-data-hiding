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
	if (argc != 3) {
		cout << "请输入两个参数，第一个为嵌入图片路径，第二个为要嵌入的信息" << endl ;
		return -1;
	}
	Mat image;
	image = imread(argv[1], IMREAD_GRAYSCALE); // Read the file

	ifstream ifile1(argv[2]);
	string toEmbed;
	ifile1 >> toEmbed;

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
	additonalInfo addInfo(toEmbed.length(), image.at<uchar>(0, 0), image.at<uchar>(0, 1),image.rows,image.cols);
	image.at<uchar>(0, 0) = 0; image.at<uchar>(0, 1) = 0;
	calcHist(&image, 1, 0, Mat(), hist, 1, &histSize, histRange, uniform, accumulate);

	assert(!histIsHorizontal(hist));


	
	
	
	int toEmbedIndex = 0;

	//维护一个描述直方图状态的mask
	while (toEmbedIndex <= toEmbed.length()) {
		pair<int, int> thisPair = find_Nth_pair(addInfo, hist);
		int thisCapacity = returnPureCapacity(thisPair, hist);
		if (thisCapacity<=0) {
			cout << "fail to embed: info is too long" << endl;
			return -1;
		}
		addInfo.pairs.push_back(thisPair);
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


	


	//初始嵌入
	image.at<uchar>(0, 0) = addInfo.pairs[0].first;
	image.at<uchar>(0, 1) = addInfo.pairs[0].second;

	//手动循环展开,寻找零值点的位置，记录非零零值点每个点的个数
	for (int pairIndex = 0; pairIndex < addInfo.pairs.size(); pairIndex++) {
		pair<int, int>thispair = addInfo.pairs[pairIndex];
		if (hist.at<float>(thispair.second) != 0) {
			int countForBitmap = 0;
			for (int j = 2; j < image.cols; j++) {
				if (image.at<uchar>(0, j) == thispair.second) {
					addInfo.bitmap1.push_back(make_pair(0, j));
					countForBitmap++;
				}
			}
			for (int i = 1; i < image.rows; i++) {
				for (int j = 0; j < image.cols; j++) {
					if (image.at<uchar>(i, j) == thispair.second) {
						addInfo.bitmap1.push_back(make_pair(i, j));
						countForBitmap++;
					}
				}
			}
			addInfo.bitmapMinLength.push_back(countForBitmap);
		}
		else {
			addInfo.bitmapMinLength.push_back(0);
		}
	}

	/*
	* 上述代码完成了初始处理，开始嵌入信息
	* 
	* 仍然使用循环展开
	* 
	* 这里是这个算法复杂度的主要组成部分，时间复杂度为O(3KMN)，K为峰值点零值点对数，M，N为图像行列数
	*/

	toEmbed = addInfo.AddInfo() + toEmbed;
	assert(toEmbed.length()==(addInfo.toembedLength+addInfo.totalAddLength));
	int embedIndex = 0;
	int count1 = 0, count2 = 0, count3 = 0;
	for (int pairIndex = 0; pairIndex < addInfo.pairs.size(); pairIndex++) {
		pair<int, int>thispair = addInfo.pairs[pairIndex];
		if (thispair.first > thispair.second) {
			//第一趟处理：先将非零零值点置0
			for (int j = 2; j < image.cols; j++) {
				if (image.at<uchar>(0, j)==thispair.second) {
					image.at<uchar>(0, j) = 0;
				}
			}
			for (int i = 1; i < image.rows; i++) {
				for (int j = 0; j < image.cols; j++) {
					if (image.at<uchar>(i, j) == thispair.second) {
						image.at<uchar>(i, j) = 0;
					}
				}
			}

			//第二趟处理：移位
			for (int j = 2; j < image.cols; j++) {
				if (image.at<uchar>(0, j) > thispair.second && image.at<uchar>(0, j) < thispair.first) {
					image.at<uchar>(0, j)--;
				}
			}
			for (int i = 1; i < image.rows; i++) {
				for (int j = 0; j < image.cols; j++) {
					if (image.at<uchar>(i, j) > thispair.second && image.at<uchar>(i, j) < thispair.first) {
						image.at<uchar>(i, j)--;
					}
				}
			}

			//第三趟处理：嵌入
			for (int j = 2; j < image.cols; j++) {
				if (image.at<uchar>(0, j)==thispair.first) {
					if (toEmbed[embedIndex] == '1') {
						image.at<uchar>(0, j)--;
					}
					embedIndex++;
					if (embedIndex == toEmbed.length()) {
						goto here;
					}
				}
			}
			for (int i = 1; i < image.rows; i++) {
				for (int j = 0; j < image.cols; j++) {
					if (image.at<uchar>(i, j) == thispair.first) {
						if (toEmbed[embedIndex] == '1') {
							image.at<uchar>(i, j)--;
						}
						embedIndex++;
						if (embedIndex == toEmbed.length()) {
							goto here;
						}
					}
				}
			}
		}
		else {
			//第一趟处理：先将非零零值点置0
			for (int j = 2; j < image.cols; j++) {
				if (image.at<uchar>(0, j) == thispair.second) {
					image.at<uchar>(0, j) = 0;
				}
			}
			for (int i = 1; i < image.rows; i++) {
				for (int j = 0; j < image.cols; j++) {
					if (image.at<uchar>(i, j) == thispair.second) {
						image.at<uchar>(i, j) = 0;
					}
				}
			}

			//第二趟处理：移位
			for (int j = 2; j < image.cols; j++) {
				if (image.at<uchar>(0, j) < thispair.second && image.at<uchar>(0, j) > thispair.first) {
					image.at<uchar>(0, j)++;
				}
			}
			for (int i = 1; i < image.rows; i++) {
				for (int j = 0; j < image.cols; j++) {
					if (image.at<uchar>(i, j) < thispair.second && image.at<uchar>(i, j) > thispair.first) {
						image.at<uchar>(i, j)++;
					}
				}
			}

			//第三趟处理：嵌入
			for (int j = 2; j < image.cols; j++) {
				if (image.at<uchar>(0, j) == thispair.first) {
					if (toEmbed[embedIndex] == '1') {
						image.at<uchar>(0, j)++;
					}
					embedIndex++;
					if (embedIndex == toEmbed.length()) {
						goto here;
					}
				}
			}
			for (int i = 1; i < image.rows; i++) {
				for (int j = 0; j < image.cols; j++) {
					if (image.at<uchar>(i, j) == thispair.first) {
						if (toEmbed[embedIndex] == '1') {
							image.at<uchar>(i, j)++;
						}
						embedIndex++;
						if (embedIndex == toEmbed.length()) {
							goto here;
						}
					}
				}
			}
		}
	}



	
here:

	imwrite("embed.bmp", image);

	cout << "success!" << endl;

	return 0;
}