#pragma once
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef long LONG;

using namespace std;

//位图文件头定义;
//其中不包含文件类型信息（由于结构体的内存结构决定，
//要是加了的话将不能正确读取文件信息）
typedef struct  tagBitMapFileHeader {
	//WORD bfType;//文件类型，必须是0x424D，即字符“BM”
	DWORD bfSize;//文件大小
	WORD bfReserved1;//保留字
	WORD bfReserved2;//保留字
	DWORD bfOffBits;//从文件头到实际位图数据的偏移字节数
}BitMapFileHeader;

typedef struct tagBitMapInfoHeader {
	DWORD biSize;//信息头大小
	LONG biWidth;//图像宽度
	LONG biHeight;//图像高度
	WORD biPlanes;//位平面数，必须为1
	WORD biBitCount;//每像素位数
	DWORD  biCompression; //压缩类型
	DWORD  biSizeImage; //压缩图像大小字节数
	LONG  biXPelsPerMeter; //水平分辨率
	LONG  biYPelsPerMeter; //垂直分辨率
	DWORD  biClrUsed; //位图实际用到的色彩数
	DWORD  biClrImportant; //本位图中重要的色彩数
}BitMapInfoHeader; //位图信息头定义

typedef struct tagRGBQUAD {
	BYTE rgbBlue; //该颜色的蓝色分量
	BYTE rgbGreen; //该颜色的绿色分量
	BYTE rgbRed; //该颜色的红色分量
	BYTE rgbReserved; //保留值
}RGBQUAD;//调色板定义

//像素信息
typedef struct tagImageData
{
	BYTE blue;
	//BYTE green;
	//BYTE red;
}ImageData;

//void showBmpHead(BitMapFileHeader pBmpHead) {
//	cout << "位图文件头:" << endl;
//	cout << "文件大小:" << pBmpHead.bfSize << endl;
//	cout << "保留字_1:" << pBmpHead.bfReserved1 << endl;
//	cout << "保留字_2:" << pBmpHead.bfReserved2 << endl;
//	cout << "实际位图数据的偏移字节数:" << pBmpHead.bfOffBits << endl << endl;
//}

//void showBmpInforHead(BitMapInfoHeader pBmpInforHead) {
//	cout << "位图信息头:" << endl;
//	cout << "结构体的长度:" << pBmpInforHead.biSize << endl;
//	cout << "位图宽:" << pBmpInforHead.biWidth << endl;
//	cout << "位图高:" << pBmpInforHead.biHeight << endl;
//	cout << "biPlanes平面数:" << pBmpInforHead.biPlanes << endl;
//	cout << "biBitCount采用颜色位数:" << pBmpInforHead.biBitCount << endl;
//	cout << "压缩方式:" << pBmpInforHead.biCompression << endl;
//	cout << "biSizeImage实际位图数据占用的字节数:" << pBmpInforHead.biSizeImage << endl;
//	cout << "X方向分辨率:" << pBmpInforHead.biXPelsPerMeter << endl;
//	cout << "Y方向分辨率:" << pBmpInforHead.biYPelsPerMeter << endl;
//	cout << "使用的颜色数:" << pBmpInforHead.biClrUsed << endl;
//	cout << "重要颜色数:" << pBmpInforHead.biClrImportant << endl;
//}

