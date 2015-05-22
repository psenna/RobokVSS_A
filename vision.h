#ifndef VISION_H
#define VISION_H

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "fieldstate.h"
#include "renderthread.h"
#include "Interface/mainwindow.h"

using cv::Mat;
using cv::Vector;
using cv::VideoCapture;
using cv::Scalar;

class Vision
{
public:
    static Vision* getInstance();
    void getData(Fieldstate *fs);
    void setCameraId(const int &id);

    friend class RenderThread;
    friend class MainWindow;

private:
    Vision();
    bool captureImage();
    void adjustImage();
    void rectifyImage(float aImg[8]);
    void convertImage();
    void renderImage(Fieldstate *fs);
    void identifyRobot(Fieldstate *fs);
    Mat thresholdImage(CvScalar min, CvScalar max);
    Mat dilateImage(const Mat &binaryFrame);
    Mat erodeImage(const Mat &binaryFrame);

    static Vision *m_Instance;
    Mat m_FrameOriginal;
    Mat m_FrameHSV;
    Mat m_FrameBinary;
    CvCapture* m_Capture;
    int m_IdCamera;
    Vector<Position> m_Found[9];
    float aImg[8]; /* pontos para a retificaçao */
    VideoCapture m_VideoCapture;
    RenderThread m_RenderThreads[9];

};

#endif // VISION_H
