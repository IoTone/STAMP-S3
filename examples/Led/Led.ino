/**
 * 2024 IoTone Japan
 * Credits: ro2828
 * Important details about cleverly handling serial mode configurations
 * using some #ifdefs : (in Japanese) https://lang-ship.com/blog/work/m5stack-atoms3-2/
 **/
/*
*******************************************************************************
* Copyright (c) 2022 by M5Stack
*                  Equipped with StampS3 sample source code
*                          配套  StampS3 示例源代码
* Visit for more information: https://docs.m5stack.com/en/core/stamps3
* 获取更多资料请访问: https://docs.m5stack.com/zh_CN/core/stamps3
*
* Describe: LED Show example.  LED展示示例
* Date: 2023/1/11
*******************************************************************************
Press button to change LED status
按下按键切换LED状态.
*/
#include <Arduino.h>
#include <FastLED.h>

#define PIN_BUTTON 0
#define PIN_LED    21
#define NUM_LEDS   1

CRGB leds[NUM_LEDS];
uint8_t led_ih             = 0;
uint8_t led_status         = 0;
String led_status_string[] = {"Rainbow", "Red", "Green", "Blue"};

#if ARDUINO_USB_CDC_ON_BOOT
  #define S3USBSerial Serial
#else
  #if ARDUINO_USB_MODE
    #define S3USBSerial USBSerial
  #else
    #error "Please, board settings -> USB CDC On Boot=Enabled"
  #endif
#endif

/* After StampS3 is started or reset
   the program in the setUp () function will be run, and this part will only be
   run once.
   在StampS3启动或者复位后，即会开始执行setup()函数中的程序，该部分只会执行一次。
*/
void setup() {
    S3USBSerial.begin(115200);
    S3USBSerial.println("StampS3 demo!");

    pinMode(PIN_BUTTON, INPUT);

    FastLED.addLeds<WS2812, PIN_LED, GRB>(leds, NUM_LEDS);
}

/* After the program in setup() runs, it runs the program in loop()
  The loop() function is an infinite loop in which the program runs repeatedly
  在setup()函数中的程序执行完后，会接着执行loop()函数中的程序
  loop()函数是一个死循环，其中的程序会不断的重复运行
*/
void loop() {
    switch (led_status) {
        case 0:
            leds[0] = CHSV(led_ih, 255, 255);
            break;
        case 1:
            leds[0] = CRGB::Red;
            break;
        case 2:
            leds[0] = CRGB::Green;
            break;
        case 3:
            leds[0] = CRGB::Blue;
            break;
        default:
            break;
    }
    FastLED.show();
    led_ih++;
    delay(15);

    if (!digitalRead(PIN_BUTTON)) {
        delay(5);
        if (!digitalRead(PIN_BUTTON)) {
            led_status++;
            if (led_status > 3) led_status = 0;
            while (!digitalRead(PIN_BUTTON))
                ;
            S3USBSerial.print("LED status updated: ");
            S3USBSerial.println(led_status_string[led_status]);
        }
    }
}
