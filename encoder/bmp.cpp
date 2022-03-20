/*
这份文件的代码不再被调用，只是在练手时使用的

*/

//#include"bmp.h"
//#include <direct.h>
//#include<iostream>
//#include <vector>
//

//从BMP文件读出数据，返回图像数据
//ImageData* Ini(const char* picName, int& width, int& height) {
//
//	BitMapFileHeader strHead;
//	RGBQUAD strPla[256];//256色调色板
//	BitMapInfoHeader strInfo;
//
//	char* currentPath = _getcwd(NULL, 256);
//	string resPath = currentPath;
//	for (size_t i = resPath.length() - 1; i > 0; i--) {
//		if (resPath[i] != '\\') { continue; }
//		else {
//			resPath = resPath.substr(0, i);
//			resPath += "\\res";
//			break;
//		}
//	}
//
//	string testPath = resPath +"\\" + picName;
//
//	const char* c_s = testPath.c_str();
//	FILE* fpi = fopen(c_s, "rb"); //请一定指定参数为'rb'，在'r'模式下读调色板的时候会发生文件内指针跳转错误
//
//	WORD bfType;
//	fread(&bfType, 1, sizeof(WORD), fpi);
//	if (0x4d42 != bfType)
//	{
//		cout << "the file is not a bmp file!" << endl;
//		return nullptr;
//	}
//
//	//读取bmp文件的文件头和信息头
//	fread(&strHead, 1, sizeof(tagBitMapFileHeader), fpi);
//	fread(&strInfo, 1, sizeof(tagBitMapInfoHeader), fpi);
//
//	for (unsigned int nCounti = 0; nCounti < strInfo.biClrUsed; nCounti++)
//	{
//		fread((char*)&(strPla[nCounti].rgbBlue), 1, sizeof(BYTE), fpi);
//		fread((char*)&(strPla[nCounti].rgbGreen), 1, sizeof(BYTE), fpi);
//		fread((char*)&(strPla[nCounti].rgbRed), 1, sizeof(BYTE), fpi);
//		fread((char*)&(strPla[nCounti].rgbReserved), 1, sizeof(BYTE), fpi);
//	}
//
//	width = strInfo.biWidth;
//	height = strInfo.biHeight;
//	//图像每一行的字节数必须是4的整数倍
//	width = (width * sizeof(ImageData) + 3) / 4 * 4;
//	ImageData* imagedata = (ImageData*)malloc(sizeof(ImageData) * width * height);
//	if (imagedata == NULL) { return nullptr; }
//	else {
//		fread(imagedata, sizeof(ImageData) * width, height, fpi);
//		fclose(fpi);
//	}
//
//	return imagedata;
//}

//int main() {
//	int width, height;
//	//ImageData* image = Ini("lena.bmp", width, height);
//	//cout << width << ' ' << height << endl;
//	vector<int> histogram(256,0);
//	return 0;
//}
