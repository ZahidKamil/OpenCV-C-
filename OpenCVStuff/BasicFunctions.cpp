#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

void main()
{
	string path = "Resources/test.png";
	Mat img = imread(path); //Mat - matrix datatype
	Mat imgGray, imgBlur, imgCanny, imgDilate, imgErode;
	cvtColor(img, imgGray, COLOR_BGR2GRAY);
	GaussianBlur(imgGray, imgBlur, Size(3, 3), 3, 0);
	Canny(imgBlur, imgCanny, 25,75); //Canny edge detector - works better with a blurred image - use trackbar

	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
	dilate(imgCanny, imgDilate, kernel);
	erode(imgDilate, imgErode, kernel);


	imshow("Image", img);
	imshow("Image Gray", imgGray);
	imshow("Image Blur", imgBlur);
	imshow("Image Canny", imgCanny);
	imshow("Image Dilate", imgDilate);
	imshow("Image Erode", imgErode);

	waitKey(0); // 0 for infinity

}