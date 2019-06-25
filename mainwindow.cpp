#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QApplication>
#include<QtWidgets>
#include<QPushButton>
#include<QMessageBox>
#include<QLabel>
#include<QFileDialog>
#include<QIcon>
#include<QString>
#include<QPixmap>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
using namespace cv;
using namespace std;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //set a background
   ui->image->setStyleSheet("background-color: black;");
   ui->output_image->setStyleSheet("background-color: black;");
   ui->threshold_input->setStyleSheet("background-color: white;");
   ui->threshold_type_in->setStyleSheet("background-color: white;");
   ui->kernel_size->setStyleSheet("background-color: white;");
   ui->sigma->setStyleSheet("background-color: white;");
   ui->median_size_input->setStyleSheet("background-color: white;");
   ui->canny_thres_input->setStyleSheet("background-color:white");
   //setting color for buttons
   //->median_button->setStyleSheet("background-color: blue;");
    //set icon for the buttons
   //ui->exit->setIcon(QIcon(QCoreApplication::applicationDirPath() + "/exit.png"));
   ui->open->setIcon(QIcon(QCoreApplication::applicationDirPath() + "/open.png"));
   ui->save_image->setIcon(QIcon(QCoreApplication::applicationDirPath() + "/save.png"));

  // ui->open->setIcon(QIcon(QCoreApplication::applicationDirPath() + "/open.png"));
   //background color of the mainwindow and a size
   this->setStyleSheet("background-color: grey;");
   this->setFixedSize(1000,700);
   //making connections between signls and slots
   //setting tool tips on the QWidjets objects
   ui->type->setToolTip("0: Binary\n "
                        "1: Binary Inverted\n"
                        "2: Threshold Truncated\n "
                        "3: Threshold to Zero \n"
                        "4: Threshold to Zero Inverted");
   ui->threshhold->setToolTip("Type the threshlod value \n"
                               "    from 0 to 255");
   ui->kernel_size->setToolTip("Enter the kenel size\n"
                                "The kernel size must be odd.");
   ui->sigma->setToolTip("Type the sigma value for filtering ");


}

MainWindow::~MainWindow()
{
    delete ui;
}
    //open an image file usig the open QPushButon
void MainWindow::on_open_clicked()
{
    QString fichier = QFileDialog::getOpenFileName(this, "Ouvrir un"
                                               " fichier", QString(), "Images (*.png *.gif *.jpg *.jpeg)");

    //QPixmap my_image(fichier);
    ui->image->setScaledContents( true );
    ui->image->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
    ui->output_image->setScaledContents( true );
    ui->output_image->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );

    //ui->image->setPixmap(my_image);
    ui->path->setText(fichier);
    const char* opencvfichier= fichier.toStdString().c_str();
    mat_input =imread(opencvfichier);
    cvtColor(mat_input, mat_input, CV_BGR2RGB );
    QImage q_output((uchar*)mat_input.data,mat_input.cols,mat_input.rows,mat_input.step,QImage::Format_RGB888);
   ui->image->setPixmap(QPixmap::fromImage(q_output));



}
//close the main indow
void MainWindow::on_exit_clicked()
   {
   this->close();
   }



////////////////////////the canny edge detection qPushButton//////////////////////////////////////////
void MainWindow::on_pushButton_clicked()
   {
    ui->image->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );

    Mat gray,dst;
    int canny_threshold=100;
    int const max_lowThreshold = 100;
    int ratio = 3;
    int kernel_size = 3;
   canny_threshold =ui->canny_thres_input->text().toInt();


    if(!ui->image->pixmap())
    {
        QMessageBox::warning(this, tr("No image"), tr("Please select image."), QMessageBox::Ok);
        return;
    }
    if (canny_threshold > max_lowThreshold|| canny_threshold< 0)
    {
            QMessageBox::warning(this, tr("Incorrect threshold value size"),
                                 tr("Please choose a value between 0 and 100"), QMessageBox::Ok);
            return;}





        cvtColor(mat_input, gray, CV_BGR2GRAY);
        Canny( gray, dst, canny_threshold,canny_threshold*ratio, kernel_size );

        QImage q_output((uchar*)dst.data,dst.cols,dst.rows,dst.step,QImage::Format_Grayscale8);
         ui->output_image->setPixmap(QPixmap::fromImage(q_output));


   }
////////////////////implementing the save image button///////////////////////////////////////

void MainWindow::on_save_image_clicked()
{

// check if there output image label is not empty
    if(!ui->output_image->pixmap())
    {
        QMessageBox::warning(this, tr("No image to be saved"), tr("Please select an image "), QMessageBox::Ok);
        return;
    }

    QString imagePath = QFileDialog::getSaveFileName(
                    this,
                    tr("Save File"),
                    "image",
                    tr("JPEG (*.jpg *.jpeg);;PNG (*.png)" )
                    );
        QFile my_file(imagePath);
           if(!my_file.open(QIODevice::WriteOnly))
                 return;

           QString img_format=  QFileInfo(imagePath).suffix();
           ui->output_image->pixmap()->save(imagePath, img_format.toLatin1());


}
//////////////////implementing the filter when clicking the filter qpushbutton filter///////

void MainWindow::on_filter_clicked(){
    int MAX_KERNEL_LENGTH = 31;
    int sigma_value;
   int kernel_length ;
   if(!ui->image->pixmap())
   {
       QMessageBox::warning(this, tr("No image"), tr("Please select image."), QMessageBox::Ok);
       return;
   }
     kernel_length = ui->kernel_size->text().toInt();
      sigma_value= ui->sigma->text().toInt();
   if(ui->kernel_size->text().isEmpty() || ui->sigma->text().isEmpty())
    {
        kernel_length=5;
        sigma_value=0;
    }
  if (kernel_length % 2 == 0)
   {
           QMessageBox::warning(this, tr("Incorrect kernel size"),
                                tr("The kernel size must be odd number "), QMessageBox::Ok);
           return;}
    if (kernel_length > MAX_KERNEL_LENGTH || kernel_length< 0)
    {
            QMessageBox::warning(this, tr("Incorrect kernel size"),
                                 tr("Please choose the size between 0 and 31"), QMessageBox::Ok);
            return;}

   Mat dst;
    GaussianBlur( mat_input, dst, Size( kernel_length, kernel_length ), sigma_value, sigma_value );
    cvtColor(dst, dst, CV_BGR2RGB );
    QImage q_output((uchar*)dst.data,dst.cols,dst.rows,dst.step,QImage::Format_RGB888);
   ui->output_image->setPixmap(QPixmap::fromImage(q_output));


}
///////////////////////convert the input image to gray level /////////////////////////////////////////////////////
void MainWindow::on_grey_level_clicked()
{
    if(!ui->image->pixmap())
    {
        QMessageBox::warning(this, tr("No image"), tr("Please select image."), QMessageBox::Ok);
        return;
    }
    //cvtColor(mat_input, mat_input, CV_BGR2RGB );
     cvtColor(mat_input, mat_output, CV_BGR2GRAY );
     QImage q_output((uchar*)mat_output.data,mat_output.cols,mat_output.rows,mat_output.step,QImage::Format_Grayscale8);
    ui->output_image->setPixmap(QPixmap::fromImage(q_output));
}

void MainWindow::on_threshold_clicked()
{

       /* 0: Binary
         1: Binary Inverted
         2: Threshold Truncated
         3: Threshold to Zero
         4: Threshold to Zero Inverted
       */
    int threshold_value ;
    int threshold_type;
    int const max_value = 255;
    int const max_type = 4;
    int const max_BINARY_value = 255;
    //initializing the threshold value;
    threshold_value = ui->threshold_input->text().toInt();
    if(ui->threshold_input->text().isEmpty() || ui->threshold_type_in->text().isEmpty())
    {
       ui->threshold_input->setText("60");
        ui->threshold_type_in->setText("0");
    }
        if (threshold_value > max_value || threshold_value < 0)
        {
            QMessageBox::warning(this, tr("Incorrect threshold value"),
                                 tr("Please choose threshold between 0 and 255"), QMessageBox::Ok);
            return;
        }

        if(!ui->image->pixmap())
        {
            QMessageBox::warning(this, tr("No image"), tr("Please select image."), QMessageBox::Ok);
            return;
        }
        //setting the threshold type
        threshold_type = ui->threshold_type_in->text().toInt();
            if (threshold_type > max_type  || threshold_type < 0)
            {
                QMessageBox::warning(this, tr("Incorrect type value"),
                                     tr("Please choose threshold type between 0 and 4"), QMessageBox::Ok);
                return;
            }

            if(!ui->image->pixmap())
            {
                QMessageBox::warning(this, tr("No image"), tr("Please select image."), QMessageBox::Ok);
                return;
            }
    Mat dst;
     cvtColor(mat_input, mat_output, CV_BGR2GRAY);
     threshold( mat_output, dst, threshold_value, max_BINARY_value,threshold_type );
     QImage q_output((uchar*)dst.data,dst.cols,dst.rows,dst.step,QImage::Format_Grayscale8);
    ui->output_image->setPixmap(QPixmap::fromImage(q_output));
}

void MainWindow::on_countours_clicked()
{
    if(!ui->image->pixmap())
    {
        QMessageBox::warning(this, tr("No image"), tr("Please select image."), QMessageBox::Ok);
        return;
    }
   Mat src_gray;
    int thresh = 100;
    //int max_thresh = 255;
    RNG rng(12345);


      /// Convert image to gray and blur it
      cvtColor( mat_input, src_gray, CV_BGR2GRAY );
      blur( src_gray, src_gray, Size(3,3) );


      Mat canny_output;
      vector<vector<Point> > contours;
      vector<Vec4i> hierarchy;

      /// Detect edges using canny
      Canny( src_gray, canny_output, thresh, thresh*2, 3 );
      /// Find contours
      findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

      /// Draw contours
      Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
      for( int i = 0; i< contours.size(); i++ )
         {
           Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
           drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
         }

      /// Show in a window drawing
      Mat dst;
      cvtColor(drawing, dst, CV_BGR2RGB);
      QImage q_output((uchar*)dst.data,dst.cols,dst.rows,dst.step,QImage::Format_RGB888);
     ui->output_image->setPixmap(QPixmap::fromImage(q_output));

    }


void MainWindow::on_median_filter_label_clicked()
{
   // medianBlur ( src, dst, i );

}

void MainWindow::on_median_button_clicked()
{
    int MAX_KERNEL_LENGTH = 31;
   int median_length ;
   if(!ui->image->pixmap())
   {
       QMessageBox::warning(this, tr("No image"), tr("Please select image."), QMessageBox::Ok);
       return;
   }
    median_length = ui->median_size_input->text().toInt();
  if (median_length% 2 == 0)
   {
           QMessageBox::warning(this, tr("Incorrect kernel size"),
                                tr("The kernel size must be odd number "), QMessageBox::Ok);
           return;}
    if (median_length > MAX_KERNEL_LENGTH || median_length<= 0)
    {
            QMessageBox::warning(this, tr("Incorrect kernel size"),
                                 tr("Please choose the size between 1 and 31"), QMessageBox::Ok);
            return;}

   Mat dst;
    medianBlur ( mat_input, dst, median_length );
    cvtColor(dst, dst, CV_BGR2RGB );
    QImage q_output((uchar*)dst.data,dst.cols,dst.rows,dst.step,QImage::Format_RGB888);
   ui->output_image->setPixmap(QPixmap::fromImage(q_output));
}

void MainWindow::on_hystogram_button_clicked()
{

    if(!ui->image->pixmap())
    {
        QMessageBox::warning(this, tr("No image"), tr("Please select image."), QMessageBox::Ok);
        return;
    }

      /// Separate the image in 3 places ( B, G and R )
      vector<Mat> bgr_planes;
      split( mat_input, bgr_planes );

      /// Establish the number of bins
      int histSize = 256;

      /// Set the ranges ( for B,G,R) )
      float range[] = { 0, 256 } ;
      const float* histRange = { range };

      bool uniform = true; bool accumulate = false;

      Mat b_hist, g_hist, r_hist;

      /// Compute the histograms:
      calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
      calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
      calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );

      // Draw the histograms for B, G and R
      int hist_w = 512; int hist_h = 400;
      int bin_w = cvRound( (double) hist_w/histSize );

      Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );

      /// Normalize the result to [ 0, histImage.rows ]
      normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
      normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
      normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );

      /// Draw for each channel
      for( int i = 1; i < histSize; i++ )
      {
          line( histImage, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ) ,
                           Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
                           Scalar( 255, 0, 0), 2, 8, 0  );
          line( histImage, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ) ,
                           Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
                           Scalar( 0, 255, 0), 2, 8, 0  );
          line( histImage, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ) ,
                           Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
                           Scalar( 0, 0, 255), 2, 8, 0  );
      }

      /// Display
      imshow("hysogram", histImage );




    }

/////////////implementyion the zoom_out QPushbutton/////////////////////

///////////////////implementing the hystogram equalizer ////////////////////////////
void MainWindow::on_pushButton_2_clicked()
{
    if(!ui->image->pixmap())
    {
        QMessageBox::warning(this, tr("No image"), tr("Please select image."), QMessageBox::Ok);
        return;
    }

    Mat dst;
    Mat src=mat_input;
    /// Convert to grayscale
    cvtColor( src, src, CV_BGR2GRAY );
    /// Apply Histogram Equalization
    equalizeHist( src, dst );
    QImage q_output((uchar*)dst.data,dst.cols,dst.rows,dst.step,QImage::Format_Grayscale8);
   ui->output_image->setPixmap(QPixmap::fromImage(q_output));
}

////////////////////adding images////////////////////////////////
void MainWindow::on_pushButton_3_clicked()
{

    Mat dst;

    double alpha = 0.5;
    double beta = ( 1.0 - alpha );
   // addWeighted( mat_input, alpha, add_input, beta, 0.0, dst);
    QString fichier = QFileDialog::getOpenFileName(this, "Ouvrir un"
                                               " fichier", QString(), "Images (*.png *.gif *.jpg *.jpeg)");

    /*const char* opencvfichier= fichier.toStdString().c_str();
    Mat add_input =imread(opencvfichier);
    if(dst.size()!=mat_input.size()|| dst.channels() != mat_input.channels())
    {
        QMessageBox::warning(this, tr("The two images "),
                                    tr("have not the same size. "), QMessageBox::Ok);


    }
    addWeighted( mat_input, alpha, add_input, beta, 0.0, dst);
    cvtColor(dst, dst, CV_BGR2RGB );
    QImage q_output((uchar*)dst.data,dst.cols,dst.rows,dst.step,QImage::Format_RGB888);
   ui->output_image->setPixmap(QPixmap::fromImage(q_output));*/


}
