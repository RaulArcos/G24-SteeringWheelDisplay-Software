#ifndef G24WHEELBUTTONS_HPP
#define G24WHEELBUTTONS_HPP

#include "common/common_libraries.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

#include "can.hpp"

#define B1_PIN GPIO_NUM_1
#define B2_PIN GPIO_NUM_3
#define B3_PIN GPIO_NUM_42
#define B4_PIN GPIO_NUM_40

#define B1_LED_PIN GPIO_NUM_2
#define B2_LED_PIN GPIO_NUM_4
#define B3_LED_PIN GPIO_NUM_41
#define B4_LED_PIN GPIO_NUM_39

#define LEVA_IZQ_PIN GPIO_NUM_15 
#define LEVA_DER_PIN GPIO_NUM_16

class G24WheelButtons {
public:
    G24WheelButtons();
    void begin();
    void update();
    static void updateTask(void *arg);
    void set_can_controller(CAN *canController);

private:
    static void IRAM_ATTR handleButtonInterrupt(void* arg);
    void handleButtonPress(gpio_num_t buttonPin);
    void handleButtonRelease(gpio_num_t buttonPin);

    static const unsigned long debounceTime = 50; // milliseconds
    volatile unsigned long lastPressTimeB1;
    volatile unsigned long lastPressTimeB2;
    volatile unsigned long lastPressTimeB3;
    volatile unsigned long lastPressTimeB4;
    volatile unsigned long lastPressTimeLevaIzq;
    volatile unsigned long lastPressTimeLevaDer;

    volatile bool buttonStateB1;
    volatile bool buttonStateB2;
    volatile bool buttonStateB3;
    volatile bool buttonStateB4;
    volatile bool buttonStateLevaIzq;
    volatile bool buttonStateLevaDer;

    CAN* canController;
};

#endif
