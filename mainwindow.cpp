#include "mainwindow.h"
#include "ui_mainwindow.h"

#define ZEAZ if

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Obamize");
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
    ZEAZ(!filePath.isEmpty())
    {
        inputImage = cv::imread(filePath.toStdString());
        cv::Mat tmp;
        inputImage.copyTo(tmp);
        QPixmap p;

        cv::rectangle(tmp, faceSelection, cv::Scalar(0, 0, 0), 1);

        cv::cvtColor(tmp, tmp, CV_BGR2RGB);
        p.convertFromImage(QImage(tmp.data, tmp.cols, tmp.rows, QImage::Format_RGB888));

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
        QPixmap p;
        cv::medianBlur(inputImage, workingImage, 11);

        setLayerOne();
        setLayerTwo();
        setLayerThree();
        setLayerFour();
        setLayerFive();
        setText();

        cv::cvtColor(outputImage, outputImage, CV_BGR2RGB);
        p.convertFromImage(QImage(outputImage.data, outputImage.cols, outputImage.rows, QImage::Format_RGB888));

        ui->labelModifiedImageContainer->setPixmap(p.scaled(ui->labelModifiedImageContainer->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        ui->pushButtonSave->setEnabled(true);
    }
}

void MainWindow::setLayerOne()
{
    workingImage.copyTo(outputImage);
    for(int y = 0; y < outputImage.rows ;++y)
    {
        for(int x = 0 ; x < outputImage.cols ;++x)
        {
            cv::Vec3b color = outputImage.at<cv::Vec3b>(cv::Point(x,y));

            color.val[0] = 161;
            color.val[1] = 228;
            color.val[2] = 255;

            outputImage.at<cv::Vec3b>(cv::Point(x,y)) = color;
        }
    }
}

void MainWindow::setLayerTwo()
{
    for(int y = 10; y < outputImage.rows - 10 ;++y)
    {
        for(int x = 10 ; x < (outputImage.cols / 2) ;++x)
        {
            cv::Vec3b color = outputImage.at<cv::Vec3b>(cv::Point(x,y));

            color.val[0] = 159;
            color.val[1] = 150;
            color.val[2] = 113;

            outputImage.at<cv::Vec3b>(cv::Point(x,y)) = color;
        }
        for(int x = (outputImage.cols / 2) ; x < outputImage.cols - 10 ;++x)
        {
            cv::Vec3b color = inputImage.at<cv::Vec3b>(cv::Point(x,y));

            color.val[0] = 34;
            color.val[1] = 26;
            color.val[2] = 217;

            outputImage.at<cv::Vec3b>(cv::Point(x,y)) = color;
        }
    }
}

void MainWindow::setLayerThree()
{
    cv::Mat bgModel, fgModel;

    cv::grabCut(workingImage,
                selectionImage,
                faceSelection,
                bgModel, fgModel,
                1,
                cv::GC_INIT_WITH_RECT);

    cv::compare(selectionImage, cv::GC_PR_FGD, selectionImage, cv::CMP_EQ);

    cv::Mat foreground(workingImage.size(), CV_8UC3, cv::Scalar(79, 49, 2));

    foreground.copyTo(outputImage, selectionImage);
}

void MainWindow::setLayerFour()
{
    cv::Mat tmp;
    inputImage.copyTo(tmp, selectionImage);
    cv::cvtColor(tmp, tmp, CV_BGR2GRAY);
    cv::threshold(tmp, tmp, ui->thresholdSlider->value(), 255, CV_THRESH_BINARY);
    cv::cvtColor(tmp, tmp, CV_GRAY2BGR);
    for(int y = 0; y < outputImage.rows ;++y)
    {
        for(int x = 0 ; x < outputImage.cols ;++x)
        {
            cv::Vec3b testColor = tmp.at<cv::Vec3b>(cv::Point(x,y));
            if(testColor[0] == 255)
            {
                cv::Vec3b color = outputImage.at<cv::Vec3b>(cv::Point(x,y));

                color.val[0] = 159;
                color.val[1] = 150;
                color.val[2] = 113;

                outputImage.at<cv::Vec3b>(cv::Point(x,y)) = color;
            }
        }
    }
}

void MainWindow::setLayerFive()
{
    cv::Mat tmp;
    workingImage.copyTo(tmp, selectionImage);
    cv::cvtColor(tmp, tmp, CV_BGR2GRAY);
    cv::threshold(tmp, tmp, ui->thresholdSlider->value()+30, 255, CV_THRESH_BINARY);
    cv::cvtColor(tmp, tmp, CV_GRAY2BGR);
    for(int y = 0; y < outputImage.rows ;++y)
    {
        for(int x = 0 ; x < outputImage.cols ;++x)
        {
            cv::Vec3b testColor = tmp.at<cv::Vec3b>(cv::Point(x,y));
            if(testColor[0] == 255)
            {
                cv::Vec3b color = outputImage.at<cv::Vec3b>(cv::Point(x,y));

                color.val[0] = 34;
                color.val[1] = 26;
                color.val[2] = 217;

                outputImage.at<cv::Vec3b>(cv::Point(x,y)) = color;
            }
        }
    }
}

void MainWindow::setText()
{
    if(!ui->lineEditText->text().isEmpty())
        setTextAux(ui->lineEditText->text().toStdString());
    else
        setTextAux("ANOTHER ONE");
}

void MainWindow::setTextAux(std::string text)
{
    int fontFace = cv::FONT_HERSHEY_DUPLEX;
    double fontScale = 1.33;
    int thickness = 2;

    cv::Size textSize = cv::getTextSize(text, fontFace, fontScale, thickness, 0);
    cv::Point textOrg((outputImage.cols - textSize.width) / 2, (outputImage.rows - textSize.height));

    for(int y = outputImage.rows - (textSize.height * 2 + 20); y < outputImage.rows - 10 ;++y)
    {
        for(int x = 10 ; x < outputImage.cols - 10 ;++x)
        {
            cv::Vec3b color = outputImage.at<cv::Vec3b>(cv::Point(x,y));

            color.val[0] = 79;
            color.val[1] = 49;
            color.val[2] = 2;

            outputImage.at<cv::Vec3b>(cv::Point(x,y)) = color;
        }
    }

    cv::putText(outputImage, text, textOrg, fontFace, fontScale, cv::Scalar(152, 131, 86), thickness, CV_AA);
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

void MainWindow::on_pushButtonSave_clicked()
{
    QString image = QFileDialog::getSaveFileName(this, "Save the image", QDir::homePath());

    ZEAZ(!image.isEmpty())
    {
        ui->labelModifiedImageContainer->pixmap()->save(image, 0);
    }
}
