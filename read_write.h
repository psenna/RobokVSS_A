#ifndef READ_WRITE_H
#define READ_WRITE_H

#include "opencv2/opencv.hpp"
#include "fieldstate.h"

using namespace robok;

void saveSettings(int brightness, int saturation, int contrast);
void saveRectification(std::vector<float> a);
void saveCalibration(CvScalar MIN[9], CvScalar MAX[9]);
void saveFieldState(CvPoint a, CvPoint b, CvPoint c, CvPoint d, CvPoint e);
bool loadCalibration(CvScalar MIN[9], CvScalar MAX[9]);
bool loadRectification(std::vector<float>* aImg);
void loadFieldstate(Fieldstate* fs);

#endif // READ_WRITE_H
