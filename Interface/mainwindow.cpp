#include "Interface/mainwindow.h"
#include "ui_mainwindow.h"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "read_write.h"
#include "Communitacion/serial.h"
#include <vector>
#include <QMouseEvent>
#include <QKeyEvent>
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

    fs = new Fieldstate();
    m_isPlaying = false;
    QApplication::quit();

    m_Vision = Vision::getInstance();

    m_Display1 = &m_Vision->m_FrameOriginal;

    m_Vision->m_DisplaySize = Size(ui->label_2->width()-1,ui->label_2->height()-1);
    m_Vision->setCameraId(0);
    m_IdCalib = -1;
    callLoadCalibration();//carrega calibragem
    on_pushButtonLoadRect_clicked();//carrega retificacao
    on_rBtnSettings_clicked();//Settings = menu inicial

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

void MainWindow::setOriginalFrameBorderPoints(){ //seta bordas do Frame Original para a retificacao
    // The 4 points where the rectify mapping is to be done , from top-left in clockwise order
    m_Vision->m_OriginalBorderPoints[0] = cv::Point2f(0,0);
    m_Vision->m_OriginalBorderPoints[1] = cv::Point2f(0, m_Vision->m_DisplaySize.height-1);
    m_Vision->m_OriginalBorderPoints[2] = cv::Point2f(m_Vision->m_DisplaySize.width-1, m_Vision->m_DisplaySize.height-1);
    m_Vision->m_OriginalBorderPoints[3] = cv::Point2f(m_Vision->m_DisplaySize.width-1, 0);
}

void MainWindow::callLoadCalibration(){  //Adaptador para chamar o Load do read_write.h

    CvScalar a[10], b[10];//tornar o tamanho deste vetor algo mais dinamico
    loadCalibration(a, b);
    for(int i=0; i<10; i++){//para nao ter que ficar mudando toda hora aqui
        m_Vision->setMinMax(a[i], b[i], i);
    }
    updateSlidersAndID();
}

void MainWindow::updateSlidersAndID() { //Atualiza as Sliders da calibragem dependendo do objeto selecionado

    int newId;

    if(ui->radioButton_6->isChecked()) newId = 0; //cor do time nosso
    if(ui->radioButton_7->isChecked()) newId = 4; //cor do time adversario
    if(ui->radioButton_8->isChecked()) newId = 1; // V
    if(ui->radioButton_9->isChecked()) newId = 2; // V
    if(ui->radioButton_10->isChecked()) newId = 3; //robos nossos
    if(ui->radioButton_11->isChecked()) newId = 5; // V
    if(ui->radioButton_12->isChecked()) newId = 6; // V
    if(ui->radioButton_13->isChecked()) newId = 7; //robos adversarios
    if(ui->radioButtonBall->isChecked()) newId = 8; //bola
    if(ui->radioButtonBorda->isChecked()) newId = 9; //bordas do campo

    if (newId != m_IdCalib){ // atualiza somente se mudar a opcao de objeto
        ui->horizontalSliderh1->setValue(m_Vision->getMin()[newId].val[0]);
        ui->horizontalSliderh2->setValue(m_Vision->getMax()[newId].val[0]);
        ui->horizontalSliders1->setValue(m_Vision->getMin()[newId].val[1]);
        ui->horizontalSliders2->setValue(m_Vision->getMax()[newId].val[1]);
        ui->horizontalSliderv1->setValue(m_Vision->getMin()[newId].val[2]);
        ui->horizontalSliderv2->setValue(m_Vision->getMax()[newId].val[2]);
        m_IdCalib = newId;
    }
}

void MainWindow::mousePressEvent(QMouseEvent* ev) //Eventos de mouse na ui
{
    if (ui->rBtnCalibrate->isChecked()) {//Calibrate

        QPoint P = ui->label_2->mapFrom(this, ev->pos());
        if(P.x() < 0 || P.y() < 0 || P.x()>ui->label_2->width()-1 || P.y()>ui->label_2->height()-1) return; //ignore se clique for fora da label
        cv::Vec3b vec = Vision::getInstance()->m_FrameHSV.at<cv::Vec3b>(cv::Point(P.x(),P.y()));

        if (ev->button() & Qt::LeftButton) { //setHsvInterval
            if(vec[0]-15 < 0) vec[0] = 15; if(vec[0]+15 > 180) vec[0] = 165;
            if(vec[1]-15 < 0) vec[1] = 15; if(vec[1]+15 > 255) vec[1] = 240;
            if(vec[2]-15 < 0) vec[2] = 15; if(vec[2]+15 > 255) vec[2] = 240;

            ui->horizontalSliderh1->setValue(vec[0]-15); ui->horizontalSliderh2->setValue(vec[0]+15);
            ui->horizontalSliders1->setValue(vec[1]-15); ui->horizontalSliders2->setValue(vec[1]+15);
            ui->horizontalSliderv1->setValue(vec[2]-15); ui->horizontalSliderv2->setValue(vec[2]+15);

            Vision::getInstance()->setMinMax(cvScalar(vec[0]-15, vec[1]-15, vec[2]-15), cvScalar(vec[0]+15, vec[1]+15, vec[2]+15), m_IdCalib);
        }
        else if (ev->button() & Qt::RightButton) { //addHSVInterval
            int h1 = m_Vision->getMin()[m_IdCalib].val[0];
            int h2 = m_Vision->getMax()[m_IdCalib].val[0];
            int s1 = m_Vision->getMin()[m_IdCalib].val[1];
            int s2 = m_Vision->getMax()[m_IdCalib].val[1];
            int v1 = m_Vision->getMin()[m_IdCalib].val[2];
            int v2 = m_Vision->getMax()[m_IdCalib].val[2];

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

            Vision::getInstance()->setMinMax(cvScalar(h1, s1, v1), cvScalar(h2, s2, v2), m_IdCalib);
        }
    }
    else if (ui->rBtnRectifyImage->isChecked()){ //Rectify
        QPoint P = ui->label_2->mapFrom(this, ev->pos());
        if(P.x() < 0 || P.y() < 0 || P.x()>ui->label_2->width()-1 || P.y()>ui->label_2->height()-1) return; //ignore se clique for fora da label
        m_Display1 = &m_Vision->m_FrameRect;
        switch (ui->comboBoxRectfy->currentIndex()) {//setBordasRectify
        case 0://A
            m_Vision->m_RectificationBorderPoints[0].x = (float) P.x();
            m_Vision->m_RectificationBorderPoints[0].y = (float) P.y();
            break;
        case 1://B
            m_Vision->m_RectificationBorderPoints[1].x = (float) P.x();
            m_Vision->m_RectificationBorderPoints[1].y = (float) P.y();
            break;
        case 2://C
            m_Vision->m_RectificationBorderPoints[2].x = (float) P.x();
            m_Vision->m_RectificationBorderPoints[2].y = (float) P.y();
            break;
        case 3://D
            m_Vision->m_RectificationBorderPoints[3].x = (float) P.x();
            m_Vision->m_RectificationBorderPoints[3].y = (float) P.y();
            break;
        default:
            break;
        }
    }else if(ui->rBtnFieldAdjust->isChecked()){
        QPoint P = ui->label_2->mapFrom(this, ev->pos());
        if(P.x() < 0 || P.y() < 0 || P.x()>ui->label_2->width()-1 || P.y()>ui->label_2->height()-1) return;   //ignore se clique for fora da label

        switch (ui->comboBoxFieldAdjust->currentIndex()) {//setBordasRectify
        case 0://A
            m_Goal1.GoalPost_1.x = (float) P.x();
            m_Goal1.GoalPost_1.y = (float) P.y();
            break;
        case 1://B
            m_Goal1.GoalPost_2.x = (float) P.x();
            m_Goal1.GoalPost_2.y = (float) P.y();
            break;
        case 2://C
            //m_Vision->bordasRectify[2].x = (float) P.x();
            //m_Vision->bordasRectify[2].y = (float) P.y();
            break;
        case 3://D
            m_Goal2.GoalPost_1.x = (float) P.x();
            m_Goal2.GoalPost_1.y = (float) P.y();
            break;
        case 4://E
            m_Goal2.GoalPost_2.x = (float) P.x();
            m_Goal2.GoalPost_2.y = (float) P.y();
            break;
        default:
            break;
        }
        fs->setGoalLeft(m_Goal1);
        fs->setGoalRight(m_Goal2);
    }
}

void MainWindow::keyPressEvent(QKeyEvent * ev){ //Eventos de teclado na ui
    if (ui->rBtnRectifyImage->isChecked()){//Rectify
        if(ev->key()>=Qt::Key_A && ev->key()<=Qt::Key_D)//ABCD
            ui->comboBoxRectfy->setCurrentIndex(ev->key()-Qt::Key_A);
    }
    if (ui->rBtnFieldAdjust->isChecked()){//Rectify
        if(ev->key()>=Qt::Key_A && ev->key()<=Qt::Key_E)//ABCDE
            ui->comboBoxFieldAdjust->setCurrentIndex(ev->key()-Qt::Key_E);
    }
}

void MainWindow::showRectify(){//mostra os pontos da retificacao no frame original
    cv::circle(m_Vision->m_FrameOriginal,
               cv::Point(m_Vision->m_RectificationBorderPoints[0].x, m_Vision->m_RectificationBorderPoints[0].y),
               5, // Raio
               cvScalar(255,0,0)); // Desenha circulo no ponto A

    cv::line(m_Vision->m_FrameOriginal,
             cv::Point(m_Vision->m_RectificationBorderPoints[0].x, m_Vision->m_RectificationBorderPoints[0].y),
             cv::Point(m_Vision->m_RectificationBorderPoints[1].x, m_Vision->m_RectificationBorderPoints[1].y),
             cv::Scalar(0,255,0)); // Desenha a linha do ponto A para o ponto B

    cv::circle(m_Vision->m_FrameOriginal,
               cv::Point(m_Vision->m_RectificationBorderPoints[1].x, m_Vision->m_RectificationBorderPoints[1].y),
               5,
               cvScalar(0,255,0)); // Desenha circulo no ponto B

    cv::line(m_Vision->m_FrameOriginal,
             cv::Point(m_Vision->m_RectificationBorderPoints[1].x, m_Vision->m_RectificationBorderPoints[1].y),
             cv::Point(m_Vision->m_RectificationBorderPoints[2].x, m_Vision->m_RectificationBorderPoints[2].y),
             cv::Scalar(255,0,255)); // Desenha a linha do ponto B para o ponto C

    cv::circle(m_Vision->m_FrameOriginal,
               cv::Point(m_Vision->m_RectificationBorderPoints[2].x,
               m_Vision->m_RectificationBorderPoints[2].y),
               5, // Raio
               cvScalar(0,0,255)); // Desenha circulo no ponto C

    cv::line(m_Vision->m_FrameOriginal,
             cv::Point(m_Vision->m_RectificationBorderPoints[2].x, m_Vision->m_RectificationBorderPoints[2].y),
             cv::Point(m_Vision->m_RectificationBorderPoints[3].x, m_Vision->m_RectificationBorderPoints[3].y),
             cv::Scalar(0,0,255)); // Desenha a linha do ponto C para o ponto D

    cv::circle(m_Vision->m_FrameOriginal,
               cv::Point(m_Vision->m_RectificationBorderPoints[3].x, m_Vision->m_RectificationBorderPoints[3].y),
               5, // Raio
               cvScalar(255,0,255)); // Desenha circulo no ponto D

    cv::line(m_Vision->m_FrameOriginal,
             cv::Point(m_Vision->m_RectificationBorderPoints[3].x, m_Vision->m_RectificationBorderPoints[3].y),
             cv::Point(m_Vision->m_RectificationBorderPoints[0].x, m_Vision->m_RectificationBorderPoints[0].y),
             cv::Scalar(255,0,0)); // Desenha a linha do ponto D para o ponto A
}

void MainWindow::showFieldAdjust(){//mostra os pontos dos centros dos postes dos gols na secao de ajuste de campo
    cv::circle(*m_Display1,
               cv::Point(fs->getGoalLeft().GoalPost_1.x, fs->getGoalLeft().GoalPost_1.y),
               5,
               cvScalar(0,0,255)); // Desenha circulo na trave A

    cv::circle(*m_Display1,
               cv::Point(fs->getGoalLeft().GoalPost_2.x, fs->getGoalLeft().GoalPost_2.y),
               5,
               cvScalar(0,0,255)); // Desenha circulo na trave B

    cv::circle(*m_Display1,
               cv::Point(fs->getGoalRight().GoalPost_1.x, fs->getGoalRight().GoalPost_1.y),
               5,
               cvScalar(0,0,255)); // Desenha circulo na trave D

    cv::circle(*m_Display1,
               cv::Point(fs->getGoalRight().GoalPost_2.x, fs->getGoalRight().GoalPost_2.y),
               5,
               cvScalar(0,0,255)); // Desenha circulo na trave E
}

void MainWindow::showGame(){//mostra os pontos do centro dos robos
    for (int i = 0; i < 3; ++i) {
        cv::circle(*m_Display1,
                   cv::Point(fs->getRobotTeamById(i).getPosition().x,
                             fs->getRobotTeamById(i).getPosition().y),
                             10,
                             cvScalar(0,255,0),
                             2 // Expessura da linha
                   );
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
    setOriginalFrameBorderPoints();//Chame esta funcao pelo menos uma vez no comeco da execucao do programa

    ui->label_3->clear();
    while (m_Vision->captureImage() && ui->rBtnSettings->isChecked())
    {
        m_Vision->adjustImage();
        ui->label_2->setPixmap(QPixmap::fromImage(Mat2QImage(*m_Display1)));
    }
}

void MainWindow::on_rBtnRectifyImage_clicked() //Rectify Image
{
    ui->stackedWidget->setCurrentIndex(1);

    while(m_Vision->captureImage() && ui->rBtnRectifyImage->isChecked()){
        m_Vision->adjustImage();
        showRectify();
        ui->label_2->setPixmap(QPixmap::fromImage(Mat2QImage(m_Vision->m_FrameOriginal)));
        ui->label_3->setPixmap(QPixmap::fromImage(Mat2QImage(*m_Display1)));
    }
}

void MainWindow::on_rBtnCalibrate_clicked() //Calibrate
{
    ui->stackedWidget->setCurrentIndex(2);
    ui->radioButton_6->setChecked(true);
    m_IdCalib = 0;
    int h1, h2, s1, s2, v1, v2;

    updateSlidersAndID();

    while(ui->rBtnCalibrate->isChecked()){
        m_Vision->adjustImage();
        updateSlidersAndID();
        m_Vision->calibrate(m_IdCalib, m_Display1);
        ui->label_2->setPixmap(QPixmap::fromImage(Mat2QImage(*m_Display1)));
        ui->label_3->setPixmap(QPixmap::fromImage(Mat2QImage(m_Vision->m_FrameBinary)));

        h1 = ui->horizontalSliderh1->value();
        h2 = ui->horizontalSliderh2->value();
        s1 = ui->horizontalSliders1->value();
        s2 = ui->horizontalSliders2->value();
        v1 = ui->horizontalSliderv1->value();
        v2 = ui->horizontalSliderv2->value();

        m_Vision->setMinMax(cvScalar(h1, s1, v1), cvScalar(h2, s2, v2), m_IdCalib);
    }
}

void MainWindow::on_rBtnFieldAdjust_clicked() //Field Adjust
{
    ui->stackedWidget->setCurrentIndex(3);

    while (m_Vision->captureImage() && ui->rBtnFieldAdjust->isChecked()) {
        m_Vision->adjustImage();
        showFieldAdjust(); // Desenhar pontos de ajuste na tela
        ui->label_2->setPixmap(QPixmap::fromImage(Mat2QImage(*m_Display1)));
        ui->label_3->setPixmap(QPixmap::fromImage(Mat2QImage(m_Vision->m_FrameOriginal)));
    }
}

void MainWindow::on_rBtnGame_clicked() //Game
{
    ui->stackedWidget->setCurrentIndex(4);

    while (m_Vision->captureImage() && ui->rBtnGame->isChecked()) {
        m_Vision->adjustImage();
        if(m_isPlaying){
            m_Vision->getData(fs);
            Default d;
            d.play(fs);
            showGame();
        }
        ui->label_2->setPixmap(QPixmap::fromImage(Mat2QImage(*m_Display1)));
        ui->label_3->setPixmap(QPixmap::fromImage(Mat2QImage(*m_Display1)));//poderia ser um display com status dos robos(objetivo, direcao, etc)
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
    m_isPlaying = !m_isPlaying;
    if(m_isPlaying)
        ui->buttonGo->setText("Stop");
    else
        ui->buttonGo->setText("GO!");
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

void MainWindow::on_pushButtonSaveRect_clicked()
{
    saveRectification(m_Vision->convertBorders(m_Vision->m_RectificationBorderPoints));
}

void MainWindow::on_pushButtonLoadRect_clicked()
{
    std::vector<float> aImg(8);
    if(loadRectification(&aImg)){
        m_Display1 = &m_Vision->m_FrameRect;//carrega retificacao
        m_Vision->setRectificationsParam(aImg);
    }
}

void MainWindow::on_pushButtonSaveField_clicked()
{
    saveFieldState(cvPoint(0,0), cvPoint(0,0), cvPoint(0,0), cvPoint(0,0), cvPoint(0,0));
}

void MainWindow::on_pushButtonLoadField_clicked()
{
    loadFieldstate(fs);
}

void MainWindow::on_pushButtonRectReset_clicked()
{
    m_Display1 = &m_Vision->m_FrameOriginal;
    m_Vision->setRectificationsParam(m_Vision->convertBorders(m_Vision->m_OriginalBorderPoints));
}

void MainWindow::on_pushButtonAutoRect_clicked()
{
    on_pushButtonRectReset_clicked();
    m_Vision->autoRectificationSet();
    m_Display1 = &m_Vision->m_FrameRect;
}
