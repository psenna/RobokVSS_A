#ifndef READ_WRITE_H
#define READ_WRITE_H

#include "opencv2/opencv.hpp"
#include "fieldstate.h"

void saveSettings(int brightness, int saturation, int contrast);
void saveRectification(float a[8]);
void saveCalibration(CvScalar MIN[9], CvScalar MAX[9]);
void saveFieldState(CvPoint a, CvPoint b, CvPoint c, CvPoint d, CvPoint e);
void loadCalibration(CvScalar MIN[9], CvScalar MAX[9]);
void loadRectification(float aImg[8]);
void loadFieldstate(Fieldstate* fs);

#endif // READ_WRITE_H
