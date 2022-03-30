#include<fstream>
#include<iostream>
#include <ctime>
using namespace std;

//°ïÖúÄãÉú³É³¤01×Ö·û´®
int main(int argc, char** argv) {
	ofstream file("input.txt",ios::app);
	if (argv[1][1] == 'r') {
		string out;
		srand((int)time(0));  //40*32=1280
		for (int i = 0; i < 40; i++)
		{
			int temp = rand();
			for (int j = 31, mask =1; j >= 0; j--) {
				if ((mask & temp) == 1) {
					out = "1" + out;
				}
				else {
					out = "0" + out;
				}
				temp >>= 1;
			}
		}

		file << out;

	}
	return 0;
}