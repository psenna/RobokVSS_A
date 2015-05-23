#include "opencv2/opencv.hpp"
#include "vision.h"

void mouseSetClick(cv::Mat *hsv, int x, int y){

    IplImage img = (*hsv);
    int h = (int) ((uchar *)(img.imageData + img.widthStep*y))[x*3];
    int s = (int) ((uchar *)(img.imageData + img.widthStep*y))[x*3 + 1];
    int v = (int) ((uchar *)(img.imageData + img.widthStep*y))[x*3 + 2];

    //std::cout << "h " << h << " s " << s << " v " << v << std::endl;

    if(h-15 < 0) h = 15;
    if(s-15 < 0) s = 15;
    if(v-15 < 0) v = 15;

    if(h+15 > 180) h = 165;
    if(s+15 > 255) s = 240;
    if(v+15 > 255) v = 240;

    Vision::getInstance()->setMinMax(cvScalar(h-15, s-15, v-15), cvScalar(h+15, s+15, v+15), 1);
}

void mouseAddClick(cv::Mat *hsv, int x, int y){

    IplImage img = (*hsv);
    Vision *vision = Vision::getInstance();

    int h = (int) ((uchar *)(img.imageData + img.widthStep*y))[x*3];
    int s = (int) ((uchar *)(img.imageData + img.widthStep*y))[x*3 + 1];
    int v = (int) ((uchar *)(img.imageData + img.widthStep*y))[x*3 + 2];

    int h1 = vision->getMin(1).val[0];
    int h2 = vision->getMax(1).val[0];
    int s1 = vision->getMin(1).val[1];
    int s2 = vision->getMax(1).val[1];
    int v1 = vision->getMin(1).val[2];
    int v2 = vision->getMax(1).val[2];


    //std::cout << "h " << h << " s " << s << " v " << v << std::endl;

    if(h-15 > 0 && h-15 < h1){
        h1 = h - 15;
    }else{
        if(h-15 < 0){
            h1 = 0;
        }
    }

    if(h+15 < 180 && h+15 > h2){
        h2 = h + 15;
    }else{
        if(h+15 > 180){
            h2 = 180;
        }
    }

    if(s-15 > 0 && s-15 < s1){
        s1 = s - 15;
    }else{
        if(s-15 < 0){
            s1 = 0;
        }
    }

    if(s+15 < 255 && s+15 > s2){
        s2 = s + 15;
    }else{
        if(s+15 > 255){
            s2 = 255;
        }
    }

    if(v-15 > 0 && v-15 < v1){
        v1 = v - 15;
    }else{
        if(v-15 < 0){
            v1 = 0;
        }
    }

    if(v+15 < 255 && v+15 > v2){
        v2 = v + 15;
    }else{
        if(v+15 > 255){
            v2 = 255;
        }
    }

    Vision::getInstance()->setMinMax(cvScalar(h1, s1, v1), cvScalar(h2, s2, v2), 1);
}

void mouseEvent(int evt, int x, int y, int flags, void* param)
{
    if(flags){}
    if (evt == CV_EVENT_LBUTTONDOWN)
    {
        mouseSetClick((cv::Mat*) param, x, y);
    }
    if (evt == CV_EVENT_RBUTTONDOWN)
    {
        mouseAddClick((cv::Mat*) param, x, y);
    }
}


