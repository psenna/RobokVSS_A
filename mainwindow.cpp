#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "vision.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Vision vision;
    Fieldstate *fs = NULL;

    while (true)
    {
        vision.setCameraId(0);
        vision.getData(fs);
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}
