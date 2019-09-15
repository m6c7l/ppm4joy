/* Name: usbconfig.h (derived from usbconfig-prototype.h)
 * Project: V-USB, virtual USB port for Atmel's(r) AVR(r) microcontrollers
 * Author: Christian Starkjohann, Manfred Constapel
 * Creation Date: 2005-04-01
 * Tabsize: 4
 * Copyright: (c) 2005 by OBJECTIVE DEVELOPMENT Software GmbH
 * License: GNU GPL v2 (see License.txt), GNU GPL v3 or proprietary (CommercialLicense.txt)
 */

#ifndef __usbconfig_h_included__
#define __usbconfig_h_included__

/* ---------------------------- Hardware Config ---------------------------- */

// ATtiny2313 -> PD[n]: 6=ICP, 5=T1, 4=T0, 3=INT1, 2=INT0, 1=TXD, 0=RXD
#define USB_CFG_IOPORTNAME                          D // PORTD
#define USB_CFG_DMINUS_BIT                          3 // PD3
#define USB_CFG_DPLUS_BIT                           2 // PD2
#define USB_CFG_CLOCK_KHZ                           (F_CPU/1000)
#define USB_CFG_CHECK_CRC                           0

/* ----------------------- Optional Hardware Config ------------------------ */

// #define USB_CFG_PULLUP_IOPORTNAME                   D
// #define USB_CFG_PULLUP_BIT                          4

/* --------------------------- Functional Range ---------------------------- */

#define USB_CFG_HAVE_INTRIN_ENDPOINT                1
#define USB_CFG_HAVE_INTRIN_ENDPOINT3               0
#define USB_CFG_EP3_NUMBER                          3
// #define USB_INITIAL_DATATOKEN                       USBPID_DATA1
#define USB_CFG_IMPLEMENT_HALT                      0
#define USB_CFG_SUPPRESS_INTR_CODE                  0
#define USB_CFG_INTR_POLL_INTERVAL                  10
#define USB_CFG_IS_SELF_POWERED                     0
#define USB_CFG_MAX_BUS_POWER                       30
#define USB_CFG_IMPLEMENT_FN_WRITE                  0
#define USB_CFG_IMPLEMENT_FN_READ                   0
#define USB_CFG_IMPLEMENT_FN_WRITEOUT               0
#define USB_CFG_HAVE_FLOWCONTROL                    0
#define USB_CFG_DRIVER_FLASH_PAGE                   0
#define USB_CFG_LONG_TRANSFERS                      0
// #define USB_RX_USER_HOOK(data, len)                 if(usbRxToken == (uchar)USBPID_SETUP) blinkLED();
// #define USB_RESET_HOOK(resetStarts)                 if(!resetStarts){hadUsbReset();}
// #define USB_SET_ADDRESS_HOOK()                      hadAddressAssigned();
#define USB_COUNT_SOF                               0
/* #ifdef __ASSEMBLER__
 * macro myAssemblerMacro
 *     in      YL, TCNT0
 *     sts     timer0Snapshot, YL
 *     endm
 * #endif
 * #define USB_SOF_HOOK                                myAssemblerMacro
 */
#define USB_CFG_CHECK_DATA_TOGGLING                 0
#define USB_CFG_HAVE_MEASURE_FRAME_LENGTH           0
#define USB_USE_FAST_CRC                            0

/* -------------------------- Device Description --------------------------- */

#define  USB_CFG_VENDOR_ID                          0xc0, 0x16 /* = 0x16c0 = 5824 = voti.nl */
#define  USB_CFG_DEVICE_ID                          0xdc, 0x05 /* = 0x05dc = 1500 */
/* This config uses obdev's shared VID/PID pair for Vendor Class devices
 * with libusb: 0x16c0/0x5dc. Use this VID/PID pair ONLY if you understand
 * the implications!
 */
#define USB_CFG_DEVICE_VERSION                      0x00, 0x01
#define USB_CFG_VENDOR_NAME                         'm', '6', 'c', '7', 'l'
#define USB_CFG_VENDOR_NAME_LEN                     5
#define USB_CFG_DEVICE_NAME                         'p', 'p', 'm', '4', 'j', 'o', 'y'
#define USB_CFG_DEVICE_NAME_LEN                     7
// #define USB_CFG_SERIAL_NUMBER                       'n', 'o', 'n', 'e'
// #define USB_CFG_SERIAL_NUMBER_LEN                   4
#define USB_CFG_DEVICE_CLASS                        0
#define USB_CFG_DEVICE_SUBCLASS                     0
#define USB_CFG_INTERFACE_CLASS                     3 // HID class
#define USB_CFG_INTERFACE_SUBCLASS                  0
#define USB_CFG_INTERFACE_PROTOCOL                  0
#define USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH        50
// #define USB_PUBLIC static

/* ------------------- Fine Control over USB Descriptors ------------------- */

#define USB_CFG_DESCR_PROPS_DEVICE                  0
#define USB_CFG_DESCR_PROPS_CONFIGURATION           0
#define USB_CFG_DESCR_PROPS_STRINGS                 0
#define USB_CFG_DESCR_PROPS_STRING_0                0
#define USB_CFG_DESCR_PROPS_STRING_VENDOR           0
#define USB_CFG_DESCR_PROPS_STRING_PRODUCT          0
#define USB_CFG_DESCR_PROPS_STRING_SERIAL_NUMBER    0
#define USB_CFG_DESCR_PROPS_HID                     0
#define USB_CFG_DESCR_PROPS_HID_REPORT              0
#define USB_CFG_DESCR_PROPS_UNKNOWN                 0

// #define usbMsgPtr_t unsigned short

/* ----------------------- Optional MCU Description ------------------------ */

/* #define USB_INTR_CFG                                MCUCR
 * #define USB_INTR_CFG_SET                            ((1 << ISC00) | (1 << ISC01))
 * #define USB_INTR_CFG_CLR                            0
 * #define USB_INTR_ENABLE                             GIMSK
 * #define USB_INTR_ENABLE_BIT                         INT0
 * #define USB_INTR_PENDING                            GIFR
 * #define USB_INTR_PENDING_BIT                        INTF0
 * #define USB_INTR_VECTOR                             INT0_vect
 */

#endif /* __usbconfig_h_included__ */
