#ifndef DRIVESEJECTOR_H
#define DRIVESEJECTOR_H

#include <QObject>
#include "RemoveDriveByLetter.h"

class DrivesEjector : public QObject
{
    Q_OBJECT
public:
    explicit DrivesEjector(QObject *parent = 0);
    void performDriveRemoval(char drive);

signals:
    void removalSuccessful();
    void removalFailed();

public slots:
};

#endif // DRIVESEJECTOR_H
