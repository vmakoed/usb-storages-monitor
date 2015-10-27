#include "drivesscanner.h"

DrivesScanner::DrivesScanner(QObject *parent) : QObject(parent)
{

}

USBInfo *DrivesScanner::scan()
{
    USBInfo *info = new USBInfo;
    getUSBInfo(info);

    return info;
}

QString *DrivesScanner::getStorageName(char drive)
{
    USBInfo *info = new USBInfo;
    getUSBInfo(info);

    QString *name = new QString;

    for(int i = 0; i < info->storagesCapacity; i++)
    {
        if(info->driveLetters[i] == drive)
        {
            name->append(QString::fromLatin1(info->productNames[i]));
            break;
        }
    }

    if(name->isEmpty())
    {
        return nullptr;
    }
    else
    {
        return name;
    }
}

void DrivesScanner::startScan()
{
    USBInfo *info = new USBInfo;
    info->storagesCapacity = 0;
    info->driveLetters[0] = '\0';
    getUSBInfo(info);

    drivesCapacity = info->storagesCapacity;
    char *oldDrives = new char[MAX_NUM_OF_STORAGES];
    strcpy(oldDrives, info->driveLetters);

    while(true)
    {
        USBInfo *infoN = new USBInfo;
        infoN->storagesCapacity = 0;

        getUSBInfo(infoN);

        if(drivesCapacity != infoN->storagesCapacity)
        {
            emit drivesChanged(infoN);
            drivesCapacity = infoN->storagesCapacity;
        }

        Sleep(500);
    }
}
