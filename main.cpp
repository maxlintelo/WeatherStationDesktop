#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QApplication>
#include <QtWidgets/QMainWindow>

QT_CHARTS_USE_NAMESPACE

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    return a.exec();
}
