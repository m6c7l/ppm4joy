/* Name: main.c
 * Project: Derived from <Testing driver features>
 * Author: Christian Starkjohann, Manfred Constapel
 * Creation Date: 2008-04-29
 * Tabsize: 4
 * Copyright: (c) 2008 by OBJECTIVE DEVELOPMENT Software GmbH
 * License: GNU GPL v2 (see License.txt), GNU GPL v3 or proprietary (CommercialLicense.txt)
 */

#include <avr/io.h>
#include <avr/interrupt.h>  /* for sei() */
#include <avr/pgmspace.h>   /* required by usbdrv.h */
#include <util/delay.h>     /* for _delay_ms() */

#include "usb/usbdrv/usbdrv.h"
#if USE_INCLUDE
#include "usb/usbdrv/usbdrv.c"
#endif

#include "ppm/ppm4joy.h"

/* ----------------------------- USB interface ----------------------------- */

/* USB report descriptor (for joysticks) */
PROGMEM const char usbHidReportDescriptor[USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH] =
        {
                0x05, 0x01,        // USAGE_PAGE (Generic Desktop)
                0x15, 0x00,        // LOGICAL_MINIMUM (0)
                0x26, 0xff, 0x00,  // LOGICAL_MAXIMUM (255)
                0x75, 0x08,        // REPORT_SIZE (6)
                0x09, 0x04,        // USAGE (Joystick)
                0xa1, 0x01,        // COLLECTION (Application)
                0x09, 0x01,        //   USAGE (Pointer)
                0xa1, 0x00,        //   COLLECTION (Physical)
                0x09, 0x30,        //     USAGE (X)
                0x09, 0x31,        //     USAGE (Y)
                0x95, 0x02,        //     REPORT_COUNT (2)
                0x81, 0x82,        //     INPUT (Data,Var,Abs,Vol)
                0xc0,              //   END_COLLECTION
                0xa1, 0x00,        //   COLLECTION (Physical)
                0x09, 0x32,        //     USAGE (Z)
                0x09, 0x33,        //     USAGE (Rx)
                0x95, 0x02,        //     REPORT_COUNT (2)
                0x81, 0x82,        //     INPUT (Data,Var,Abs,Vol)
                0xc0,              //   END_COLLECTION
                0x09, 0x34,        //   USAGE (Ry)
                0x09, 0x35,        //   USAGE (Rz)
                0x09, 0x36,        //   USAGE (Slider)
                0x09, 0x37,        //   USAGE (Dial)
                0x95, 0x04,        //   REPORT_COUNT (2)
                0x81, 0x82,        //   INPUT (Data,Var,Abs,Vol)
                0xc0               // END_COLLECTION
        };

static uchar reportBuffer[CHANNELS];
static uchar idleRate; /* repeat rate for keyboards, never used for mice */

usbMsgLen_t usbFunctionSetup(uchar data[8]) {
    usbRequest_t *rq = (void *) data;
    /* The following requests are never used. But since they are required by
     * the specification, we implement them here.
     */
    if ((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS) { /* class request type */
        if (rq->bRequest == USBRQ_HID_GET_REPORT) { /* wValue: ReportType (highbyte), ReportID (lowbyte) */
            /* we only have one report type, so don't look at wValue */
            usbMsgPtr = (void *) &reportBuffer;
            return sizeof(reportBuffer);
        } else if (rq->bRequest == USBRQ_HID_GET_IDLE) {
            usbMsgPtr = &idleRate;
            return 1;
        } else if (rq->bRequest == USBRQ_HID_SET_IDLE) {
            idleRate = rq->wValue.bytes[1];
        }
    } else {
        /* no vendor specific requests implemented */
    }
    return 0; /* default for not implemented requests: return no data back to host */
}

/* ------------------------------------------------------------------------- */

int main(void) {

    uchar i;
    usbInit();
    usbDeviceDisconnect(); /* enforce re-enumeration, do this while interrupts are disabled! */
    i = 0;
    while (--i) { /* fake USB disconnect for > 250 ms */
        _delay_ms(1);
    }
    usbDeviceConnect();
    sei();

    initialize();

    for (;;) { /* main event loop */

        usbPoll();

        for (i = 0; i < sizeof(reportBuffer); i++) {
            reportBuffer[i] = ppm[i];
        }

        if (usbInterruptIsReady()) {
            usbSetInterrupt((void *) &reportBuffer, sizeof(reportBuffer));
        }

    }

    return 0;
}

/* ------------------------------------------------------------------------- */
