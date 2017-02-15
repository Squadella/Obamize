#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QTimer>
#include <QLabel>
#include <QDebug>
#include <QStyle>
#include <QDesktopWidget>
#include <QSettings>

#include <ctime>
#include <iostream>
#include <cstdlib>

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv/cxcore.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

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
    void on_pushButtonBrowse_clicked();
    void on_pushButtonProcess_clicked();
    void setLayerOne();
    void setLayerTwo();
    void setLayerThree();
    void setLayerFour();
    QString dialogOpenFile();
    void on_sliderMarginTop_sliderMoved(int position);
    void on_sliderMarginLeft_sliderMoved(int position);
    void on_sliderWidth_sliderMoved(int position);

    void on_sliderHeight_sliderMoved(int position);

private:
    QString filePath;
    Ui::MainWindow *ui;
    int marginTop;
    int marginLeft;
    int width;
    int height;
    cv::Rect faceSelection;
    cv::Mat workingImage;
    cv::Mat inputImage;
    cv::Mat outputImage;
    cv::Mat selectionImage;

    void onPositionChangeEvent();
    void setLayerFive();
};

#endif // MAINWINDOW_H
