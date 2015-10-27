#include "RemoveDriveByLetter.h"

int removeDrive(char drive)
{
	char driveLetter = drive;
    driveLetter &= ~0x20;

	if ( driveLetter < 'A' || driveLetter > 'Z' ) {
		return 1;
	}

	wchar_t szRootPath[] = L"X:\\";   // "X:\"  -> for GetDriveType
	szRootPath[0] = driveLetter;

	wchar_t szDevicePath[] = L"X:";   // "X:"   -> for QueryDosDevice
	szDevicePath[0] = driveLetter;

	wchar_t szVolumeAccessPath[] = L"\\\\.\\X:";   // "\\.\X:"  -> to open the volume
	szVolumeAccessPath[4] = driveLetter;

	long DeviceNumber = -1;

	HANDLE hVolume = CreateFile(szVolumeAccessPath, 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL);
	if (hVolume == INVALID_HANDLE_VALUE) {
		return 1;
	}

	STORAGE_DEVICE_NUMBER sdn;
	DWORD dwBytesReturned = 0;
	long res = DeviceIoControl(hVolume, IOCTL_STORAGE_GET_DEVICE_NUMBER, NULL, 0, &sdn, sizeof(sdn), &dwBytesReturned, NULL);
	if ( res ) {
		DeviceNumber = sdn.DeviceNumber;
	}
	CloseHandle(hVolume);

	if ( DeviceNumber == -1 ) {
		return 1;
	}

	UINT DriveType = GetDriveType(szRootPath);

	char szDosDeviceName[MAX_PATH];
	res = QueryDosDevice(szDevicePath, (LPWSTR)szDosDeviceName, MAX_PATH);
	if ( !res ) {
		return 1;
	}

	DEVINST DevInst = GetDrivesDevInstByDeviceNumber(DeviceNumber, DriveType, szDosDeviceName);

	if ( DevInst == 0 ) {
		return 1;
	}

	PNP_VETO_TYPE VetoType = PNP_VetoTypeUnknown; 
	WCHAR VetoNameW[MAX_PATH];
	VetoNameW[0] = 0;
	bool bSuccess = false;


	DEVINST DevInstParent = 0;
	res = CM_Get_Parent(&DevInstParent, DevInst, 0); 

    for ( long tries=1; tries<=3; tries++ ) {
		VetoNameW[0] = 0;

        res = CM_Request_Device_EjectW(DevInstParent, &VetoType, VetoNameW, MAX_PATH, 0);
        bSuccess = (res==CR_SUCCESS && VetoType==PNP_VetoTypeUnknown);
		if ( bSuccess )  { 
			break;
		}

        Sleep(500);
	}

	if ( bSuccess ) {
		printf("Success\n\n");
		return 0;
	}

	printf("failed\n");
	
	printf("Result=0x%2X\n", res);

	if ( VetoNameW[0] ) {
		printf("VetoName=%ws)\n\n", VetoNameW);
	}	
	return 1;
}


DEVINST GetDrivesDevInstByDeviceNumber(long DeviceNumber, UINT DriveType, char* szDosDeviceName)
{
	bool IsFloppy = (strstr(szDosDeviceName, "\\Floppy") != NULL); // who knows a better way?

	GUID* guid;

	switch (DriveType) {
	case DRIVE_REMOVABLE:
		if ( IsFloppy ) {
			guid = (GUID*)&GUID_DEVINTERFACE_FLOPPY;
		} else {
			guid = (GUID*)&GUID_DEVINTERFACE_DISK;
		}
		break;
	case DRIVE_FIXED:
		guid = (GUID*)&GUID_DEVINTERFACE_DISK;
		break;
	case DRIVE_CDROM:
		guid = (GUID*)&GUID_DEVINTERFACE_CDROM;
		break;
	default:
		return 0;
	}

	// Get device interface info set handle for all devices attached to system
	HDEVINFO hDevInfo = SetupDiGetClassDevs(guid, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);

	if (hDevInfo == INVALID_HANDLE_VALUE)	{
		return 0;
	}

	// Retrieve a context structure for a device interface of a device information set
	DWORD dwIndex = 0;
	long res;

	BYTE Buf[1024];
	PSP_DEVICE_INTERFACE_DETAIL_DATA pspdidd = (PSP_DEVICE_INTERFACE_DETAIL_DATA)Buf;
	SP_DEVICE_INTERFACE_DATA         spdid;
	SP_DEVINFO_DATA                  spdd;
	DWORD                            dwSize;
	
	spdid.cbSize = sizeof(spdid);

	while ( true )	{
		res = SetupDiEnumDeviceInterfaces(hDevInfo, NULL, guid, dwIndex, &spdid);
		if ( !res ) {
			break;
		}

		dwSize = 0;
		SetupDiGetDeviceInterfaceDetail(hDevInfo, &spdid, NULL, 0, &dwSize, NULL); // check the buffer size

		if ( dwSize!=0 && dwSize<=sizeof(Buf) ) {

			pspdidd->cbSize = sizeof(*pspdidd); // 5 Bytes!

			ZeroMemory(&spdd, sizeof(spdd));
			spdd.cbSize = sizeof(spdd);

			long res = SetupDiGetDeviceInterfaceDetail(hDevInfo, &spdid, pspdidd, dwSize, &dwSize, &spdd);
			if ( res ) {
				// open the disk or cdrom or floppy
				HANDLE hDrive = CreateFile(pspdidd->DevicePath, 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
				if ( hDrive != INVALID_HANDLE_VALUE ) {
					// get its device number
					STORAGE_DEVICE_NUMBER sdn;
					DWORD dwBytesReturned = 0;
					res = DeviceIoControl(hDrive, IOCTL_STORAGE_GET_DEVICE_NUMBER, NULL, 0, &sdn, sizeof(sdn), &dwBytesReturned, NULL);
					if ( res ) {
						if ( DeviceNumber == (long)sdn.DeviceNumber ) {  // match the given device number with the one of the current device
							CloseHandle(hDrive);
							SetupDiDestroyDeviceInfoList(hDevInfo);
							return spdd.DevInst;
						}
					}
					CloseHandle(hDrive);
				}
			}
		}
		dwIndex++;
	}

	SetupDiDestroyDeviceInfoList(hDevInfo);

	return 0;
}

