#include "Interface/mainwindow.h"
#include "ui_mainwindow.h"
#include "vision.h"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "read_write.h"
#include <vector>
#include <QMouseEvent>
#include <QtWidgets/QMessageBox>

#include <stdio.h>

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
    ui->radioButton_6->setChecked(true);
    int id = 0, novo;
    int h1, h2, s1, s2, v1, v2;

    atualizaSliders(-1);

    while(ui->radioButton_3->isChecked()){
        vision->calibrate(id);
        ui->label_2->setPixmap(QPixmap::fromImage(Mat2QImage(vision->m_FrameOriginal)));
        ui->label_3->setPixmap(QPixmap::fromImage(Mat2QImage(vision->m_FrameBinary)));

        id = atualizaSliders(id);

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

void MainWindow::on_buttonSaveCalib_clicked()
{
    salvaCalibracao(vision->getAllMin(), vision->getAllMax());
}

void MainWindow::on_buttonLoadCalib_clicked()
{
    CvScalar a[9], b[9];
    loadCalibragem(a, b);
    for(int i=0; i<9; i++){
        vision->setMinMax(a[i], b[i], i);
    }
    atualizaSliders(-1);
}

void MainWindow::mousePressEvent( QMouseEvent* ev )
{
    if(ui->radioButton_3->isChecked()){
        if(ev->button() & Qt::RightButton){
        QPoint P = ui->label_2->mapFrom(this, ev->pos());
        cv::Vec3b vec = Vision::getInstance()->m_FrameHSV.at<cv::Vec3b>(cv::Point(P.x(),P.y()));

        if(vec[0]-15 < 0) vec[0] = 15; if(vec[0]+15 > 180) vec[0] = 165;
        if(vec[1]-15 < 0) vec[1] = 15; if(vec[1]+15 > 255) vec[1] = 240;
        if(vec[2]-15 < 0) vec[2] = 15; if(vec[2]+15 > 255) vec[2] = 240;

        ui->horizontalSliderh1->setValue(vec[0]-15); ui->horizontalSliderh2->setValue(vec[0]+15);
        ui->horizontalSliders1->setValue(vec[1]-15); ui->horizontalSliders2->setValue(vec[1]+15);
        ui->horizontalSliderv1->setValue(vec[2]-15); ui->horizontalSliderv2->setValue(vec[2]+15);
        }
        else if(ev->button() & Qt::LeftButton){
            //implementar add intervalo HSV
        }
    }
}


int MainWindow::atualizaSliders(int id){
    int novo;

    if(ui->radioButton_6->isChecked()) novo = 0;
    if(ui->radioButton_7->isChecked()) novo = 4;
    if(ui->radioButton_8->isChecked()) novo = 1;
    if(ui->radioButton_9->isChecked()) novo = 2;
    if(ui->radioButton_10->isChecked()) novo = 3;
    if(ui->radioButton_11->isChecked()) novo = 5;
    if(ui->radioButton_12->isChecked()) novo = 6;
    if(ui->radioButton_13->isChecked()) novo = 7;
    if(ui->radioButtonBall->isChecked()) novo = 8;

    if(novo!=id){
        ui->horizontalSliderh1->setValue(vision->getAllMin()[novo].val[0]);
        ui->horizontalSliderh2->setValue(vision->getAllMax()[novo].val[0]);
        ui->horizontalSliders1->setValue(vision->getAllMin()[novo].val[1]);
        ui->horizontalSliders2->setValue(vision->getAllMax()[novo].val[1]);
        ui->horizontalSliderv1->setValue(vision->getAllMin()[novo].val[2]);
        ui->horizontalSliderv2->setValue(vision->getAllMax()[novo].val[2]);
        return novo;
    }
    return id;
}
