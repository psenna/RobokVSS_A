#include "math.h"

struct Position
{
    float x, y, number_of_pixels;

    /*
     * Operator < Overloading
     * This is used in std::sort() function, descending order.
     * @param other the Position object to be compared with this.
     */
    bool operator<(const Position& other) const
    {
        return number_of_pixels > other.number_of_pixels;
    }

    /*
     * Calculates the distance between two Position objects.
     * @param pos object which distance from this is desired.
     */
    float distance(Position pos){
      return sqrt((x - pos.x) * (x - pos.x) + (y - pos.y) * (y - pos.y));
    }

};

struct Goal
{
    Position post_1;
    Position post_2;
};

struct Comando
{
    int velocidadeEsq;
    int velodicadeDir;
    int sentidoEsq;
    int sentidoDir;
    int id;
};
