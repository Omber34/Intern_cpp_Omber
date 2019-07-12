#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
int main(int argc, char** argv)
{
	
	cv::Mat frame;
	cv::VideoCapture camera(0);
	int answ = 0;
	cv::Point p = { 20,20 };
	while (true)
	{
		camera.read(frame);
		cv::cvtColor(frame, frame, CV_BGR2GRAY);
		cv::putText(frame,
			"Here is some text",
			p, // Coordinates
			cv::FONT_HERSHEY_COMPLEX_SMALL, // Font
			1.0, // Scale. 2.0 = 2x bigger
			cv::Scalar(255, 255, 255), // BGR Color
			1, // Line Thickness (Optional)
			CV_AA); // Anti-alias (Optional)
		cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE); 
		cv::imshow("Display window", frame); 
		answ = cv::waitKey(100);
		if (answ == 32)
			cv::imwrite("saved_gray_photo.png", frame);
	}
	
	return 0;
}