#ifndef READ_WRITE_H
#define READ_WRITE_H

#include "opencv2/opencv.hpp"

void saveSettings(int brightness, int saturation, int contrast);
void saveRectfication(CvPoint a, CvPoint b, CvPoint c, CvPoint d);
void saveCalibration(CvScalar MIN[9], CvScalar MAX[9]);
void saveFieldState(CvPoint a, CvPoint b, CvPoint c, CvPoint d, CvPoint e);
void loadCalibration(CvScalar MIN[9], CvScalar MAX[9]);

#endif // READ_WRITE_H
