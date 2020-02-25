/*#include <iostream>
#include <opencv2/opencv.hpp>

// global vars


int main()
{
  // output variables
  cv::Mat dst, cdst, cdstP;

  cv::Mat src = cv::imread("test.jpg", cv::IMREAD_GRAYSCALE);
  cv::resize(src, src, cv::Size(), 0.25,0.25);

  if (src.empty())
  {
    std::cout << "Error opening the image file!" << "\n";
    return -1;
  }

  // edge detection
  cv::Canny(src, dst, 50, 200, 3);

  // copy edgesto images thatwill be displayed in the bgr image
  cv::cvtColor(dst, cdst, cv::COLOR_GRAY2BGR);
  cdstP = cdst.clone();

  // standard hough line transform
  std::vector<cv::Vec2f> lines;
  cv::HoughLines(dst, lines, 1, CV_PI/180, 150, 0, 0);
  // draw the lines
  for (size_t i=0; i<lines.size(); i++)
  {
    cv::Point pt_1, pt_2;
    float rho = lines[i][0];
    float theta = lines[i][1];

    double a = cos(theta);
    double b = sin(theta);

    double x_0 = a*rho;
    double y_0 = b*rho;

    pt_1.x = cvRound(x_0 + 1000*(-b));
    pt_1.y = cvRound(y_0 + 1000*(a));
    pt_2.x = cvRound(x_0 - 1000*(-b));
    pt_2.y = cvRound(y_0 - 1000*(a));

    cv::line(cdst, pt_1, pt_2, cv::Scalar(0, 0, 255), 3, cv::LINE_AA);
  }

*/
  /* probablistic hough line transform
  std::vector<cv::Vec4i> p_lines;
  cv::HoughLines(dst, p_lines, 1, CV_PI/180, 150, 50, 10);

  // draw lines
  for (size_t i=0; i<p_lines.size(); i++)
  {
    cv::Vec4i l = p_lines[i];
    cv::line(cdstP, cv::Point(l[0],l[1]), cv::Point(l[2],l[3]), cv::Scalar(0, 0, 255), 3, cv::LINE_AA);
  }*/
  
/*
  cv::imshow("Source", src);
  cv::imshow("Detected lines - standard hough line transform", cdst);
  cv::imshow("Detected lines - probablistic hough line transform", cdstP);

  cv::waitKey();
  return 0;
}
*/

#include <iostream>
#include <opencv2/opencv.hpp>


cv::Mat src, edges;
cv::Mat src_gray;
cv::Mat std_hough, probablistic_hough;

int canny_min = 50;
int canny_max = 150;

int c1_trackbar = canny_max;
int c2_trackbar = canny_max;

int canny_thresh_1 = 50;
const int canny_thresh_1_max = 150;
int canny_thresh_2 = 50;
const int canny_thresh_2_max = 150;

int hough_thresh = 219;
const int hough_thresh_max = 200;

const char* std_name = "Standard hough lines";
const char* probablistic_name = "Probablistic hough lines demo";

int main()
{
  src = cv::imread("test.jpg", cv::IMREAD_COLOR);
  cv::resize(src, src, cv::Size(), 0.25, 0.25);

  // check for image
  if (src.empty())
  {
    std::cout << "Error opening the image!" << "\n";
    return -1;
  }

  // gray image
  cv::cvtColor(src, src_gray, cv::COLOR_RGB2GRAY);

  cv::Canny(src, edges, 80,270,3);
  cv::namedWindow("Canny edges", cv::WINDOW_AUTOSIZE);
  cv::imshow("Canny edges", edges);

  // probablistic hough
  std::vector<cv::Vec4i> p_lines;
  cv::cvtColor(edges, probablistic_hough, cv::COLOR_GRAY2BGR);

  cv::HoughLinesP(edges, p_lines, 1, CV_PI/180, hough_thresh, 30, 10);
  cv::namedWindow(probablistic_name, cv::WINDOW_AUTOSIZE);
  // cv::createTrackbar(probablistic_name, "hough lines", &hough_thresh, hough_thresh_max);
  //cv::HoughLinesP(edges, p_lines, 1, CV_PI/180, 50,50,10);

  // show result
  for (size_t i=0; i<p_lines.size(); i++)
  {
    cv::Vec4i l = p_lines[i];
    cv::line(probablistic_hough, cv::Point(l[0],l[1]), cv::Point(l[2],l[3]), cv::Scalar(0, 0, 255), 3, cv::LINE_AA);
  }
  cv::imshow(probablistic_name, probablistic_hough);
  //
  cv::waitKey();
  return 0;
}


