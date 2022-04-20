#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/imgproc.hpp"
#include <iostream>
#include<fstream>
using namespace cv;
using namespace std;

int main(int argc, char** argv) {
	if (argc == 3) {
		ifstream ifile1(argv[1]);
		string sourceInfo;
		ifile1 >> sourceInfo;

		ifstream ifile2(argv[2]);
		string ouputInfo;
		ifile2 >> ouputInfo;

		int index = 0;
		while (1) {
			if (sourceInfo[index] != ouputInfo[index]) {
				cout<< "未通过字符串检验，第一次不匹配的位置为" << index + 1 << endl;
				break;
				
			}
			index++;
			if (index == sourceInfo.size() - 1 || index == ouputInfo.size() - 1) {
				cout << index;
				break;
			}
		}
		return -1;
	}
	if (argc != 5) {
		cout << "请输入四个参数，前两个为嵌入图片路径和要嵌入的信息，后两个为decoder生成的图片和输出文本" << endl;
		return -1;
	}
	ifstream ifile1(argv[2]); 
	string sourceInfo ;
	ifile1 >> sourceInfo;

	ifstream ifile2(argv[4]);
	string ouputInfo;
	ifile2 >> ouputInfo;
	
	Mat sourceImage;
	sourceImage = imread(argv[1], IMREAD_GRAYSCALE);

	Mat outputImage;
	outputImage = imread(argv[3], IMREAD_GRAYSCALE);


	bool infoFlag = true, imageFlag = true;
	if (sourceInfo.length() != ouputInfo.length()) {
		cout << "未通过字符串检验，字符串长度不相等"<<endl ;
		infoFlag = false;
	}
	if (infoFlag) {
		for (int i = 0; i < sourceInfo.length(); i++) {
			if (sourceInfo[i] != ouputInfo[i]) {
				infoFlag = false;
				cout << "未通过字符串检验，第一次不匹配的位置为" << i + 1 << endl;
				break;
			}
		}
	}


	for (int i = 0; i < sourceImage.rows; i++) {
		for (int j = 0; j < sourceImage.cols; j++) {
			if (sourceImage.at<uchar>(i, j) != outputImage.at<uchar>(i, j)) {
				int x = sourceImage.at<uchar>(i, j), y = outputImage.at<uchar>(i, j);
				cout << "未通过图片检验，第一次不匹配的位置为" << i << ' '<< j << endl;
				imageFlag = false;
				goto here;
			}
		}
	}

here:
	if (infoFlag ) {
		cout << "已通过字符串检验" << endl;
	}
	if (imageFlag) {
		cout << "已通过图片检验" << endl;
	}
	return 0;
}