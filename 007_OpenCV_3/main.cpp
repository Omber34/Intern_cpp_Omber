#include <vector>

#include <iostream>
#include <chrono>
#include "opencv2/core.hpp"
#include "opencv2/calib3d.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
using namespace cv;
using namespace cv::xfeatures2d;
using std::cout;
using std::endl;
template<class _alh>
void test_alhorithm(const char * msg, Mat & ietalon, Mat & iscene)
{
	std::chrono::time_point<std::chrono::system_clock> start, end;
	start = std::chrono::system_clock::now();
	int minHessian = 400;
	Ptr<KAZE> detector = KAZE::create();
	std::vector<KeyPoint> keypoints_object, keypoints_scene;
	Mat descriptors_object, descriptors_scene;
	detector->detectAndCompute(ietalon, noArray(), keypoints_object, descriptors_object);
	detector->detectAndCompute(iscene, noArray(), keypoints_scene, descriptors_scene);

	Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create(DescriptorMatcher::FLANNBASED);
	std::vector< std::vector<DMatch> > knn_matches;
	matcher->knnMatch(descriptors_object, descriptors_scene, knn_matches, 2);

	size_t matches = knn_matches.size();
	const float ratio_thresh = 0.75f;
	std::vector<DMatch> good_matches;
	for (size_t i = 0; i < knn_matches.size(); i++)
	{
		if (knn_matches[i][0].distance < ratio_thresh * knn_matches[i][1].distance)
		{
			good_matches.push_back(knn_matches[i][0]);
		}
	}
	size_t _matches = good_matches.size();

	Mat img_matches;
	drawMatches(ietalon, keypoints_object, iscene, keypoints_scene, good_matches, img_matches, Scalar::all(-1),
		Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	std::vector<Point2f> obj;
	std::vector<Point2f> scene;
	for (size_t i = 0; i < good_matches.size(); i++)
	{

		obj.push_back(keypoints_object[good_matches[i].queryIdx].pt);
		scene.push_back(keypoints_scene[good_matches[i].trainIdx].pt);
	}
	Mat H = findHomography(obj, scene, RANSAC);

	std::vector<Point2f> obj_corners(4);
	obj_corners[0] = Point2f(0, 0);
	obj_corners[1] = Point2f((float)ietalon.cols, 0);
	obj_corners[2] = Point2f((float)ietalon.cols, (float)ietalon.rows);
	obj_corners[3] = Point2f(0, (float)ietalon.rows);
	std::vector<Point2f> scene_corners(4);
	perspectiveTransform(obj_corners, scene_corners, H);
	//-- Draw lines between the corners (the mapped object in the scene - image_2 )
	line(img_matches, scene_corners[0] + Point2f((float)ietalon.cols, 0),
		scene_corners[1] + Point2f((float)ietalon.cols, 0), Scalar(0, 255, 0), 4);
	line(img_matches, scene_corners[1] + Point2f((float)ietalon.cols, 0),
		scene_corners[2] + Point2f((float)ietalon.cols, 0), Scalar(0, 255, 0), 4);
	line(img_matches, scene_corners[2] + Point2f((float)ietalon.cols, 0),
		scene_corners[3] + Point2f((float)ietalon.cols, 0), Scalar(0, 255, 0), 4);
	line(img_matches, scene_corners[3] + Point2f((float)ietalon.cols, 0),
		scene_corners[0] + Point2f((float)ietalon.cols, 0), Scalar(0, 255, 0), 4);

	end = std::chrono::system_clock::now();
	imshow("Good Matches & Object detection", img_matches);
	std::cout << "Alh: " << msg << endl;
	std::cout << "proceccing time: " << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << endl;
	std::cout << "found matches: " << matches << std::endl;
	std::cout << "good matches: " << _matches << std::endl;
}


int main(int argc, char* argv[])
{
	Mat img_object = imread("etalon_img.jpg");
	Mat img_scene = imread("scene_img.jpg");
	test_alhorithm<SURF>("SURF", img_object, img_scene);
	test_alhorithm<KAZE>("KAZE", img_object, img_scene);
	test_alhorithm<AKAZE>("AKAZE", img_object, img_scene);
	waitKey();
	return 0;
}