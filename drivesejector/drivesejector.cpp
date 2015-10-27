#include "drivesejector.h"

DrivesEjector::DrivesEjector(QObject *parent) : QObject(parent)
{

}

void DrivesEjector::performDriveRemoval(char drive)
{
    if(removeDrive(drive))
    {
        emit removalFailed();
    }
    else
    {
        emit removalSuccessful();
    }
}

