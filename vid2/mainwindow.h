#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QImage>
#include <QPainter>
#include <opencv2/opencv.hpp>
#include "opencv2/videoio.hpp"
#include <QMessageBox>

using namespace cv;

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

    void on_ButtonTrans_clicked();

    void on_ButtonLoad_clicked();

    void on_ButtonReset_clicked();

    void on_histogumb_clicked();

    void on_uveljavi_clicked();


private:
    Ui::MainWindow *ui;
        Mat slikaorg;
        Mat slikatrans;
        double min;
        double max;
        double usermin;
        double usermax;
};

#endif // MAINWINDOW_H
