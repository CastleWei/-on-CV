//#include <opencv.hpp>
#include <iostream>
//using namespace cv;
using namespace std;

void _3main(){
	//Mat img;
	//VideoCapture cap(1);
	//while (1){
	//	cap >> img;
	//	imshow("", img);
	//	int key = waitKey(30);
	//}
	
	int ba = 3, bb = 8;
	cout << "ba= " << ba << endl;
	cout << "bb= " << bb << endl;
	int x;
	cin >> x;
	//˵�������� �ʺű��ʽ �� ���ø�ֵ
	int &b = (x > 2 ? ba : bb);
	cout << "b= " << b << endl;
	b = 7;
	cout << "new ba= " << ba << endl;
	cout << "new bb= " << bb << endl;


}