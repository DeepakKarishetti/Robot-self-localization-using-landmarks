#include <fstream>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <cassert>

// dimensions of the chessboard used
const cv::Size chessboard = cv::Size(9,6);
const float sq_edge_len = 0.023f; // in m

void create_board_pos(cv::Size chessboard, float sq_edge_len, std::vector<cv::Point3f>& corners)
{
	for (int i=0; i<=chessboard.height; i++)
	{
		
		for(int j=0; j<=chessboard.width; j++)
		{
			corners.push_back(cv::Point3f (j * sq_edge_len, i * sq_edge_len, 0.0f));
		}
	}
}


void get_corners(std::vector<cv::Mat> images, std::vector<std::vector<cv::Point2f> >& found_corners, bool show_result=false)
{
	for (std::vector<cv::Mat>::iterator iter = images.begin(); iter != images.end(); iter++)
	{
		std::vector<cv::Point2f> points_buffer;
		bool found = cv::findChessboardCorners(*iter, chessboard, points_buffer, cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE);

		if (found)
		{
			found_corners.push_back(points_buffer);
		}

		if (show_result)
		{
			cv::drawChessboardCorners(*iter, chessboard, points_buffer, found);
			cv::imshow("corners", *iter);

			cv::waitKey(0);
		}
	}
}

void camera_calib(std::vector<cv::Mat> calib_images, cv::Size chessboard, float sq_edge_len, cv::Mat& camera_matrix, cv::Mat distort)
{
	std::vector<std::vector<cv::Point2f> > cb_im_points;
	get_corners(calib_images, cb_im_points, false);
	
	std::vector<std::vector<cv::Point3f> > world_sp_corner_pts(1);
	
	create_board_pos(chessboard, sq_edge_len, world_sp_corner_pts[0]);
	world_sp_corner_pts.resize(cb_im_points.size(), world_sp_corner_pts[0]);
  std::cout << world_sp_corner_pts.size() << "\n";
	
	std::vector<cv::Mat> rvecs, tvecs;
  std::cout << "where" << "\n";

	distort = cv::Mat::zeros(5, 1, CV_64F);

  std::cout << "problem?";

  /*assert(world_sp_corner_pts.size() == cb_im_points.size());
  for (size_t i = 0; i < world_sp_corner_pts.size(); ++i) {
    std::cout << "i is " << i << std::endl;
    std::cout << world_sp_corner_pts.at(i).size() << "\n" << cb_im_points.at(i).size() << std::endl; 
    assert(world_sp_corner_pts.at(i).size() == cb_im_points.at(i).size());
  }*/

	cv::calibrateCamera(world_sp_corner_pts, cb_im_points, chessboard, camera_matrix, distort, rvecs, tvecs);

  std::cout << "" << "\n";
}


bool save_param(cv::Mat camera_matrix, cv::Mat distort)
{
  std::ofstream camera_params;
  camera_params.open("camera_calib.txt");

  if (camera_params)
  {
    uint16_t rows = camera_matrix.rows;
    uint16_t cols = camera_matrix.cols;

    for (int i=0; i<=rows; i++)
    {
      for (int j=0; j<=cols; j++)
      {
        double val = camera_matrix.at<double>(i,j);
        camera_params << val << "\n";
        // std::cout << val <<"\n";
      }
    }

    rows = distort.rows;
    cols = distort.cols;

    for (int i=0; i<=rows; i++)
    {
      for (int j=0; j<=cols; j++)
      {
        double val = distort.at<double>(i,j);
        camera_params << val << "\n";
      }
    }

    // camera_params.close();
    return true;
  }
  return false;
}


int main()
{
	cv::Mat c_frame;
	cv::Mat draw_to_frame;	
	cv::Mat camera_matrix = cv::Mat::eye(3,3,CV_64F);
	
	cv::Mat distort;

	std::vector<cv::Mat> saved_images;

	std::vector<std::vector<cv::Point2f> > marker_corners, rej_candidates;

	cv::VideoCapture camera(1);

	if(!camera.isOpened())
	{
		std::cout << "Error opening the camera!" << "\n";
		system("PAUSE");
		return 0;
	}
	int fps = 20;

	cv::namedWindow("camera", cv::WINDOW_AUTOSIZE);

	while (true)
	{
    if (!camera.read(c_frame))
    {
      break;
    }

		// camera >> frame;
		//if (frame.empty())
		//{
		//	break;
		//}
		// camera calib

		/*//conver to gray
		cv::Mat gray_image;
		cvtColor(c_frame, gray_image, CV_BGR2GRAY);
		// find chessboard corners
		pattern_found = cv::findChessboardCorners(gray_image, cv::Size(int(board_w), int(board_h)), corners, cb_flags);
		
		// draw the corners
		if (pattern_found)
		{
			cv::drawChessboardCorners(c_frame, cv::Size(int(board_w), int(board_h)), corners, pattern_found);
		}
		*/

		std::vector<cv::Vec2f> found_points;
	  //	bool found = false;
		
		bool found = cv::findChessboardCorners(c_frame, chessboard, found_points, cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE);

		c_frame.copyTo(draw_to_frame);
		cv::drawChessboardCorners(draw_to_frame, chessboard, found_points, found);	

		if (found)
		{
			cv::imshow("camera", draw_to_frame);
		}
		else
		{
			cv::imshow("camera", c_frame);
		}
		char key_press = cv::waitKey(5);

		switch(key_press)
		{
		case ' ':
      std::cout << "*******************" << "\n";
			// save image
			if (found)
			{
				cv::Mat temp;
				c_frame.copyTo(temp);
				saved_images.push_back(temp);
        std::cout << saved_images.size() << "\n";
			}
		  break;

		case 13: //enter key
			// start calib
			if (saved_images.size() > 1)
			{
				camera_calib(saved_images, chessboard, sq_edge_len, camera_matrix, distort);
        //std::char file_name = "camera_params.txt";
        //save_param(camera_matrix, distort);
			}
			break;
	
    case 27:
			// exit
			return 0;
			break;
		}
		
	}
	return 0;
}

