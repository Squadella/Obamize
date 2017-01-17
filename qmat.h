#ifndef U_QMAT_H
#define U_QMAT_H

#include <QTimer>
#include <QLabel>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv/cxcore.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <ctime>
#include <iostream>

class QMat : public QWidget
{
    Q_OBJECT
    public:
    QMat(QWidget *parent = 0);
        QMat(cv::Mat newMat, QWidget *parent = 0);
        QMat(cv::Mat (*mat_function)(), int waitTime = 30, QWidget *parent = 0);
        void setMat(cv::Mat newMat);
        void setFunction(cv::Mat (*new_function)());
    void setWaitingTime(int newt);
    //double FPS();

    private:
        void setLabel();

        cv::Mat _mat;
        QLabel* _Label;
        cv::Mat (*_function)();
        QTimer* _timer;

    public slots:
        void getFunctionMat();

    signals:
        void matChanged();
};

#endif