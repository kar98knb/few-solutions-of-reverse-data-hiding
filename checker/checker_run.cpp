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
				cout<< "δͨ���ַ������飬��һ�β�ƥ���λ��Ϊ" << index + 1 << endl;
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
		cout << "�������ĸ�������ǰ����ΪǶ��ͼƬ·����ҪǶ�����Ϣ��������Ϊdecoder���ɵ�ͼƬ������ı�" << endl;
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
		cout << "δͨ���ַ������飬�ַ������Ȳ����"<<endl ;
		infoFlag = false;
	}
	if (infoFlag) {
		for (int i = 0; i < sourceInfo.length(); i++) {
			if (sourceInfo[i] != ouputInfo[i]) {
				infoFlag = false;
				cout << "δͨ���ַ������飬��һ�β�ƥ���λ��Ϊ" << i + 1 << endl;
				break;
			}
		}
	}


	for (int i = 0; i < sourceImage.rows; i++) {
		for (int j = 0; j < sourceImage.cols; j++) {
			if (sourceImage.at<uchar>(i, j) != outputImage.at<uchar>(i, j)) {
				int x = sourceImage.at<uchar>(i, j), y = outputImage.at<uchar>(i, j);
				cout << "δͨ��ͼƬ���飬��һ�β�ƥ���λ��Ϊ" << i << ' '<< j << endl;
				imageFlag = false;
				goto here;
			}
		}
	}

here:
	if (infoFlag ) {
		cout << "��ͨ���ַ�������" << endl;
	}
	if (imageFlag) {
		cout << "��ͨ��ͼƬ����" << endl;
	}
	return 0;
}