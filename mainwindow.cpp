#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qmat.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->labelOriginalImageContainer->setFixedWidth(500);
    ui->labelOriginalImageContainer->setFixedHeight(500);

    ui->labelModifiedImageContainer->setFixedWidth(500);
    ui->labelModifiedImageContainer->setFixedHeight(500);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//Display the dialog that ask a folder from user
QString MainWindow::dialogOpenFile()
{
    return QFileDialog::getOpenFileName(this,
                                        "Open Image", "/home", "Image Files (*.png *.jpg *.bmp)");
}

void MainWindow::on_pushButtonBrowse_clicked()
{
    QString tmp = dialogOpenFile();

    ui->labelOriginalImageContainer->setPixmap(QPixmap(tmp).scaled(ui->labelOriginalImageContainer->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    cv::Mat inputImage = cv::imread(tmp.toStdString());
    cv::Mat BckpImg = inputImage;
    cv::Mat detectedBorders = inputImage;
    cv::Mat layer1 = inputImage;

    cv::medianBlur(layer1, layer1, 3);
    cv::cvtColor(layer1, detectedBorders, CV_BGR2GRAY);
    cv::Canny(detectedBorders, detectedBorders, 50, 150, 3);
    //Itterate for bigger zone.
    cv::dilate(detectedBorders, detectedBorders, cv::Mat());
    //cv::MemStorage *storage =
    cv::hconcat(BckpImg, detectedBorders);
    cv::vconcat(BckpImg, detectedBorders);

    detectedBorders.convertTo(inputImage, CV_8U);

    QMat qmat(inputImage, ui->labelModifiedImageContainer);
    qmat.show();

    qDebug() << tmp;
}
