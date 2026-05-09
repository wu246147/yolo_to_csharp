#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "raivasalgobbdet.h"
#include "raivasalgseg.h"

void drawRotatedRect(cv::Mat& img,
                     const cv::RotatedRect& rr,
                     float rad,
                     const cv::Scalar& color,
                     int thickness = 2)
{
    // 1. 四点轮廓
    cv::Point2f pts[4];
    rr.points(pts);
    for(int i = 0; i < 4; ++i)
    {
        cv::line(img, pts[i], pts[(i + 1) % 4], color, thickness);
    }

    // 2. 方向箭头：从中心沿长边方向画一条线段
    float angle = rad * 180.f / CV_PI;          // 度→弧度
    float cosA = std::cos(rad), sinA = std::sin(rad);
    // OpenCV 里 width 总是 ≥ height，所以长边就是 width
    float arrLen = min(rr.size.width * 0.5f, 40.f); // 箭头长度
    cv::Point2f end = rr.center + arrLen * cv::Point2f(cosA, sinA);
    cv::arrowedLine(img, rr.center, end, color, thickness, cv::LINE_AA, 0, 0.2);

    // 3. 文字：角度（保留 1 位小数）
    char buf[32];
    std::snprintf(buf, sizeof(buf), "%.1f", angle);
    cv::putText(img, buf, rr.center + cv::Point2f(5, -5),
                cv::FONT_HERSHEY_SIMPLEX, 0.5, color, 1, cv::LINE_AA);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

cv::Mat img;
int len;

static std::map<int, std::string> classNames =
{
    {0, "obj"},
} ;

void MainWindow::on_pushButton_3_clicked()
{
    std::string path = ui->lineEdit_openmodel->text().toStdString();
    bool useGPU = ui->checkBox->isChecked();

    float thre = ui->doubleSpinBox_thre->value();
    initOBBModel(path.data(), useGPU);
    setOBBPara(thre);
}


void MainWindow::on_pushButton_clicked()
{
    std::string path = ui->lineEdit_openfile->text().toStdString();
    img = cv::imread(path, 0);
    // cv::rotate(img, img, cv::ROTATE_180);
    auto start = std::chrono::high_resolution_clock::now();
    runOBB(img, len);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "OCR total:" << duration << "ms" << std::endl;
    std::cout << "-----------------------------------" << std::endl;
}


void MainWindow::on_pushButton_4_clicked()
{
    std::cout << "result len:" << len << std::endl;
    cv::Mat imgGray = img.clone();
    cv::Mat imgShow;
    cv::cvtColor(imgGray, imgShow, cv::COLOR_GRAY2BGR);
    if(!imgShow.empty())
    {
        for(int id = 0; id < len; id++)
        {
            int locateOCRClassId ;
            float locateOCRScore;
            float locateOCRX ;
            float locateOCRY ;
            float locateOCRW ;
            float locateOCRH ;
            float locateOCRAngle;
            float rad;
            // std::cout << "getOCRAndDepthResult:" << id << std::endl;
            getOBBResult(id, &locateOCRClassId, &locateOCRScore, &locateOCRX, &locateOCRY,
                         &locateOCRW, &locateOCRH,
                         &locateOCRAngle, &rad);

            cv::RotatedRect box(cv::Point2f(locateOCRX, locateOCRY), cv::Size2f(locateOCRW, locateOCRH), locateOCRAngle);

            cv::Rect r = box.boundingRect();
            drawRotatedRect(imgShow, box, rad, cv::Scalar(0, 0, 255), 2);
            cv::putText(imgShow, classNames[locateOCRClassId], cv::Point(r.tl().x, r.tl().y - 10), cv::FONT_HERSHEY_SIMPLEX,
                        1, cv::Scalar(0, 0, 0), 2);


            // // std::cout << "drawRect:" << id << std::endl;
            // cv::Rect box(locateOCRX, locateOCRY, locateOCRW, locateOCRH);
            // cv::rectangle(imgShow, box, cv::Scalar(0, 0, 255), 2, 8);
            // cv::rectangle(imgShow, cv::Point(box.tl().x, box.tl().y - 40),
            //               cv::Point(box.br().x, box.tl().y), cv::Scalar(0, 255, 255), -1);
            // putTextHusky(imgShow, classNames[locateOCRClassId].data(), cv::Point(box.tl().x, box.tl().y - 30), cv::Scalar(0, 0, 0), 30, "Arial", true, true);
            // // putTextHusky(imgShow, std::to_string(locateOCRClassId).data(), cv::Point(box.tl().x, box.tl().y - 30), cv::Scalar(0, 0, 0), 30, "Arial", true, true);
            // std::cout << std::to_string(locateOCROrder) << std::endl;
            // std::cout << classNames[locateOCRClassId].data() << std::endl;
            // putTextHusky(imgShow, (std::to_string(locateOCRRow) + " " + std::to_string(locateOCROrder)).data(), cv::Point(box.tl().x, box.tl().y + box.height),
            //              cv::Scalar(0, 255, 0), 30, "Arial", true, true);
        }
        // std::cout << "finish show result" << std::endl;
        cv::namedWindow("result", cv::WINDOW_NORMAL);
        cv::imshow("result", imgShow);
        cv::waitKey(100);
    }

}


void MainWindow::on_pushButton_init_seg_clicked()
{
    std::string path = ui->lineEdit_openmodel_seg->text().toStdString();
    bool useGPU = ui->checkBox->isChecked();

    float thre = ui->doubleSpinBox_thre_seg->value();
    initSegModel(path.data(), useGPU);
    setSegPara(thre);

}


void MainWindow::on_pushButton_det_seg_clicked()
{
    std::string path = ui->lineEdit_openfile_seg->text().toStdString();
    img = cv::imread(path, 0);
    auto start = std::chrono::high_resolution_clock::now();
    runSeg(img, len);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "Seg total:" << duration << "ms" << std::endl;
    std::cout << "-----------------------------------" << std::endl;

}


void MainWindow::on_pushButton_showresult_seg_clicked()
{
    std::cout << "result len:" << len << std::endl;
    cv::Mat imgGray = img.clone();
    cv::Mat imgShow;
    cv::cvtColor(imgGray, imgShow, cv::COLOR_GRAY2BGR);
    if(!imgShow.empty())
    {
        for(int id = 0; id < len; id++)
        {
            int locateOCRClassId ;
            float locateOCRScore;
            float locateOCRX ;
            float locateOCRY ;
            float locateOCRW ;
            float locateOCRH ;
            cv::Mat mask;
            std::cout << "getOCRAndDepthResult:" << id << std::endl;
            getSegResult(id, &locateOCRClassId, &locateOCRScore, &locateOCRX, &locateOCRY,
                         &locateOCRW, &locateOCRH,
                         mask);

            //显示框
            cv::Rect2d box(locateOCRX - locateOCRW / 2, locateOCRY - locateOCRH / 2, locateOCRW, locateOCRH);
            cv::rectangle(imgShow, box, cv::Scalar(0, 0, 255), 2);

            //显示mask
            cv::Vec3b color(rand() % 255, rand() % 255, rand() % 255);
            for(int y = 0; y < imgShow.rows; y++)
                for(int x = 0; x < imgShow.cols; x++)
                    if(mask.at<uchar>(y, x) > 0)
                        imgShow.at<cv::Vec3b>(y, x) =
                            imgShow.at<cv::Vec3b>(y, x) * 0.5 + color * 0.5;

            //显示类别
            cv::putText(imgShow, std::to_string(locateOCRClassId + 1), cv::Point(box.tl().x, box.tl().y - 10), cv::FONT_HERSHEY_SIMPLEX,
                        1, cv::Scalar(0, 0, 0), 2);


        }
        // std::cout << "finish show result" << std::endl;
        cv::namedWindow("result", cv::WINDOW_NORMAL);
        cv::imshow("result", imgShow);
        cv::waitKey(100);
    }
}

