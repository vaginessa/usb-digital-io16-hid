// usb_io_device.h
// *** WIP

#ifndef USB_IO_DEVICE_H__
#define USB_IO_DEVICE_H__

#define USBIO16_LIB_VER 0x02

#ifdef __cplusplus
extern "C" {
#endif

#ifndef USBRL_API
#ifdef _MSC_VER
    /* The original DLL has cdecl calling convention */
#define USBRL_CALL __cdecl
#define USBRL_API __declspec(dllimport) USBRL_CALL
#else /* _MSC_VER */
#define USBRL_API
#endif /*_MSC_VER */
#endif /*USBRL_API*/

enum pin_mode
{
    INPUT_MODE  = 0,
    OUTPUT_MODE = 1
};

enum pin_level
{
    LOW_LVL   = 0,
    HIGH_LVL  = 1,    /* Could not define HIGH_LEVEL - conflicts with Win8 SDK! */
    LOW_LEVEL = 0,          /*compat*/
    HIGHT_LEVEL = HIGH_LVL, /*compat*/
};

enum input_pin_mode
{
    NO_INNER_PULL_UP  = 0,
    INNER_PULL_UP     = 1,
    NO_INNNER_PULL_UP = NO_INNER_PULL_UP, /*compat*/
};

enum work_led_mode
{
    WORK_LED_OFF   = 0,
    WORK_LED_BLINK = 1,
    CLOSE_WORK_LED = WORK_LED_OFF,  /*compat*/
    OPEN_WORK_LED  = WORK_LED_BLINK, /*compat*/
};

struct pin_info
{
    unsigned pinIndex;
    unsigned pinMode;
    unsigned pinValue;
};


/* USB I/O  board information structure */
struct usb_io_device_info
{
    unsigned char *serial_number;
    char *device_path;
    struct usb_io_device_info* next;
};

typedef struct usb_io_device_info *pusb_io_device_info_t;


/** Init the USB IO Library
  @return:  0 on success and -1 on error.
 */
int USBRL_API usb_io_init(void);

/** Finalize the USB IO Library.
  This function frees all of the static data associated with
  USB IO Library. It should be called at the end of execution to avoid memory leaks.
  @return:This function returns 0 on success and -1 on error.
 */
int USBRL_API usb_io_uninit(void);


/** Enumerate the USB IO Devices.
  @return:  pointer to a linked list of struct usb_io_device_info for all usb io devices
  attached to the system, or NULL in the case of failure.
  Free this linked list by calling free_usb_io_device_list().
 */
pusb_io_device_info_t USBRL_API usb_io_get_device_list(void);

/** Free a linked list created by get_usb_io_device_list()
 */
void USBRL_API usb_io_free_device_list(struct usb_io_device_info*);

/** Open a usb I/O device
  @return: handle to the device on success or 0 on failure.
 */
intptr_t USBRL_API usb_io_open_device(struct usb_io_device_info *device_info);

/** Close the USB device opened by open_usb_io_device()
 */
void USBRL_API usb_io_close_device(intptr_t hHandle);

/* Set the green indicator  LED on or off
  @param  hHandle:  the device handle returned by usb_io_open_device
  @param  led_mode:    open or close
  @return:  0 on success and others on error.
  @note: when ON, the green LED will flash
  */
int USBRL_API usb_io_set_work_led_mode(intptr_t hHandle, enum work_led_mode led_mode);

/** Set the mode of I/O pin
  @param  hHandle:  the device handle returned by usb_io_open_device
      pinIndex:   pin number (0 to 15)
      mode:       input mode or output mode
      innerPullUp: pull-up on/off, when mode = INPUT_MODE.

    @returns: 0 on success, others on failure.
    */
int USBRL_API usb_io_set_pin_mode(intptr_t hHandle, unsigned pinIndex, enum pin_mode mode, enum input_pin_mode innerPullUp);

/** Set the value of output pin
    @param  hHandle:  the device handle returned by usb_io_open_device
        pinIndex:   pin number  (0 to 15)
        level:      low or high level
    @note  ouputPinIndex must be a output pin that was set by set_usb_io_pin_mode() function.
    @returns: 0 on success, other on failure
    */
int USBRL_API usb_io_write_output_pin_value(intptr_t hHandle, unsigned ouputPinIndex, enum pin_level level);

/* Get the value of input pin of pinIndex
    @param  hHandle:  the device handle returned by usb_io_open_device
            pinIndex:   pin number (0 to 15)
            level:      a non-null pointer. the value of input pin stored in this param. 0 -- low level, 1 -- high level
    @returns: 0 on success, other on failure
    */
int USBRL_API usb_io_read_input_pin_value(intptr_t hHandle, unsigned pinIndex, unsigned *level);

/* Get the value of all pins
    @param  hHandle:  the device handle returned by usb_io_open_device
	@param info [out]  array of pin_info
    @returns:  0 on success, other on failure
    */
int USBRL_API usb_io_get_all_pin_info(intptr_t hHandle, struct pin_info info[16]);

/* --------------- Added pa01 --------------------------- */

/** Get the library version
 */
int USBRL_API usb_io16_lib_version(void);

/** Open a device by serial number (string)
    @param  dev_list list returned from usb_io_get_device_list()
    @return a handle to the device on success or NULL on failure
    Example: usb_io_device_open_with_serial_number(devlist, "abcd")
*/
intptr_t USBRL_API usb_io_device_open_with_serial_number(pusb_io_device_info_t dev_list,
                                                         const char *serial_number);

/* --- Helpers for scripting.
All pointers passed & returned as opaque intptr_t values
---*/

/** Return next dev. info struct pointer in the list returned by usb_relay_device_enumerate()
  @note Native C callers: just use usb_io_device_info.next
*/
pusb_io_device_info_t USBRL_API usb_io_device_next_dev(pusb_io_device_info_t ptr_device_info);

/** Get the ID string of the device.
 @return pointer to const C string (1-byte, 0-terminated)
 @note Native C callers: just use usb_io_device_info.serial_number
*/
intptr_t USBRL_API usb_io_device_get_id_string(pusb_io_device_info_t ptr_device_info);

#ifdef __cplusplus
}
#endif

#endif // USB_IO_DEVICE_H__
