#include <opencv2/opencv.hpp>
#include <iostream>

// ************************* Global variables *******************************
cv::Mat frame, edges;
cv::Mat src_gray,binary;
cv::Mat standard_hough, probabilistic_hough;

// init thresh
int min_threshold = 50;
int max_trackbar = 109;

const char* standard_name = "Standard Hough line transform";
const char* probabilistic_name = "Probabilistic Hough line transform";

// trackbar
int s_trackbar = max_trackbar;
int p_trackbar = max_trackbar;

// ********************** Standard_Hough transform **************************
void std_hough(int, void*)
{
  std::vector<cv::Vec2f> s_lines;
  cv::cvtColor(edges, standard_hough, cv::COLOR_GRAY2BGR);

  // Use Standard Hough Transform
  cv::HoughLines(edges, s_lines, 1, CV_PI/180, min_threshold + s_trackbar, 0, 0 );

  // Show the result
  for(size_t i = 0; i < s_lines.size(); i++)
     {
      float r = s_lines[i][0], t = s_lines[i][1];
      double cos_t = cos(t), sin_t = sin(t);
      double x0 = r*cos_t, y0 = r*sin_t;
      double alpha = 1000;

       cv::Point pt1(cvRound(x0 + alpha*(-sin_t)), cvRound(y0 + alpha*cos_t));
       cv::Point pt2(cvRound(x0 - alpha*(-sin_t)), cvRound(y0 - alpha*cos_t));
       cv::line(standard_hough, pt1, pt2, cv::Scalar(255,0,0), 3, cv::LINE_AA);
     }

   cv::imshow(standard_name, standard_hough);
}

// *************************Probabilistic_Hough *****************************
void p_hough(int, void*)
{
  std::vector<cv::Vec4i> p_lines;
  cv::cvtColor(edges, probabilistic_hough, cv::COLOR_GRAY2BGR);

  // Use Probabilistic Hough Transform
  cv::HoughLinesP(edges, p_lines, 1, CV_PI/180, min_threshold + p_trackbar, 30, 10);

  // Show the result
  for(size_t i = 0; i < p_lines.size(); i++)
  {
    std::cout << p_lines.at(0) << "\n";

    cv::Vec4i l = p_lines[i];

    double slope = atan2(l[3]-l[1],l[2]-l[0])*180.0/CV_PI;
    std::cout << slope << "\n";

    // draw only horizontal lines
    if (abs(slope) < 80 && abs(slope) > 35)
    {
      cv::line(probabilistic_hough, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0,255,0), 3, cv::LINE_AA);
    }

  }
  
  cv::imshow( probabilistic_name, probabilistic_hough );
}

