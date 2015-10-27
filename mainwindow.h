#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QListWidget>
#include <QThread>

#include "drivewidget.h"
#include "drivesscanner/drivesscanner.h"
#include "drivesejector/drivesejector.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QLabel *title;
    QPushButton *closeButton;

    QList<DriveWidget*> *drivesList;
    QListWidget *drivesListBox;

    DrivesScanner *drivesScanner;
    DrivesEjector *drivesEjector;

    USBInfo *info;

    QThread *scanThread;

    int clickX;
    int clickY;

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void createListBox();
    void composeMainWindow();
    void setDrives(QList<Drive *> *srcDrivesList);
    void addDrive(Drive *drive);
    void resetDrivesList(USBInfo *info);
    void displayRemovalError();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
};

#endif // MAINWINDOW_H
