#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <strsafe.h>

#include "../common/common.h"

int _cdecl main(void)
{
  HANDLE hFile = NULL;
  DWORD dwError = 0;
  char buffer[SHARED_BUFFER_SIZE] = {0};
  BOOL bSuccess = FALSE;
  DWORD dwBytesRead = 0;
  
  printf("PID=%d TID=%d \r\n", GetCurrentProcessId(), GetCurrentThreadId());
  
  do 
  {
    
    // Open a handle to the device. Note: When you attempt to open a handle to a 
    // device from a Win32 process, symbolic links in the Global DOS Devices 
    // namespace are used to interpret device names. 
    hFile = CreateFile(
     "\\??\\HelloWorld\\Foobar",
     GENERIC_READ | GENERIC_WRITE, 
     0, 
     NULL,
     OPEN_EXISTING, 
     0, 
     NULL);
     if(hFile == INVALID_HANDLE_VALUE) 
     {
       dwError = GetLastError();
       printf("CreateFile Error: %08X \r\n", dwError);
       break;
     }
     
     // Read data from the device using the ReadFile API
     bSuccess = ReadFile(
       hFile,
       buffer,
       sizeof(buffer),
       &dwBytesRead,
       NULL);
     if(bSuccess == 0)
     {
       dwError = GetLastError();
       printf("ReadFile Error: %08X \r\n", dwError);
       break;
     }
     printf("ReadFile Result: %s \r\n",buffer);
     
     // Write data to the device using the WriteFile API
     StringCchCopyA(buffer,_countof(buffer),"WriteFile");
     bSuccess = WriteFile(hFile, buffer, sizeof(buffer), NULL, NULL);
     if(bSuccess == 0)
     {
       dwError = GetLastError();
       printf("WriteFile Error: %08X \r\n", dwError);
       break;
     }
    
     // Write/Read data to/from the device using the DeviceIoControl API
     StringCchCopyA(buffer,_countof(buffer),"DeviceIoControl");
     bSuccess = DeviceIoControl(
       hFile,
       IOCTL_REQUEST_RESPONSE,
       buffer,
       sizeof(buffer),
       buffer,
       sizeof(buffer),
       &dwBytesRead,
       NULL
     );
     if(bSuccess == 0)
     {
       dwError = GetLastError();
       printf("DeviceIoControl Error: %08X \r\n", dwError);
       break;
     }
     printf("DeviceIoControl Result: %s",buffer);
  
  } while(0);
  
  // Close handle to the device
  if(hFile)
  {
   CloseHandle(hFile);
   hFile = NULL;
  }
  
  return 0;
}