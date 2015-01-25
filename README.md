# python_newport_1918_powermeter

## Introduction

This python module contains higher level functions to communicate with [Newport 1918 power meter] (http://www.newport.com/1918-R-HandHeld-Optical-Power-and-Energy-Meter/509478/1033/info.aspx#tab_Overview) on a Windows computer. It uses python ctypes to access methods in the Newport's usbdll.dll driver. 

For connecting the hardware to the computer, see the [reference manual](http://assets.newport.com/webDocuments-EN/images/RevA1918-RPowerMeterUsersManual.pdf)


## Installing the powermeter and usbdriver 
[Click here to download the newport powermeter Application and driver](http://assets.newport.com/webDocuments-EN/images/Computer_Interface_Software_v3.0.2.zip)

* On a 32 bit computer:
	1. Unzip the file
	2. Install the drivers and the power application found PowerMeter 3.0.2\win32
	3. run both PMSetup32.msi and USBDriverSetup32.msi (in sub folder). This should install drivers and the newport powermeter application at C:\Program Files\Newport
	4. LIBNAME (argument that is needed to intialize the instrument) in the case  will be r'C:\Program Files\Newport\Newport USB Driver\Bin\usbdll.dll'

* On a 64 bit computer:
	1. Unzip the file.
	2. Install the drivers and the power application found PowerMeter 3.0.2\x86Onx64
	3. run both PMSetup32on64.msi and USBDriverSetup32on64.msi (in sub folder). This should install drivers and the newport powermeter application at C:\Program Files (x86)\Newport
	4. LIBNAME (argument that is needed to intialize the instrument) in the case  will be r'C:\Program Files (x86)\Newport\Newport USB Driver\Bin\usbdll.dll'

Before you run the python module, make sure that you can run the newport powermeter application. This will ensure that the drivers are installed properly and there is no problem communicating with the instrument.

## Getting the Product ID 

The variable "product_id" is needed to intialize the instrument. For my case, the product_id was 0xCEC7. I am not sure if this will change on another computer. To find the product_id or PID,
 
1. Open the Windows Device Manager
2. Expand the Human Interface Devices node
3. Double-click the device of interest -- the USB Human Interface Device Properties window appears
4. Click the Details tab. 
5. In the Property drop-down box, select Hardware Ids. The product id is some thinglike PID_ABC1, then use product_id = 0xABC1. 

[See Here fore more detailed explaination !] (http://thecurlybrace.blogspot.com/2010/07/how-to-find-usb-device-vendor-and.html)

## Additional notes
* There are many methods in the usbdll.dll, you can see all the methods in [NewPDll.h] (https://github.com/plasmon360/python_newport_1918_powermeter/blob/master/NewpDll.h). The methods I use to connect/disconnection and read/write commands are:

1. newp_usb_init_system - this function opens all USB instruments.
2. newp_usb_get_device_info - this function retrieves the USB address of all open instruments.
3. newp_usb_get_ascii - this function reads the response data from an instrument.
4. newp_usb_send_ascii - this function sends the passed in command to an instrument.
5. newp_usb_send_binary - this function sends the passed in binary data to an instrument.
6. newp_usb_uninit_system - this function closes all USB instruments.

* A full list of commands that can be sent to the instrument can be found in the [reference manual](http://assets.newport.com/webDocuments-EN/images/RevA1918-RPowerMeterUsersManual.pdf)


