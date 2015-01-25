# python_newport_1918_powermeter

Python module contains higher level functions to communicate with Newport 1918 power meter on windows. I use python ctypes to access low level functions in the Newport's usbdll.dll driver.

The power meter application and drivers can be found at 
http://www.newport.com/1918-R-HandHeld-Optical-Power-and-Energy-Meter/509478/1033/info.aspx#tab_Literature

On a new 32 bit computer: install the drivers and the power application found in the 
PowerMeter 3.0.2\win32
run both PMSetup32.msi and USBDriverSetup32.msi (in sub folder). this should
install drivers and the power meter at C:\Program Files\Newport
LIBNAME will be r'C:\Program Files\Newport\Newport USB Driver\Bin\usbdll.dll'

On a new 64bit computer: install the drivers and the power meter application found in 
PowerMeter 3.0.2\x86Onx64
run both PMSetup32on64.msi and USBDriverSetup32On64.msi (in sub folder). this should
install drivers and the power meter at C:\Program Files (x86)\Newport
LIBNAME will be r'C:\Program Files (x86)\Newport\Newport USB Driver\Bin\usbdll.dll'

Before you run this program make sure, you can run the newport power meter applicaiton. This will ensure that the drivers are installed properly and there is no problem communicating with the instrument.

For my case the product_id=0xCEC7. I am not sure if this will change on other computer.  
To Find the product_id or PID, 1) Open the Windows Device Manager, 2)Expand the Human Interface Devices node, 3)Double-click the device of interest -- the USB Human Interface Device Properties window appears, 4)Click the Details tab. 5) In the Property drop-down box, select Hardware Ids The product id is some thinglike PID_CEC7
(source: http://thecurlybrace.blogspot.com/2010/07/how-to-find-usb-device-vendor-and.html)

In the usbdll.dll, I am using the "Use USB Address" method to connect and read/write data. The methods that belong to this group are:

newp_usb_init_system - this function opens all USB instruments.
newp_usb_get_device_info - this function retrieves the USB address of all open instruments.
newp_usb_get_ascii - this function reads the response data from an instrument.
newp_usb_send_ascii - this function sends the passed in command to an instrument.
newp_usb_send_binary - this function sends the passed in binary data to an instrument.
newp_usb_uninit_system - this function closes all USB instruments.

There are many more methods in the dll, but these are sufficient for everything one might require.


