#ifndef VISION_H
#define VISION_H

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "fieldstate.h"

using cv::Mat;
using cv::Vector;
using cv::VideoCapture;
using cv::Scalar;

class Vision
{
private:
    Mat frame_original_;
    Mat frame_hsv_;
    Mat frame_binary_;
    CvCapture* capture_;
    int id_camera_;
    Vector<Position> found_[9];
    float aImg[8]; /* pontos para a retificaçao */

public:
    Vision();
    void getData(Fieldstate *fs);
    void set_camera_id(const int &id);

private:
    bool captureImage();
    void adjustImage();
    void rectifyImage(float aImg[8]);
    void convertImage();
    void thresholdImage(CvScalar min, CvScalar max);
    void renderImage(Fieldstate *fs);
    void identifyRobot(Fieldstate *fs);

};

#endif // VISION_H
