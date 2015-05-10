#include "renderthread.h"
#include "vision.h"
#include "floodfill.hpp"

RenderThread::RenderThread()
{

}

int RenderThread::getNumber() const
{
    return m_Number;
}

void RenderThread::setNumber(const int &number)
{
    m_Number = number;
}

void RenderThread::setMinMax(const CvScalar &min, const CvScalar &max)
{
    m_Min = min;
    m_Max = max;
}

void RenderThread::run()
{
    cv::Mat binaryFrame;
    Vision *vision = Vision::getInstance();
    binaryFrame = vision->thresholdImage(m_Min, m_Max);
    binaryFrame = vision->dilateImage(binaryFrame);
    binaryFrame = vision->erodeImage(binaryFrame);
    vision->m_Found[m_Number] = DetectColors(binaryFrame.clone(), 80, 1, 1500);
}

