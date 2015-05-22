#include "Interface/mainwindow.h"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <QApplication>
#include <pthread.h>
#include <stdio.h>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    pthread_t display_thread;
    if(pthread_create(&display_thread, NULL, &MainWindow::display_helper, &w)) {
        printf("Erro");
    }

   return a.exec();
}
