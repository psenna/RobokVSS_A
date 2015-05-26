#include "Interface/mainwindow.h"
#include "ui_mainwindow.h"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "read_write.h"
#include "serial.h"
#include <vector>
#include <QMouseEvent>
#include <QMessageBox>

#include <stdio.h>

using namespace cv;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{    
    ui = (new Ui::MainWindow);
    ui->setupUi(this);
    this->setWindowTitle("Robok VSS System");
    this->showMaximized();
    //connect( ui->myQuitButton, SIGNAL( clicked() ), qApp, SLOT( quit() ) );

    QApplication::quit();

    // Instanciar Vision
    m_Vision = Vision::getInstance();

    m_Display1 = &m_Vision->m_FrameOriginal;
    m_Display2 = &m_Vision->m_FrameBinary;
    m_Vision->setCameraId(0);
    callLoadCalibration();//carrega calibragem
    on_rBtnSettings_clicked();
}

MainWindow::~MainWindow()
{
    delete m_Display1;
    delete m_Display2;
    delete m_Vision;
    delete serial;
    delete fs;
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)//Evento de fechar a ui
{
    delete ui;
    m_Vision->closeCapture();
    exit(0);
}

void MainWindow::callLoadCalibration(){  //Adaptador para chamar o Load do read_write.h

    CvScalar a[10], b[10];//tornar o tamanho deste vetor algo mais dinamico
    loadCalibration(a, b);
    for(int i=0; i<10; i++){//para nao ter que ficar mudando toda hora aqui
        m_Vision->setMinMax(a[i], b[i], i);
    }
    updateSliders(-1);
}

int MainWindow::updateSliders(int id){ //Atualiza as Sliders da calibragem dependendo do objeto selecionado
    int novo;

    if(ui->radioButton_6->isChecked()) novo = 0; //cor do time nosso
    if(ui->radioButton_7->isChecked()) novo = 4; //cor do time adversario
    if(ui->radioButton_8->isChecked()) novo = 1; // V
    if(ui->radioButton_9->isChecked()) novo = 2; // V
    if(ui->radioButton_10->isChecked()) novo = 3; //robos nossos
    if(ui->radioButton_11->isChecked()) novo = 5; // V
    if(ui->radioButton_12->isChecked()) novo = 6; // V
    if(ui->radioButton_13->isChecked()) novo = 7; //robos adversarios
    if(ui->radioButtonBall->isChecked()) novo = 8; //bola
    if(ui->radioButtonBorda->isChecked()) novo = 9; //bordas do campo

    if(novo!=id){ // atualiza somente se mudar a opcao de objeto
        ui->horizontalSliderh1->setValue(m_Vision->getMin()[novo].val[0]);
        ui->horizontalSliderh2->setValue(m_Vision->getMax()[novo].val[0]);
        ui->horizontalSliders1->setValue(m_Vision->getMin()[novo].val[1]);
        ui->horizontalSliders2->setValue(m_Vision->getMax()[novo].val[1]);
        ui->horizontalSliderv1->setValue(m_Vision->getMin()[novo].val[2]);
        ui->horizontalSliderv2->setValue(m_Vision->getMax()[novo].val[2]);
        return novo;
    }
    return id;
}

void MainWindow::mousePressEvent(QMouseEvent* ev) //Eventos de mouse na ui (criar uma nova classe de mouse e passar isso pra ela)
{
    if (ui->rBtnCalibrate->isChecked()) {

        QPoint P = ui->label_2->mapFrom(this, ev->pos());
        cv::Vec3b vec = Vision::getInstance()->m_FrameHSV.at<cv::Vec3b>(cv::Point(P.x(),P.y()));

        if (ev->button() & Qt::LeftButton) { //setHsvInterval
            if(vec[0]-15 < 0) vec[0] = 15; if(vec[0]+15 > 180) vec[0] = 165;
            if(vec[1]-15 < 0) vec[1] = 15; if(vec[1]+15 > 255) vec[1] = 240;
            if(vec[2]-15 < 0) vec[2] = 15; if(vec[2]+15 > 255) vec[2] = 240;

            ui->horizontalSliderh1->setValue(vec[0]-15); ui->horizontalSliderh2->setValue(vec[0]+15);
            ui->horizontalSliders1->setValue(vec[1]-15); ui->horizontalSliders2->setValue(vec[1]+15);
            ui->horizontalSliderv1->setValue(vec[2]-15); ui->horizontalSliderv2->setValue(vec[2]+15);

            Vision::getInstance()->setMinMax(cvScalar(vec[0]-15, vec[1]-15, vec[2]-15), cvScalar(vec[0]+15, vec[1]+15, vec[2]+15), 1);
        }
        else if (ev->button() & Qt::RightButton) { //addHSVInterval
            int h1 = m_Vision->getMin()[1].val[0];
            int h2 = m_Vision->getMax()[1].val[0];
            int s1 = m_Vision->getMin()[1].val[1];
            int s2 = m_Vision->getMax()[1].val[1];
            int v1 = m_Vision->getMin()[1].val[2];
            int v2 = m_Vision->getMax()[1].val[2];

            if (vec[0]-15 > 0 && vec[0]-15 < h1) {
                h1 = vec[0] - 15;
            } else {
                if (vec[0]-15 < 0) {
                    h1 = 0;
                }
            }

            if (vec[0]+15 < 180 && vec[0]+15 > h2) {
                h2 = vec[0] + 15;
            } else {
                if(vec[0]+15 > 180){
                    h2 = 180;
                }
            }

            if(vec[1]-15 > 0 && vec[1]-15 < s1){
                s1 = vec[1] - 15;
            }else{
                if(vec[1]-15 < 0){
                    s1 = 0;
                }
            }

            if(vec[1]+15 < 255 && vec[1]+15 > s2){
                s2 = vec[1] + 15;
            }else{
                if(vec[1]+15 > 255){
                    s2 = 255;
                }
            }

            if(vec[2]-15 > 0 && vec[2]-15 < v1){
                v1 = vec[2] - 15;
            }else{
                if(vec[2]-15 < 0){
                    v1 = 0;
                }
            }

            if(vec[2]+15 < 255 && vec[2]+15 > v2){
                v2 = vec[2] + 15;
            }else{
                if(vec[2]+15 > 255){
                    v2 = 255;
                }
            }

            ui->horizontalSliderh1->setValue(h1); ui->horizontalSliderh2->setValue(h2);
            ui->horizontalSliders1->setValue(s1); ui->horizontalSliders2->setValue(s2);
            ui->horizontalSliderv1->setValue(v1); ui->horizontalSliderv2->setValue(v2);

            Vision::getInstance()->setMinMax(cvScalar(h1, s1, v1), cvScalar(h2, s2, v2), 1);
        }
    }
}

//********************************************************************************************
//********************************************************************************************
//*********** ACESSE AS FUNCOES ABAIXO PELA .ui , SAO TODAS FUNCOES DE EVENTO DA UI***********
//********************************************************************************************
//********************************************************************************************

void MainWindow::on_buttonSaveCalib_clicked() //Save da Calibragem
{
    saveCalibration(m_Vision->getMin(), m_Vision->getMax());
}

void MainWindow::on_buttonLoadCalib_clicked() //Load da Calibragem
{
    callLoadCalibration();
}

void MainWindow::on_rBtnSettings_clicked() //Settings
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->label_3->clear();
    while (m_Vision->captureImage() && ui->rBtnSettings->isChecked())
    {
        ui->label_2->setPixmap(QPixmap::fromImage(Mat2QImage(m_Vision->m_FrameOriginal)));
    }
}

void MainWindow::on_rBtnRectifyImage_clicked() //Rectify Image
{
    ui->stackedWidget->setCurrentIndex(1);
    m_Vision->autoRetification();

    while(m_Vision->captureImage() && ui->rBtnRectifyImage->isChecked()){
        m_Vision->m_FrameRect = m_Vision->m_FrameOriginal.clone();
        m_Vision->adjustImage();
        ui->label_2->setPixmap(QPixmap::fromImage(Mat2QImage(m_Vision->m_FrameRect)));
        ui->label_3->setPixmap(QPixmap::fromImage(Mat2QImage(m_Vision->m_FrameOriginal)));
    }
}

void MainWindow::on_rBtnCalibrate_clicked() //Calibrate
{
    ui->stackedWidget->setCurrentIndex(2);
    ui->radioButton_6->setChecked(true);
    int id = 0;
    int h1, h2, s1, s2, v1, v2;

    updateSliders(-1);

    while(ui->rBtnCalibrate->isChecked()){
        m_Vision->calibrate(id);
        ui->label_2->setPixmap(QPixmap::fromImage(Mat2QImage(m_Vision->m_FrameOriginal)));
        ui->label_3->setPixmap(QPixmap::fromImage(Mat2QImage(m_Vision->m_FrameBinary)));

        id = updateSliders(id);

        h1 = ui->horizontalSliderh1->value();
        h2 = ui->horizontalSliderh2->value();
        s1 = ui->horizontalSliders1->value();
        s2 = ui->horizontalSliders2->value();
        v1 = ui->horizontalSliderv1->value();
        v2 = ui->horizontalSliderv2->value();

        m_Vision->setMinMax(cvScalar(h1, s1, v1), cvScalar(h2, s2, v2), id);
    }
}

void MainWindow::on_rBtnFieldAdjust_clicked() //Field Adjust
{
    ui->stackedWidget->setCurrentIndex(3);

    while (m_Vision->captureImage() && ui->rBtnFieldAdjust->isChecked()) {
        ui->label_2->setPixmap(QPixmap::fromImage(Mat2QImage(m_Vision->m_FrameOriginal)));
        ui->label_3->setPixmap(QPixmap::fromImage(Mat2QImage(m_Vision->m_FrameOriginal)));
    }
}

void MainWindow::on_rBtnGame_clicked() //Game
{
    ui->stackedWidget->setCurrentIndex(4);

    while (m_Vision->captureImage() && ui->rBtnGame->isChecked()) {
        ui->label_2->setPixmap(QPixmap::fromImage(Mat2QImage(m_Vision->m_FrameOriginal)));
        ui->label_3->setPixmap(QPixmap::fromImage(Mat2QImage(m_Vision->m_FrameOriginal)));
    }
}

void MainWindow::on_buttonScanDevices_clicked()
{
    QStringList stringList = serial->scan();
    ui->comboBoxdevicesgame->clear();
    ui->comboBoxdevicesgame->addItems(stringList);
}

void MainWindow::on_buttonGo_clicked()
{
    m_Vision->getData(fs);
}

void MainWindow::on_horizontalSliderBrightness_valueChanged(int value)
{
    char Brightness[70];
    sprintf(Brightness, "uvcdynctrl --device=/dev/video%d --set='Brightness' %d", m_Vision->m_IdCamera, value);
    system(Brightness);
}

void MainWindow::on_horizontalSliderSaturation_valueChanged(int value)
{
    char Saturation[70];
    sprintf(Saturation, "uvcdynctrl --device=/dev/video%d --set='Saturation' %d", m_Vision->m_IdCamera, value);
    system(Saturation);
}

void MainWindow::on_horizontalSliderContrast_valueChanged(int value)
{
    char Contrast[70];
    sprintf(Contrast, "uvcdynctrl --device=/dev/video%d --set='Contrast' %d", m_Vision->m_IdCamera, value);
    system(Contrast);
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    m_Vision->m_IdCamera = index;
    m_Vision->closeCapture();
}

void MainWindow::on_pushButton_10_clicked()
{
    saveRectification(m_Vision->aImg);
}

void MainWindow::on_pushButton_9_clicked()
{
    loadRectification(m_Vision->aImg);
}

void MainWindow::on_pushButtonSaveField_clicked()
{
    saveFieldState(cvPoint(0,0), cvPoint(0,0), cvPoint(0,0), cvPoint(0,0), cvPoint(0,0));
}

void MainWindow::on_pushButtonLoadField_clicked()
{
    loadFieldstate(fs);
}

void MainWindow::on_pushButtonAutoRect_clicked()
{
    m_Vision->autoRetification();
}
