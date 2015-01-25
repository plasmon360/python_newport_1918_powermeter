///////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright(c) 2011 by Newport Corporation
// All Rights Reserved Worldwide
//-----------------------------------------------------------------------
// Module:		NewpDll.h
// Description:	Header file for USB interface DLL
///////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __usbdll_h__
#define __usbdll_h__

#ifdef __cplusplus
extern "C"
{
#undef  EXPORT
#define EXPORT _stdcall
#else
#define EXPORT EXTERN_C _stdcall
#endif 

#define USBDUPLICATEADDRESS 1	// More than one device on the bus has the same device ID
#define USBADDRESSNOTFOUND -2	// The device ID cannot be found among the open devices on the bus
#define USBINVALIDADDRESS -3	// The device ID is outside the valid range of 0 - 31

typedef void (__stdcall *DeviceStateChanged)(int handle, int nState);
extern DeviceStateChanged g_lpDeviceStateChangedCB;

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Description:
// This function opens all devices on the USB bus.  This function simply calls 'newp_usb_open_devices' 
// with 'nProductID' set to zero and with 'bUseUSBAddress' set to true.  'newp_usb_open_devices' must 
// be called before any of the other USB functions are called.
//
// Returns: 
// Zero for success, non-zero for failure.
///////////////////////////////////////////////////////////////////////////////////////////////////////
long EXPORT newp_usb_init_system (void);

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Description:
// This function opens the devices on the USB bus with the specified product ID.  This function simply 
// calls 'newp_usb_open_devices' with the passed in 'nProductID' and with 'bUseUSBAddress' set to true.  
// 'newp_usb_open_devices' must be called before any of the other USB functions are called.
//
// Parameters:
// nProductID:		The product ID (from NewportPwrMtr.inf), where zero means all products.
//
// Returns: 
// Zero for success, non-zero for failure.
///////////////////////////////////////////////////////////////////////////////////////////////////////
long EXPORT newp_usb_init_product (int nProductID);

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Description:
// This function opens the devices on the USB bus with the specified product ID and allows the USB 
// addressing scheme to be specified.  'newp_usb_open_devices' must be called before any of the other 
// USB functions are called.
//
// Parameters:
// nProductID:		The product ID (from NewportPwrMtr.inf), where zero means all products.
// bUseUSBAddress:	If true then the 'DeviceID' (used in other functions below) is the device's USB 
//					address, otherwise the 'DeviceID' is the index into the device information (which 
//					eliminates USB address conflicts).  This flag should be false if 'DeviceKey' will
//					be used instead of 'DeviceID' to reference an open device on the USB bus.
// nNumDevices:		Returns the number of devices that were opened.
//
// Returns: 
// Zero for success, non-zero for failure.
///////////////////////////////////////////////////////////////////////////////////////////////////////
long EXPORT newp_usb_open_devices (int nProductID, bool bUseUSBAddress, int* nNumDevices);

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Description:
// This function closes all devices on the USB bus.  After it is called, no USB communication can occur 
// until 'newp_usb_open_devices' is called again.
///////////////////////////////////////////////////////////////////////////////////////////////////////
void EXPORT newp_usb_uninit_system (void);

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Description:
// This function initializes event handling for devices with the specified product ID.  When a device
// is attached to the USB bus it is opened without affecting the other devices that are open or
// communicating.  When a device is detached from the USB bus it is closed without affecting the other 
// devices that are open or communicating.  This function must be called before any of the other USB
// functions are called.
//
// Parameters:
// nProductID:				The product ID (from NewportPwrMtr.inf), where zero means all products.
// lpDeviceStateChangedCB:	The address of the DeviceStateChanged callback function.
//
// Returns: 
// Zero for success, non-zero for failure.
///////////////////////////////////////////////////////////////////////////////////////////////////////
long EXPORT newp_usb_event_init (int nProductID, DeviceStateChanged lpDeviceStateChangedCB);

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Description:
// This function assigns a unique key to the device referenced by 'handle' and adds it to the list of 
// attached devices.  This allows an identifier that is more meaningful than the 'handle' to be used 
// to reference a specific device.
//
// Parameters:
// DeviceKey:		The device key.
// handle:			The device handle.
//
// Returns: 
// Zero for success, non-zero for failure.
///////////////////////////////////////////////////////////////////////////////////////////////////////
long EXPORT newp_usb_event_assign_key (char* DeviceKey, int handle);

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Description:
// This function removes a device key from the list of attached devices.
//
// Parameters:
// DeviceKey:		The device key.
//
// Returns: 
// Zero for success, non-zero for failure.
///////////////////////////////////////////////////////////////////////////////////////////////////////
long EXPORT newp_usb_event_remove_key (char* DeviceKey);

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Description:
// This function retrieves two arrays of equal size that represent the attached devices:  
// one of device keys and another of device handles.
//
// Parameters:
// ppDeviceKeys:	A pointer to an array of null terminated characters, 
//					where each element contains the device key.
// pDeviceHandles:	An integer array of device handles.
//
// Returns: 
// Zero for success, non-zero for failure.
///////////////////////////////////////////////////////////////////////////////////////////////////////
long EXPORT newp_usb_event_get_attached_devices (char** ppDeviceKeys, int* pDeviceHandles);

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Description:
// This function retrieves the device key that is associated with the specified device handle.
//
// Parameters:
// handle:			The device handle.
// DeviceKey:		The device key.
//
// Returns: 
// Zero for success, non-zero for failure.
///////////////////////////////////////////////////////////////////////////////////////////////////////
long EXPORT newp_usb_event_get_key_from_handle (int handle, char* DeviceKey);

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Description:
// This function retrieves device information for all devices that are open on the USB bus.  This 
// function must be called in order to determine the proper 'DeviceID' for each open device.  The data
// is returned in a character buffer in the following format:
//    <DeviceID1>,<DeviceDescription1>;<DeviceID2>,<DeviceDescription2>;<DeviceIDX>,<DeviceDescriptionX>;
// The data for each device is separated by a semicolon and the data for a device is comma delimited.
// Each 'DeviceID' must be converted to an integer in order to be used with 'newp_usb_get_ascii' or
// 'newp_usb_send_ascii'.  The device description data is the same response that is returned by a 
// "*IDN?" query.
//
// Parameters:
// Buffer:			Character buffer used to hold the device information.
//
// Returns: 
// Zero for success, non-zero for failure.
///////////////////////////////////////////////////////////////////////////////////////////////////////
long EXPORT newp_usb_get_device_info (char* Buffer);

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Description:
// This function retrieves an array of device keys used to specify a particular device that is open on 
// the USB bus.  Each key is unique and consists of the Model and Serial Number strings concatenated 
// together (which eliminates USB address conflicts).  This function must be called before referencing 
// an open device by 'DeviceKey'.  Calling this function eliminates the need to call 
// 'newp_usb_get_device_info', and allows for the option (if needed) of referencing an open device by 
// 'DeviceID' since the array index is the 'DeviceID'.
//
// Parameters:
// ppBuffer:		A pointer to an array of null terminated characters, where the array index is the
//					'DeviceID' and each element contains the Model / Serial Number key.
//
// Returns: 
// Zero for success, non-zero for failure.
///////////////////////////////////////////////////////////////////////////////////////////////////////
long EXPORT newp_usb_get_model_serial_keys (char** ppBuffer);

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Description:
// This function retrieves device information for all devices that are open on the USB bus.
//
// arInstruments:		Integer array of device IDs, where each ID is the USB address.
// arInstrumentsModel:	Integer array of model numbers.
// arInstrumentsSN:		Integer array of serial numbers.
// nArraySize:			An integer that tells how many elements are in the integer arrays.
//
// Returns: 
// Zero for success, non-zero for failure.
///////////////////////////////////////////////////////////////////////////////////////////////////////
long EXPORT GetInstrumentList (int* arInstruments, int* arInstrumentsModel, int* arInstrumentsSN, int* nArraySize);

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Description:
// This function reads the response data from the specified device.
//
// Parameters:
// DeviceKey:		The device key.
// Buffer:			Character buffer used to hold the response data.
// Length:			The length of the character buffer.
// BytesRead:		The number of bytes read.
//
// Returns: 
// Zero for success, non-zero for failure.
///////////////////////////////////////////////////////////////////////////////////////////////////////
long EXPORT newp_usb_read_by_key (char* DeviceKey, char* Buffer, unsigned long Length, unsigned long* BytesRead);

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Description:
// This function reads the response data from the specified device.
//
// Parameters:
// DeviceID:		The USB address of the device, or the index into the device information.  The valid 
//					range is from 0 - 31.
// Buffer:			Character buffer used to hold the response data.
// Length:			The length of the character buffer.
// BytesRead:		The number of bytes read.
//
// Returns: 
// Zero for success, non-zero for failure.
///////////////////////////////////////////////////////////////////////////////////////////////////////
long EXPORT newp_usb_get_ascii (long DeviceID, char* Buffer, unsigned long Length, unsigned long* BytesRead);

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Description:
// This function sends the passed in command to the specified device.
//
// Parameters:
// DeviceKey:		The device key.
// Command:			Character buffer used to hold the command.
// Length:			The length of the character buffer.
//
// Returns: 
// Zero for success, non-zero for failure.
///////////////////////////////////////////////////////////////////////////////////////////////////////
long EXPORT newp_usb_write_by_key (char* DeviceKey, char* Command, unsigned long Length);

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Description:
// This function sends the passed in command to the specified device.
// This function concatenates a carriage-return to the command string if it does not have one.
//
// Parameters:
// DeviceID:		The USB address of the device, or the index into the device information.  The valid 
//					range is from 0 - 31.
// Command:			Character buffer used to hold the command.
// Length:			The length of the character buffer.
//
// Returns: 
// Zero for success, non-zero for failure.
///////////////////////////////////////////////////////////////////////////////////////////////////////
long EXPORT newp_usb_send_ascii (long DeviceID, char* Command, unsigned long Length);

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Description:
// This function sends the passed in binary data to the specified device.
//
// Parameters:
// DeviceKey:		The device key.
// Command:			Character buffer used to hold the command.
// Length:			The length of the character buffer.
//
// Returns: 
// Zero for success, non-zero for failure.
///////////////////////////////////////////////////////////////////////////////////////////////////////
long EXPORT newp_usb_write_binary_by_key (char* DeviceKey, char* Command, unsigned long Length);

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Description:
// This function sends the passed in binary data to the specified device.
//
// Parameters:
// DeviceID:		The USB address of the device, or the index into the device information.  The valid 
//					range is from 0 - 31.
// Command:			Character buffer used to hold the command.
// Length:			The length of the character buffer.
//
// Returns: 
// Zero for success, non-zero for failure.
///////////////////////////////////////////////////////////////////////////////////////////////////////
long EXPORT newp_usb_send_binary (long DeviceID, char* Command, unsigned long Length);




#ifdef __cplusplus
}
#endif

#endif //#ifndef __espdll_h__
