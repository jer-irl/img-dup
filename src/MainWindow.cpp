#include "MainWindow.hpp"

#include <QHBoxLayout>
#include <QFileDialog>
#include <QMenuBar>

#include <algorithm>

namespace fs = boost::filesystem;

namespace imgdup {

MainWindow::MainWindow() {
    central_ = new QWidget(this);
    setCentralWidget(central_);
    leftImageView_ = new QLabel(central_);
    rightImageView_ = new QLabel(central_);

    setupLayout();

    QMenu *imgDupMenu = menuBar()->addMenu("ImgDupMenu");
    QAction *checkAction = new QAction("Start Checking", imgDupMenu);
    imgDupMenu->addAction(checkAction);
    connect(checkAction, &QAction::triggered, this, &MainWindow::startProcessing);
}

MainWindow::~MainWindow() {
    delete leftImageView_;
    delete rightImageView_;
    delete centralWidget()->layout();
    delete centralWidget();
}

void MainWindow::setupLayout() {
    central_->setLayout(new QHBoxLayout);
    central_->layout()->addWidget(leftImageView_);
    central_->layout()->addWidget(rightImageView_);
}

void MainWindow::startProcessing() {
    rootDir_ = QFileDialog::getExistingDirectory(this, "What root directory?");
    imageProcessor_.processDirectory(rootDir_.toStdString());

    for (const fs::directory_entry &entry : fs::recursive_directory_iterator(rootDir_.toStdString())) {
        fs::path path = entry.path();
        if (!fs::is_regular_file(path)) {
            continue;
        }

        const std::vector<std::string> extensionsToCheck{".jpg", ".jpeg", ".png"};
        bool isImage = std::any_of(
                extensionsToCheck.cbegin(), extensionsToCheck.cend(), [&path](const std::string &ext) {
            return ext == path.extension();
        });
        if (!isImage) {
            continue;
        }

        std::vector<fs::path> closest = imageProcessor_.closestForImage(path, 2);

        QPixmap leftPixmap(path.string().c_str());
        leftImageView_->setPixmap(leftPixmap);

        QPixmap rightPixmap(closest[1].string().c_str());
        rightImageView_->setPixmap(rightPixmap);
    }
}

} // ns imgdup