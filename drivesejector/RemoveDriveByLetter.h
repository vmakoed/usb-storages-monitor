#include <stdio.h>

#include <windows.h>

#include <Setupapi.h>
#include <winioctl.h>
#include <winioctl.h>
#include <cfgmgr32.h>

#pragma comment (lib, "setupapi.lib")

//-------------------------------------------------
DEVINST GetDrivesDevInstByDeviceNumber(long DeviceNumber, UINT DriveType, char* szDosDeviceName);
int removeDrive(char drive);
//-------------------------------------------------