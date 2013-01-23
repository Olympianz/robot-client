#ifndef OUTPUT_HANDLER_H
#define OUTPUT_HANDLER_H

#include <vector>

#include "ArNetworking.h"

#include "pcl/io/io.h"
#include "pcl/io/file_io.h"
#include "pcl/io/pcd_io.h"
#include "pcl/point_types.h"
#include "pcl/point_cloud.h"
#include <pcl/visualization/cloud_viewer.h>
#include <pcl/filters/voxel_grid.h>

#include <cv.h>

#include "PCLutils.h"


// This class handles output data from server. Currently it supports the
// built in Aria server services: ArServerInfoRobot and ArServerInfoSensor
class OutputHandler {
public:
  OutputHandler(ArClientBase *client, PCLViewer *viewer, int robotColor);
  virtual ~OutputHandler();

  void handleUpdateInfo(ArNetPacket *packet);
  void handleSensorInfo(ArNetPacket *packet);
  pcl::PointCloud<pcl::PointXYZRGB>::Ptr getRobotCloud() {
    return myRobotCloud;
  }
  pcl::PointCloud<pcl::PointXYZRGB>::Ptr getRobotCloudFiltered() {
    return myRobotCloudFiltered;
  }
  ArClientBase *getClient() { return myClient; }

protected:
  std::vector<RobotInfo *> myRobotInfos;
  ArClientBase *myClient;
  PCLViewer *myViewer;
  pcl::PointCloud<pcl::PointXYZRGB>::Ptr myRobotCloud;
  int myRobotColor;
  MyPoint myVoxelLeaf;
  pcl::PointCloud<pcl::PointXYZRGB>::Ptr myRobotCloudFiltered;
  cv::KalmanFilter *kalmanFilter;

  static const int myDensityDivisor;

private:
  ArFunctor1C<OutputHandler, ArNetPacket *> handleUpdateInfoftr;
  ArFunctor1C<OutputHandler, ArNetPacket *> handleSensorInfoftr;
};




// This class provides functionality to handle PCL data packets from server
// and displaying them in a PCL viewer.
class PCLOutputHandler : public OutputHandler {
public:
  PCLOutputHandler(ArClientBase *client, PCLViewer *viewer, int robotColor,
		   int color, int xo, int yo, int to, int rf = 1000);
  ~PCLOutputHandler();
  void handlePCLdata(ArNetPacket *packet);
  std::vector<TimeStampedPCL *> *getLaserClouds() {
    return &myLaserClouds;
  }
  pcl::PointCloud<pcl::PointXYZRGB>::Ptr getLaserCloud() {
    return myLaserCloud;
  }

private:
  std::vector<TimeStampedPCL *> myLaserClouds;
  // This cloud is an aggregate of all points in the list.
  // CloudViewer is refreshed each time a new cloud is added to it.
  // If we added each new cloud to the viewer, it would keep refreshing.
  // To remedy this, a separate cloud is needed to store all the points.
  pcl::PointCloud<pcl::PointXYZRGB>::Ptr myLaserCloud;
  ArFunctor1C<PCLOutputHandler, ArNetPacket *> handlePCLdataftr;
  int myColor;
  int myXoffset;
  int myYoffset;
  int myThetaOffset;
  int myRequestFreq;
  MyPoint myMinVals;
  MyPoint myMaxVals;

  void setMinMax(const pcl::PointXYZRGB &point);
  void printClouds();
  void updateRobotLocation(ArNetPacket *packet, long timeStamp);
  void updateLaserReadings(ArNetPacket *packet, long timeStamp);
  void filterRobotLocation(pcl::PointXYZRGB &measured);
};


// Some helpful functions

double calcRegionDensity(pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud,
    			 const MyPoint &minVal, const MyPoint &maxVal,
			 int divisor);
pcl::PointCloud<pcl::PointXYZRGB>::Ptr
voxelFilter(pcl::PointCloud<pcl::PointXYZRGB>::Ptr source,
    	    const MyPoint &leafSize);
#endif
