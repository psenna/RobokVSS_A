#include "read_write.h"
#include <stdio.h>
#include <stdlib.h>

void salvaConfiguracao(int brilho, int saturacao, int contraste){
    FILE *arq;
    arq = fopen("calib", "wb");

      if(arq) {
        int escrita[3];
        escrita[0] = brilho;
        escrita[1] = saturacao;
        escrita[2] = contraste;
        fwrite(escrita, 1, sizeof(int) * 3, arq);
      }

      fclose(arq);
}

void salvaRetifica(CvPoint a, CvPoint b, CvPoint c, CvPoint d){
    FILE *arq;
    arq = fopen("retificacao", "wb");
      if(arq) {
        int escrita[8];
        escrita[0] = a.x;
        escrita[1] = a.y;
        escrita[2] = b.x;
        escrita[3] = b.y;
        escrita[4] = c.x;
        escrita[5] = c.y;
        escrita[6] = d.x;
        escrita[7] = d.y;
        fwrite(escrita, 1, sizeof(int) * 8, arq);
      }

      fclose(arq);
}

void salvaCalibracao(CvScalar MIN[9], CvScalar MAX[9]){
    FILE *arq;
    arq = fopen("calib", "wb");

      if(arq) {
        int escrita[54];
        for(int i = 0; i < 9; i++){
         escrita[6*i+0] = MIN[i].val[0];
         escrita[6*i+1] = MAX[i].val[0];
         escrita[6*i+2] = MIN[i].val[1];
         escrita[6*i+3] = MAX[i].val[1];
         escrita[6*i+4] = MIN[i].val[2];
         escrita[6*i+5] = MAX[i].val[2];
        }

        fwrite(escrita, 1, sizeof(int) * 6 * 9, arq);
      }

      fclose(arq);
}

void salvaEstadoCampo(CvPoint a, CvPoint b, CvPoint c, CvPoint d, CvPoint e){
    FILE* arq;
    arq = fopen("field", "wb");

      if(arq) {
        int escrita[10];

         escrita[0] = a.x;
         escrita[1] = a.y;
         escrita[2] = b.x;
         escrita[3] = b.y;
         escrita[4] = c.x;
         escrita[5] = c.y;
         escrita[6] = d.x;
         escrita[7] = d.y;
         escrita[8] = e.x;
         escrita[9] = e.y;

        fwrite(escrita, 1, sizeof(int) * 10, arq);
      }

      fclose(arq);
}

void loadCalibragem(CvScalar MIN[9], CvScalar MAX[9]){
    FILE* arq = fopen("calib","rb");

    if (arq != NULL)
    {
        int leitura[54];
        fread(leitura, 1, sizeof(int) * 6 * 9, arq);

        for(int i = 0; i < 9; i++ ){
            MIN[i].val[0] = leitura[(6*i)+0];
            MAX[i].val[0] = leitura[(6*i)+1];
            MIN[i].val[1] = leitura[(6*i)+2];
            MAX[i].val[1] = leitura[(6*i)+3];
            MIN[i].val[2] = leitura[(6*i)+4];
            MAX[i].val[2] = leitura[(6*i)+5];
        }
        fclose(arq);
    }
}
