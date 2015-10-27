#include "mainwindow.h"
#include "drivesscanner/drivesscanner.h"
#include "drive.h"
#include "drivewidget.h"
#include <QApplication>
#include <QFile>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile File(":qss/stylesheet.qss");
    File.open(QFile::ReadOnly);
    QString StyleSheet = QLatin1String(File.readAll());
    a.setStyleSheet(StyleSheet);

    MainWindow w;
    w.show();

    return a.exec();
}
