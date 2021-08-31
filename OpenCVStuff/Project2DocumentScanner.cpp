#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Mat imgOriginal, imgGray, imgBlur, imgCanny, imgThre, imgDil, imgErode, imgWarp, imgCrop;
vector<Point> initialPoints, docPoints;

//Width and height of A4 paper
float w = 420, h = 596;
Mat preProcessing(Mat img)
{
	cvtColor(img, imgGray, COLOR_BGR2GRAY);
	GaussianBlur(imgGray, imgBlur, Size(3, 3), 3, 0);
	Canny(imgBlur, imgCanny, 25, 75);
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
	dilate(imgCanny, imgDil, kernel);
	//erode(imgDil, imgErode, kernel);
	return imgDil;
}

vector<Point> getContours(Mat image)
{

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	findContours(image, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	//drawContours(img, contours, -1, Scalar(255, 0, 255), 2);
	vector<vector<Point>> conPoly(contours.size());
	vector<Rect> boundRect(contours.size());

	vector<Point> biggest;
	//Initially maxArea will be 0, then we keep updating the area as it increases in size.
	//The paper will have the max area.
	int maxArea = 0;

	for (int i = 0; i < contours.size(); i++)
	{
		int area = contourArea(contours[i]);
		//cout << area << endl;



		if (area > 1000)
		{
			float peri = arcLength(contours[i], true);
			//Finding the minimum number of vertices of a Polygon given the points of a contour. Reduces the number of coordinate points.
			approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);

			if (area > maxArea && conPoly[i].size() == 4) {

				//drawContours(imgOriginal, conPoly, i, Scalar(255, 0, 255), 5);
				biggest = { conPoly[i][0],conPoly[i][1] ,conPoly[i][2] ,conPoly[i][3] };
				maxArea = area;
			}
			//drawContours(imgOriginal, conPoly, i, Scalar(255, 0, 255), 2);
			//rectangle(imgOriginal, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 5);
		}
	}
	return biggest;
}

void drawPoints(vector<Point> points, Scalar color)
{
	for (int i = 0; i < points.size(); i++)
	{
		cout << "points: " << points[i] << endl;
		circle(imgOriginal, points[i], 10, color, FILLED);
		putText(imgOriginal, to_string(i), points[i], FONT_HERSHEY_PLAIN, 4, color, 4);
	}
}

//Reorders the points to top-left: 0, top-right: 1, bottom-left: 2, bottom-right:3
	//Calculating the sum of each coordinate x+y
	/*
	* (23,45), (400,30), (350,400), (25,300) = [68,430,750,325]
	* where we can see that the lowest belongs to top-left i.e. (0,0) and the highest bottom-right (1,1)
	* vector<Point> =
	*/
vector<Point> reorder(vector<Point> points)
{
	vector<Point> newPoints;

	vector<int> sumPoints, subPoints;
	for (int i = 0; i < 4; i++)
	{
		sumPoints.push_back(points[i].x + points[i].y);
		subPoints.push_back(points[i].x - points[i].y);
	}

	newPoints.push_back(points[min_element(sumPoints.begin(), sumPoints.end()) - sumPoints.begin()]); //0 - top-left
	newPoints.push_back(points[max_element(subPoints.begin(), subPoints.end()) - subPoints.begin()]); //1 - bottom-left
	newPoints.push_back(points[min_element(subPoints.begin(), subPoints.end()) - subPoints.begin()]); //2 - top-right
	newPoints.push_back(points[max_element(sumPoints.begin(), sumPoints.end()) - sumPoints.begin()]); //3 - bottom-right
	
	return newPoints;
}

Mat getWarp(Mat img, vector<Point> points, float w, float h)
{
	Point2f source[4] = { points[0], points[1], points[2], points[3] };
	Point2f destination[4] = { {0.0f,0.0f}, {w,0.0f}, {0.0f, h}, {w,h} };


	//USing transformation matrix
	Mat matrix = getPerspectiveTransform(source, destination);
	warpPerspective(img, imgWarp, matrix, Point(w, h));

	return imgWarp;
}

void main()
{

	string path = "Resources/paper.jpg";
	imgOriginal = imread(path);
	//resize(imgOriginal, imgOriginal, Size(), 0.5, 0.5);

	//Convert to grayscale, add blue and then use canny edge detector to find all edges. Once the edges are found we know the coordinates of the paper.
	//Based on the coordinates we can extract the 4 corners of the paper. Then we can warp the image to get the top view so that it looks scanned.

	//Preprocessing

	// Preprpcessing - Step 1 
	imgThre = preProcessing(imgOriginal);

	// Get Contours - Biggest  - Step 2
	initialPoints = getContours(imgThre);
	cout << "intialPoints " << initialPoints << endl;
	//drawPoints(initialPoints, Scalar(0, 0, 255));
	docPoints = reorder(initialPoints);
	//drawPoints(docPoints, Scalar(0, 255, 0));

	//Warp
	imgWarp = getWarp(imgOriginal, docPoints, w, h);
	
	//Crop
	int cropVal = 5;
	Rect roi(cropVal, cropVal, w - (2 * cropVal), h - (2 * cropVal)); //(x_coordinate of top-left, y-coordinate of top-right, width, height)
	imgCrop = imgWarp(roi);
	imshow("Image", imgOriginal);
	imshow("Image Dilation", imgThre);
	imshow("Image Warp", imgWarp);
	imshow("Image Crop", imgCrop);
	waitKey(0);
}