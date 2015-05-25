#include "read_write.h"
#include <stdio.h>
#include <stdlib.h>


void saveSettings(int brightness, int saturation, int contrast) {

    FILE *file;
    file = fopen("calib", "wb");

    if (file) {
        int writing[3];
        writing[0] = brightness;
        writing[1] = saturation;
        writing[2] = contrast;
        fwrite(writing, 1, sizeof(int) * 3, file);
    }

    fclose(file);
}

void saveRectification(float a[8]) {

    FILE *file;
    file = fopen("retificacao", "wb");

    if (file) {
        int writing[8];
        writing[0] = a[0];
        writing[1] = a[1];
        writing[2] = a[2];
        writing[3] = a[3];
        writing[4] = a[4];
        writing[5] = a[5];
        writing[6] = a[6];
        writing[7] = a[7];
        fwrite(writing, 1, sizeof(float) * 8, file);
    }

    fclose(file);
}

void saveCalibration(CvScalar MIN[10], CvScalar MAX[10]){
    FILE *arq;
    arq = fopen("calib", "wb");

    if(arq) {
        int escrita[60];
        for(int i = 0; i < 10; i++){
            escrita[6*i+0] = MIN[i].val[0];
            escrita[6*i+1] = MAX[i].val[0];
            escrita[6*i+2] = MIN[i].val[1];
            escrita[6*i+3] = MAX[i].val[1];
            escrita[6*i+4] = MIN[i].val[2];
            escrita[6*i+5] = MAX[i].val[2];
        }

        fwrite(escrita, 1, sizeof(int) * 6 * 10, arq);
    }

    fclose(arq);
}

void saveFieldState(CvPoint a, CvPoint b, CvPoint c, CvPoint d, CvPoint e){

    FILE* file;
    file = fopen("field", "wb");

    if(file) {
        int writing[10];

        writing[0] = a.x;
        writing[1] = a.y;
        writing[2] = b.x;
        writing[3] = b.y;
        writing[4] = c.x;
        writing[5] = c.y;
        writing[6] = d.x;
        writing[7] = d.y;
        writing[8] = e.x;
        writing[9] = e.y;

        fwrite(writing, 1, sizeof(int) * 10, file);
    }

    fclose(file);
}

void loadCalibration(CvScalar MIN[10], CvScalar MAX[10]) {

    FILE* file = fopen("calib", "rb");

    if (file != NULL)
    {
        int reading[60];
        fread(reading, 1, sizeof(int) * 6 * 10, file);

        for (int i = 0; i < 10; i++ ) {
            MIN[i].val[0] = reading[(6*i)+0];
            MAX[i].val[0] = reading[(6*i)+1];
            MIN[i].val[1] = reading[(6*i)+2];
            MAX[i].val[1] = reading[(6*i)+3];
            MIN[i].val[2] = reading[(6*i)+4];
            MAX[i].val[2] = reading[(6*i)+5];
        }
        fclose(file);
    }
}

void loadRectification(float aImg[8]){
    FILE *arq = fopen("retificacao","rb");

    if (arq != NULL)
    {
        int leitura[8];
        fread(leitura, 1, sizeof(float) * 8, arq);
        for(int i = 0; i < 8; i++){
            aImg[i] = leitura[i];
        }
        fclose(arq);
    }
}

void loadFieldstate(Fieldstate *fs){
    FILE *arq = fopen("field","rb");
    int a[10];
    if (arq != NULL)
    {
        int leitura[10];
        fread(leitura, 1, sizeof(int) * 10, arq);

        for(int i=0; i<10; i++)
        a[i] = leitura[i];

        fclose(arq);
    }

    Goal goal, goal2;
    goal.GoalPost_1.x = a[0];
    goal.GoalPost_1.y = a[1];

    goal.GoalPost_2.x = a[2];
    goal.GoalPost_2.y = a[3];

    fs->setGoalLeft(goal);

    goal2.GoalPost_1.x = a[4];
    goal2.GoalPost_1.y = a[5];

    goal2.GoalPost_2.x = a[6];
    goal2.GoalPost_2.y = a[7];

    fs->setGoalRight(goal2);

    Position fieldCenter;
    fieldCenter.x = a[8];
    fieldCenter.y = a[9];
    fs->setFieldCenter(fieldCenter);
}
