#include "vision.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "encontraObjeto.hpp"

using namespace std;

Vision::Vision()
{

}

/* get Data
 * Método responsável por toda a manipulaçao dos
 * frames capturados, recebe como parametro o Fieldstate
 * e atualiza a posicao de cada objeto no campo.
 */

void Vision::getData(Fieldstate *fs){
    if(captureImage()){
        //adjustImage();
        convertImage();
        renderImage(fs);
        imshow("janela4", frameOriginal);
        waitKey(0);
    }
}

/* Adjust Image
 * Método responsável pelos ajustes na imagem.
 * brilho, contraste, saturaçao e retificaçao.
 */

void Vision::adjustImage(){
    float aImg[8] = {0.0, 0, frameOriginal.cols/2, 0, 0, frameOriginal.rows/2, frameOriginal.cols/2, frameOriginal.rows/2};
    rectifyImage(aImg);
}


/* faz a captura da imagem */
bool Vision::captureImage(){
    VideoCapture cap(id_camera);

    if (!cap.read(frameOriginal))
    {
        cout << "check your device" << endl;
        return false;
    }
    return true;
}

/* retifica a imagem, aWorld = limites da imagem, aWImg = limites da area a retificar */
void Vision::rectifyImage(float aImg[8]){
    float aWorld[8] = {0, 0, frameOriginal.cols, 0, 0, frameOriginal.rows, frameOriginal.cols, frameOriginal.rows};
    CvMat mImg, mWorld;
    cvInitMatHeader(&mImg, 4, 2, CV_32FC1, aImg,0);
    cvInitMatHeader(&mWorld, 4, 2, CV_32FC1, aWorld,0);

    CvMat* hist = cvCreateMat(3, 3, CV_32FC1);
    cvFindHomography(&mImg, &mWorld, hist, 0, 0.0, NULL);
    IplImage temp = frameOriginal;
    cvWarpPerspective(&temp, &temp, hist);
    Mat frameAuxiliar(&temp);
    frameOriginal = frameAuxiliar.clone();
    frameAuxiliar.release();
}


/* converte a imagem capturada de BGR para HSV */
void Vision::convertImage(){
    cvtColor(frameOriginal, frameHSV, CV_BGR2HSV);
}

/* binariza a imagem. min e max sao os intervalos HSV para binarizaçao */
void Vision::thresholdImage(CvScalar min, CvScalar max){
    inRange(frameHSV, (Scalar) min,(Scalar) max, frameBinary);
}

/*  Render Image
 * método que chama a binarizaçao de todas as imagens
 * e guarda a posicao das cores encontradas no vetor 'encontrados'.
 *
 * índices de encontrados:
 * 0 e 4 - cor time nosso e adversário respectivamente
 * 1,2 e 3 - robos nossos
 * 5,6 e 7 - robos adversarios
 * 8 - bola
 */

void Vision::renderImage(Fieldstate *fs){
    for(int i = 0; i < 9; i++){
        thresholdImage(cvScalar(100,100,100), cvScalar(200, 200, 200));
        encontrados[i] = detectaCores(frameBinary.clone(), 80, 1, 40000); 
    }
    identifyRobot(fs);
}


/* identify Robot
 * método responsável por fazer o casamento entre
 * as posiçoes das cores dos times e dos jogadores no
 * campo, para se descobrir onde é o centro de cada robo
 * e qual sua orientaçao.
 */

void Vision::identifyRobot(Fieldstate *fs){
    double menor_distancia=0;
    int menorDistanciaId;
    for(int i=1; i<4; i++)
    {
        for(unsigned int j=0; j<encontrados[0].size(); j++)
        {
            if(encontrados[i][0].distance(encontrados[0][j]) < menor_distancia || menor_distancia == 0)
            {
                menor_distancia = encontrados[i][0].distance(encontrados[0][j]);
                menorDistanciaId = j;
            }
            if(menor_distancia != 0){
                double x = (encontrados[i][0].x + encontrados[0][menorDistanciaId].x) / 2;
                double y = (encontrados[i][0].y + encontrados[0][menorDistanciaId].y) / 2;
                fs->get_robotTeamById(j).set_position((int) x, (int) y);
            }
        }

    }
}

void Vision::set_cameraid(int id){
    id_camera = id;
}
