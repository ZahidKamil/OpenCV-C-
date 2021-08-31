#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

/*
Detect color using HSV space. Then find contours, take the x and y coordinates of those contours and create a circle at that point. 
*/


VideoCapture cap(0);
Mat img;
void findColor(Mat img);

//Vectors in vector to hold {x,y,color} where color = 0 | 1 | 2 for purple, green, blue
vector<vector<int>> newPoints; 

//Colors to detect <<hmin, smin, vmin, hmax, smax, vmax >>
vector<vector<int>> myColors{ {124,48,117,143,170,255}, //purple
								{68,72,156,102,126,255}, //green
									{83,0,127,160,253,255} }; //blue

//Colors to show when the colors are detected.
vector<Scalar> myColorValues{ {255,0,255}, //purple 
								{0,255,0},
									{255,0,0} }; //green

void findColor(Mat img);
Point getContours(Mat imgDil);
void drawOnCanvas(vector<vector<int>> newPoints, vector<Scalar> myColorValues);

void main()
{
	
	
	while (true)
	{
		cap.read(img);
		findColor(img);
		drawOnCanvas(newPoints, myColorValues);
		imshow("Image", img);
		waitKey(1); // 0 for infinity

	}


}


Point getContours(Mat imgDil)
{
	// Vectors inside a vector. for each vector there is a contour, where each contour is a Point
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	//drawContours(img, contours, -1, Scalar(255, 0, 255), 2);

	vector<vector<Point>> conPoly(contours.size());
	vector<Rect> boundRect(contours.size());
	Point myPoint(0, 0);

	for (auto i = 0; i < contours.size(); i++)
	{
		int area = contourArea(contours[i]);
		cout << area << endl;

		if (area > 1000)
		{
			float perimeter = arcLength(contours[i], true);
			approxPolyDP(contours[i], conPoly[i], 0.02 * perimeter, true); //approximating a curve to a specificed precision to find the minimum nummber of contours that can represent the shape

			cout << conPoly[i].size() << endl;
			//Bounding box
			boundRect[i] = boundingRect(conPoly[i]);
			myPoint.x = boundRect[i].x + boundRect[i].width / 2; //draw from the center
			myPoint.y = boundRect[i].y;

			rectangle(img, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 5);
			drawContours(img, contours, i, Scalar(255, 0, 255), 2);
		}
	}

	return myPoint;
}


void findColor(Mat img)
{
	/*
	Array of all the min and max of colors. Loop through it and find an image. Have multiple masks for each color 
	*/
	Mat imgHSV;
	cvtColor(img, imgHSV, COLOR_BGR2HSV);
	//For each of the colors find a mask
	for (int i = 0; i < myColors.size(); i++)
	{
		Scalar lower(myColors[i][0], myColors[i][1], myColors[i][2]);
		Scalar upper(myColors[i][3], myColors[i][4], myColors[i][5]);
		Mat mask;
		inRange(imgHSV, lower, upper, mask);
		//imshow(to_string(i), mask);
		Point myPoints = getContours(mask);

		//Adding the points/contours to the vector of vectors of int myPoints
		if (myPoints.x != 0 && myPoints.y != 0)
		{
			newPoints.push_back({ myPoints.x, myPoints.y, i });
		}
		
	}
	//return newPoints;

}

void drawOnCanvas(vector<vector<int>> newPoints, vector<Scalar> myColorValues)
{
	for (int i = 0; i < newPoints.size(); i++)
	{
		circle(img, Point(newPoints[i][0], newPoints[i][1]), 10, myColorValues[newPoints[i][2]], FILLED);
	}
}

