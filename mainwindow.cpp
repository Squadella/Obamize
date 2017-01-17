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

//Display the dialog that ask a folder from user
QString MainWindow::dialogOpenFile()
{
    return QFileDialog::getOpenFileName(this,
                                        "Open Image", "/home", "Image Files (*.png *.jpg *.bmp)");
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
        cv::Mat detectedBorders = inputImage;

        cv::cvtColor(inputImage, detectedBorders, CV_BGR2GRAY);
        cv::Canny(detectedBorders, detectedBorders, 50, 150, 3);
        detectedBorders.convertTo(inputImage, CV_8U);

        QMat qmat(inputImage, ui->labelModifiedImageContainer);
        qmat.show();

        ui->pushButtonSave->setEnabled(true);
    }
}
