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

CvScalar RenderThread::getMax() const
{
    return m_Max;
}

CvScalar RenderThread::getMin() const
{
    return m_Min;
}

void RenderThread::setMax(const CvScalar &max)
{
    m_Max = max;
}

void RenderThread::setMin(const CvScalar &min)
{
    m_Min = min;
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

    // Obter frame binarizado com os intervalos min e max
    binaryFrame = vision->thresholdImage(m_Min, m_Max);

    // Aplicar dilatação no frame binarizado
    binaryFrame = vision->dilateImage(binaryFrame);

    // Aplicar erosão na imagem no frame resultante da dilatação
    binaryFrame = vision->erodeImage(binaryFrame);

    // Detectar cores a partir do frame binarizado resultante
    vision->m_Found[m_Number] = DetectColors(binaryFrame.clone(), 80, 1, 1500);
}

