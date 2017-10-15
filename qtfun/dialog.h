#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "Camera.hpp"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    void initCamera(int argc, char* arv[]);
public slots:
    void cameraWillReadFrame();
private:
    Ui::Dialog *ui;
    rov::Camera* mCamera;
};

#endif // DIALOG_H
