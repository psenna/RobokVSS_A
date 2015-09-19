#ifndef MOTION_H
#define MOTION_H

#include "fieldstate.h"

class Motion
{
public:
    static void movimenta(robok::Fieldstate *fs);
    static void verificarAngulo(float angRelativo, int id, robok::Fieldstate * fs);
    static void fazCurva(float angRelativo, int id, robok::Fieldstate * fs, int dist);
    static float previsaoAngulo(robok::Fieldstate * fs, float angRelativo, int id);

};

#endif // MOTION_H
