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

void FindObjectFlood(Mat binary_frame, int row, int column, std::vector<Position> &objects, int min_size, int max_size);
std::vector<Position> DetectColors(Mat binary_frame, int min_size, unsigned int number_of_objects, int max_size);

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


void FindObjectFlood(Mat binary_frame, int row, int column, std::vector<Position> &objects, int min_size, int max_size) //realiza o FloodFill em uma area fechada branca
{
    Point object_center;  //centro da area branca
    int pixels_counter = 1;	//contador para determinar a quantidade de pixels brancos dentro da area
    std::stack<int> row_stack, column_stack;	//pilhas para controle de pontos ja processados. As duas pilhas representam as coordenadas x (coluna) e y (linha) de um ponto
    row_stack.push(row);	//adiciona o primeiro ponto a pilha (coordenada y)
    column_stack.push(column);	//adiciona o primeiro ponto a pilha (coordenada x)
    binary_frame.at<char>(row, column) = 0;

    // Enquanto houver pontos na pilha
    while (column_stack.size() > 0)
    {

        // Pegar os pontos do topo das pilhas
        row = row_stack.top();
        column = column_stack.top();

        // Somar ao centro do objeto para depois fazer uma média aritmética
        object_center.x += column;
        object_center.y += row;

        // Remover da pilha os pontos a ser processados
        column_stack.pop();
        row_stack.pop();


        // Vizinho de cima
        FloodNeighbour(binary_frame, row + 1, column, row_stack, column_stack, pixels_counter);

        // Vizinho de baixo
        FloodNeighbour(binary_frame, row - 1, column, row_stack, column_stack, pixels_counter);

        // Vizinho da direita
        FloodNeighbour(binary_frame, row, column + 1, row_stack, column_stack, pixels_counter);

        // Vizinho da esquerda
        FloodNeighbour(binary_frame, row, column - 1, row_stack, column_stack, pixels_counter);

        // Vizinhos Diagonais
        FloodNeighbour(binary_frame, row - 1, column - 1, row_stack, column_stack, pixels_counter);
        FloodNeighbour(binary_frame, row + 1, column + 1, row_stack, column_stack, pixels_counter);
        FloodNeighbour(binary_frame, row + 1, column - 1, row_stack, column_stack, pixels_counter);
        FloodNeighbour(binary_frame, row - 1, column + 1, row_stack, column_stack, pixels_counter);

    } // end while

    //Verifica se a quantidade de pixels dentro da área branca esta dentro de um intervalo predefinido
    if (pixels_counter > min_size && pixels_counter < max_size) {
        Position object;
        object.number_of_pixels = pixels_counter;
        object.x = object_center.x/pixels_counter;      // seta suas coordenadas x
        object.y = object_center.y/pixels_counter;      // e y usando a média aritmetica de todas as coordenadas somadas pela quantidade de pixels
        objects.push_back(object);                      //adiciona o objeto ao vetor de todas as areas brancas fechadas ja encontradas
    }

}

std::vector<Position> DetectColors(Mat binary_frame, int min_size, unsigned int number_of_objects, int max_size)
{
    std::vector<Position> objects;

    //set a fronteira do frame binário em 0
    //remove a necessidade de verificar se as comparações do flood estão dentro da matriz (12 comparações/pixelBranco a menos)
    for (int i = 0; i < binary_frame.rows; ++i) {
        binary_frame.at<char>(i, 0) = 0;
        binary_frame.at<char>(i, binary_frame.cols - 1) = 0;
    }
    for (int j = 0; j < binary_frame.cols; ++j) {
        binary_frame.at<char>(0, j) = 0;
        binary_frame.at<char>(binary_frame.rows - 1, j) = 0;
    }

    objects.clear();

    // Percorrer toda o binary_frame de 10 em 10 pixels para encontrar áreas brancas
    for (int j, i = 0; i < binary_frame.rows - 1; i = i + 10)
        for (j = 0; j < binary_frame.cols - 1; j = j + 10)
            if (binary_frame.at<char>(i, j) != BLACK_PIXEL)                         // Quando um ponto branco é encontrado
                 FindObjectFlood(binary_frame, i, j, objects, min_size, max_size);  // Adiciona o centro do objeto ao std::vector objetos

    std::sort(objects.begin(), objects.end());                                      // Ordena objetos em ordem crescente pela quantidade de pixels

    // Ajustar tamanho do std::vector para exatamente a quantidade de objetos
    if (objects.size() > number_of_objects)
        objects.resize(number_of_objects);

    return objects;
}

#endif // FLOODFILL_HPP
