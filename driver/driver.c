#include <Ntddk.h>
#include <ntstrsafe.h>

#include "driver.h"
#include "../common/common.h"

#define TRACE(...) KdPrint(("HelloWorld: "__VA_ARGS__))

extern PDEVICE_OBJECT g_pDeviceObject = NULL;
extern UNICODE_STRING g_DeviceName = RTL_CONSTANT_STRING(L"\\Device\\HelloWorld");
extern UNICODE_STRING g_DosDeviceName = RTL_CONSTANT_STRING(L"\\??\\HelloWorld");

// This is the first routine called after a driver is loaded, and is responsible 
// for initializing the driver. 
NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath)
{
	int i = 0;
	NTSTATUS status = STATUS_UNSUCCESSFUL;
  
  PAGED_CODE();
  
  TRACE(">DriverEntry PID=%d TID=%d \r\n", PsGetCurrentProcessId(), PsGetCurrentThreadId());
  
  TRACE("RegistryPath: %wZ \r\n", RegistryPath);

  do
  {
    // Create a device object
    status = IoCreateDevice(
      DriverObject,
      0,
      &g_DeviceName,
      FILE_DEVICE_UNKNOWN,
      // Verify that someone has the right to open a handle to a device even if 
      // additional name components are present
      FILE_DEVICE_SECURE_OPEN, 
      // TRUE = Exclusive. The I/O manager allows only one handle to be opened 
      // by normal means to an exclusive device.
      FALSE,
      &g_pDeviceObject);
    if(!NT_SUCCESS(status))
    {
      TRACE("IoCreateDevice Error: %08X \r\n", status);
      break;
    }
    
    // Create a symbolic link. The symbolic link is optional for the functioning
    // of the driver, BUT is required for the user mode application to be able 
    // to communicate with your driver using the CreateFile() API. 
    status = IoCreateSymbolicLink(&g_DosDeviceName, &g_DeviceName);
    if(!NT_SUCCESS(status))
    {
      TRACE("IoCreateSymbolicLink Error: %08X \r\n", status);
      break;
    }

    // Configure a single DriverDispatch function to process all IRP types
    for (i = 0; i <= IRP_MJ_MAXIMUM_FUNCTION; i++)
    {
      DriverObject->MajorFunction[i] = DriverDispatch;
    }
    
    // Configure a DriverUnload function that is called by the I/O manager just
    // prior to unloading the driver
    DriverObject->DriverUnload = DriverUnload;

    // When the I/O manager creates an IRP_MJ_READ or IRP_MJ_WRITE request, it 
    // inspects the DO_DIRECT_IO and DO_BUFFERED_IO flags to decide how to 
    // describe the data buffer in the new I/O request packet (IRP). This 
    // decision is final and cannot be changed after the DriverEntry routine 
    // completes. 
    g_pDeviceObject->Flags |= DO_BUFFERED_IO;
    
    // While this flag is set, the I/O manager will refuse to attach other 
    // device objects to yours or to open a handle to your device. You have to 
    // clear the flag because your device object initially arrives in the world 
    // with the flag set. When DriverEntry returns, the I/O manager 
    // automatically traverses the list of device objects linked from the driver 
    // object and clears this flag, so strictly speaking, clearing this flag 
    // here is unnecessary, but informative. 
    g_pDeviceObject->Flags &= (~DO_DEVICE_INITIALIZING);

  } while(0);

  TRACE("<DriverEntry \r\n");

  // If an error status is returned, the DriverUnload routine will not be called. 
	return status;
}

// This routine is the primary conduit for communication with user mode
// programs. It processes IRPs sent from the I/O manager (The I/O manager 
// intermediates user/kernel mode communication). Each IRP that the I/O 
// manager passes down is assigned a major function code of the form IRP_MJ_XXX. 
// These codes tell the driver what sort of operation it should perform to 
// satisfy the I/O request.
NTSTATUS DriverDispatch(IN PDEVICE_OBJECT DeviceObject, IN PIRP irp)
{
  NTSTATUS status = STATUS_UNSUCCESSFUL;
  PIO_STACK_LOCATION irpStack = NULL;
  
  TRACE(">DriverDispatch PID=%d TID=%d \r\n", PsGetCurrentProcessId(), PsGetCurrentThreadId());
  
  // This is set to a operation dependent value. For example, on successful 
  // completion of an IRP_MJ_READ, this is set to the number of bytes read.
  irp->IoStatus.Information = 0;
  
  // Get the I/O stack location to determine the major function code for (what 
  // operation is being requested).
  irpStack = IoGetCurrentIrpStackLocation(irp);
  TRACE("IRP Major Function Code: 0x%08X \r\n", irpStack->MajorFunction);
  switch (irpStack->MajorFunction)
  {
    // Sent when the user mode application has called the CreateFile function.
    case IRP_MJ_CREATE:
     TRACE("IRP_MJ_CREATE: %wZ \r\n", &irpStack->FileObject->FileName);
     status = STATUS_SUCCESS;
     break;
    
    // Sent when the user mode application has called the ReadFile function.
    case IRP_MJ_READ:
     TRACE("IRP_MJ_READ \r\n");
     RtlStringCbCopyA(irp->AssociatedIrp.SystemBuffer, irpStack->Parameters.Read.Length, "IRP_MJ_READ");
     irp->IoStatus.Information = irpStack->Parameters.Read.Length;
     status = STATUS_SUCCESS;
     break;
    
    // Sent when the user mode application has called the WriteFile function.
    case IRP_MJ_WRITE:
     TRACE("IRP_MJ_WRITE: %s \r\n", irp->AssociatedIrp.SystemBuffer);
     status = STATUS_SUCCESS;
     break;
    
    // Sent when the user mode application has called the DeviceIoControl function.
    case IRP_MJ_DEVICE_CONTROL: 
     TRACE("IRP_MJ_DEVICE_CONTROL \r\n");
     if(irpStack->Parameters.DeviceIoControl.IoControlCode == IOCTL_REQUEST_RESPONSE) {
      // irpStack->Parameters.DeviceIoControl.InputBufferLength
      TRACE("IOCTL_REQUEST_RESPONSE: %s \r\n", irp->AssociatedIrp.SystemBuffer);
      RtlStringCbCopyA(irp->AssociatedIrp.SystemBuffer, irpStack->Parameters.DeviceIoControl.OutputBufferLength, "IRP_MJ_DEVICE_CONTROL");
      irp->IoStatus.Information = irpStack->Parameters.DeviceIoControl.OutputBufferLength;
      status = STATUS_SUCCESS;
     } else {
        status = STATUS_NOT_IMPLEMENTED;
     }
     break; 
    
    // Sent when the last handle for a file object that is associated with the 
    // target device object has been closed (but, due to outstanding I/O 
    // requests, might not have been released).
    case IRP_MJ_CLEANUP:
     TRACE("IRP_MJ_CLEANUP \r\n");
     status = STATUS_SUCCESS;
     break;
    
    // Sent when the last handle of the file object that is associated with the 
    // target device object has been closed and released. All outstanding I/O 
    // requests have been completed or cancelled.
    case IRP_MJ_CLOSE:
     TRACE("IRP_MJ_CLOSE \r\n");
     status = STATUS_SUCCESS;
     break;
    
    // Default handling for IRP types that are not supported.
    default:
     TRACE("IRP_MJ_XXX \r\n");
     status = STATUS_NOT_IMPLEMENTED;
     break;
  }
  
  // Complete the IRP by setting the completion status, calling IoCompleteRequest
  // to return the IRP to the I/O manager, returning the completion status. 
  irp->IoStatus.Status = status;
  IoCompleteRequest(irp, IO_NO_INCREMENT);
  TRACE("<DriverDispatch \r\n");
  return status;
}

// This routine is called just prior to the driver being unloaded. It should 
// clean up after any global initialization that DriverEntry might have done.
void DriverUnload(IN PDRIVER_OBJECT DriverObject)
{
  NTSTATUS status = STATUS_UNSUCCESSFUL;

  TRACE(">DriverUnload PID=%d TID=%d \r\n", PsGetCurrentProcessId(), PsGetCurrentThreadId());

  // Delete symbolic link.
  status = IoDeleteSymbolicLink(&g_DosDeviceName);
  if(!NT_SUCCESS(status))
  {
    TRACE("IoDeleteSymbolicLink Error: %08X \r\n", status);
  }

  // Delete device object.
  IoDeleteDevice(g_pDeviceObject);
  g_pDeviceObject = NULL;

  TRACE("<DriverUnload \r\n");
}
