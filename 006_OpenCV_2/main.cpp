#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <cmath>
using namespace cv;
using namespace std;
const char* source_window = "Source image";
const char* result_window = "Result image";
struct VisitorUP
{
	Point res;
	VisitorUP() : res(200,200) {}
	void operator() (const Point & p)
	{
		if (p.y < res.y)
			res = p;
	}
	Point getRes()
	{
		return res;
	}
};
struct VisitorLeft
{
	Point res;
	VisitorLeft() : res(200, 200) {}
	void operator() (const Point & p)
	{
		if (p.x < res.x)
			res = p;
	}
	Point getRes()
	{
		return res;
	}
};
struct VisitorRight
{
	Point res;
	VisitorRight() : res(200, 200) {}
	void operator() (const Point & p)
	{
		if (p.x > res.x)
			res = p;
	}
	Point getRes()
	{
		return res;
	}
};
vector<Point> findTriangle(cv::Mat const & img);
float fingAngle(vector<Point> const & tri);
int main(int, char** argv)
{
	Mat rot_mat(2, 3, CV_32FC1);
	Mat warp_mat(2, 3, CV_32FC1);
	Mat src, rotate_dst;
	src = imread("pic1.jpg");		
	std::cout << "angle: " << fingAngle(findTriangle(src));
	Point center = Point(src.cols / 2, src.rows / 2);
	double angle = fingAngle(findTriangle(src));	
	rot_mat = getRotationMatrix2D(center, angle, 1.0f);
	warpAffine(src, rotate_dst, rot_mat, src.size());
	namedWindow(source_window, WINDOW_AUTOSIZE);
	imshow(source_window, src);	
	namedWindow(result_window, WINDOW_AUTOSIZE);
	imshow(result_window, rotate_dst);
	waitKey(0);
	return 0;
}
//высчитывает угол между верхней вертикальной линией найденого контура и х-ортой окна, что бы потом н него повернуть....
float fingAngle(vector<Point> const & tri)
{
	Point2f vec1 = { 0.0f, 1.0f };
	Point vec2 = { tri[1].x - tri[0].x ,tri[1].y - tri[0].y };
	return acos((vec1.x*vec2.x+vec1.y*vec2.y) / (sqrt(vec1.x*vec1.x + vec1.y*vec1.y) * sqrt(vec2.x*vec2.x + vec2.y*vec2.y)))*180 / 3.14f;
}
//превращает набор точек контура в триугольник с самым большим трешхолдом(что бы контур был только книги), что бы легче было его обрабатывать 
vector<Point> findTriangle(cv::Mat const & img)
{
	Mat canny;
	Canny(img, canny, 255, 255 * 2);
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(canny, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
	vector<vector<Point> > contours1;
	auto big_triangle = std::max_element(contours.begin(), contours.end(), [](vector<Point>const & left, vector<Point>const & right)
	{ return left.size() < right.size(); });	
		contours1.push_back({ std::for_each((*big_triangle).begin(), (*big_triangle).end(),VisitorUP()).getRes(),
		std::for_each((*big_triangle).begin(), (*big_triangle).end(),VisitorLeft()).getRes(),
		std::for_each((*big_triangle).begin(),(*big_triangle).end(),VisitorRight()).getRes(),
			});	
	return contours1[0];
}