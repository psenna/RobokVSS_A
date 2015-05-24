#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/opencv.hpp>

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

private slots:

    void on_buttonSaveCalib_clicked();

    void on_buttonLoadCalib_clicked();

    int updateSliders(int id);

    void on_rBtnSettings_clicked();

    void on_rBtnRectifyImage_clicked();

    void on_rBtnCalibrate_clicked();

    void on_rBtnFieldAdjust_clicked();

    void on_rBtnGame_clicked();

private:
    Ui::MainWindow *ui;
    cv::Mat *m_Display1;
    cv::Mat *m_Display2;
};

#endif // MAINWINDOW_H
