#include "vision.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

// Constante para o parâmetro SIZE usado nas técnicas erode e dilate.
#define SIZE 2

using namespace std;

Vision* Vision::m_Instance = 0;

Vision::Vision()
{
    m_VideoCapture.open(0);

    for (int i = 0; i < 9; i++)
    {
        m_RenderThreads[i].setMinMax(cvScalar(0, 0, 0), cvScalar(100, 100, 100));
        m_RenderThreads[i].setNumber(i);
    }
}

Vision* Vision::getInstance()
{
    if (!m_Instance)
        m_Instance = new Vision();
    return m_Instance;
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
        //renderImage(fs);
        imshow("Janela 4", m_FrameOriginal);
        cv::waitKey(1);
    }
}

/* Adjust Image
 * Método responsável pelos ajustes na imagem.
 * brilho, contraste, saturaçao e retificaçao.
 */
void Vision::adjustImage(){
    float aImg[8] = {0.0, 0, m_FrameOriginal.cols/2, 0, 0, m_FrameOriginal.rows/2, m_FrameOriginal.cols/2, m_FrameOriginal.rows/2};
    rectifyImage(aImg);
}


/* faz a captura da imagem */
bool Vision::captureImage(){

    if (!m_VideoCapture.isOpened())
        m_VideoCapture.open(m_IdCamera);

    if (!m_VideoCapture.read(m_FrameOriginal))
    {
        cout << "Please check your device." << endl;
        return false;
    }

    return true;

}

/* retifica a imagem, aWorld = limites da imagem, aWImg = limites da area a retificar */
void Vision::rectifyImage(float aImg[8]){
    float aWorld[8] = {0, 0, m_FrameOriginal.cols, 0, 0, m_FrameOriginal.rows, m_FrameOriginal.cols, m_FrameOriginal.rows};
    CvMat mImg, mWorld;
    cvInitMatHeader(&mImg, 4, 2, CV_32FC1, aImg, 0);
    cvInitMatHeader(&mWorld, 4, 2, CV_32FC1, aWorld, 0);

    CvMat* hist = cvCreateMat(3, 3, CV_32FC1);
    cvFindHomography(&mImg, &mWorld, hist, 0, 0.0, NULL);
    IplImage temp = m_FrameOriginal;
    cvWarpPerspective(&temp, &temp, hist);
    Mat frameAuxiliar(&temp);
    m_FrameOriginal = frameAuxiliar.clone();
    frameAuxiliar.release();
}


/* converte a imagem capturada de BGR para HSV */
void Vision::convertImage()
{
    cvtColor(m_FrameOriginal, m_FrameHSV, CV_BGR2HSV);
}

/* binariza a imagem. min e max sao os intervalos HSV para binarizaçao */
cv::Mat Vision::thresholdImage(CvScalar min, CvScalar max)
{
    cv::Mat binaryFrame;
    inRange(m_FrameHSV, (Scalar) min, (Scalar) max, binaryFrame);
    return binaryFrame;
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


void Vision::renderImage(Fieldstate *fs)
{
    for (int i = 0; i < 9; i++)    
        m_RenderThreads[i].start();

    identifyRobot(fs);

}


cv::Mat Vision::dilateImage(const cv::Mat &binaryFrame)
{
    Mat result;

    // Tipos podem ser:
    // MORPH_RECT, MORPH_CROSS, MORPH_ELLIPSE
    // Precisamos ver qual será melhor para o projeto
    Mat element = cv::getStructuringElement(cv::MORPH_CROSS,
                                            cv::Size(2 * SIZE + 1, 2 * SIZE + 1),
                                            cv::Point(SIZE, SIZE));
    cv::dilate(binaryFrame, result, element);
}

cv::Mat Vision::erodeImage(const cv::Mat &binaryFrame)
{
    Mat result;

    // Tipos podem ser:
    // MORPH_RECT, MORPH_CROSS, MORPH_ELLIPSE
    // Precisamos ver qual será melhor para o projeto
    Mat element = cv::getStructuringElement(cv::MORPH_CROSS,
                                            cv::Size(2 * SIZE + 1, 2 * SIZE + 1),
                                            cv::Point(SIZE, SIZE));
    cv::erode(binaryFrame, result, element);
}


/* identify Robot
 * método responsável por fazer o casamento entre
 * as posiçoes das cores dos times e dos jogadores no
 * campo, para se descobrir onde é o centro de cada robo
 * e qual sua orientaçao.
 */

void Vision::identifyRobot(Fieldstate *fs){
    double menor_distancia = 0;
    int menorDistanciaId;
    for(int i = 1; i < 4; i++)
    {
        for(unsigned int j = 0; j < m_Found[0].size(); j++)
        {
            if(m_Found[i][0].distance(m_Found[0][j]) < menor_distancia || menor_distancia == 0)
            {
                menor_distancia = m_Found[i][0].distance(m_Found[0][j]);
                menorDistanciaId = j;
            }
            if(menor_distancia != 0) {
                double x = (m_Found[i][0].x + m_Found[0][menorDistanciaId].x) / 2;
                double y = (m_Found[i][0].y + m_Found[0][menorDistanciaId].y) / 2;
                fs->getRobotTeamById(j).setPosition((int) x, (int) y);
            }
        }

    }
}

void Vision::setCameraId(const int &id){
    m_IdCamera = id;
}
