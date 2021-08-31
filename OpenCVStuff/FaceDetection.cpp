#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>

using namespace std;
using namespace cv;

void main()
{
	string path = "Resources/test.png"; //faces-1.jpg
	Mat img = imread(path);
	CascadeClassifier faceCascade;
	faceCascade.load("Resources/haarcascade_frontalface_default.xml");
		
	if (faceCascade.empty())
	{
		cout << "XML file not loaded" << endl;
	}

	vector<Rect> faces;
	// scale factor - 1.1 defines how much the image size is reduced to scale to better detect faces. 
	// reducing the size by 10% thereby increasing the chance that a face is found. https://stackoverflow.com/questions/36218385/parameters-of-detectmultiscale-in-opencv-using-python

	faceCascade.detectMultiScale(img, faces, 1.5, 3);
	for (int i = 0; i < faces.size(); i++)
	{
		rectangle(img, faces[i].tl(), faces[i].br(), Scalar(255, 0, 255), 3);
	}
	imshow("Image", img);
	waitKey(0);
}