#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>
#include <glob.h>

// landmarks saved on file
std::vector<cv::Mat> landmarks;

void add_landmarks()
{
  std::vector<cv::String> f_path;
  glob("/home/deepak/ubuntu/Robotics/CV/project/build/landmarks/*.jpg",f_path,false);

  size_t f_count = f_path.size();
  for (size_t i=0; i<f_count; i++)
  {
    landmarks.push_back(cv::imread(f_path[i]));
  }
}




// detect SURF features and matching using RANSAC
std::vector<cv::DMatch> ransac(cv::Mat image_ref, cv::Mat image_next)
{
  // create a surf object
  cv::Ptr<cv::Feature2D> f2d = cv::xfeatures2d::SURF::create(1000.0,4,2,true,false);

  // detect keypoints
  std::vector<cv::KeyPoint> key_pt_ref, key_pt_next;
  f2d->detect(image_ref,key_pt_ref);

  std::cout << "Detected points in the image_ref: " << key_pt_ref.size() << "\n";

  // draw keypoints oon the imahe
  cv::Mat im_out;
  cv::drawKeypoints(image_ref,key_pt_ref,im_out);
  cv::Scalar(0,0,255),cv::DrawMatchesFlags::DEFAULT;

  // extract descriptors for all the keypoints
  cv::Mat descriptors_ref, descriptors_next;
  
  // match these points to a new image
  // for each, find the k nearest neighbour
  cv::Mat im_out_next;
  // cv::Mat image_next = cv::imread("img4.png");
  // cv::Mat image_next = cv::imread("mural02.JPG");
  f2d->detect(image_next,key_pt_next);
  std::cout << "Detected points in image_next: " << key_pt_next.size() << "\n";
  // for each imahe find k nearest neighbours
  cv::BFMatcher matcher(cv::NORM_L2);
  std::vector<std::vector<cv::DMatch> > knn_matches;
  f2d->compute(image_ref,key_pt_ref,descriptors_ref);
  f2d->compute(image_next,key_pt_next,descriptors_next);
  matcher.knnMatch(descriptors_ref,descriptors_next,knn_matches,2);

  // fit homography to tentative matches
  // eliminate outliers using RANSAC
  // return a mask showing which points are inliers
  // draw the obtained matches
  cv::Mat im_matches;
  cv::drawMatches(image_ref, key_pt_ref, image_next, key_pt_next, knn_matches, im_matches, cv::Scalar(0,0,255),cv::DrawMatchesFlags::DEFAULT);

  // cv::imshow("Good matches!", im_matches);
  // filter matches and get
  // good matches
  const float th_ratio = 0.7f;
  std::vector<cv::DMatch> good_matches;

  for(size_t i=0; i<knn_matches.size(); i++)
  {
    if (knn_matches[i][0].distance < th_ratio*knn_matches[i][1].distance)
    {
      good_matches.push_back(knn_matches[i][0]);
    }
  }

  std::cout << "knn matches are: " << knn_matches.size() << "\n";
  std::cout << "Good matches are: " << good_matches.size() << "\n\n";

  // get the keypoints from the good matches
  std::vector<cv::Point2f> ref_pts;
  std::vector<cv::Point2f> oth_pts;
  for (size_t i=0; i<good_matches.size(); i++)
  {
    ref_pts.push_back(key_pt_ref[good_matches[i].queryIdx].pt);
    oth_pts.push_back(key_pt_next[good_matches[i].trainIdx].pt);
  }

  std::vector <unsigned char> inliers_mask(key_pt_next.size());
  
  // find compute homography
  cv::Mat homography = cv::findHomography(oth_pts,ref_pts,cv::FM_RANSAC,5.0,inliers_mask);

  return good_matches;

}




/*
int main()
{
  add_landmarks();
  std::cout << landmarks.size() << "\n";


  cv::VideoCapture video("test_vid.mp4");

  if (!video.isOpened())
  {
    std::cout << "Error with the video source!" << "\n";
    return -1;
  }

  int frame_counter = 1;

  while (true)
  {
    cv::Mat frame;
    video >> frame;

    if (frame.empty())
    {
      std::cout << "END of the video!" << "\n";
      break;
    }
 
    std::cout << "Frame number: " << frame_counter << "\n";
    // run the surf matching 
    int land_match_counter = 0;
  
    std::vector<cv::DMatch> matches_found;

    matches_found = ransac(landmarks.at(land_match_counter), frame);
    // check to move on to the next landmark
    // landmark_1 = 26
    if (frame_counter == 1 || frame_counter == 70)
    {
      cv::imshow("frames", frame);
      cv::waitKey(0);
    }
    if (matches_found.size() > 25)
    {
      land_match_counter += 1;
      if (land_match_counter >= landmarks.size())
      {
        break;
      }
    }
    frame_counter += 1;
  }

    









  return EXIT_SUCCESS;
}*/
