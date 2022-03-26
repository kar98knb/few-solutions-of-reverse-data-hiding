#define _CRT_SECURE_NO_WARNINGS

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/imgproc.hpp"
#include <iostream>
#include<fstream>
#include <cassert>
#include<json/json.h>
using namespace cv;
using namespace std;


int main(int argc, char** argv) {
	if (argc != 3) {
		cout << "请输入两个参数，第一个为图片路径，第二个为存放信息的json文件" << endl;
		return -1;
	}

	Mat image;
	image = imread(argv[1], IMREAD_GRAYSCALE); // Read the file

	string addInfo = argv[2];
	Json::Reader reader;
	Json::Value root;
	ifstream ifs;
	ifs.open(addInfo);
	if (!reader.parse(ifs, root, false))
	{
		cout << "reader parse error: " << strerror(errno) << endl;
		return -1;
	}

	int length = root["length"].asInt();
	vector<int> mask;
	for (int i = 0; i < 256; i++) {
		mask.push_back(root["mask"][i].asInt());
	}

	
	for (int flag = 0, index = 0, i = 0; i < 256; i++) {
		if (mask[i] == 3) { flag = 1; index = i;}
		else if (mask[i] == -2) { flag = -1;  index = i;}

		if (flag == 0) {continue;}
		else if (flag == 1) {
			if (mask[i] == 2) {
				mask[index+1] = 2;
				mask[i] = 1;
				flag = 0;
			}
		}
		else if (flag == -1) {
			if (mask[i] == -3) {
				mask[index] = -2;
				mask[i-1] = -1;
				flag = 0;
			}
		}
	}


	string embedInfo;
	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j++) {
			if (mask[image.at<uchar>(i, j)] == 0) {
				continue;
			}
			else if (mask[image.at<uchar>(i, j)] == 1) {
				image.at<uchar>(i, j)--;
			}
			else if (mask[image.at<uchar>(i, j)] == 2) {
				embedInfo += "1";
				image.at<uchar>(i, j)--;
			}
			else if (mask[image.at<uchar>(i, j)] == 3) {
				embedInfo += "0";
			}
			else if (mask[image.at<uchar>(i, j)] == -1) {
				image.at<uchar>(i, j)++;
			}
			else if (mask[image.at<uchar>(i, j)] == -2) {
				embedInfo += "1";
				image.at<uchar>(i, j)++;
			}
			else if (mask[image.at<uchar>(i, j)] == -3) {
				embedInfo += "0";
			}

			if (embedInfo.length() == length) {
				goto here;
			}
		}
	}


here:
	imwrite("output.bmp", image);
	cout << embedInfo << endl;

	return 0;
}