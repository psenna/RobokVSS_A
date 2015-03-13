#ifndef ENCONTRAOBJETO3_HPP
#define ENCONTRAOBJETO3_HPP

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stack>
#include <vector>

using cv::Mat;
using cv::Point;
using std::stack;

void FindObjectFlood(Mat frameBin, int l, int c, std::vector<Position> &objects, int min_size, int max_size);
std::vector<Position> DetectColors(Mat frameBin, int min_size, unsigned int number_of_objects, int max_size);



void FindObjectFlood(Mat frameBin, int l, int c, std::vector<Position> &objects, int min_size, int max_size)
{
    Point centObj;
    int pixels_counter = 1;
    std::stack<int> pilhaL, pilhaC;
    pilhaL.push(l);
    pilhaC.push(c);
    frameBin.at<char>(l, c) = 0;

    while (pilhaC.size() > 0) {
        l = pilhaL.top();
        c = pilhaC.top();
        centObj.x += c;
        centObj.y += l;
        pilhaL.pop();
        pilhaC.pop();

        if (frameBin.at<char>(l+1, c) != 0) {
            pilhaL.push(l+1);
            pilhaC.push(c);
            frameBin.at<char>(l+1, c) = 0;
            pixels_counter++;
        }

        if (frameBin.at<char>(l-1, c) != 0) {
            pilhaL.push(l-1);
            pilhaC.push(c);
            frameBin.at<char>(l-1, c) = 0;
            pixels_counter++;
        }

        if (frameBin.at<char>(l,c+1) != 0) {
            pilhaL.push(l);
            pilhaC.push(c+1);
            frameBin.at<char>(l, c+1) = 0;
            pixels_counter++;
        }

        if (frameBin.at<char>(l, c-1) != 0) {
            pilhaL.push(l);
            pilhaC.push(c-1);
            frameBin.at<char>(l,c-1) = 0;
            pixels_counter++;
        }

        //diagonais:
        if (frameBin.at<char>(l-1,c-1) != 0) {
            pilhaL.push(l-1);
            pilhaC.push(c-1);
            frameBin.at<char>(l-1, c-1) = 0;
            pixels_counter++;
        }
        if (frameBin.at<char>(l+1,c+1) != 0) {
            pilhaL.push(l+1);
            pilhaC.push(c+1);
            frameBin.at<char>(l+1,c+1) = 0;
            pixels_counter++;
        }

        if (frameBin.at<char>(l+1,c-1) != 0) {
            pilhaL.push(l+1);
            pilhaC.push(c-1);
            frameBin.at<char>(l+1,c-1) = 0;
            pixels_counter++;
        }

        if (frameBin.at<char>(l-1,c+1) != 0) {
            pilhaL.push(l-1);
            pilhaC.push(c+1);
            frameBin.at<char>(l-1, c+1) = 0;
            pixels_counter++;
        }
    } // end while

    if (pixels_counter > min_size && pixels_counter < max_size) {
        Position object;
        object.number_of_pixels = pixels_counter;
        object.x = centObj.x/pixels_counter;
        object.y = centObj.y/pixels_counter;
        objects.push_back(object);
    }
}

std::vector<Position> DetectColors(Mat frameBin, int min_size, unsigned int number_of_objects, int max_size)
{
    std::vector<Position> objects;

    //double t, time;
    //t = (double)cvGetTickCount();

        //set a fronteira do frame binário em 0
        //remove a necessidade de verificar se as comparações do flood estão dentro da matriz (12 comparações/pixelBranco a menos)
        for (int i = 0; i < frameBin.rows; ++i) {
            frameBin.at<char>(i, 0) = 0;
            frameBin.at<char>(i, frameBin.cols - 1) = 0;
        }
        for (int j = 0; j < frameBin.cols; ++j) {
            frameBin.at<char>(0, j) = 0;
            frameBin.at<char>(frameBin.rows - 1, j) = 0;
        }

        objects.clear();

        for (int j, i = 0; i < frameBin.rows - 1; i = i + 10)
            for (j = 0; j < frameBin.cols - 1; j = j + 10)
                if (frameBin.at<char>(i, j) != 0) //encontrou ponto branco
                    FindObjectFlood(frameBin, i, j, objects, min_size, max_size); //adiciona um obj em flood ao std::vector

    std::sort(objects.begin(), objects.end());//ordena objetos em ordem decrecente de quant de pixels

    if (objects.size() > number_of_objects) // 3 = quantidade máxima de cores para um tipo de objeto (ex.: 3 jogadores, 1 bola)
        objects.resize(number_of_objects);

   // time = (double)cvGetTickCount() - t;
  //  t = time/((double)cvGetTickFrequency()*1000.);
   // std::cout << "time: " << t << std::endl;
    return objects;
}

#endif // ENCONTRAOBJETO3_HPP
