#ifndef VISAO_H
#define VISAO_H

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "fieldstate.h"
using namespace cv;

class Visao
{
private:
    Mat frameOriginal;
    Mat frameHSV;
    Mat frameBinary;
    CvCapture* capture;
    int id_camera;
    Vector<posicao> encontrados[9];
    float aImg[8]; /* pontos para a retificaçao */

public:
    Visao();
    void getData(Fieldstate *fs);
    void set_cameraid(int id);

private:
    void captureImage();
    void adjustImage();
    void rectifyImage(float aImg[8]);
    void convertImage();
    void thresholdImage(CvScalar min, CvScalar max);
    void renderImage(Fieldstate *fs);
    void identifyRobot(Fieldstate *fs);

};

#endif // VISAO_H
