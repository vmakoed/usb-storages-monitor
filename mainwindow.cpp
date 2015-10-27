#include "mainwindow.h"
#include <QVBoxLayout>
#include <QDebug>
#include <QMessageBox>
#include <QMouseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{    
    createListBox();
    composeMainWindow();

    drivesScanner = new DrivesScanner;
    drivesEjector = new DrivesEjector;

    connect(drivesScanner, &DrivesScanner::drivesChanged, this, &MainWindow::resetDrivesList);
    connect(drivesEjector, &DrivesEjector::removalFailed, this, &MainWindow::displayRemovalError);

    resetDrivesList(drivesScanner->scan());

    scanThread = new QThread;
    drivesScanner->moveToThread(scanThread);

    connect(scanThread, &QThread::started, drivesScanner, &DrivesScanner::startScan);
    scanThread->start();
}

MainWindow::~MainWindow()
{

}

void MainWindow::resetDrivesList(USBInfo *info)
{
    drivesListBox->clear();
    drivesList->clear();

    QList<Drive*> *foundDrivesList = new QList<Drive*>;

    for (int i = 0; i < info->storagesCapacity; i++)
    {
        foundDrivesList->append(new Drive(info->driveLetters[i], info->productNames[i]));
    }

    if (foundDrivesList->isEmpty() == false) setDrives(foundDrivesList);
}

void MainWindow::displayRemovalError()
{
    QMessageBox::information(this, tr("USB Monitor"), tr("The drive is still in use."));
}

void MainWindow::createListBox()
{
    drivesListBox = new QListWidget;
    drivesList = new QList<DriveWidget*>;

    drivesListBox->setSelectionMode(QAbstractItemView::NoSelection);
}

void MainWindow::composeMainWindow()
{
    title = new QLabel("USB Monitor");
    title->setObjectName("app-title");

    closeButton = new QPushButton;
    closeButton->setObjectName("close-button");
    connect(closeButton, &QPushButton::clicked, this, &MainWindow::close);

    QHBoxLayout *headerLayout = new QHBoxLayout;
    headerLayout->addWidget(title);
    headerLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Fixed));
    headerLayout->addWidget(closeButton);

    QWidget *header = new QWidget;
    header->setLayout(headerLayout);
    header->setObjectName("header");

    QVBoxLayout *rootLayout = new QVBoxLayout;
    rootLayout->addWidget(header);
    rootLayout->addWidget(drivesListBox);
    rootLayout->setContentsMargins(0, 0, 0, 0);

    QWidget *centralWidget = new QWidget;
    centralWidget->setLayout(rootLayout);
    this->setCentralWidget(centralWidget);

    this->setWindowFlags(Qt::FramelessWindowHint);
}

void MainWindow::setDrives(QList<Drive*> *srcDrivesList)
{
    for(int i = 0; i < srcDrivesList->size(); i++)
    {
        addDrive(srcDrivesList->at(i));
    }
}

void MainWindow::addDrive(Drive *drive)
{
    DriveWidget *driveWidget = new DriveWidget(drive);
    drivesList->append(driveWidget);

    connect(driveWidget, &DriveWidget::ejectDriveButtonPressed, drivesEjector, &DrivesEjector::performDriveRemoval);

    drivesListBox->addItem(new QListWidgetItem);
    drivesListBox->setItemWidget(drivesListBox->item(drivesListBox->count() - 1), drivesList->at(drivesListBox->count() - 1));
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    clickX = event->x();
    clickY = event->y();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    move(event->globalX() - clickX, event->globalY() - clickY);
}

