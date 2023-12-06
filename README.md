# ProximityLock


## HidPassMain 

For HidPassMain to work you need to create pass.h file with next format. 

```c
#ifndef __PASS__H__
#define __PASS__H__

char password[] = "xxxxxxxxxxxx";
uint pass_size = x;

#endif
```

### Required libraryes 

* https://www.arduino.cc/reference/en/language/functions/usb/keyboard/
* https://github.com/FastLED/FastLED

### Used hardware 

For the HidPass side custom ESP32-S2 development board was used with embeeded fully adressable RGB LED.

* https://blnlabs.gumroad.com/l/ESP32-S2DevelopmentBoard




## BLEdetect 

You need to update UUID_defined vaule with UUID of your BLE Beacon generated on your phone.

```c
char UUID_defined []= "fcc65ddf-4df8-4bd3-8bc2-010d0aae5aa6";
```