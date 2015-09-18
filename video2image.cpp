#include "opencv2/opencv.hpp"
#include <boost/filesystem.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

using namespace cv;
using namespace std;
using boost::filesystem::path;


int main(int argc, char *argv[]) {

  int key = 0;

  if(argc < 3) {
    printf("Not enough parameters! Exit.");
    printf("Usage:   video2image.exe  /path/to/video  sample_rate  [/path/to/output/dir] [resize_height] [resize_width]\n");
    printf("Example: video2image.exe  ../testVideo/plane.avi 1 \n");
    return -1;
  }

  path videoPath(argv[1]);

  BOOST_ASSERT_MSG( boost::filesystem::exists(videoPath), "Cannot find video file." );

  path videoName = videoPath.filename().stem();
  path videoDir  = videoPath.parent_path();

  double rate = atof(argv[2]);

  path outputDir;
  /// Generate output dir.
  if ( argc>3 ) {
    outputDir = path(argv[3]) / videoName;  /// Use videoName as folder name.
  } else {
    outputDir = videoDir / videoName;
  }


  path outDir_ImageData( outputDir/"ImageData" );
  path outDir_ImageSets( outputDir/"ImageSets" );

  boost::filesystem::create_directories(outDir_ImageData);
  boost::filesystem::create_directories(outDir_ImageSets);
  BOOST_ASSERT_MSG( boost::filesystem::exists(outDir_ImageData), "Create dir ImageData failed." );
  BOOST_ASSERT_MSG( boost::filesystem::exists(outDir_ImageSets), "Create dir ImageSets failed." );

  int resize_height = -1;
  int resize_width  = -1;
  if ( argc>4 ) {
    resize_height = atoi(argv[4]);
    resize_width  = atoi(argv[5]);
  }

  cv::VideoCapture cap;
  cap.open(videoPath.string());
  if(!cap.isOpened()) {
    printf("Error: could not load video.\n");
    return -1;
  }

  ofstream imset_file;
  path imset_filepath(outDir_ImageSets / path(videoName.string()+".txt" ));
  imset_file.open( imset_filepath.string() );
  BOOST_ASSERT_MSG( imset_file.is_open(), "Error: Cannot open image set file.\n" );


  Mat frame;
  Mat resizeFrame;
  cap >> frame;

  int fps = int(cap.get(CV_CAP_PROP_FPS));
  int length = int(cap.get(CV_CAP_PROP_FRAME_COUNT));
  CvSize size = cvSize((int)cap.get(CV_CAP_PROP_FRAME_WIDTH),(int)cap.get(CV_CAP_PROP_FRAME_HEIGHT));
  if(fps==0)
    return -1;

  const int sampleStep = int(rate*fps);
  /// Max number of frame to sample (if no plane frames are skipped.)
  const int maxNrSample = length/sampleStep +1;
  std::cout << "FPS: " << fps << std::endl;
  std::cout << "length: " << length << std::endl;
  std::cout << "Sample step: every " << sampleStep << " frames." << std::endl;

  int i= 1;
  int cnt = 0;
  //while( frame.data) {
  while( ! frame.empty() ) {
    //if(frame.empty()){ break; }
    //imshow("video", frame);
    //key = waitKey( int(1000/fps) );

    if( (i-1) % sampleStep != 0 ) {
      cap >> frame;
      i+=1;
      continue;
    }

    double minVal, maxVal;

    if ( argc>4 ) {
      /// Need to do resizing
      resize( frame, resizeFrame, Size(resize_height,resize_width) );
      minMaxLoc( resizeFrame, &minVal, &maxVal);
    } else {
      minMaxLoc( frame, &minVal, &maxVal);
    }

    ///  Skip a plane frame.
    if( minVal == maxVal) {
      cap >> frame;
      continue;
    }

    stringstream imgName;
    imgName << videoName.string() << "_" << i << ".jpg";

    path imgPath = outDir_ImageData / path( imgName.str() ) ;

    if ( argc>4 ) {
      imwrite( imgPath.string(), resizeFrame );
    } else {
      imwrite( imgPath.string(), frame );
    }
    imset_file << imgPath.stem().string() <<"\n";

    cap >> frame;
    i+=1;
    printf("\r Sampled %d / ( max %d)     ", ++cnt, maxNrSample);
  }
  imset_file.close();

  return 0;
}
