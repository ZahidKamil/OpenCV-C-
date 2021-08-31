#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

// Resize and Crop

void main()
{
	string path = "Resources/test.png";
	Mat img = imread(path); //Mat - matrix datatype
	Mat imgResize, imgCrop;

	cout << img.size() << endl; //[860 x 1060]
	//resize(img, imgResize, Size(480, 640)); 
	resize(img, imgResize, Size(), 0.5,0.5); // To scale instead of reizing 

	Rect roi(100, 550, 300, 250); // (x_start, y_start, width, height)
	imgCrop = img(roi);


	imshow("Image", img);
	imshow("Image Resize", imgResize);
	imshow("Image Crop", imgCrop);



	waitKey(0); // 0 for infinity

}