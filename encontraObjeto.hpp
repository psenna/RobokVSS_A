#ifndef ENCONTRAOBJETO3_HPP
#define ENCONTRAOBJETO3_HPP

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stack>
#include <vector>
using namespace cv;
using namespace std;

void findObjFlood(Mat frameBin, int l, int c, vector<Position> &objetos, int tamanhoMin, int tamanhoMax){//encontra um objeto por flood
    Point centObj;
    int npix=1;
    stack <int>pilhaL, pilhaC;
    pilhaL.push(l);
    pilhaC.push(c);
    frameBin.at<char>(l,c) = 0;

    while(pilhaC.size() > 0){
        l = pilhaL.top();
        c = pilhaC.top();
        centObj.x += c;
        centObj.y += l;
        pilhaL.pop();
        pilhaC.pop();
        if(frameBin.at<char>(l+1,c) != 0){
            pilhaL.push(l+1);
            pilhaC.push(c);
            frameBin.at<char>(l+1,c) = 0;
            npix++;
        }
        if(frameBin.at<char>(l-1,c) != 0){
            pilhaL.push(l-1);
            pilhaC.push(c);
            frameBin.at<char>(l-1,c) = 0;
            npix++;
        }
        if(frameBin.at<char>(l,c+1) != 0){
            pilhaL.push(l);
            pilhaC.push(c+1);
            frameBin.at<char>(l,c+1) = 0;
            npix++;
        }
        if(frameBin.at<char>(l,c-1) != 0){
            pilhaL.push(l);
            pilhaC.push(c-1);
            frameBin.at<char>(l,c-1) = 0;
            npix++;
        }
        //diagonais:
        if(frameBin.at<char>(l-1,c-1) != 0){
            pilhaL.push(l-1);
            pilhaC.push(c-1);
            frameBin.at<char>(l-1,c-1) = 0;
            npix++;
        }
        if(frameBin.at<char>(l+1,c+1) != 0){
            pilhaL.push(l+1);
            pilhaC.push(c+1);
            frameBin.at<char>(l+1,c+1) = 0;
            npix++;
        }
        if(frameBin.at<char>(l+1,c-1) != 0){
            pilhaL.push(l+1);
            pilhaC.push(c-1);
            frameBin.at<char>(l+1,c-1) = 0;
            npix++;
        }
        if(frameBin.at<char>(l-1,c+1) != 0){
            pilhaL.push(l-1);
            pilhaC.push(c+1);
            frameBin.at<char>(l-1,c+1) = 0;
            npix++;
        }
    }
    if(npix>tamanhoMin && npix<tamanhoMax){
    Position obj;
    obj.number_of_pixels = npix;
    obj.x = centObj.x/npix;
    obj.y = centObj.y/npix;
    objetos.push_back(obj);
    }
}

std::vector<Position> detectaCores(Mat frameBin, int tamanhoMin, unsigned int numObjetos, int tamanhoMax){
    std::vector<Position> objetos;

    //double t, time;
    //t = (double)cvGetTickCount();

        //set a fronteira do frame binário em 0
        //remove a necessidade de verificar se as comparações do flood estão dentro da matriz (12 comparações/pixelBranco a menos)
        for(int i=0;i<frameBin.rows;++i){
            frameBin.at<char>(i,0) = 0;
            frameBin.at<char>(i,frameBin.cols-1) = 0;}
        for(int j=0;j<frameBin.cols;++j){
            frameBin.at<char>(0,j) = 0;
            frameBin.at<char>(frameBin.rows-1,j) = 0;}

        objetos.clear();
        for(int j,i=0;i<frameBin.rows-1;i=i+10)
            for(j=0;j<frameBin.cols-1;j=j+10)
                if(frameBin.at<char>(i,j) != 0)//encontrou ponto branco
                    findObjFlood(frameBin, i,j, objetos, tamanhoMin, tamanhoMax);//adiciona um obj em flood ao std::vector


    sort(objetos.begin(),objetos.end());//ordena objetos em ordem decrecente de quant de pixels

    if(objetos.size() > numObjetos)// 3 = quantidade máxima de cores para um tipo de objeto (ex.: 3 jogadores, 1 bola)
        objetos.resize(numObjetos);

   // time = (double)cvGetTickCount() - t;
  //  t = time/((double)cvGetTickFrequency()*1000.);
   // std::cout << "time: " << t << std::endl;
    return objetos;
}

#endif // ENCONTRAOBJETO3_HPP
