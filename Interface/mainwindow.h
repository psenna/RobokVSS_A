#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/opencv.hpp>

using cv::Mat;

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
    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

    void on_radioButton_3_clicked();

    void on_radioButton_4_clicked();

    void on_radioButton_5_clicked();

    void on_buttonSaveCalib_clicked();

    void on_buttonLoadCalib_clicked();

    int atualizaSliders(int id);

private:
    Ui::MainWindow *ui;
    Mat *display1;
    Mat *display2;
};

#endif // MAINWINDOW_H
