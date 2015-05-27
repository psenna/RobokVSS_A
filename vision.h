#ifndef VISION_H
#define VISION_H

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "fieldstate.h"
#include "renderthread.h"
#include <vector>

using cv::Mat;
using cv::VideoCapture;
using cv::Scalar;

class Vision
{
public:
    static Vision* getInstance();
    void getData(Fieldstate *fs);
    void calibrate(int id, Mat* FrameAlvo);
    //void retification();
    void setCameraId(const int &id);
    void setMinMax(const CvScalar &min, const CvScalar &max, const int &id);
    bool captureImage();
    CvScalar* getMin();
    CvScalar* getMax();
    void setRetificationsParam(float a, float b, float c, float d, float e, float f, float g, float h);
    void closeCapture();
    void autoRetificationSet();


    friend class RenderThread;
    friend class MainWindow;

private:
    Vision();
    void adjustImage();
    void rectifyImage();
    void convertImage(Mat* FrameAlvo);
    void renderImage(Fieldstate *fs);
    void identifyRobot(Fieldstate *fs);
    Mat thresholdImage(Scalar min, Scalar max);
    Mat dilateImage(const Mat &binaryFrame);
    Mat erodeImage(const Mat &binaryFrame);

    static Vision *m_Instance;
    Mat m_FrameOriginal;
    Mat m_FrameRect;
    Mat m_FrameHSV;
    Mat m_FrameBinary;
    CvCapture* m_Capture;
    int m_IdCamera;

    std::vector<Position> m_Found[10];
    float aImg[8]; /* pontos para a retificaçao */
    VideoCapture m_VideoCapture;
    CvScalar m_Max[10];
    CvScalar m_Min[10];
    RenderThread m_RenderThreads[10];
    cv::Size tamDisplay;

};

#endif // VISION_H
