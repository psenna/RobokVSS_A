#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include <QtCore>
#include "opencv2/opencv.hpp"

class RenderThread : public QThread
{
public:
    RenderThread();

    void run();

    int getNumber() const;
    void setNumber(const int &number);

    CvScalar getMax() const;
    CvScalar getMin() const;
    void setMax(const CvScalar &max);
    void setMin(const CvScalar &min);
    void setMinMax(const CvScalar &min, const CvScalar &max);

private:
    int m_Number;
    CvScalar m_Max;
    CvScalar m_Min;
};

#endif // RENDERTHREAD_H
