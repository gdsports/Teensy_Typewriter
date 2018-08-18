/*
 * MIT License
 *
 * Copyright (c) 2018 gdsports625@gmail.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

/*
 * Teensy Typewriter
 *
 * Type "Hello, world!<ENTER>" then the printer prints "Hello, world!" then
 * advances to the next line.
 *
 * How is this not like a real typewriter? As characters are typed on a real
 * typewriter they appear on paper. Using this program, nothing is printed
 * until the Enter key is pressed. This is a consquence of the design of the
 * thermal printer. There is no way to print each character as it is entered.
 *
 */

#include "USBHost_t36.h"
#include "USBPrinter_t36.h" // https://github.com/gdsports/USBPrinter_t36

USBHost myusb;
USBHub hub1(myusb);
KeyboardController keyboard1(myusb);
USBPrinter uprinter(myusb);

USBDriver *drivers[] = {&hub1, &keyboard1, &uprinter};
#define CNT_DEVICES (sizeof(drivers)/sizeof(drivers[0]))
const char * driver_names[CNT_DEVICES] = {"Hub1", "Keyboard1", "Printer1" };
bool driver_active[CNT_DEVICES] = {false, false, false};

void setup()
{
  Serial.begin(115200);
  while (!Serial && (millis() < 3000)) ; // wait for Arduino Serial Monitor
  Serial.println("\n\nTeensy Typewriter ");

  myusb.begin();
  uprinter.begin();
  keyboard1.attachPress(OnPress);
}

void loop()
{
  myusb.Task();

  // Print out information about different devices.
  for (uint8_t i = 0; i < CNT_DEVICES; i++) {
    if (*drivers[i] != driver_active[i]) {
      if (driver_active[i]) {
        Serial.printf("*** Device %s - disconnected ***\n", driver_names[i]);
        driver_active[i] = false;
      } else {
        Serial.printf("*** Device %s %x:%x - connected ***\n", driver_names[i], drivers[i]->idVendor(), drivers[i]->idProduct());
        driver_active[i] = true;

        const uint8_t *psz = drivers[i]->manufacturer();
        if (psz && *psz) Serial.printf("  manufacturer: %s\n", psz);
        psz = drivers[i]->product();
        if (psz && *psz) Serial.printf("  product: %s\n", psz);
        psz = drivers[i]->serialNumber();
        if (psz && *psz) Serial.printf("  Serial: %s\n", psz);

        // If this is a new Printer device.
        if (drivers[i] == &uprinter) {
          // Lets try first outputting something to our UPrinter to see if it will go out...
          uprinter.println();
        }
      }
    }
  }
}

void OnPress(int key)
{
  Serial.print("key '");
  switch (key) {
    case KEYD_UP       : Serial.print("UP"); break;
    case KEYD_DOWN    : Serial.print("DN"); break;
    case KEYD_LEFT     : Serial.print("LEFT"); break;
    case KEYD_RIGHT   : Serial.print("RIGHT"); break;
    case KEYD_INSERT   : Serial.print("Ins"); break;
    case KEYD_DELETE   : Serial.print("Del"); break;
    case KEYD_PAGE_UP  : Serial.print("PUP"); break;
    case KEYD_PAGE_DOWN: Serial.print("PDN"); break;
    case KEYD_HOME     : Serial.print("HOME"); break;
    case KEYD_END      : Serial.print("END"); break;
    case KEYD_F1       : Serial.print("F1"); break;
    case KEYD_F2       : Serial.print("F2"); break;
    case KEYD_F3       : Serial.print("F3"); break;
    case KEYD_F4       : Serial.print("F4"); break;
    case KEYD_F5       : Serial.print("F5"); break;
    case KEYD_F6       : Serial.print("F6"); break;
    case KEYD_F7       : Serial.print("F7"); break;
    case KEYD_F8       : Serial.print("F8"); break;
    case KEYD_F9       : Serial.print("F9"); break;
    case KEYD_F10      : Serial.print("F10"); break;
    case KEYD_F11      : Serial.print("F11"); break;
    case KEYD_F12      : Serial.print("F12"); break;
    default:
      uprinter.write((char)keyboard1.getKey());
      Serial.print((char)key);
      break;
  }
  Serial.print("'  ");
  Serial.print(key);
  Serial.print(" MOD: ");
  Serial.print(keyboard1.getModifiers(), HEX);
  Serial.print(" OEM: ");
  Serial.print(keyboard1.getOemKey(), HEX);
  Serial.print(" LEDS: ");
  Serial.println(keyboard1.LEDS(), HEX);

  //Serial.print("key ");
  //Serial.print((char)keyboard1.getKey());
  //Serial.println();
}

