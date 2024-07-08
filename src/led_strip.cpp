/**
 * @file led_strip.cpp
 * @author Raúl Arcos Herrera
 * @brief This file contains the implementation of the LED Strip class for Link G4+ ECU.
 */ 

#include "../include/led_strip.hpp"

void LedStrip::display_warning(int warning){
    if(warning == STOP_CAR_WARNING){
        for(int i = 0; i < NUM_PIXELS; i++){
            _ws2812b.setPixelColor(i, 255, 0, 0);
        }
        _ws2812b.show();
    }
}

void LedStrip::display_rpm(int rpm){
    int rpm_leds = map(rpm, 0, 8000, 0, NUM_PIXELS);
    for(int i = 0; i < rpm_leds; i++){
        _ws2812b.setPixelColor(i, 0, 255, 0);
    }
    _ws2812b.show();
}

void LedStrip::display_startup(){
    for(int i = 0; i < NUM_PIXELS; i++){
        _ws2812b.setPixelColor(i, 0, 0, 255);
    }
    _ws2812b.show();
}


