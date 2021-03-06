#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/opencv.hpp>
#include "Communitacion/serial.h"
#include "fieldstate.h"
#include "Strategy/default.h"
#include "Strategy/doubleattack.h"
#include "Vision/vision.h"
#include "motion.h"
#include <QtSerialPort/QtSerialPort>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void mousePressEvent( QMouseEvent* ev);
    void keyPressEvent(QKeyEvent * ev);
    void closeEvent(QCloseEvent *event);
    void callLoadCalibration();

private:
    Ui::MainWindow *ui;
    cv::Mat *m_Display1;
    cv::Mat *m_Display2;
    Vision *m_Vision;
    Serial serial;
    Fieldstate *fs;
    bool m_isPlaying;
    int m_IdCalib;
    Goal m_Goal1, m_Goal2;

    void showRectify();
    void showFieldAdjust();
    void showGame();
    void setOriginalFrameBorderPoints();
    void updateSlidersAndID();

private slots:

    void on_buttonSaveCalib_clicked();
    void on_buttonLoadCalib_clicked();
    void on_rBtnSettings_clicked();
    void on_rBtnRectifyImage_clicked();
    void on_rBtnCalibrate_clicked();
    void on_rBtnFieldAdjust_clicked();
    void on_rBtnGame_clicked();
    void on_buttonScanDevices_clicked();
    void on_buttonGo_clicked();
    void on_horizontalSliderBrightness_valueChanged(int value);
    void on_horizontalSliderSaturation_valueChanged(int value);
    void on_horizontalSliderContrast_valueChanged(int value);
    void on_comboBox_currentIndexChanged(int index);
    void on_pushButtonSaveField_clicked();
    void on_pushButtonLoadField_clicked();
    void on_pushButtonRectReset_clicked();
    void on_pushButtonAutoRect_clicked();
    void on_pushButtonLoadRect_clicked();
    void on_pushButtonSaveRect_clicked();

    void on_buttonConnectDevice_clicked();
    void on_ButtonPowerOff_clicked();
};

#endif // MAINWINDOW_H
