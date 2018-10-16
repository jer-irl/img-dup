#include "MainWindow.hpp"

#include <QHBoxLayout>

namespace imgdup {

MainWindow::MainWindow() {
    QWidget *central = new QWidget;
    setCentralWidget(central);
    leftImageView = new QLabel(central);
    rightImageView = new QLabel(central);
    central->setLayout(new QHBoxLayout);
}

MainWindow::~MainWindow() {
    delete leftImageView;
    delete rightImageView;
    delete centralWidget()->layout();
    delete centralWidget();
}

} // ns imgdup