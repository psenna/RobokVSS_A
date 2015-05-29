#ifndef FLOODFILL_HPP
#define FLOODFILL_HPP

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stack>
#include <vector>

#define BLACK_PIXEL 0

using cv::Mat;
using cv::Point;
using std::stack;

void FindObjectFlood(Mat frameBin, int l, int c, std::vector<Position> &objects, int min_size, int max_size);
std::vector<Position> DetectColors(Mat frameBin, int min_size, unsigned int number_of_objects, int max_size);

void FloodNeighbour(Mat &frame, int row, int column, std::stack<int> &row_stack, std::stack<int> &col_stack, int &counter)
{
    // Verifica se vizinho contém ponto branco
    if (frame.at<char>(row, column) != BLACK_PIXEL)
    {
        col_stack.push(column);                             // se sim, adicione a coordenada y na pilha
        row_stack.push(row);                                // E a coordenada x
        frame.at<char>(row, column) = BLACK_PIXEL;          // Pinta o ponto processado de preto
        counter++;                                          // Incrementa o contador
    }
}


void FindObjectFlood(Mat frameBin, int l, int c, std::vector<Position> &objects, int min_size, int max_size) //realiza o FloodFill em uma area fechada branca
{
    Point centObj;  //centro da area branca
    int pixels_counter = 1;	//contador para determinar a quantidade de pixels brancos dentro da area
    std::stack<int> pilhaL, pilhaC;	//pilhas para controle de pontos ja processados. As duas pilhas representam as coordenadas x (coluna) e y (linha) de um ponto
    pilhaL.push(l);	//adiciona o primeiro ponto a pilha (coordenada y)
    pilhaC.push(c);	//adiciona o primeiro ponto a pilha (coordenada x)
    frameBin.at<char>(l, c) = 0;

    while (pilhaC.size() > 0) {		//enquanto houver pontos na pilha
        l = pilhaL.top();
        c = pilhaC.top();       //processar o proximo ponto no topo da pilha
        centObj.x += c;			//soma as coordenadas x
        centObj.y += l;			//e y para futuramente realizar uma media aritmetica
        pilhaC.pop();
        pilhaL.pop();           //remover o ponto a processar da pilha


         //Verifica se o ponto atual possui um vizinho branco (8 comparações, 4 vizinhos diagonais, 2 horizontais e 2 verticais)

        // Vizinho de cima
        FloodNeighbour(frameBin, l+1, c, pilhaL, pilhaC, pixels_counter);

        // Vizinho de baixo
        FloodNeighbour(frameBin, l-1, c, pilhaL, pilhaC, pixels_counter);

        // Vizinho da direita
        FloodNeighbour(frameBin, l, c+1, pilhaL, pilhaC, pixels_counter);

        // Vizinho da esquerda
        FloodNeighbour(frameBin, l, c-1, pilhaL, pilhaC, pixels_counter);

        // Vizinhos Diagonais
        FloodNeighbour(frameBin, l-1, c-1, pilhaL, pilhaC, pixels_counter);
        FloodNeighbour(frameBin, l+1, c+1, pilhaL, pilhaC, pixels_counter);
        FloodNeighbour(frameBin, l+1, c-1, pilhaL, pilhaC, pixels_counter);
        FloodNeighbour(frameBin, l-1, c+1, pilhaL, pilhaC, pixels_counter);

    } // end while

    if (pixels_counter > min_size && pixels_counter < max_size) { //Verifica se a quantidade de pixels dentro da area branca esta dentro de um intervalo predefinido
        Position object;		//cria-se um novo objeto
        object.number_of_pixels = pixels_counter;	//seta a quantidade de pixels
        object.x = centObj.x/pixels_counter;		//seta suas coordenadas x
        object.y = centObj.y/pixels_counter;		// e y usando a media aritmetica de todas as coordenadas somadas pela quantidade de pixels
        objects.push_back(object);			//adiciona o objeto ao vetor de todas as areas brancas fechadas ja encontradas
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

#endif // FLOODFILL_HPP
