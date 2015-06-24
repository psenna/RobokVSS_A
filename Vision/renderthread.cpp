#include "renderthread.h"
#include "vision.h"
#include "floodfill.hpp"

// Ajustar intervalo da quantidade pixels dos objetos a serem encontrados
#define MIN_SIZE 6
#define MAX_SIZE 1000

RenderThread::RenderThread()
{
}

void RenderThread::run()
{
    cv::Mat binary_frame;
    Vision *vision = Vision::getInstance();

    // Obter frame binarizado com os intervalos min e max
    binary_frame = vision->thresholdImage((Scalar) vision->getMin()[m_ThreadNumber],
                                          (Scalar) vision->getMax()[m_ThreadNumber]);

    // Aplicar erosão no frame binarizado
    binary_frame = vision->erodeImage(binary_frame);

    // Aplicar dilatação na imagem no frame resultante da erosão
    binary_frame = vision->dilateImage(binary_frame);

    // Detectar cores a partir do frame binarizado resultante
    vision->m_Found[m_ThreadNumber] = DetectColors(binary_frame.clone(), MIN_SIZE, m_ObjectsAmount, MAX_SIZE);

    if(m_ThreadNumber == 8){
        std::cout << "achados: " << vision->m_Found[8].size() << endl;
    }

    // Terminar a Thread
    this->exit();
}

int RenderThread::getThreadNumber() const
{
    return m_ThreadNumber;
}

void RenderThread::setThreadNumber(const int &number)
{
    m_ThreadNumber = number;
}

int RenderThread::getObjectsAmount() const
{
    return m_ObjectsAmount;
}

void RenderThread::setObjectsAmount(const int &amount)
{
    m_ObjectsAmount = amount;
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
