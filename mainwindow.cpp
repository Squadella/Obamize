#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qmat.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
    ui->labelOriginalImageContainer->setPixmap(QPixmap(tmp));

    cv::Mat inputImage = cv::imread(tmp.toStdString());
    QMat qmat(inputImage, ui->labelModifiedImageContainer);
    qmat.show();

    qDebug() << tmp;
}
