#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>
#include <glob.h>

// print image file names 
void file_names()
{
  glob_t glob_result;
  glob("*.JPG",GLOB_TILDE,NULL,&glob_result);
  std::cout << "Images found in the directory: " << "\n";
  for (unsigned int i=0; i<glob_result.gl_pathc; i++)
  {
    std::cout << glob_result.gl_pathv[i] << "\n";
  }
  std::cout << "\n";
}


// read all images 
std::vector<cv::Mat> all_images()
{
  std::vector<cv::String> f_path;
  glob("/home/deepak/ubuntu/Robotics/CV/project/build/landmarks/*.jpg",f_path,false);

  std::vector<cv::Mat> images;
  size_t count = f_path.size(); // number of files in the folder
  for (size_t i=0; i<count; i++)
  {
    images.push_back(cv::imread(f_path[i]));
  }
  return images;
}

void create_pano(cv::Mat frame_1, cv::Mat frame_2)
{
for (size_t im_count=1; im_count<images.size(); im_count++)
  {
    cv::Size im_resize(4032*0.2,3024*0.2);
    cv::Mat image_next = images.at(im_count);
    cv::resize(image_next,image_next,im_resize);
    std::cout << "Case " << im_count << ": " << "\n";
    std::cout << image_next.size() << "\n";
   
    cv::Mat H_next = ransac(ortho_im,image_next);
    cv::Mat warp_im;
    cv::warpPerspective(image_next, warp_im, H_next, ortho_im.size());

    cv::imwrite("Orthophoto2.jpeg",warp_im);
    //
    cv::Vec3b pixel_val;
    // check each pixel in both the images abd then blend
    for (size_t i=0; i<ortho_im.rows; i++)
    {
      for (size_t j=0; j<ortho_im.cols; j++)
      {
        if (ortho_im.at<cv::Vec3b>(i,j)[0] != 0 && warp_im.at<cv::Vec3b>(i,j)[0] != 0)
        {
          ortho_im.at<cv::Vec3b>(i,j)[0] = (ortho_im.at<cv::Vec3b>(i,j)[0]+warp_im.at<cv::Vec3b>(i,j)[0])*0.5;
          ortho_im.at<cv::Vec3b>(i,j)[1] = (ortho_im.at<cv::Vec3b>(i,j)[1]+warp_im.at<cv::Vec3b>(i,j)[1])*0.5;
          ortho_im.at<cv::Vec3b>(i,j)[2] = (ortho_im.at<cv::Vec3b>(i,j)[2]+warp_im.at<cv::Vec3b>(i,j)[2])*0.5;
        }
        else if (ortho_im.at<cv::Vec3b>(i,j)[0] == 0 && warp_im.at<cv::Vec3b>(i,j)[0] != 0)
        {
          ortho_im.at<cv::Vec3b>(i,j)[0] = warp_im.at<cv::Vec3b>(i,j)[0];
          ortho_im.at<cv::Vec3b>(i,j)[1] = warp_im.at<cv::Vec3b>(i,j)[1];
          ortho_im.at<cv::Vec3b>(i,j)[2] = warp_im.at<cv::Vec3b>(i,j)[2];
        }
        else
        {
          ortho_im.at<cv::Vec3b>(i,j)[0] = ortho_im.at<cv::Vec3b>(i,j)[0];
          ortho_im.at<cv::Vec3b>(i,j)[1] = ortho_im.at<cv::Vec3b>(i,j)[1];
          ortho_im.at<cv::Vec3b>(i,j)[2] = ortho_im.at<cv::Vec3b>(i,j)[2];
        }
      }
    }

    cv::imwrite("Fused_ims.jpeg",ortho_im);
}
