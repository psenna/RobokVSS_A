#include "Interface/mainwindow.h"
#include "ui_mainwindow.h"
#include "vision.h"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <vector>
#include <pthread.h>
#include <stdio.h>

using namespace cv;

Vision *vision; //*****BUGOU NA HORA DE COLOCAR COMO ATRIBUTO DE MAINWINDOW.H*****

QImage IplImage2QImage(const IplImage *iplImage);

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    ui = (new Ui::MainWindow);
    ui->setupUi(this);
    this->showMaximized();

    vision = Vision::getInstance();

    display1 = &vision->m_FrameOriginal;
    display2 = &matNula;
}

void *MainWindow::display(){
    Vision::getInstance()->setCameraId(0);
    while(Vision::getInstance()->captureImage()){
        ui->label_2->setPixmap(QPixmap::fromImage(Mat2QImage(*display1)));
        ui->label_3->setPixmap(QPixmap::fromImage(Mat2QImage(*display2)));
    }
}

void *MainWindow::display_helper(void *param){ //http://stackoverflow.com/questions/1151582/pthread-function-from-a-class
    return ((MainWindow *)param)->display();
}

QImage MainWindow::Mat2QImage(Mat matImg){  //simplesmente converte Mat->IplImage->QImage
    IplImage img = (matImg);
    QImage qImg = IplImage2QImage(&img);
    return qImg;
}

void MainWindow::capturar(){
    CvCapture *cap;     //cria um objeto do tipo CvCapture para captar as imagens a cam
    cap = cvCaptureFromCAM(0);   //seta que a captura ocorrera no dispositivo video0(por default é a webcam do note)
    IplImage *frame, *hsv, *img, *bin;   //cria um objeto de imagem, para guardar a imagem capturada;
    frame = cvQueryFrame(cap);  //faz a requisição de um frame e o guarda no objeto préviamente instanciado

    while(frame){    //enquanto frame nao for null, ou seja, houver alguma imagem
        cvWaitKey(10);   //aguarda 10 microseg.
        img = cvCloneImage(frame);  //clona a imagem capturada (sem isso não funciona)
        hsv = cvCloneImage(frame);  //clona

        cvCvtColor(img, hsv, CV_BGR2HSV);  //converte a img original em hsv e guarda em imagem.hsv

        // isso nao é importante agora, somente um método pra colocar imagem em uma label, já que o opencv nao dispoe de outro recurso pra colocar video na UI
        QImage qimg;
        qimg = IplImage2QImage(img);
        ui->label_2->setPixmap(QPixmap::fromImage(qimg));
        qimg = IplImage2QImage(hsv);
        ui->label_3->setPixmap(QPixmap::fromImage(qimg));


        cvReleaseImage(&hsv);
        cvReleaseImage(&img); //desaloca a memória
        frame = cvQueryFrame(cap);  // frame recebe uma nova imagem da captura.
    }

    cvReleaseCapture(&cap); //desaloca o espaço alocado pra captura.
}



MainWindow::~MainWindow()
{
    delete ui;
}

QImage IplImage2QImage(const IplImage *iplImage)
{
    int height = iplImage->height;
    int width = iplImage->width;

    if  (iplImage->depth == IPL_DEPTH_8U && iplImage->nChannels == 3)
    {
        const uchar *qImageBuffer = (const uchar*)iplImage->imageData;
        QImage img(qImageBuffer, width, height, QImage::Format_RGB888);
        return img.rgbSwapped();
    } else if  (iplImage->depth == IPL_DEPTH_8U && iplImage->nChannels == 1){
        const uchar *qImageBuffer = (const uchar*)iplImage->imageData;
        QImage img(qImageBuffer, width, height, QImage::Format_Indexed8);

        QVector<QRgb> colorTable;
        for (int i = 0; i < 256; i++){
            colorTable.push_back(qRgb(i, i, i));
        }
        img.setColorTable(colorTable);
        return img;
    }else{
        return QImage();
    }
}

void MainWindow::on_radioButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);

    display1 = &vision->m_FrameOriginal;
    display2 = &vision->m_FrameBinary;
}

void MainWindow::on_radioButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);

    display1 = &vision->m_FrameOriginal;
    display2 = &vision->m_FrameOriginal; //frameretificado
}

void MainWindow::on_radioButton_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);

    display1 = &vision->m_FrameOriginal;
    display2 = &vision->m_FrameBinary;
}

void MainWindow::on_radioButton_4_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);

    display1 = &vision->m_FrameOriginal;
    display2 = &vision->m_FrameOriginal;    //pintado nele os pontos de ajuste
}

void MainWindow::on_radioButton_5_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
    display1 = &vision->m_FrameOriginal;
    display2 = &matNula;
}
