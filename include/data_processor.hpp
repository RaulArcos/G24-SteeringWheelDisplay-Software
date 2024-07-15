#ifndef DATAPROCESSOR_HPP
#define DATAPROCESSOR_HPP

#include "common/common_libraries.hpp"
#include "common/display_id.hpp"
#include "led_strip.hpp"

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

class DataProcessor {
public:
    DataProcessor() = default;
    char* process(std::vector<float> data);
    void send_serial(byte type, unsigned int value);
    void send_serial_frame_0(int rpmh, int rpml, int tpsh, int tpsl, int brkh, int brkl, int gear);
    void send_serial_frame_1(int lfws, int rfws, int lrws, int rrws, int maph, int mapl, int ect);
    void send_serial_frame_2(int lambh, int lambl, int lamth, int lamtl, int bvolth, int bvoltl, int iat);
    void send_serial_change_display(int display);
    void set_led_strip(LedStrip *led_strip){
        _led_strip = led_strip;
    }

private:
    LedStrip *_led_strip;
};

#endif