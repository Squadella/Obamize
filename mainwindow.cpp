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
    QMat qmat(inputImage, ui->labelModifiedImageContainer);
    qmat.show();

    qDebug() << tmp;
}
