#include "mainwindow.h"

#include <QApplication>
#include <QPushButton>
#include <QObject>


int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow w;
    QObject::connect(&w, &MainWindow::quitGame, &a, &QApplication::quit);
    w.show();
    return a.exec();
}
