# legacy-driver-helloworld

A simple windows device driver and Win32 client demonstrating basic user/kernel mode communication techniques. The compiled code will run on Windows XP and newer versions of Windows including Vista, 7, and 8. 

## Prerequisites

You will need the following software:

* Windows 8.1 x64
* [Windows Driver Kit Version 7.1.0](https://www.microsoft.com/en-us/download/details.aspx?id=11800) 
* [Git for Windows](http://msysgit.github.io/)
* [Windows Sysinternals Suite](https://technet.microsoft.com/en-us/sysinternals/bb842062.aspx)

## Building the Code

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

Start the Sysinternals DbgView.exe utility as Administrator and enable the ``Capture Kernel`` and ``Enable Verbose Kernel Output`` settings. Checked (debug) versions of the driver will output trace statements that can be viewed with this tool.  

### 6. Install the Device Driver

```
install.bat
```

NOTE: If you are installing a checked version of the driver, you should see trace statements in DbgView.

### 7. Run the Win32 client program

```
run.bat
```

NOTE: If you are installing a checked version of the driver, you should see trace statements in DbgView. You should also see trace statements appear in the console. 
