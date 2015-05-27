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
    void calibrate(int id, Mat* frameAlvo);
    //void retification();
    void setCameraId(const int &id);
    void setMinMax(const CvScalar &min, const CvScalar &max, const int &id);
    bool captureImage();
    CvScalar* getMin();
    CvScalar* getMax();
    void setRetificationsParam(std::vector<float> aImg);
    void closeCapture();
    void autoRetificationSet();
    std::vector<float> convertBordas(std::vector<cv::Point2f> bordas);

    friend class RenderThread;
    friend class MainWindow;

private:
    Vision();
    void adjustImage();
    void rectifyImage();
    void convertImage(Mat* frameAlvo);
    void renderImage();
    void identifyRobot(Fieldstate *fs);
    Mat thresholdImage(Scalar min, Scalar max);
    Mat dilateImage(const Mat &binaryFrame);
    Mat erodeImage(const Mat &binaryFrame);

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
    cv::Size tamDisplay;
    std::vector<cv::Point2f> bordasRectify; /* pontos para a retificaçao */
    std::vector<cv::Point2f> bordasFrameOriginal; /* pontos para a retificaçao */
};

#endif // VISION_H
