#include "drivewidget.h"
#include <QHBoxLayout>

#include <QDebug>

DriveWidget::DriveWidget(Drive *srcdrive)
{  
    drive = srcdrive;

    mountPointLabel = new QLabel(*(new QString(drive->getMountPoint())));
    mountPointLabel->setObjectName("mount-point-label");

    productLabel = new QLabel(QString(drive->getProduct()));
    productLabel->setObjectName("product-label");

    ejectDriveButton = new QPushButton("Eject");
    ejectDriveButton->setObjectName("eject-button");

    connect(ejectDriveButton, &QPushButton::clicked, this, &DriveWidget::sendEjectSignal);

    QHBoxLayout *rootLayout = new QHBoxLayout;
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);

    rootLayout->addWidget(mountPointLabel);
    rootLayout->addWidget(productLabel);
    rootLayout->addSpacerItem(new QSpacerItem(10, 0, QSizePolicy::Expanding, QSizePolicy::Fixed));
    rootLayout->addWidget(ejectDriveButton);
    rootLayout->setAlignment(Qt::AlignLeft);
    setLayout(rootLayout);
}

Drive *DriveWidget::getDrive() const
{
    return drive;
}

void DriveWidget::setDrive(Drive *value)
{
    drive = value;
}

void DriveWidget::sendEjectSignal()
{
    emit ejectDriveButtonPressed(drive->getMountPoint());
}

