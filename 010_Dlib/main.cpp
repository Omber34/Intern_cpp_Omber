#include <vector>
#include <dlib/opencv.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>

using namespace cv;
using namespace dlib;
using std::cout;
using std::endl;
using std::exception;
int main()
{
	cv::Mat im = cv::imread("left.jpg");
	image_window win;
	std::vector<cv::Point2d> image_points;
	std::vector<cv::Point3d> model_points;

	// 3D model points.
	model_points.push_back(cv::Point3d(0.0f, 0.0f, 0.0f));               // Nose tip
	model_points.push_back(cv::Point3d(0.0f, -330.0f, -65.0f));          // Chin
	model_points.push_back(cv::Point3d(-225.0f, 170.0f, -135.0f));       // Left eye left corner
	model_points.push_back(cv::Point3d(225.0f, 170.0f, -135.0f));        // Right eye right corner
	model_points.push_back(cv::Point3d(-150.0f, -150.0f, -125.0f));      // Left Mouth corner
	model_points.push_back(cv::Point3d(150.0f, -150.0f, -125.0f));       // Right mouth corner

	frontal_face_detector detector = get_frontal_face_detector();
	shape_predictor pose_model;
	deserialize("shape_predictor_68_face_landmarks.dat") >> pose_model;

	cv_image<bgr_pixel> cimg(im);

	// Detect faces 
	std::vector<dlib::rectangle> faces = detector(cimg);
	std::vector<full_object_detection> shapes;
	// Find the pose of each face.
	for (size_t i = 0; i < faces.size(); ++i)
		shapes.push_back(pose_model(cimg, faces[i]));

	image_points.push_back(Point2d(shapes[0].part(30).x(), shapes[0].part(30).y()));    // Nose tip
	image_points.push_back(Point2d(shapes[0].part(8).x(), shapes[0].part(8).y()));    // Chin
	image_points.push_back(Point2d(shapes[0].part(45).x(), shapes[0].part(45).y()));     // Left eye left corner
	image_points.push_back(Point2d(shapes[0].part(36).x(), shapes[0].part(36).y()));    // Right eye right corner
	image_points.push_back(Point2d(shapes[0].part(54).x(), shapes[0].part(54).y()));    // Left Mouth corner
	image_points.push_back(Point2d(shapes[0].part(48).x(), shapes[0].part(48).y()));    // Right mouth corner

// Display it all on the screen
	double focal_length = im.cols; // Approximate focal length.
	Point2d center = cv::Point2d(im.cols / 2, im.rows / 2);
	cv::Mat camera_matrix = (cv::Mat_<double>(3, 3) << focal_length, 0, center.x, 0, focal_length, center.y, 0, 0, 1);
	cv::Mat dist_coeffs = cv::Mat::zeros(4, 1, cv::DataType<double>::type); // Assuming no lens distortion
	// Output rotation and translation
	cv::Mat rotation_vector; // Rotation in axis-angle form
	cv::Mat translation_vector;
	// Solve for pose

	std::vector<Point3d> nose_end_point3D;
	std::vector<Point2d> nose_end_point2D;
	nose_end_point3D.push_back(Point3d(0, 0, 1000.0));

	cv::solvePnP(model_points, image_points, camera_matrix, dist_coeffs, rotation_vector, translation_vector);
	projectPoints(nose_end_point3D, rotation_vector, translation_vector, camera_matrix, dist_coeffs, nose_end_point2D);
	cv::line(im, image_points[0], nose_end_point2D[0], cv::Scalar(255, 0, 0), 2);

	char side;
	int deltaLeft = image_points[0].x - image_points[2].x;
	int deltaRight = image_points[3].x - image_points[0].x;
	int face_center = (deltaLeft + deltaRight) / 2;
	int angle;

	if (deltaLeft > deltaRight) {
		side = 'R';
		angle = (90 * deltaRight / face_center) - 90;
	}
	else {
		side = 'L';
		angle = (90 * deltaLeft / face_center) - 90;
	}

	if (angle > 45) {
		system("notepad.exe");
		cv::waitKey(10);
	}

	cimg = im;
	win.clear_overlay();
	win.set_image(cimg);
	//win.add_overlay(render_face_detections(shapes));
	while (!win.is_closed())
	{
	}

}

