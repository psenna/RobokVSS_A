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
public:
    Vision();
    void getData(Fieldstate* fs);
    void setCameraId(const int& id);

private:
    bool captureImage();
    void adjustImage();
    void rectifyImage(float aImg[8]);
    void convertImage();
    void thresholdImage(CvScalar min, CvScalar max);
    void renderImage(Fieldstate* fs);
    void identifyRobot(Fieldstate* fs);

    Mat m_FrameOriginal;
    Mat m_FrameHSV;
    Mat m_FrameBinary;
    CvCapture* m_Capture;
    int m_IdCamera;
    Vector<Position> m_Found[9];
    float aImg[8]; /* pontos para a retificaçao */

};

#endif // VISION_H
