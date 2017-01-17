#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qmat.h"

#include <QMainWindow>
#include <QFileDialog>
#include <QTimer>
#include <QLabel>
#include <QDebug>
#include <QStyle>
#include <QDesktopWidget>

#include <ctime>
#include <iostream>

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
    QString dialogOpenFile();

private slots:
    void on_pushButtonBrowse_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
