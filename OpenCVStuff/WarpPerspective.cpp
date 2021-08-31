#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

// Resize and Crop

float w = 250, h = 350;
Mat matrix, matrix1, imgWarp, imgWarp1;
void main()
{
	string path = "Resources/card.jpg";
	Mat img = imread(path);

	Point2f source[4] = { {389,353},{539,402}, {230,506}, {390,559} }; // pre-defined points located from paint
	Point2f destination[4] = { {0.0f,0.0f}, {w,0.0f}, {0.0f, h}, {w,h} };
	
	Point2f source1[4] = { {226,298}, {365,346}, {56, 439}, {205,496} };

	//USing transformation matrix
	matrix = getPerspectiveTransform(source, destination);
	warpPerspective(img, imgWarp, matrix, Point(w, h));

	matrix1 = getPerspectiveTransform(source1, destination);
	warpPerspective(img, imgWarp1, matrix1, Point(w, h));

	for (int i = 0; i < size(source); i++)
	{
		circle(img, source[i], 10, Scalar(0, 69, 255), FILLED);
	}
	imshow("Image", img);
	imshow("Image Warp", imgWarp);
	imshow("Image Warp1", imgWarp1);
	waitKey(0); // 0 for infinity

}