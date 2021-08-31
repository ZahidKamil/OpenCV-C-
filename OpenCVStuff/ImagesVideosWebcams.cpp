#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

// Importing images
//void main()
//{
//	string path = "Resources/test.png";
//	Mat img = imread(path); //Mat - matrix datatype
//	imshow("Image", img);
//	waitKey(0); // 0 for infinity
//
//}

// Video 
//void main()
//{
//	string path = "Resources/2.mp4";
//	VideoCapture cap(path);
//	Mat img;
//	while (true)
//	{
//		cap.read(img);
//		
//		imshow("Image", img);
//		waitKey(20); // 0 for infinity
//	}
//
//
//}

// Video - webcam
void main()
{
	//string path = "Resources/2.mp4";
	VideoCapture cap(0);
	Mat img;
	while (true)
	{
		cap.read(img);

		imshow("Image", img);
		waitKey(1); // 0 for infinity
	}


}

