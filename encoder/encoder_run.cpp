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



void usingHis(Mat image, string toEmbed);
void usingDE(Mat image, string toEmbed);

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

pair<int, int> find_Nth_pair(addInfoHis addInfo, Mat hist) {

	int minIndex1, minIndex2 , maxIndex;
	if (addInfo.mask[0] != 0) {
		bool flag = false;;
		for (int i = 0; i < 256; i++)
		{
			if (!flag&&addInfo.mask[i] == 0) {
				maxIndex = i;
				flag = true;
			}
			if (flag) {
				if ((hist.at<float>(i) > hist.at<float>(maxIndex)) && addInfo.mask[i] == 0) {
					maxIndex = i;
				}
			}
		}
	}
	else {
		maxIndex = 0;
		for (int i = 0; i < 256; i++)
		{
			if ((hist.at<float>(i) > hist.at<float>(maxIndex)) && addInfo.mask[i] == 0) {
				maxIndex = i;
			}
		}
	}
	
	minIndex1 = maxIndex;
	minIndex2 = maxIndex;
	for (int i = maxIndex; i < 256; i++)
	{
		if (addInfo.mask[i] != 0) {
			break;
		}
		if (hist.at<float>(i) == 0) {
			minIndex1 = i;
			break;
		}
		if ((hist.at<float>(i) < hist.at<float>(minIndex1)) && addInfo.mask[i] == 0) {
			minIndex1 = i;
			continue;
		}
		
	}

	for (int i = maxIndex; i >= 0; i--)
	{
		if (addInfo.mask[i] != 0) {
			break;
		}
		if (hist.at<float>(i) == 0) {
			minIndex2 = i;
			break;
		}
		if ((hist.at<float>(i) < hist.at<float>(minIndex2)) && addInfo.mask[i] == 0) {
			minIndex2 = i;
			continue;
		}
	}
	

	if (hist.at<float>(minIndex2) < hist.at<float>(minIndex1)) {
		return make_pair(maxIndex, minIndex2);
	}
	else {
		return make_pair(maxIndex, minIndex1);
	}
}

bool isExpandable(int mean, int dif) {
	int min = (2 * (255 - mean) < 2 * mean + 1) ? 2 * (255 - mean): 2 * mean + 1;
	if (abs(2 * dif + 1) < min && abs(2 * dif) < min) {
		return true;
	}
	else {
		return false;
	}
}

bool isChangeable(int mean, int dif) {
	int min = (2 * (255 - mean) < 2 * mean + 1) ? 2 * (255 - mean) : 2 * mean + 1;
	if (abs(2 * (dif>>1) + 1) < min && abs(2 * (dif>>1)) < min) {
		return true;
	}
	else {
		return false;
	}
}

string limitString(int limit, int count) {
	count -= 1;
	string ans;
	for (int i = 0; i < limit; i++,count>>=1) {
		ans = to_string(count & 1) + ans;
	}
	return ans;
}

string compressBitmap(string bitmap, int limit) {
	string ans;
	string temp;
	bool isone = bitmap[0]=='1'? true:false;
	int maxLimit = 1 << limit;
	int minLimit = limit;
	for (int i = 0; i < bitmap.length(); i++) {
		if (isone && bitmap[i] == '1') {
			if (temp.length() < maxLimit) {
				temp += bitmap[i];
			}
			else {
				ans += '1';
				ans += '1';
				ans += limitString(limit,temp.length());
				temp.clear();
				temp += bitmap[i];
			}
		}
		else if (isone && bitmap[i] == '0') {
			if (temp.length() < minLimit) {
				while (temp.size() < minLimit) {
					temp += bitmap[i];
					i++;
				}
				ans += '0';
				ans += '0';
				ans += temp;
				temp.clear();
				temp += bitmap[i];
				isone = (bitmap[i] == '1' ? true : false);
			}
			else {
				ans += '1';
				ans += '1';
				ans += limitString(limit, temp.length());
				temp.clear();
				temp += bitmap[i];
				isone = (bitmap[i] == '1' ? true : false);
			}
			
		}
		else if (!isone && bitmap[i] == '1') {
			if (temp.length() < minLimit) {
				while (temp.size() < minLimit) {
					temp += bitmap[i];
					i++;
				}
				ans += '0';
				ans += '0';
				ans += temp;
				temp.clear();
				temp += bitmap[i];
				isone = (bitmap[i] == '1' ? true : false);
			}
			else {
				ans += '1';
				ans += '0';
				ans += limitString(limit, temp.length());
				temp.clear();
				temp += bitmap[i];
				isone = (bitmap[i] == '1' ? true : false);
			}
		}
		else if(!isone && bitmap[i] == '0') {
			if (temp.length() < maxLimit) {
				temp+=bitmap[i];
			}
			else {
				ans += '1';
				ans += '0';
				ans += limitString(limit, temp.length());
				temp.clear();
				temp += bitmap[i];
			}
		}
	}

	if (temp.size() != 0) {
		if (temp.size() < minLimit) {
			while (temp.size() < minLimit) {
				temp.push_back('0');
			}
			ans += '0';
			ans += '0';
			ans += temp;
		}
		else {
			if (isone) {
				ans += '1';
				ans += '1';
				ans += limitString(limit, temp.length());
			}
			else {
				ans += '1';
				ans += '0';
				ans += limitString(limit, temp.length());
			}
		}
	}
	return ans;
}


int main(int argc, char** argv)
{
	if (argc != 4) {
		cout << "请输入两个参数，第一个为嵌入图片路径，第二个为要嵌入的信息，第三个为你想使用的嵌入算法，-h为直方图算法，-d为差分扩展" << endl ;
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


	if (argv[3][1]=='h') {
		usingHis(image, toEmbed);
		return 0;
	}
	else if (argv[3][1] == 'd') {
		usingDE(image, toEmbed);
		return 0;
	}
	else {
		cout << "请输入正确参数";
		return -1;
	}
	return -1;
}


void usingHis(Mat image, string toEmbed) {
	int histSize = 256;
	float range[] = { 0, 256 }; //the upper boundary is exclusive
	const float* histRange[] = { range };
	bool uniform = true, accumulate = false;
	Mat hist;
	addInfoHis addInfo(toEmbed.length(), image.at<uchar>(0, 0), image.at<uchar>(0, 1), image.rows, image.cols);
	image.at<uchar>(0, 0) = 0; image.at<uchar>(0, 1) = 0;
	calcHist(&image, 1, 0, Mat(), hist, 1, &histSize, histRange, uniform, accumulate);

	assert(!histIsHorizontal(hist));

	/*for (int i = 0; i < 256; i++) {
		cout << i << ',' << hist.at<float>(i) << endl;
	}*/


	int toEmbedIndex = 0;

	//维护一个描述直方图状态的mask
	while (toEmbedIndex <= toEmbed.length()) {
		pair<int, int> thisPair = find_Nth_pair(addInfo, hist);
		int thisCapacity = returnPureCapacity(thisPair, hist);
		if (thisCapacity <= 0) {
			cout << "fail to embed: info is too long" << endl;
			cout << "这个算法对于这张图片的容量限制为" << toEmbedIndex << "位" << endl;
			exit(-1);
		}
		addInfo.pairs.push_back(thisPair);
		toEmbedIndex += thisCapacity;
		//峰值点在零值点右侧
		if (thisPair.first > thisPair.second) {
			addInfo.mask[thisPair.second] = -2;
			for (int i = thisPair.second + 1; i < thisPair.first; i++) {
				addInfo.mask[i] = -1;
			}
			addInfo.mask[thisPair.first] = -3;
		}
		//峰值点在零值点左侧
		else {
			addInfo.mask[thisPair.first] = 3;
			for (int i = thisPair.first + 1; i < thisPair.second; i++) {
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
	assert(toEmbed.length() == (addInfo.toembedLength + addInfo.totalAddLength));
	int embedIndex = 0;
	int count1 = 0, count2 = 0, count3 = 0;
	for (int pairIndex = 0; pairIndex < addInfo.pairs.size(); pairIndex++) {
		pair<int, int>thispair = addInfo.pairs[pairIndex];
		if (thispair.first > thispair.second) {
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
				if (image.at<uchar>(0, j) == thispair.first) {
					if (toEmbed[embedIndex] == '1') {
						image.at<uchar>(0, j)--;
					}
					embedIndex++;
					if (embedIndex == toEmbed.length()) {
						goto here1;
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
							goto here1;
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
						goto here1;
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
							goto here1;
						}
					}
				}
			}
		}
	}
here1:
	imwrite("embed.bmp", image);
	cout << "success!" << endl;
}

void usingDE(Mat image, string toEmbed) {
	addInfoDE addInfoDE;
	string bitmap;
	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j+=2) {
			int mean = (image.at<uchar>(i, j) + image.at<uchar>(i, j + 1))>>1;
			int dif = image.at<uchar>(i, j) - image.at<uchar>(i, j + 1);
			if (isExpandable(mean, dif)) {
				if (dif == 0 || dif == -1) {
					addInfoDE.EZ.push_back(make_pair(i, j));
					bitmap += '1';
				}
				else if(abs(dif)<=DEthreshold) {
					addInfoDE.EN1.push_back(make_pair(i, j));
					bitmap += '1';
				}
				else {
					addInfoDE.EN2.push_back(make_pair(i, j));
					bitmap += '0';
					if (dif != 1 && dif != -2) {
						addInfoDE.LSB+=to_string(1& dif);
					}
				}
			}
			else if (isChangeable(mean, dif)) {
				addInfoDE.CN.push_back(make_pair(i, j));
				bitmap += '0';
				if (dif != 1 && dif != -2) {
					addInfoDE.LSB += to_string(1 & dif);
				}
			}
			else {
				addInfoDE.NC.push_back(make_pair(i, j));
				bitmap += '0';
			}
		}
	}

	string compressMap = compressBitmap(bitmap, runlengthcode);
	cout << "EZ:"<< addInfoDE.EZ.size() << endl;
	cout << "EN1:" << addInfoDE.EN1.size() << endl;
	cout << "EN2:" << addInfoDE.EN2.size() << endl;
	cout << "CN:" << addInfoDE.CN.size() << endl;
	cout << "NC:" << addInfoDE.NC.size() << endl;
	cout << "bitmap length:" << bitmap.length() << endl;
	cout << "compressMap length:" << compressMap.length() << endl;
	cout << "LSB length:" << addInfoDE.LSB.size() << endl;
	int offset = offsetLength;
	int capacity = addInfoDE.EZ.size() + addInfoDE.EN1.size() + addInfoDE.EN2.size() - addInfoDE.LSB.size() - compressMap.size()- 2*offset;
	if (toEmbed.size() > capacity) {
		cout << "嵌入失败，超出容量限制"<<endl;
		exit(-1);
	}

	string allEmbed = limitString(offset, toEmbed.size()+1) + compressMap + addInfoDE.LSB + toEmbed;
	allEmbed = limitString(offset, allEmbed.size() + 33) + allEmbed;
	
	int index = 0;
	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j += 2) {
			/*if (i == 0 && j == 0) {
				int a = 0;
			}*/
			int mean = (image.at<uchar>(i, j) + image.at<uchar>(i, j + 1)) >> 1;
			int dif = image.at<uchar>(i, j) - image.at<uchar>(i, j + 1);
			if (isExpandable(mean, dif)) {
				if (dif == 0 || dif == -1) {
					//cout << i << ' ' << j <<' ' << (int)(image.at<uchar>(i, j)) << ' ' << (int)image.at<uchar>(i, j + 1) << ' ';
					int newdif = 2 * dif + (allEmbed[index] == '1' ? 1 : 0);
					int newx = mean + ((newdif > 0) ? ((newdif + 1) / 2) : (newdif / 2));
					int newy = mean - (newdif >>1);
					image.at<uchar>(i, j) = newx; image.at<uchar>(i, j + 1) = newy;
					//cout << ' ' << newx << ' ' << newy << ' ' << (allEmbed[index] == '1' ? 1 : 0) << endl;
				}
				else if (abs(dif) <= DEthreshold) {
					//cout << i << ' ' << j << ' ' << (int)image.at<uchar>(i, j) << ' ' << (int)image.at<uchar>(i, j + 1) << ' ';
					int newdif = 2 * dif + (allEmbed[index] == '1' ? 1 : 0);
					int newx = mean + ((newdif > 0) ? ((newdif + 1) / 2) : (newdif  / 2));
					int newy = mean - (newdif >>1);
					image.at<uchar>(i, j) = newx; image.at<uchar>(i, j + 1) = newy;
					//cout << ' ' << newx << ' ' << newy << ' ' << (allEmbed[index] == '1' ? 1 : 0) << endl;
				}
				else {
					//cout << i << ' ' << j << ' ' << (int)image.at<uchar>(i, j) << ' ' << (int)image.at<uchar>(i, j + 1) << ' ';
					int newdif = 2* (dif>>1) + (allEmbed[index] == '1' ? 1 : 0);
					int newx = mean + ((newdif > 0) ? ((newdif + 1) / 2) : (newdif  / 2));
					int newy = mean - (newdif >>1);
					image.at<uchar>(i, j) = newx; image.at<uchar>(i, j + 1) = newy;
					//cout << ' ' << newx << ' ' << newy << ' ' << (allEmbed[index] == '1' ? 1 : 0) << endl;
				}
			}
			else if (isChangeable(mean, dif)) {
				//cout << i << ' ' << j << ' ' << (int)image.at<uchar>(i, j) << ' ' << (int)image.at<uchar>(i, j + 1) << ' ';
				int newdif = 2*(dif >> 1) + (allEmbed[index] == '1' ? 1 : 0);
				int newx = mean + ((newdif > 0) ? ((newdif + 1) / 2) : (newdif  / 2));
				int newy = mean - (newdif >>1);
				image.at<uchar>(i, j) = newx; image.at<uchar>(i, j + 1) = newy;
				//cout << ' ' << newx << ' ' << newy << ' ' << (allEmbed[index] == '1' ? 1 : 0) << endl;
			}
			else {
				continue;
			}

			index++;
			if (index == allEmbed.size()) {
				goto here2;
			}
		}
	}

here2:
	imwrite("embed.bmp", image);
	cout << "success!" << endl;
}

