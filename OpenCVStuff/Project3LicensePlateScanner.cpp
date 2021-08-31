#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>

using namespace std;
using namespace cv;

using namespace std;
using namespace cv;

void main()
{
	//string path = "Resources/2.mp4";
	VideoCapture cap(0);
	Mat img;


	CascadeClassifier plateCascade;
	plateCascade.load("Resources/plate_number2.xml");

	if (plateCascade.empty())
	{
		cout << "XML file not loaded" << endl;
	}

	vector<Rect> plates;
	// scale factor - 1.1 defines how much the image size is reduced to scale to better detect faces. 
	// reducing the size by 10% thereby increasing the chance that a face is found. https://stackoverflow.com/questions/36218385/parameters-of-detectmultiscale-in-opencv-using-python

	while (true)
	{
		cap.read(img);
		plateCascade.detectMultiScale(img, plates, 1.5, 3);
		for (int i = 0; i < plates.size(); i++)
		{
			Mat imgCrop = img(plates[i]);
			//imshow(to_string(i), imgCrop);
			imwrite("Resources/Plates/" + to_string(i) + ".png", imgCrop);
			rectangle(img, plates[i].tl(), plates[i].br(), Scalar(255, 0, 255), 3);
		}
		imshow("Image", img);
		waitKey(1); // 0 for infinity
	}

	imshow("Image", img);
	waitKey(1);


}