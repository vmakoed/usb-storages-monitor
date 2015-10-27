#include "drivesejector.h"

DrivesEjector::DrivesEjector(QObject *parent) : QObject(parent)
{

}

void DrivesEjector::removeDrive(char drive)
{
    if (removeDrive(drive))
    {
        emit removalSuccessful();
    }
    else
    {
        emit removalFailed();
    }
}

