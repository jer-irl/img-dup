#include "MainWindow.hpp"

#include <QHBoxLayout>
#include <QFileDialog>
#include <QMenuBar>
#include <QKeyEvent>
#include <QApplication>

#include <algorithm>

namespace fs = boost::filesystem;

namespace imgdup {

MainWindow::MainWindow() {
    central_ = new QWidget(this);
    setCentralWidget(central_);

    leftContainer_ = new QWidget(central_);
    rightContainer_ = new QWidget(central_);

    leftImageView_ = new QLabel(leftContainer_);
    rightImageView_ = new QLabel(rightContainer_);

    leftImageLabel_ = new QLabel(leftContainer_);
    rightImageLabel_ = new QLabel(rightContainer_);

    matchesTable_ = new QTableWidget(central_);
    matchesTable_->setColumnCount(1);
    connect(matchesTable_, &QTableWidget::currentCellChanged, this, &MainWindow::currentCellChanged);

    setupLayout();

    QMenu *imgDupMenu = menuBar()->addMenu("ImgDupMenu");
    QAction *checkAction = new QAction("Start Checking", imgDupMenu);
    imgDupMenu->addAction(checkAction);
    connect(checkAction, &QAction::triggered, this, &MainWindow::startProcessing);
}

void MainWindow::setupLayout() {
    central_->setLayout(new QHBoxLayout);
    central_->layout()->addWidget(leftContainer_);
    central_->layout()->addWidget(rightContainer_);
    central_->layout()->addWidget(matchesTable_);

    leftContainer_->setLayout(new QVBoxLayout);
    leftContainer_->layout()->addWidget(leftImageView_);
    leftContainer_->layout()->addWidget(leftImageLabel_);

    rightContainer_->setLayout(new QVBoxLayout);
    rightContainer_->layout()->addWidget(rightImageView_);
    rightContainer_->layout()->addWidget(rightImageLabel_);
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

        toCheck_.insert(path);
    }

    checkPicture(*toCheck_.begin());
}

void MainWindow::checkPicture(const boost::filesystem::path &path) {
    currentPath_ = path;
    currentMatches_ = imageProcessor_.closestForImage(path, 10);
    currentMatches_.erase(currentMatches_.begin());
    updateComparisonView();
}

void MainWindow::updateComparisonView() {
    QPixmap leftPixmap(currentPath_.string().c_str());
    leftImageView_->setPixmap(leftPixmap);
    leftImageLabel_->setText(currentPath_.string().c_str());

    while (matchesTable_->rowCount() > 0) {
        matchesTable_->removeRow(0);
    }
    for (std::size_t i = 0; i < currentMatches_.size(); ++i) {
        auto *item = new QTableWidgetItem;
        item->setText(currentMatches_[i].string().c_str());
        matchesTable_->insertRow(static_cast<int>(i));
        matchesTable_->setItem(static_cast<int>(i), 0, item);
    }

    displayMatchIndexed(0);
}

void MainWindow::displayMatchIndexed(std::size_t idx) {
    QPixmap rightPixmap(currentMatches_[idx].string().c_str());
    rightImageView_->setPixmap(rightPixmap);
    rightImageLabel_->setText(currentMatches_[idx].string().c_str());
}

void MainWindow::currentCellChanged(int row, int, int, int){
    if (row == -1) {
        return;
    }
    displayMatchIndexed(static_cast<std::size_t>(row));
}

void MainWindow::keyPressEvent(QKeyEvent *event) {

    QTableWidgetItem *currentTableItem = matchesTable_->currentItem();
    if (currentTableItem == nullptr) {
        matchesTable_->selectRow(0);
        currentTableItem = matchesTable_->currentItem();
    }

    switch (event->key()) {
        case Qt::Key_D:
            fs::remove(currentTableItem->text().toStdString());
            matchesTable_->selectRow(currentTableItem->row() + 1);
            matchesTable_->removeRow(currentTableItem->row());
            break;

        case Qt::Key_N:
            toCheck_.erase(currentPath_);
            if (toCheck_.empty()) {
                QApplication::quit();
            }
            checkPicture(*toCheck_.begin());
            break;

        case Qt::Key_Down:
            if (currentTableItem->row() < matchesTable_->rowCount() - 1) {
                matchesTable_->selectRow(currentTableItem->row() + 1);
            }
            break;

        case Qt::Key_Up:
            if (currentTableItem->row() > 0) {
                matchesTable_->selectRow(currentTableItem->row() - 1);
            }
            break;

        default:
            break;
    }
}

} // ns imgdup