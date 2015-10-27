#ifndef DRIVEWIDGET_H
#define DRIVEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>

#include "drive.h"

class DriveWidget : public QWidget
{
    Q_OBJECT

private:
    Drive *drive;

    QLabel *mountPointLabel;
    QLabel *productLabel;

    QPushButton *ejectDriveButton;

public:
    explicit DriveWidget(Drive *srcdrive);

    Drive *getDrive() const;
    void setDrive(Drive *value);

    void sendEjectSignal();
signals:
    void ejectDriveButtonPressed(char drive);

public slots:
};

#endif // DRIVEWIDGET_H
