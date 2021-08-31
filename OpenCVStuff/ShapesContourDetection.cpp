#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

Mat imgGray, imgBlur, imgCanny, imgDil;

void getContours(Mat imgDil, Mat img)
{
	// Vectors inside a vector. for each vector there is a contour, where each contour is a Point
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	//drawContours(img, contours, -1, Scalar(255, 0, 255), 2);

	vector<vector<Point>> conPoly(contours.size());
	vector<Rect> boundRect(contours.size()); 
	string objType;
	for (auto i = 0; i < contours.size(); i++)
	{
		int area = contourArea(contours[i]);
		cout << area << endl;

		if (area > 500)
		{
			float perimeter = arcLength(contours[i], true);
			approxPolyDP(contours[i], conPoly[i], 0.02 * perimeter, true); //approximating a curve to a specificed precision to find the minimum nummber of contours that can represent the shape
			
			cout << conPoly[i].size() << endl;
			//Bounding box
			boundRect[i] = boundingRect(conPoly[i]);
			

			int objCor = (int)conPoly[i].size();
			if (objCor == 3)
				objType = "Triangle";
			else if (objCor == 4)
			{
				float aspRatio = (float)boundRect[i].width / (float)boundRect[i].height;
				cout << aspRatio << endl;
				if (0.95 < aspRatio < 1.05)
					objType = "Square";
				else
					objType = "Rectangle";
			}	
			else if (objCor == 5)
				objType = "Pentagon";
			else if (objCor == 6)
				objType = "Hexagon";
			else if (objCor == 10)
				objType = "Star";
			else
				objType = "Circle";

			drawContours(img, contours, i, Scalar(255, 0, 255), 2);
			rectangle(img, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 5);
			putText(img, objType, { boundRect[i].x, boundRect[i].y - 5 }, FONT_HERSHEY_PLAIN, 1.75, Scalar(0, 69, 255), 1);
		}
	}
}


 //Importing images
void main()
{
	string path = "Resources/shapes4.png";
	Mat img = imread(path); //Mat - matrix datatype

	//Detect edges and find contour points
	cvtColor(img, imgGray, COLOR_BGR2GRAY);
	GaussianBlur(imgGray, imgBlur, Size(3, 3), 3, 0);
	Canny(img, imgCanny, 25, 75);
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
	dilate(imgCanny, imgDil, kernel);

	getContours(imgDil, img);


	imshow("Image", img);
	//imshow("Image Gray", imgGray);
	//imshow("Image Blur", imgBlur);
	//imshow("Image Canny", imgCanny);
	//imshow("Image Dilate", imgDil);

	waitKey(0); // 0 for infinity

}