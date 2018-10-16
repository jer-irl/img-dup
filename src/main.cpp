#include "MainWindow.hpp"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication::setApplicationName("img-dup");
    QApplication::setOrganizationName("Jeremy Schroeder");

    QApplication imgDupApp(argc, argv);

    imgdup::MainWindow mainWindow;
    mainWindow.show();

    return QApplication::exec();
}