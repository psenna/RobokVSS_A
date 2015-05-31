#ifndef VISION_H
#define VISION_H

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "fieldstate.h"
#include "renderthread.h"
#include <vector>

// Define Border Points
#define A 0
#define B 1
#define C 2
#define D 3

using cv::Mat;
using cv::VideoCapture;
using cv::Scalar;
using robok::Fieldstate;

class Vision
{
public:
    static Vision* getInstance();
    void getData(Fieldstate *fs);
    void calibrate(int id, Mat* target_frame);
    //void retification();
    void setCameraId(const int &id);
    void setMinMax(const CvScalar &min, const CvScalar &max, const int &id);
    bool captureImage();
    CvScalar* getMin();
    CvScalar* getMax();
    void setRectificationsParam(std::vector<float> paramsVector);
    void closeCapture();
    void autoRectificationSet();
    std::vector<float> convertBorders(std::vector<cv::Point2f> border_points);

    friend class RenderThread;
    friend class MainWindow;

private:
    Vision();  
    void rectifyImage();
    void adjustImage();
    void convertImage(Mat* target_frame);
    void renderImage();
    void identifyRobot(Fieldstate *fs);
    Mat thresholdImage(Scalar min, Scalar max);
    Mat dilateImage(const Mat &binary_frame);
    Mat erodeImage(const Mat &binary_frame);

    static Vision *m_Instance;
    Mat m_FrameOriginal;
    Mat m_FrameRect;
    Mat m_FrameHSV;
    Mat m_FrameBinary;
    int m_IdCamera;

    std::vector<Position> m_Found[10];
    cv::VideoCapture m_VideoCapture;
    CvScalar m_Max[10];
    CvScalar m_Min[10];
    RenderThread m_RenderThreads[10];
    cv::Size m_DisplaySize;
    std::vector<cv::Point2f> m_RectificationBorderPoints;
    std::vector<cv::Point2f> m_OriginalBorderPoints;
};

#endif // VISION_H
