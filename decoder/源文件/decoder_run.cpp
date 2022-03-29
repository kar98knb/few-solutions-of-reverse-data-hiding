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


void Ini(string embedInfo,additonalInfo &addinfo) {
	string totalAddLength = embedInfo.substr(0, 16);
	string toembedLength = embedInfo.substr(32, 16);
	for (int i = 15,mask = 1; i >= 0; i--) {
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
	string lengthForBitmapMinLength = embedInfo.substr(48, 8);
	for (int i = 7, mask = 1; i >= 0; i--) {
		if (imageGrayscaleAt_0_0[i] == '1') {
			addinfo.imageGrayscaleAt_0_0 += mask;
		}
		if (imageGrayscaleAt_0_1[i] == '1') {
			addinfo.imageGrayscaleAt_0_1 += mask;
		}
		if (lengthForBitmapMinLength[i] == '1') {
			addinfo.lengthForBitmapMinLength += mask;
		}
		mask <<= 1;
	}
}

int main(int argc, char** argv) {
	if (argc != 2) {
		cout << "请输入图片路径" << endl;
		return -1;
	}

	Mat image;
	image = imread(argv[1], IMREAD_GRAYSCALE); // Read the file

	additonalInfo addinfo(image.rows,image.cols);
	pair<int, int> firstpair = make_pair(image.at<uchar>(0, 0), image.at<uchar>(0, 1));
	image.at<uchar>(0, 0) = 0; image.at<uchar>(0, 1) = 0;
	if (image.empty())
	{
		cout << "Could not open or find the image" << endl;
		return -1;
	}

	addinfo.pairs.push_back(firstpair);
	string embedInfo;
	
	int times = 0;
	pair<int, int> thispair = firstpair;
	bool isIni = false;
	if (firstpair.first > firstpair.second) {

		for (int j = 2; j < image.cols; j++) {
			if (image.at<uchar>(0, j) == firstpair.first) {
				embedInfo += '0';
				if ((!isIni) && embedInfo.length() == 56) {
					Ini(embedInfo, addinfo);
					isIni = true;
				}
				if (embedInfo.length() == (addinfo.toembedLength + addinfo.totalAddLength)) {
					goto here;
				}
			}
			else if (image.at<uchar>(0, j) == firstpair.first - 1) {
				embedInfo += '1';
				image.at<uchar>(0, j)++;
				if ((!isIni) && embedInfo.length() == 56) {
					Ini(embedInfo, addinfo);
					isIni = true;
				}
				if (embedInfo.length() == (addinfo.toembedLength + addinfo.totalAddLength)) {
					goto here;
				}
			}
			
		}
		for (int i = 1; i < image.rows; i++) {
			for (int j = 0; j < image.cols; j++) {
				if (image.at<uchar>(i, j) == firstpair.first) {
					embedInfo += '0';
					if (embedInfo.length() == 56) {
						Ini(embedInfo, addinfo);
					}
					if (embedInfo.length() == (addinfo.toembedLength + addinfo.totalAddLength)) {
						goto here;
					}
				}
				else if (image.at<uchar>(i, j) == firstpair.first - 1) {
					embedInfo += '1';
					image.at<uchar>(i, j)++;
					if ((!isIni) && embedInfo.length() == 56) {
						Ini(embedInfo, addinfo);
						isIni = true;
					}
					if (embedInfo.length() == (addinfo.toembedLength + addinfo.totalAddLength)) {
						goto here;
					}
				}
			}
		}

	}
	else {

		for (int j = 2; j < image.cols; j++) {
			if (image.at<uchar>(0, j) == firstpair.first) {
				embedInfo += '0';
				if ((!isIni) && embedInfo.length() == 56) {
					Ini(embedInfo, addinfo);
					isIni = true;
				}
				if (embedInfo.length() == (addinfo.toembedLength + addinfo.totalAddLength)) {
					goto here;
				}
			}
			else if (image.at<uchar>(0, j) == firstpair.first + 1) {
				embedInfo += '1';
				image.at<uchar>(0, j)--;
				if ((!isIni) && embedInfo.length() == 56) {
					Ini(embedInfo, addinfo);
					isIni = true;
				}
				if (embedInfo.length() == (addinfo.toembedLength + addinfo.totalAddLength)) {
					goto here;
				}
			}
		}
		for (int i = 1; i < image.rows; i++) {
			for (int j = 0; j < image.cols; j++) {
				if (image.at<uchar>(i, j) == firstpair.first) {
					embedInfo += '0';
					if ((!isIni) && embedInfo.length() == 56) {
						Ini(embedInfo, addinfo);
						isIni = true;
					}
					if (embedInfo.length() == (addinfo.toembedLength + addinfo.totalAddLength)) {
						goto here;
					}
				}
				else if (image.at<uchar>(i, j) == firstpair.first + 1) {
					embedInfo += '1';
					image.at<uchar>(i, j)--;
					if ((!isIni) && embedInfo.length() == 56) {
						Ini(embedInfo, addinfo);
						isIni = true;
					}
					if (embedInfo.length() == (addinfo.toembedLength + addinfo.totalAddLength)) {
						goto here;
					}
				}
				
			}
		}

	}




	
	//提取全部的信息（如果提取了第一对点的信息就完成了全部信息的提取，那么以下代码就不会执行）
	while (1) {
		if (embedInfo.length() == (addinfo.toembedLength + addinfo.totalAddLength)) {
			goto here;
		}
		times++;
		string max = embedInfo.substr(56 + 16 * times, 8);
		string min = embedInfo.substr(56 + 16 * times + 8, 8);
		int thisPairMax = 0, thisPairMin = 0;
		for (int i = 0, mask = 1; i < 8; i++) {
			if (max[7 - i] == '1') {
				thisPairMax += mask;
			}
			if (min[7 - i] == '1') {
				thisPairMin += mask;
			}
			mask <<= 1;
		}

		thispair.first = thisPairMax;
		thispair.second = thisPairMin;
		addinfo.pairs.push_back(thispair);
		if (thispair.first > thispair.second) {
			for (int j = 2; j < image.cols; j++) {
				if (image.at<uchar>(0, j) == thispair.first) {
					embedInfo += '0';
				}
				else if (image.at<uchar>(0, j) == thispair.first - 1) {
					embedInfo += '1';
					image.at<uchar>(0, j)++;
				}
				if (embedInfo.length() == (addinfo.toembedLength + addinfo.totalAddLength)) {
					goto here;
				}
			}
			for (int i = 1; i < image.rows; i++) {
				for (int j = 0; j < image.cols; j++) {
					if (image.at<uchar>(i, j) == thispair.first) {
						embedInfo += '0';
					}
					else if (image.at<uchar>(i, j) == thispair.first - 1) {
						embedInfo += '1';
						image.at<uchar>(i, j)++;
					}
					if (embedInfo.length() == (addinfo.toembedLength + addinfo.totalAddLength)) {
						goto here;
					}
				}
			}
		}
		else {
			for (int j = 2; j < image.cols; j++) {
				if (image.at<uchar>(0, j) == thispair.first) {
					embedInfo += '0';
				}
				else if (image.at<uchar>(0, j) == thispair.first + 1) {
					embedInfo += '1';
					image.at<uchar>(0, j)--;
				}

				if (embedInfo.length() == (addinfo.toembedLength + addinfo.totalAddLength)) {
					goto here;
				}

			}
			for (int i = 1; i < image.rows; i++) {
				for (int j = 0; j < image.cols; j++) {
					if (image.at<uchar>(i, j) == thispair.first) {
						embedInfo += '0';
					}
					else if (image.at<uchar>(i, j) == thispair.first + 1) {
						embedInfo += '1';
						image.at<uchar>(i, j)--;
					}
					if (embedInfo.length() == (addinfo.toembedLength + addinfo.totalAddLength)) {
						goto here;
					}
				}
			}
		}
		
	}

here:
	cout << "嵌入的信息为：" << embedInfo.substr(addinfo.totalAddLength) << endl;

	//构造附加信息类的实例
	int row = 0;
	int col = 0;
	for (int i = 1; i < row; i <<= 1) {
		row++;
	}
	for (int i = 1; i < col; i <<= 1) {
		col++;
	}
	times++;
	int timeFor_bitmapMinLength = 0;
	int timeFor_bitmap1 = 0;
	int temp_lengthForBitmapMinLength = addinfo.lengthForBitmapMinLength;

	while (temp_lengthForBitmapMinLength != 0) {
		string bitmapMinLength = embedInfo.substr(56 + 16 * times + 8 * timeFor_bitmapMinLength, 8);
		int thisbitmapMinLength = 0;
		for (int i = 7, mask = 1; i >= 0; i++) {
			if (bitmapMinLength[7 - i] == '1') {
				thisbitmapMinLength += mask;
			}
			mask <<= 1;
		}
		addinfo.bitmapMinLength.push_back(thisbitmapMinLength);
		temp_lengthForBitmapMinLength--;
		timeFor_bitmapMinLength++;
		continue;
	}

	for (int i = 0; i < addinfo.bitmapMinLength.size(); i++) {
		int temp = addinfo.bitmapMinLength[i];
		string bitmap1 = embedInfo.substr(56 + 16 * times + 8 * timeFor_bitmapMinLength + (row + col) * timeFor_bitmap1, (row + col));
		string rows = bitmap1.substr(0, row), cols = bitmap1.substr(row);
		int thisrow = 0, thiscol = 0;
		for (int rowsIndex = rows.length() - 1, mask = 1; rowsIndex >= 0; rowsIndex--) {
			if (rows[rowsIndex] == '1') {
				thisrow += mask;
			}
			mask <<= 1;
		}
		for (int colsIndex = cols.length() - 1, mask = 1; colsIndex >= 0; colsIndex--) {
			if (cols[colsIndex] == '1') {
				thisrow += mask;
			}
			mask <<= 1;
		}
		addinfo.bitmap1.push_back(make_pair(thisrow, thiscol));
	}

	//进行图像复原

	for (int pairIndex = 0; pairIndex < addinfo.pairs.size(); pairIndex++) {
		pair<int, int>thispair = addinfo.pairs[pairIndex];
		if (thispair.first > thispair.second) {

			//第一趟处理：移位
			for (int j = 2; j < image.cols; j++) {
				if (image.at<uchar>(0, j) >= thispair.second && image.at<uchar>(0, j) < thispair.first) {
					image.at<uchar>(0, j)++;
				}
			}
			for (int i = 1; i < image.rows; i++) {
				for (int j = 0; j < image.cols; j++) {
					if (image.at<uchar>(i, j) >= thispair.second && image.at<uchar>(i, j) < thispair.first) {
						image.at<uchar>(i, j)++;
					}
				}
			}
		}
		else {
			//第一趟处理：移位
			for (int j = 2; j < image.cols; j++) {
				if (image.at<uchar>(0, j) <= thispair.second && image.at<uchar>(0, j) > thispair.first) {
					image.at<uchar>(0, j)--;
				}
			}
			for (int i = 1; i < image.rows; i++) {
				for (int j = 0; j < image.cols; j++) {
					if (image.at<uchar>(i, j) <= thispair.second && image.at<uchar>(i, j) > thispair.first) {
						image.at<uchar>(i, j)--;
					}
				}
			}
		}

		int count = 0;
		for (int i = 0; i < addinfo.bitmapMinLength.size(); i++) {
			for (int j = 0; j < addinfo.bitmapMinLength[i]; j++) {
				image.at<uchar>(addinfo.bitmap1[count + j].first, addinfo.bitmap1[count + j].second) = thispair.second;
			}
			count += i;
		}
	}

there:
	imwrite("output.bmp", image);


	return 0;
}