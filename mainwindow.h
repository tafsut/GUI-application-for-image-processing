#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QtWidgets>
#include<QPixmap>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include<opencv2/core/core.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/video/video.hpp>
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <string>
using namespace cv;
using namespace std;


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_open_clicked();

    void on_exit_clicked();



    void on_pushButton_clicked();

    void on_filter_clicked();

    void on_grey_level_clicked();

    void on_threshold_clicked();

    void on_countours_clicked();
    void threshold_slider(int value);

    void on_threshold_slider_valueChanged(int value);

    void on_save_image_clicked();

    void on_median_filter_label_clicked();

    void on_median_button_clicked();

    void on_hystogram_button_clicked();



    void on_pushButton_2_clicked();


    void on_pushButton_3_clicked();

public slots:
    void mouseHandler(int event, int x, int y, int, void*);

private:
    Ui::MainWindow *ui;
    QString fichier;
    QSlider*slider;
    QImage img;
   // QImage img1;
    Mat mat_input;
    Mat mat_output;
    QImage q_input;
    QImage q_output;

    /////////////////////
    Mat dst;
    QString fichierAdd;

};


#endif // MAINWINDOW_H
