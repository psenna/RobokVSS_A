#include "math.h"

struct posicao//objeto encontrado em flood do frame binarizado
{
    float x, y, npix; // npix = quantidade de pixeis no objeto
    bool operator<(const posicao& o) const //usado no std::sort(), ordem decrescente
    {
        return npix > o.npix;
    }

    float dist(posicao p){
      return sqrt((x - p.x)*(x - p.x) + (y - p.y) * (y - p.y));
    }
};

struct Goal
{
    posicao post_1;
    posicao post_2;
};
