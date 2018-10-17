#pragma once

#include "ImgDup.hpp"
#include <QMainWindow>
#include <QLabel>
#include <QTableWidget>
#include <opencv2/img_hash.hpp>
#include <boost/filesystem.hpp>
#include <set>

namespace imgdup {

class MainWindow : public QMainWindow {
public:
    MainWindow();

    void checkPicture(const boost::filesystem::path &path);
    void updateComparisonView();
    void displayMatchIndexed(std::size_t idx);
    void currentCellChanged(int row, int, int, int);

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    void startProcessing();

    void setupLayout();

    QWidget *central_;
    QWidget *leftContainer_;
    QWidget *rightContainer_;
    QLabel *leftImageView_;
    QLabel *rightImageView_;
    QLabel *leftImageLabel_;
    QLabel *rightImageLabel_;
    QTableWidget *matchesTable_;

    ImageProcessor<cv::img_hash::PHash> imageProcessor_;
    QString rootDir_;
    std::set<boost::filesystem::path> toCheck_;
    boost::filesystem::path currentPath_;
    std::vector<boost::filesystem::path> currentMatches_;
};

} // ns imgdup