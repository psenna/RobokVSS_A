#ifndef READ_WRITE_H
#define READ_WRITE_H

#include "opencv2/opencv.hpp"

void salvaConfiguracao(int brilho, int saturacao, int contraste);
void salvaRetifica(CvPoint a, CvPoint b, CvPoint c, CvPoint d);
void salvaCalibracao(CvScalar MIN[9], CvScalar MAX[9]);
void salvaEstadoCampo(CvPoint a, CvPoint b, CvPoint c, CvPoint d, CvPoint e);
void loadCalibragem(CvScalar MIN[9], CvScalar MAX[9]);

#endif // READ_WRITE_H
