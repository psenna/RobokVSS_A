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
    binaryFrame = vision->thresholdImage((Scalar) vision->getMin()[m_Number], (Scalar) vision->getMax()[m_Number]);

    // Aplicar erosão no frame binarizado
    binaryFrame = vision->erodeImage(binaryFrame);

    // Aplicar dilatação na imagem no frame resultante da erosão
    binaryFrame = vision->dilateImage(binaryFrame);


    // Detectar cores a partir do frame binarizado resultante
    vision->m_Found[m_Number] = DetectColors(binaryFrame.clone(), 6, 4, 1000);   

}

