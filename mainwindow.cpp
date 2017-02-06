#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //setFixedSize(QSize(1100, 650));

    this->marginTop = ui->sliderMarginTop->value();
    this->marginLeft = ui->sliderMarginLeft->value();
    this->width = 100;
    this->height = 100;


    cv::Rect rectangle(marginLeft, marginTop, width, height);
    faceSelection = rectangle;

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

void MainWindow::onPositionChangeEvent()
{
    if(!filePath.isEmpty())
    {
        inputImage = cv::imread(filePath.toStdString());
        QPixmap p;

        cv::rectangle(inputImage, faceSelection, cv::Scalar(0, 0, 0), 1);

        cv::cvtColor(inputImage, inputImage, CV_BGR2RGB);
        p.convertFromImage(QImage(inputImage.data, inputImage.cols, inputImage.rows, QImage::Format_RGB888));

        ui->labelOriginalImageContainer->setPixmap(p.scaled(ui->labelOriginalImageContainer->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}

void MainWindow::on_pushButtonBrowse_clicked()
{
    filePath = dialogOpenFile();
    onPositionChangeEvent();
    ui->pushButtonProcess->setEnabled(true);
}

void MainWindow::on_pushButtonProcess_clicked()
{
    if(!filePath.isEmpty())
    {
        cv::Mat inputImage = cv::imread(filePath.toStdString());
        cv::Mat image;
        inputImage.copyTo(image);
        QPixmap p;

        setLayerOne(image);
        setLayerTwo(image);

        cv::Mat result;
        cv::Mat bgModel, fgModel;

        cv::grabCut(inputImage,
                    result,
                    faceSelection,
                    bgModel, fgModel,
                    1,
                    cv::GC_INIT_WITH_RECT);

        cv::compare(result, cv::GC_PR_FGD, result, cv::CMP_EQ);

        //cv::Mat foreground(inputImage.size(), CV_8UC3, cv::Scalar(255, 255, 255));
        inputImage.copyTo(image, result);

        cv::cvtColor(image, image, CV_BGR2RGB);

        p.convertFromImage(QImage(image.data, image.cols, image.rows, QImage::Format_RGB888));

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
    for(int y = 10; y < inputImage.rows - 10 ;++y)
    {
        for(int x = 10 ; x < (inputImage.cols / 2) ;++x)
        {
            cv::Vec3b color = inputImage.at<cv::Vec3b>(cv::Point(x,y));

            color.val[0] = 159;
            color.val[1] = 150;
            color.val[2] = 113;

            inputImage.at<cv::Vec3b>(cv::Point(x,y)) = color;
        }
        for(int x = (inputImage.cols / 2) ; x < inputImage.cols - 10 ;++x)
        {
            cv::Vec3b color = inputImage.at<cv::Vec3b>(cv::Point(x,y));

            color.val[0] = 34;
            color.val[1] = 26;
            color.val[2] = 217;

            inputImage.at<cv::Vec3b>(cv::Point(x,y)) = color;
        }
    }
}

void MainWindow::on_sliderMarginTop_sliderMoved(int position)
{
    marginTop = ((position * (inputImage.rows - height)) / 100);

    while(marginTop+height > inputImage.rows)
        --height;

    if(height == 0)
        ++height;

    cv::Rect rectangle(marginLeft,  marginTop, width, height);
    faceSelection = rectangle;
    onPositionChangeEvent();
}

void MainWindow::on_sliderHeight_sliderMoved(int position)
{
    height = ((position * inputImage.rows - marginTop) / 100);

    while(marginTop+height > inputImage.rows)
        --marginTop;

    if(height == 0)
        ++height;

    cv::Rect rectangle(marginLeft, marginTop, width, height);
    faceSelection = rectangle;
    onPositionChangeEvent();
}

void MainWindow::on_sliderWidth_sliderMoved(int position)
{
    width = ((position * (inputImage.cols - marginLeft)) / 100);

    while(marginLeft+width > inputImage.cols)
        --marginLeft;

    if(width == 0)
        ++width;

    cv::Rect rectangle(marginLeft, marginTop, width, height);
    faceSelection = rectangle;
    onPositionChangeEvent();
}

void MainWindow::on_sliderMarginLeft_sliderMoved(int position)
{
    marginLeft = ((position * (inputImage.cols - width)) / 100);

    while(marginLeft+width > inputImage.cols)
        --width;

    if(width == 0)
        ++width;

    cv::Rect rectangle(marginLeft, marginTop, width, height);
    faceSelection = rectangle;
    onPositionChangeEvent();
}

QString MainWindow::dialogOpenFile()
{
    return QFileDialog::getOpenFileName(this,
                                        "Open Image", "/home", "Image Files (*.png *.jpg *.bmp)");
}



