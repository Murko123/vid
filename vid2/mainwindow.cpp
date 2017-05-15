#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


        ui->label_org->setScaledContents( true );
        ui->label_tran->setScaledContents(true);


        min=-100.00;
        max=100.00;
    }

    MainWindow::~MainWindow()
    {
        delete ui;
    }


    void MainWindow::on_ButtonTrans_clicked()
    {


            usermin = ui->lineEdit_MIN->text().toDouble();
            usermax = ui->lineEdit_MAX->text().toDouble();

                    if(usermin<min){
                        usermin=min;
                    }
                    if(usermax>max){
                        usermax=max;
                    }

            double pixel;

            for(int j=0;j<slikatrans.cols;j++){

            for(int i=0;i<slikatrans.rows;i++){



                    for(int k=0;k<3;k++){

                        pixel=(slikaorg.at<Vec3b>(i,j)[k]-usermin)/(usermax-usermin);

                        if(pixel>255){
                            pixel=255;
                        }else if(pixel<0){
                            pixel=0;
                        }

                        slikatrans.at<Vec3b>(i,j)[k] = pixel;
                    }
                }
            }


            ui->label_tran->setPixmap(
                        QPixmap::fromImage(QImage(slikatrans.data,
                                                  slikatrans.cols,
                                                  slikatrans.rows,
                                                  slikatrans.step,
                                                  QImage::Format_RGB888)));

            on_histogumb_clicked();

        }


    void MainWindow::on_ButtonLoad_clicked()
    {

          slikaorg = imread(ui->lineEdit->text().toStdString());
            cvtColor(slikaorg, slikaorg, CV_BGR2RGB);
            slikatrans=slikaorg;


            ui->label_org->setPixmap(
                        QPixmap::fromImage(QImage(slikaorg.data,
                                                  slikaorg.cols,
                                                  slikaorg.rows,
                                                  slikaorg.step,
                                                  QImage::Format_RGB888)));

            ui->label_tran->setPixmap(
                        QPixmap::fromImage(QImage(slikatrans.data,
                                                  slikatrans.cols,
                                                  slikatrans.rows,
                                                  slikatrans.step,
                                                  QImage::Format_RGB888)));

            on_histogumb_clicked();

        }



    void MainWindow::on_ButtonReset_clicked()
    {
        MainWindow::on_ButtonLoad_clicked();
    }


    void MainWindow::on_histogumb_clicked(){

            Mat slika;
            if(ui->izbira->currentIndex()==0) slika=slikatrans;
            if(ui->izbira->currentIndex()==1) cvtColor(slikatrans, slika, CV_BGR2HSV);
            if(ui->izbira->currentIndex()==2) cvtColor(slikatrans, slika, CV_BGR2Lab);

              std::vector<Mat> kanali;
              split(slika, kanali);

              int velikosthistogram = 30;
              float dolzina[] = { 0, 256 } ;
              const float* histogramdol = { dolzina };
              int i = 1;

              Mat histogram1, histogram2, histogram3;

              int sirina = 501; int visina = 151;
              int racun = cvRound( (double) sirina/velikosthistogram );

              Mat slikahisto1( visina, sirina, CV_8UC3, Scalar( 0,0,0) );



                  calcHist( &kanali[0], 1, 0, Mat(), histogram1, 1, &velikosthistogram, &histogramdol, true, false );
                  calcHist( &kanali[1], 1, 0, Mat(), histogram2, 1, &velikosthistogram, &histogramdol, true, false );
                  calcHist( &kanali[2], 1, 0, Mat(), histogram3, 1, &velikosthistogram, &histogramdol, true, false );

                  normalize(histogram1, histogram1, 0, slikahisto1.rows, NORM_MINMAX, -1, Mat() );
                  normalize(histogram2, histogram2, 0, slikahisto1.rows, NORM_MINMAX, -1, Mat() );
                  normalize(histogram3, histogram3, 0, slikahisto1.rows, NORM_MINMAX, -1, Mat() );


                  while( i < velikosthistogram)
                  {

                      line( slikahisto1, Point( racun*(i-1), visina - cvRound(histogram1.at<float>(i-1)) ) ,
                                       Point( racun*(i), visina - cvRound(histogram1.at<float>(i)) ),
                                       Scalar( 255, 0, 0), 2, 8, 0  );
                      line( slikahisto1, Point( racun*(i-1), visina - cvRound(histogram2.at<float>(i-1)) ) ,
                                       Point( racun*(i), visina - cvRound(histogram2.at<float>(i)) ),
                                       Scalar( 0, 0, 255), 2, 8, 0  );
                      line( slikahisto1, Point( racun*(i-1), visina - cvRound(histogram3.at<float>(i-1)) ) ,
                                       Point( racun*(i), visina - cvRound(histogram3.at<float>(i)) ),
                                       Scalar( 0, 255, 0), 2, 8, 0  );
                      i++;

                  }

                  ui->histo->setPixmap(
                              QPixmap::fromImage(QImage(slikahisto1.data,
                                                        slikahisto1.cols,
                                                        slikahisto1.rows,
                                                        slikahisto1.step,
                                                        QImage::Format_RGB888)));


    }

    void MainWindow::on_uveljavi_clicked()
    {

        MainWindow::on_ButtonLoad_clicked();

        Mat slikica;
        std::vector<Mat> channels;
        Mat maska;
        Mat mask1, mask2;
        int izbranabarva=ui->barva123->text().toInt();
        int izbranoodstopanje= ui->odstopanje123->text().toInt();
        int min=izbranabarva-izbranoodstopanje;
        int max=izbranabarva+izbranoodstopanje;


        cvtColor(slikaorg, slikica, CV_BGR2HSV);
        split(slikica,channels);

        int k=0;
        for(int i=0;i<channels[0].cols;i++){
            for(int j=0;j<channels[0].rows;j++){
                if(channels[0].at<Vec3b>(i,j)[0]>k)
                    k=channels[0].at<Vec3b>(i,j)[0];
            }
        }

        if(min>0 && max<360){
            threshold( channels[0], mask1, max, 360, THRESH_TOZERO_INV );
            threshold( channels[0], mask2, min, 360, THRESH_TOZERO );
            bitwise_and(mask1,mask2,maska);

            Mat rez = channels[1];
            bitwise_and(rez,maska,channels[1]);
            merge(channels,slikatrans);

            ui->label_tran->setPixmap(
                        QPixmap::fromImage(QImage(slikatrans.data,
                                                  slikatrans.cols,
                                                  slikatrans.rows,
                                                  slikatrans.step,
                                                  QImage::Format_RGB888)));


        }


        if(min<0 || max>360){

            if(max>360)max=max-360;
            if(min<0)min=360-min;


            threshold( channels[0], mask1, min, 360, THRESH_TOZERO_INV );
            threshold( channels[0], mask2, max, 360, THRESH_TOZERO );
            bitwise_and(mask1,mask2,maska);

            Mat rez = channels[1];
            bitwise_and(rez,maska,channels[1]);
            merge(channels,slikatrans);

            ui->label_tran->setPixmap(
                        QPixmap::fromImage(QImage(slikatrans.data,
                                                  slikatrans.cols,
                                                  slikatrans.rows,
                                                  slikatrans.step,
                                                  QImage::Format_RGB888)));



        }





        on_histogumb_clicked();
    }
