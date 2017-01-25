#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setFixedSize(QSize(1100, 650));

    ui->pushButtonProcess->setEnabled(false);
    ui->pushButtonSave->setEnabled(false);

    ui->labelOriginalImageContainer->setFixedWidth(500);
    ui->labelOriginalImageContainer->setFixedHeight(500);

    ui->labelModifiedImageContainer->setFixedWidth(500);
    ui->labelModifiedImageContainer->setFixedHeight(500);

    QRect desktop = QDesktopWidget().screenGeometry();
    move(desktop.width() / 2 - 550, desktop.height() / 2 - 325);

    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::close);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonBrowse_clicked()
{
    filePath = dialogOpenFile();

    ui->labelOriginalImageContainer->setPixmap(QPixmap(filePath).scaled(ui->labelOriginalImageContainer->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->pushButtonProcess->setEnabled(true);
}

void MainWindow::on_pushButtonProcess_clicked()
{
    if(!filePath.isEmpty())
    {
        cv::Mat inputImage = cv::imread(filePath.toStdString());
        QPixmap p;

        setLayerOne(inputImage);
        setLayerTwo(inputImage);

        cv::cvtColor(inputImage, inputImage, CV_BGR2RGB);
        p.convertFromImage(QImage(inputImage.data, inputImage.cols, inputImage.rows, QImage::Format_RGB888));

        ui->labelModifiedImageContainer->setPixmap(p.scaled(ui->labelModifiedImageContainer->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        ui->pushButtonSave->setEnabled(true);
    }
}

void MainWindow::setLayerOne(cv::Mat inputImage)
{
    for(int y = 0; y < inputImage.rows ;++y)
    {
        for(int x = 0 ; x < inputImage.cols ;++x)
        {
            cv::Vec3b color = inputImage.at<cv::Vec3b>(cv::Point(x,y));

            color.val[0] = 161;
            color.val[1] = 228;
            color.val[2] = 255;

            inputImage.at<cv::Vec3b>(cv::Point(x,y)) = color;
        }
    }
}

void MainWindow::setLayerTwo(cv::Mat inputImage)
{
    for(int y = 30; y < inputImage.rows - 30 ;++y)
    {
        for(int x = 30 ; x < (inputImage.cols / 2) ;++x)
        {
            cv::Vec3b color = inputImage.at<cv::Vec3b>(cv::Point(x,y));

            color.val[0] = 159;
            color.val[1] = 150;
            color.val[2] = 113;

            inputImage.at<cv::Vec3b>(cv::Point(x,y)) = color;
        }
        for(int x = (inputImage.cols / 2) ; x < inputImage.cols - 30 ;++x)
        {
            cv::Vec3b color = inputImage.at<cv::Vec3b>(cv::Point(x,y));

            color.val[0] = 34;
            color.val[1] = 26;
            color.val[2] = 217;

            inputImage.at<cv::Vec3b>(cv::Point(x,y)) = color;
        }
    }
}

QString MainWindow::dialogOpenFile()
{
    return QFileDialog::getOpenFileName(this,
                                        "Open Image", "/home", "Image Files (*.png *.jpg *.bmp)");
}
