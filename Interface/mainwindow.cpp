#include "Interface/mainwindow.h"
#include "ui_mainwindow.h"
#include "vision.h"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <vector>

using namespace cv;

Vision *vision; //*****BUGOU NA HORA DE COLOCAR COMO ATRIBUTO DE MAINWINDOW.H*****

QImage IplImage2QImage(const IplImage *iplImage);
QImage Mat2QImage(Mat matImg);

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    ui = (new Ui::MainWindow);
    ui->setupUi(this);
    this->setWindowTitle("Robok VSS System");
    this->showMaximized();

    vision = Vision::getInstance();
    Fieldstate *fs = NULL;

    display1 = &vision->m_FrameOriginal;
    display2 = &vision->m_FrameBinary;
    vision->setCameraId(0);

    on_radioButton_clicked();
}

QImage Mat2QImage(Mat matImg){  //simplesmente converte Mat->IplImage->QImage
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
    ui->label_3->clear();
    while(vision->captureImage() && ui->radioButton->isChecked()){
        ui->label_2->setPixmap(QPixmap::fromImage(Mat2QImage(vision->m_FrameOriginal)));
    }
}

void MainWindow::on_radioButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);

    while(vision->captureImage() && ui->radioButton_2->isChecked()){
        ui->label_2->setPixmap(QPixmap::fromImage(Mat2QImage(vision->m_FrameOriginal)));
        ui->label_3->setPixmap(QPixmap::fromImage(Mat2QImage(vision->m_FrameOriginal)));
    }
}

void MainWindow::on_radioButton_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    int id = 0;
    int h1, h2, s1, s2, v1, v2;

    while(vision->captureImage() && ui->radioButton_3->isChecked()){
        vision->calibrate(id);
        ui->label_2->setPixmap(QPixmap::fromImage(Mat2QImage(vision->m_FrameOriginal)));
        ui->label_3->setPixmap(QPixmap::fromImage(Mat2QImage(vision->m_FrameBinary)));

        if(ui->radioButton_6->isChecked()) id = 0;
        if(ui->radioButton_7->isChecked()) id = 4;
        if(ui->radioButton_8->isChecked()) id = 1;
        if(ui->radioButton_9->isChecked()) id = 2;
        if(ui->radioButton_10->isChecked()) id = 3;
        if(ui->radioButton_11->isChecked()) id = 5;
        if(ui->radioButton_12->isChecked()) id = 6;
        if(ui->radioButton_13->isChecked()) id = 7;
        if(ui->radioButtonBall->isChecked()) id = 8;

        h1 = ui->horizontalSliderh1->value();
        h2 = ui->horizontalSliderh2->value();
        s1 = ui->horizontalSliders1->value();
        s2 = ui->horizontalSliders2->value();
        v1 = ui->horizontalSliderv1->value();
        v2 = ui->horizontalSliderv2->value();

        vision->setMinMax(cvScalar(h1, s1, v1), cvScalar(h2, s2, v2), id);
    }
}

void MainWindow::on_radioButton_4_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);

    while(vision->captureImage() && ui->radioButton_4->isChecked()){
        ui->label_2->setPixmap(QPixmap::fromImage(Mat2QImage(vision->m_FrameOriginal)));
        ui->label_3->setPixmap(QPixmap::fromImage(Mat2QImage(vision->m_FrameOriginal)));
    }
}

void MainWindow::on_radioButton_5_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);

    while(vision->captureImage() && ui->radioButton_5->isChecked()){
        ui->label_2->setPixmap(QPixmap::fromImage(Mat2QImage(vision->m_FrameOriginal)));
        ui->label_3->setPixmap(QPixmap::fromImage(Mat2QImage(vision->m_FrameOriginal)));
    }
}
