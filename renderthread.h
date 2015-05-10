#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include <QtCore>
#include "opencv2/opencv.hpp"

class RenderThread : public QThread
{
public:
    RenderThread();

    int getNumber() const;
    void setNumber(const int &number);
    void run();
    void setMinMax(const CvScalar &min, const CvScalar &max);

private:
    int m_Number;
    CvScalar m_Max;
    CvScalar m_Min;
};

#endif // RENDERTHREAD_H
