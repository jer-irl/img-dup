#pragma once

#include "ImgDup.hpp"
#include <QMainWindow>
#include <QLabel>

namespace imgdup {

class MainWindow : public QMainWindow {
public:
    MainWindow();
    ~MainWindow();

private:
    void startProcessing();

    void setupLayout();

    QWidget *central_;
    QLabel *leftImageView_;
    QLabel *rightImageView_;
    QLabel *leftImageLabel_;
    QLabel *rightImageLabel_;
    ImageProcessor imageProcessor_;
    QString rootDir_;
};

} // ns imgdup