# legacy-driver-helloworld

A simple windows device driver and Win32 client demonstrating basic user/kernel mode communication techniques. The compiled code will run on Windows XP and newer versions of Windows including Vista, 7, and 8. 

## Prerequisites

You will need the following software:

* Windows 8.1 x64
* [Windows Driver Kit Version 7.1.0](https://www.microsoft.com/en-us/download/details.aspx?id=11800) 
* [Git for Windows](http://msysgit.github.io/)
* [Windows Sysinternals Suite](https://technet.microsoft.com/en-us/sysinternals/bb842062.aspx)

## Building and Running the Code

### 1. Clone the source code to your machine
 
```
git clone git@github.com:pmolchanov/legacy-driver-helloworld.git
```

NOTE: Make sure that the repository resides in a folder structure that does not contain spaces (this will cause fatal error U1087 at build time).

### 2. Add your Code Signing Certificates

64-bit versions of Windows Vista and newer require all driver code to be digitally signed. You will find two 0-byte placeholder certificates in the ``certs`` folder that you should replace with real certificates that you supply:
 
* certificate.pfx - This is the Software Publisher Certificate. You can purchase one of these from a Certificate Authority such as [GlobalSign](https://www.globalsign.com/en/code-signing/microsoft-authenticode/).
* cross-certificate.pfx - This is the Microsoft Cross-Certificate. This can be obtained freely from [Microsoft](https://msdn.microsoft.com/en-us/library/windows/hardware/dn170454.aspx). 


### 3. Update settings in the setenv.bat file
At a minimum, you will want to update the ``PFX_PWD=`` setting with the password that protects the PFX file containing your code signing certificate. You may also want to modify the ``BUILD_ENV=`` setting to change the build environment.   

### 4. Build the code

Open an Administrator Command Prompt and cd into the repository's root directory. Run the following command to initialize the WDK build environment: 

```
setenv.bat
```

To compile the Win32 client and kernel mode driver, run the following command: 

```
build.bat
```

### 5. Start DbgView

Start the Sysinternals DbgView.exe utility as Administrator and enable the ``Capture Kernel`` and ``Enable Verbose Kernel Output`` settings. Both Checked (Debug) and Free (Release) versions of the driver will output trace statements that can be viewed with this tool.  

### 6. Install the Device Driver

```
install.bat
```

NOTE: You should see trace statements in DbgView.

### 7. Run the Win32 client program

```
run.bat
```

NOTE: You should see trace statements in DbgView and in the Win32 client console. 

## References

* [Windows Driver Kit 7.1.0 Documentation in CHM format](http://download.microsoft.com/download/3/3/C/33CFEF4D-21DA-4229-BC17-3EAC7A7EABE1/WDKDocs_12112009.chm)
* [Programming the Windows Driver Model eBook](https://www-user.tu-chemnitz.de/~heha/oney_wdm/content.htm)
* [Kernel Mode Driver Tutorial: Part 1: The Skeleton KMD](http://www.reverse-engineering.info/SystemCoding/SkeletonKMD_Tutorial.htm)
* [Essentials Of Building Windows Drivers](http://www.wd-3.com/archive/winbuild.htm)
* [Getting Started Writing Windows Drivers](http://www.osronline.com/article.cfm?article=20)
* [Building and deploying a basic WDF Kernel Mode Driver](http://www.codeproject.com/Articles/13090/Building-and-deploying-a-basic-WDF-Kernel-Mode-Dri)
* [Driver Development Part 1: Introduction to Drivers](http://www.codeproject.com/Articles/9504/Driver-Development-Part-1-Introduction-to-Drivers)
* [Introduction to Device Drivers](http://www.catch22.net/tuts/introduction-device-drivers)
* [Userland/Kernel communication – DeviceIoControl method](http://ericasselin.com/userlandkernel-communication-deviceiocontrol-method)
* [TDI Overview](http://codemachine.com/article_tdi.html)
* [An Introduction To Writing TDI Filter Drivers](https://www.iseclab.org/papers/Writing_TDI_Drivers.pdf)
* [VirtualBox Windows Kernel Debugging Tips](https://www.virtualbox.org/wiki/Windows_Kernel_Debugging)
