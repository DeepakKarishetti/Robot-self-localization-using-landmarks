# Robot self-localization using visual landmarks

```Robot self-localization based on visual landmarks using Hough line transform and particle filters```

**Project abstract:** 
 - Robot localization has always been a very important problem in the field of robotics.  There have been numerous 
algorithms proposed to solve the problem of efficiently determining the pose of the robot with respect to the real world 
coordinates. This is of so much importance because of the fact that the pose of the robot is very influential in
controlling its behaviour.  This is a much challenging problem when we try to solve it in computer vision, when we do 
not have much information about the robot’s environmentand this proves to have an important application in developing indoor
navigation systems.

 - This project focuses on the different methods that have been experimented to enable afairly accurate robot localization
using just a webcam with the help of simple computer vision techniques and a particle filter. 
 - The main objective of this project is to detect the wall-floor edges, using which we can extract the path the robot can 
travel along, using the Hough transform (HT) technique.  
 - It is along this path the robot would move forward extracting the landmark features, with which a fairly accurate map of 
the path is generated.This map with enough data about the robot’s environment is then fed into a particle filter, which 
would give an estimation of the robot’s current pose after multiple iterations with robot moving within the boundaries of 
the generated map.
 - The general experimentation is done using captured videos from locations where wecan spot a clear separation between the 
wall-floor edges assuming that the lighting remains the same and the robot moves in the same pace as in the video. Also, we
assume that wealready have a predefined set of landmark features before the robot starts moving in thatenvironment. It is 
also made sure that the robot is not traversed on a textured or a highlyreflective path with too many obstacles along the way.

**Dependencies:**
 - OpenCV
 - C++ 11 or higher

>> The complete dataset created is not present in this repo due to its size
