#ifndef LED_STRIP_HPP
#define LED_STRIP_HPP

#include <Adafruit_NeoPixel.h>

#define PIN_WS2812B 6
#define NUM_PIXELS 18 

#define STOP_CAR_WARNING 0

class LedStrip{
public:
    LedStrip(): _ws2812b(NUM_PIXELS, PIN_WS2812B, NEO_GRB + NEO_KHZ800) {}

    void begin(){
        _ws2812b.begin();
    }   
    void display_warning(int warning);
    void display_rpm(int rpm);
    void display_startup();

private:
    Adafruit_NeoPixel _ws2812b;
};

#endif