#include "vision.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <sys/time.h>

// Constante para o parâmetro SIZE usado nas técnicas erode e dilate.
#define SIZE 2

using namespace std;
using namespace cv;

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
    bordasFrameOriginal.resize(4);
    bordasRectify.resize(4);

    m_RenderThreads[0].setQuantObj(3);
    m_RenderThreads[1].setQuantObj(1);
    m_RenderThreads[2].setQuantObj(1);
    m_RenderThreads[3].setQuantObj(1);
    m_RenderThreads[4].setQuantObj(3);
    m_RenderThreads[5].setQuantObj(1);
    m_RenderThreads[6].setQuantObj(1);
    m_RenderThreads[7].setQuantObj(1);
    m_RenderThreads[8].setQuantObj(1);
    m_RenderThreads[9].setQuantObj(4);
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

    double ti,tf; // ti = tempo inicial // tf = tempo final
    ti = time();

    convertImage(&m_FrameRect);
    renderImage();
    for (int i = 0; i < 4; ++i) {
        std::cout<<i<<" e "<<m_Found[i].size()<<"\n";

    }
    identifyRobot(fs);

    std::cout<<"lol";

    tf = time();
    cout << "Tempo gasto em milissegundos " << (tf-ti)*1000 << endl;

    cv::waitKey(1);
}

void Vision::calibrate(int id, Mat* frameAlvo) {

    if (captureImage()) {
        //adjustImage();
        convertImage(frameAlvo);
        m_FrameBinary = thresholdImage(m_Min[id], m_Max[id]);
        m_FrameBinary = erodeImage(m_FrameBinary);
        m_FrameBinary = dilateImage(m_FrameBinary);
    }
}

void Vision::autoRetificationSet(){
    if(captureImage()){
        convertImage(&m_FrameOriginal);

        m_RenderThreads[9].start();
        m_RenderThreads[9].wait();

        if(m_Found[9].size()<4)return; //Erro: Calibre novamente as Bordas do campo

        std::vector<float> a(8);
        float medx=0, medy=0;  //guardará a média dos pontos, ponto central

        for(int i=0; i<4; i++){
            medx += m_Found[9][i].x/4;
            medy += m_Found[9][i].y/4;
        }

        for(int i=0; i<4; i++){             //compara com o ponto central para separar os pontos
            if(medx - m_Found[9][i].x > 0 && medy - m_Found[9][i].y > 0){
                a[0] = m_Found[9][i].x;
                a[1] = m_Found[9][i].y;
            }
            if(medx - m_Found[9][i].x > 0 && medy - m_Found[9][i].y < 0){
                a[2] = m_Found[9][i].x;
                a[3] = m_Found[9][i].y;
            }
            if(medx - m_Found[9][i].x < 0 && medy - m_Found[9][i].y < 0){
                a[4] = m_Found[9][i].x;
                a[5] = m_Found[9][i].y;
            }
            if(medx - m_Found[9][i].x < 0 && medy - m_Found[9][i].y > 0){
                a[6] = m_Found[9][i].x;
                a[7] = m_Found[9][i].y;
            }
        }

        setRetificationsParam(a);
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
    //resize image to the display size
    resize(m_FrameOriginal, m_FrameOriginal, tamDisplay);
    cvWaitKey(1);
    return true;

}

/* retifica a imagem, bordasFrameOriginal = limites da imagem, bordasRectify = limites da area a retificar */
void Vision::rectifyImage(){

    //codigo copiado e modificado de: http://opencvexamples.blogspot.com/2014/01/perspective-transform.html#.VWV1Aq1hiko

    // Get the Perspective Transform Matrix i.e. lambda
    Mat lambda = findHomography(bordasRectify,bordasFrameOriginal,0);
    // Apply the Perspective Transform just found to the src image
    warpPerspective(m_FrameOriginal,m_FrameRect,lambda,m_FrameOriginal.size());
    //resize image to the display size
    resize(m_FrameRect, m_FrameRect, tamDisplay);
}


/* converte a imagem capturada de BGR para HSV */
void Vision::convertImage(Mat* frameAlvo)
{
    cvtColor(*frameAlvo, m_FrameHSV, CV_BGR2HSV);
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

void Vision::renderImage()
{
    for (int i = 0; i < 8; ++i)
        m_RenderThreads[i].start();
    for (int i = 0; i < 8; ++i) {
        m_RenderThreads[i].wait();
    }
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
    return result;
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
    return result;
}

/* identify Robot
 * método responsável por fazer o casamento entre
 * as posiçoes das cores dos times e dos jogadores no
 * campo, para se descobrir onde é o centro de cada robo
 * e qual sua orientaçao.
 */

void Vision::identifyRobot(Fieldstate *fs){
    double menor_distancia;
    int menorDistanciaId;
    Robot robot;
    for(int i = 1; i < 4; i++)
    {
        if(!m_Found[i].empty()){
            menor_distancia = 9999;
            for(unsigned int j = 0; j < m_Found[0].size(); j++)
            {
                if(m_Found[i][0].distance(m_Found[0][j]) < menor_distancia || menor_distancia == 0)
                {
                    menor_distancia = m_Found[i][0].distance(m_Found[0][j]);
                    menorDistanciaId = j;
                }
            }
            if(menor_distancia != 0) {
                float x = (m_Found[i][0].x + m_Found[0][menorDistanciaId].x) / 2;
                float y = (m_Found[i][0].y + m_Found[0][menorDistanciaId].y) / 2;
                robot.setPosition(x,y);
                fs->setRobotTeamById(robot,i-1);
                std::cout<<"teste: "<< x << "\n";
                std::cout<<fs->getRobotTeamById(i-1).getPosition().x<<"\n";
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

std::vector<float> Vision::convertBordas(std::vector<cv::Point2f> bordas){
    std::vector<float> a(8);
    a[0] = bordas[0].x;
    a[1] = bordas[0].y;
    a[2] = bordas[1].x;
    a[3] = bordas[1].y;
    a[4] = bordas[2].x;
    a[5] = bordas[2].y;
    a[6] = bordas[3].x;
    a[7] = bordas[3].y;
    return a;
}

void Vision::setRetificationsParam(std::vector<float> aImg){
    // The 4 points that select quadilateral on the input , from top-left in clockwise order
    // These four pts are the sides of the rect box used as input
    bordasRectify[0] = Point2f(aImg[0],aImg[1]);
    bordasRectify[1] = Point2f(aImg[2],aImg[3]);
    bordasRectify[2] = Point2f(aImg[4],aImg[5]);
    bordasRectify[3] = Point2f(aImg[6],aImg[7]);
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
