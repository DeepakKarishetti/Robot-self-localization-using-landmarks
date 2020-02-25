/* *************************************************************************
 * Computer Vision final project
 * Deepak R Karishetti
 * 1046936
 * *************************************************************************
*/

#include "hough_lines.cpp"
#include "feature_extraction.cpp"
#include "camera_calibration.cpp"
#include "panaroma.cpp"
#include <iostream>
#include <opencv2/opencv.hpp>

// **************************************************************************
// select points from the image by mouse clicks 
std::vector<cv::Point> im_points;

void CallBackFunc(int evt, int x, int y, int flags, void* userdata)
{
  if (evt == CV_EVENT_LBUTTONDOWN)
  {
    cv::Point2f temp(x,y);
    std::cout << "Point" << ": (" << x << "," << y << ")" << "\n";
    im_points.push_back(temp);
  }
}

// function to convert an image into orthophoto 
cv::Mat orthophoto(cv::Mat image)
{
  //cv::Mat image = cv::imread("mural01.JPG");
  std::cout << "Image size: " << image.size() << "\n";
  // resize imahe
  // cv::Size size(4032*0.2,3024*0.2);
  // cv::resize(image,image,size);
  std::cout << "After resizing: " << image.size() << "\n\n";
  cv::namedWindow("disp",1);
  cv::setMouseCallback("disp",CallBackFunc,NULL);
  cv::imshow("disp",image);

  cv::waitKey(0);

  // finding centroid
  int sum_x = 0;
  int sum_y = 0;

  cv::Mat disp_im = image.clone();

  for(std::vector<cv::Point>::iterator it = im_points.begin(); it!=im_points.end(); ++it)
  {
    int x = (*it).x;
    sum_x += x;
    int y = (*it).y;
    sum_y += y;

    int w = 8;
    int h = 8;
    cv::Rect rect(x,y,w,h);

    cv::rectangle(disp_im,rect,cv::Scalar(0,0,255));
  }
  //cv::imshow("Selected points!",disp_im);
  //cv::waitKey(10);

  // image points in Point2f format
  cv::Point2f image_pts[4];

  image_pts[0] = cv::Point2f((im_points.at(0).x),(im_points.at(0).y));
  image_pts[1] = cv::Point2f((im_points.at(1).x),(im_points.at(1).y));
  image_pts[2] = cv::Point2f((im_points.at(2).x),(im_points.at(2).y));
  image_pts[3] = cv::Point2f((im_points.at(3).x),(im_points.at(3).y));

  // centroid of the points
  cv::Point2f centroid(sum_x,sum_y);
  centroid.x = sum_x/4;
  centroid.y = sum_y/4;

  std::cout << "\nCentroid: (" << centroid.x << "," << centroid.y << ")"<< "\n";

  double o_w = 159.3;
  double  o_h = 217.8;
  
  cv::Point2f ortho_pts[4];
  // define ideal point location in orthophoto
  ortho_pts[0] = cv::Point2f((centroid.x-o_w*0.5),(centroid.y-o_h*0.5));
  ortho_pts[1] = cv::Point2f((centroid.x+o_w*0.5),(centroid.y-o_h*0.5));
  ortho_pts[2] = cv::Point2f((centroid.x+o_w*0.5),(centroid.y+o_h*0.5));
  ortho_pts[3] = cv::Point2f((centroid.x-o_w*0.5),(centroid.y+o_h*0.5));
  
  std::cout << "\nImage points selected: " << im_points.size() << "\n\n";

  // caculate perspective transform from four pairs of corresponding points
  cv::Mat T_perspective = cv::getPerspectiveTransform(image_pts,ortho_pts);

  cv::Mat im_out;
  cv::Size or_sz(image.size().width*4,image.size().height*2);
  cv::warpPerspective(image,im_out,T_perspective,or_sz);
  return im_out;
}

// **************************************************************************
int main()
{
  // frames from the camera
  // int source = 1;
  // cv::VideoCapture video(source);

  // read the first frame
  cv::Mat im_ref = cv::imread("l_1.jpg");
  cv::resize(im_ref,im_ref,cv::Size(), 0.15,0.15);

  cv::VideoCapture video("test_vid.mp4");
  
  if (!video.isOpened())
  {
    std::cout << "Error opening the video source!" << "\n";
    system("PAUSE");
    return -1;
  }

  // get dimensions of the frame
  int w = video.get(cv::CAP_PROP_FRAME_WIDTH);
  int h = video.get(cv::CAP_PROP_FRAME_HEIGHT);
 
  double fps = video.get(cv::CAP_PROP_FPS);
  double frame_count = video.get(cv::CAP_PROP_FRAME_COUNT);

  std::cout << "Dimension of the video frame: (" << w << "x" << h << ")" << "\n";
  std::cout << "Video FPS: " << fps << "\n";
  std::cout << "Number of frames: " << frame_count << "\n";

	// calls for feature extraction
  add_landmarks();
	std::cout << landmarks.size() << "\n";

	int frame_counter = 1;
	
	cv::Mat ortho_im = orthophoto(im_ref);

  //************************ write video to file ****************************
  ///*
  const cv::String video_out("CV_project_output.avi");
  cv::VideoWriter output_video(video_out, cv::VideoWriter::fourcc('D','I','V','X'), 30.0, cv::Size(int(w), int(h)), true);
  //*/

  while(true)
  {
    // get frames from source
    cv::Mat frame;
    video >> frame;

    // no more frames
    if (frame.empty())
    {
      std::cout << "END of the video!" << "\n";
      break;
    }

    // resize the frames
    cv::resize(frame, frame, cv::Size(), 0.5, 0.5);

    // gray imahe
  	cv::cvtColor(frame, src_gray, cv::COLOR_RGB2GRAY);

  	// canny edge detector
  	cv::Canny(src_gray, edges, 50, 200, 3);
  	cv::imshow("canny", edges);

  	// trackbars for thresholds
  	// **************** std hough *********************
  	cv::namedWindow(standard_name, cv::WINDOW_AUTOSIZE);
  	cv::createTrackbar("Threshold", standard_name, &s_trackbar, max_trackbar, std_hough);

  	// **************** prob hough ********************
  	cv::namedWindow(probabilistic_name, cv::WINDOW_AUTOSIZE);
  	cv::createTrackbar("Threshold", probabilistic_name, &p_trackbar, max_trackbar, p_hough);

  	// *************** call hough functions ****************
  	// std_hough(0, 0);
  	p_hough(0, 0);


    // ************************ SURF and RANSAC *****************************
	  std::cout << "\nFrame number: " << frame_counter << "\n";
    // run the surf matching 
    int land_match_counter = 0;
  
    std::vector<cv::DMatch> matches_found;

    matches_found = ransac(landmarks.at(land_match_counter), frame);
    // check to move on to the next landmark
    // landmark_1 = 26
    // if (frame_counter == 1 || frame_counter == 70)
    // {
    //   cv::imshow("frames", frame);
    //   cv::waitKey(0);
    // }
    if (matches_found.size() > 25)
    {
      land_match_counter += 1;
      if (land_match_counter >= landmarks.size())
      {
        break;
      }
    }
    frame_counter += 1;

    // ************** write video to file *****************
    output_video << frame;

    // exit on key press
    if (cv::waitKey(33) == 27)
    {
      break;
    }
  }
  return 0;
}
