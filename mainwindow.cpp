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
        cv::Mat image = inputImage;
        QPixmap p;
        setLayerOne(inputImage);
        setLayerTwo(inputImage);

        cv::cvtColor(inputImage, inputImage, CV_BGR2RGB);
        p.convertFromImage(QImage(inputImage.data, inputImage.cols, inputImage.rows, QImage::Format_RGB888));
        double thresh = 127;
        double maxValue = 255;

        cv::Rect rectangle(1,1,image.cols-1,image.rows-1);

        cv::Mat result; // segmentation result (4 possible values)
        cv::Mat bgModel,fgModel; // the models (internally used)

        // GrabCut segmentation
        cv::grabCut(image,    // input image
                    result,   // segmentation result
                    rectangle,// rectangle containing foreground
                    bgModel,fgModel, // models
                    1,        // number of iterations
                    cv::GC_INIT_WITH_RECT); // use rectangle
        // Get the pixels marked as likely foreground
        cv::compare(result,cv::GC_PR_FGD,result,cv::CMP_EQ);
        // Generate output image
        cv::Mat foreground(image.size(),CV_8UC3,cv::Scalar(255,255,255));
        //cv::Mat background(image.size(),CV_8UC3,cv::Scalar(255,255,255));
        image.copyTo(foreground,result); // bg pixels not copied

        /*
        mask = np.zeros(img.shape[:2],np.uint8)
        bgdModel = np.zeros((1,65),np.float64)
        fgdModel = np.zeros((1,65),np.float64)
        rect = (50,50,450,290)
        cv2.grabCut(img,mask,rect,bgdModel,fgdModel,5,cv2.GC_INIT_WITH_RECT)
        mask2 = np.where((mask==2)|(mask==0),0,1).astype('uint8')
        img = img*mask2[:,:,np.newaxis]*/

        //blurr
        //remove blue

        cv::cvtColor(foreground, foreground, CV_BGR2RGB);
        p.convertFromImage(QImage(foreground.data, foreground.cols, foreground.rows, QImage::Format_RGB888));

        //threshold(inputImage, inputImage, thresh, maxValue, cv::THRESH_BINARY_INV);

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
