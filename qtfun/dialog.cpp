#include "dialog.h"
#include "ui_dialog.h"
#include <iostream>
#include "Camera.hpp"
#include <QTimer>
#include <QImage>
#include <string.h>
#include <stdlib.h>
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(cameraWillReadFrame()));
    timer->start(50);
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::cameraWillReadFrame(){
    rov::FrameRef frame = this->mCamera->readFrame();
    if (frame == nullptr) return;
    uint32_t frameLen = frame->getPlaneLength(0);
    void* frameBuffer = malloc(frameLen);
    memset(frameBuffer, 0, frameLen);
    frame->getPlane(0, frameBuffer, &frameLen);

    QImage image = QImage::fromData((uchar*)frameBuffer, frameLen);
    image = image.scaled(ui->label->size());
    free(frameBuffer);
    ui->label->setPixmap(QPixmap::fromImage(image));
    delete frame;
}

void Dialog::initCamera(int argc, char *argv[]) {
    if (argc < 1) {
        std::cout<<"please input device name\n";
        return;
    }
    this->mCamera = new rov::Camera(argv[1], O_RDWR| O_NONBLOCK);
    this->mCamera->setFrameBufferCount(2);
    this->mCamera->setFrameBufferType(V4L2_BUF_TYPE_VIDEO_CAPTURE);
    this->mCamera->setFps(30);
    this->mCamera->setFrameField(V4L2_FIELD_ANY);
    this->mCamera->setFrameFormat(V4L2_PIX_FMT_MJPEG);
    this->mCamera->setFrameHeight(1280);
    this->mCamera->setFrameWidth(720);
    this->mCamera->configure(VIDIOC_S_PARM);
    this->mCamera->configure(VIDIOC_S_FMT);
    this->mCamera->configure(VIDIOC_REQBUFS);
    this->mCamera->queryBuffer();
    this->mCamera->configure(VIDIOC_STREAMON);
}
