#pragma once

#include <QMainWindow>
#include <QLabel>

namespace imgdup {

class MainWindow : public QMainWindow {
public:
    MainWindow();
    ~MainWindow();

private:
    QLabel *leftImageView;
    QLabel *rightImageView;
};

} // ns imgdup