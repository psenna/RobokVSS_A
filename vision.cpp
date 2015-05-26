#include "vision.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <sys/time.h>

// Constante para o parâmetro SIZE usado nas técnicas erode e dilate.
#define SIZE .5

using namespace std;

Vision* Vision::m_Instance = 0;

double time();

Vision::Vision()
{
    m_VideoCapture.open(1);

    for (int i = 0; i < 10; i++)
    {
        this->setMinMax(cvScalar(0, 0, 0), cvScalar(0, 0, 0), i);
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

        double ti,tf; // ti = tempo inicial // tf = tempo final
        ti = time();

        //adjustImage();
        convertImage();
        renderImage(fs);


        tf = time();
        cout << "Tempo gasto em milissegundos " << (tf-ti)*1000 << endl;

//        std::cout << "size: " << this->m_Found[1].size() << std::endl;
//        if(m_Found[1].size() > 0){
//            std::cout << "tam: " << this->m_Found[1].at(0).number_of_pixels << std::endl;
//            cv::circle(m_FrameOriginal, cvPoint(m_Found[1].at(0).x, m_Found[1].at(0).y), 10, cvScalar(255,0,0));
//        }

        cv::waitKey(1);
    }
}

void Vision::calibrate(int id) {

    if (captureImage()) {
        //adjustImage();
        convertImage();
        //cv::setMouseCallback("Original Frame", mouseEvent, &m_FrameHSV);
        m_FrameBinary = thresholdImage(m_Min[id], m_Max[id]);
        m_FrameBinary = erodeImage(m_FrameBinary);
        m_FrameBinary = dilateImage(m_FrameBinary);
    }
}

void Vision::retification(){
    if(captureImage()){
        convertImage();
        setRetificationsParam(0,0,m_FrameOriginal.cols,0,0, m_FrameOriginal.rows,m_FrameOriginal.cols,m_FrameOriginal.rows);
        rectifyImage();
    }
}

void Vision::autoRetification(){
    if(captureImage()){
        convertImage();

        cv::Mat binaryFrame = thresholdImage(m_Min[9], m_Max[9]);

        m_RenderThreads[9].start();
        m_RenderThreads[9].wait();

        float minx = 8888, miny = 8888, maxx = 0, maxy=0; //botar taam max

        for(int i=0; i<4; i++){
            minx = min(m_Found[9][i].x, minx);
            miny = min(m_Found[9][i].y, miny);
            maxx = max(m_Found[9][i].x, maxx);
            maxy = max(m_Found[9][i].y, maxy);
        }

        setRetificationsParam(minx, miny,maxx , miny, minx, maxy, maxx, maxy);

        rectifyImage();
    }
}

/* Adjust Image
 * Método responsável pelos ajustes na imagem.
 * brilho, contraste, saturaçao e retificaçao.
 */
void Vision::adjustImage(){
    rectifyImage();
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
    cvWaitKey(1);
    return true;

}

/* retifica a imagem, aWorld = limites da imagem, aImg = limites da area a retificar */
void Vision::rectifyImage(){
    //float aWorld[8] = {0, 0, 540, 0, 0, 405, 540, 405};
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
cv::Mat Vision::thresholdImage(Scalar min, Scalar max)
{
    cv::Mat binaryFrame;
    inRange(m_FrameHSV, min, max, binaryFrame);

    return binaryFrame;
}

/*  Render Image
 * método que chama a binarizaçao de todas as imagens
 * e guarda a posicao das cores encontradas no vetor 'encontrados'.
 *  H: 0 - 180  S: 0 - 255  V: 0 - 255
 *
 * índices de encontrados:
 * 0 e 4 - cor time nosso e adversário respectivamente
 * 1,2 e 3 - robos nossos
 * 5,6 e 7 - robos adversarios
 * 8 - bola
 * 9 - bordas do campo
 */

void Vision::renderImage(Fieldstate *fs)
{
    for (int i = 0; i < 9; i++)    
        m_RenderThreads[i].start();

    //identifyRobot(fs);
}


cv::Mat Vision::dilateImage(const cv::Mat &binaryFrame)
{
    Mat result;

    // Tipos podem ser:
    // MORPH_RECT, MORPH_CROSS, MORPH_ELLIPSE
    // Precisamos ver qual será melhor para o projeto
    Mat element = cv::getStructuringElement(cv::MORPH_CROSS,
                                            cv::Size(1 * SIZE + 1, 1 * SIZE + 1),
                                            cv::Point(SIZE, SIZE));
    cv::dilate(binaryFrame, result, element);
    return result;
}

cv::Mat Vision::erodeImage(const cv::Mat &binaryFrame)
{
    Mat result;

    // Tipos podem ser:
    // MORPH_RECT, MORPH_CROSS, MORPH_ELLIPSE
    // Precisamos ver qual será melhor para o projeto
    Mat element = cv::getStructuringElement(cv::MORPH_CROSS,
                                            cv::Size(1 * SIZE + 1, 1 * SIZE + 1),
                                            cv::Point(SIZE, SIZE));
    cv::erode(binaryFrame, result, element);
    return result;
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
        if(!m_Found[i].empty()){
            for(unsigned int j = 0; j < m_Found[0].size(); j++)
            {
                if(m_Found[i][0].distance(m_Found[0][j]) < menor_distancia || menor_distancia == 0)
                {
                    menor_distancia = m_Found[i][0].distance(m_Found[0][j]);
                    menorDistanciaId = j;
                }
            }
            if(menor_distancia != 0) {
                double x = (m_Found[i][0].x + m_Found[0][menorDistanciaId].x) / 2;
                double y = (m_Found[i][0].y + m_Found[0][menorDistanciaId].y) / 2;
                m_Found[0][menorDistanciaId].x = 9999;
                m_Found[0][menorDistanciaId].y = 9999;
                fs->getRobotTeamById(i-1).setPosition((int) x, (int) y);
            }
        }
    }
}

void Vision::setCameraId(const int &id){
    m_IdCamera = id;
}

void Vision::setMinMax(const CvScalar &min, const CvScalar &max, const int &id){
    m_Min[id] = min;
    m_Max[id] = max;
}

void Vision::setRetificationsParam(float a, float b, float c, float d, float e, float f, float g, float h){
    aImg[0] = a;
    aImg[1] = b;
    aImg[2] = c;
    aImg[3] = d;
    aImg[4] = e;
    aImg[5] = f;
    aImg[6] = g;
    aImg[7] = h;
}

CvScalar* Vision::getMin(){
    return m_Min;
}

CvScalar* Vision::getMax(){
    return m_Max;
}

void Vision::closeCapture(){
    m_VideoCapture.release();
}

double time()
{
    struct timeval tv;
    gettimeofday(&tv, 0);
    return tv.tv_sec + tv.tv_usec/1e6;
}
