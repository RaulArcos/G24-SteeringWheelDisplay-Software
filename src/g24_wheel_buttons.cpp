/**
 * @file g24_wheel_buttons.cpp
 * @brief Implementation of the G24WheelButtons class
*/

#include "../include/g24_wheel_buttons.hpp"

G24WheelButtons::G24WheelButtons()
  : lastPressTimeB1(0), lastPressTimeB2(0), lastPressTimeB3(0), lastPressTimeB4(0),
    lastPressTimeLevaIzq(0), lastPressTimeLevaDer(0),
    buttonStateB1(false), buttonStateB2(false), buttonStateB3(false), buttonStateB4(false),
    buttonStateLevaIzq(false), buttonStateLevaDer(false), canController(nullptr) {}

void G24WheelButtons::begin() {
    pinMode(B1_PIN, INPUT);
    pinMode(B2_PIN, INPUT);
    pinMode(B3_PIN, INPUT);
    pinMode(B4_PIN, INPUT);
    pinMode(LEVA_IZQ_PIN, INPUT);
    pinMode(LEVA_DER_PIN, INPUT);

    pinMode(B1_LED_PIN, OUTPUT);
    pinMode(B2_LED_PIN, OUTPUT);
    pinMode(B3_LED_PIN, OUTPUT);
    pinMode(B4_LED_PIN, OUTPUT);

    attachInterruptArg(digitalPinToInterrupt(B1_PIN), handleButtonInterrupt, (void*)B1_PIN, CHANGE);
    attachInterruptArg(digitalPinToInterrupt(B2_PIN), handleButtonInterrupt, (void*)B2_PIN, CHANGE);
    attachInterruptArg(digitalPinToInterrupt(B3_PIN), handleButtonInterrupt, (void*)B3_PIN, CHANGE);
    attachInterruptArg(digitalPinToInterrupt(B4_PIN), handleButtonInterrupt, (void*)B4_PIN, CHANGE);
    attachInterruptArg(digitalPinToInterrupt(LEVA_IZQ_PIN), handleButtonInterrupt, (void*)LEVA_IZQ_PIN, CHANGE);
    attachInterruptArg(digitalPinToInterrupt(LEVA_DER_PIN), handleButtonInterrupt, (void*)LEVA_DER_PIN, CHANGE);

    xTaskCreate(updateTask, "updateTask", 2048, this, 1, NULL);
}

void IRAM_ATTR G24WheelButtons::handleButtonInterrupt(void* arg) {
    gpio_num_t buttonPin = (gpio_num_t)reinterpret_cast<intptr_t>(arg);
    G24WheelButtons* instance = static_cast<G24WheelButtons*>(arg);

    int buttonState = gpio_get_level(buttonPin);

    if (buttonState == LOW) {
        instance->handleButtonPress(buttonPin);
    } else {
        instance->handleButtonRelease(buttonPin);
    }
}

void G24WheelButtons::handleButtonPress(gpio_num_t buttonPin) {
    unsigned long currentTime = millis();
    volatile unsigned long* lastPressTime;

    switch (buttonPin) {
        case B1_PIN:
            lastPressTime = &lastPressTimeB1;
            break;
        case B2_PIN:
            lastPressTime = &lastPressTimeB2;
            break;
        case B3_PIN:
            lastPressTime = &lastPressTimeB3;
            break;
        case B4_PIN:
            lastPressTime = &lastPressTimeB4;
            break;
        case LEVA_IZQ_PIN:
            lastPressTime = &lastPressTimeLevaIzq;
            break;
        case LEVA_DER_PIN:
            lastPressTime = &lastPressTimeLevaDer;
            break;
        default:
            return;
    }

    if (currentTime - *lastPressTime > debounceTime) {
        *lastPressTime = currentTime;

        // Set button state to pressed
        switch (buttonPin) {
            case B1_PIN:
                buttonStateB1 = true;
                break;
            case B2_PIN:
                buttonStateB2 = true;
                break;
            case B3_PIN:
                buttonStateB3 = true;
                break;
            case B4_PIN:
                buttonStateB4 = true;
                break;
            case LEVA_IZQ_PIN:
                buttonStateLevaIzq = true;
                break;
            case LEVA_DER_PIN:
                buttonStateLevaDer = true;
                break;
        }
    }
}

void G24WheelButtons::handleButtonRelease(gpio_num_t buttonPin) {
    // Set button state to released
    switch (buttonPin) {
        case B1_PIN:
            buttonStateB1 = false;
            break;
        case B2_PIN:
            buttonStateB2 = false;
            break;
        case B3_PIN:
            buttonStateB3 = false;
            break;
        case B4_PIN:
            buttonStateB4 = false;
            break;
        case LEVA_IZQ_PIN:
            buttonStateLevaIzq = false;
            break;
        case LEVA_DER_PIN:
            buttonStateLevaDer = false;
            break;
    }
}

void G24WheelButtons::update() {
    if (buttonStateB1) {
        digitalWrite(B1_LED_PIN, HIGH);
    } else {
        digitalWrite(B1_LED_PIN, LOW);
    }

    if (buttonStateB2) {
        digitalWrite(B2_LED_PIN, HIGH);
    } else {
        digitalWrite(B2_LED_PIN, LOW);
    }

    if (buttonStateB3) {
        digitalWrite(B3_LED_PIN, HIGH);
    } else {
        digitalWrite(B3_LED_PIN, LOW);
    }

    if (buttonStateB4) {
        digitalWrite(B4_LED_PIN, HIGH);
    } else {
        digitalWrite(B4_LED_PIN, LOW);
    }

    if (canController) {
        canController->send_frame(canController->createBoolMessage(buttonStateLevaIzq, buttonStateLevaDer, buttonStateB1, buttonStateB2, buttonStateB3, buttonStateB4, 0, 0));
    }
    vTaskDelay(10)
}

void G24WheelButtons::set_can_controller(CAN *canController) {
    this->canController = canController;
}

void G24WheelButtons::updateTask(void *arg) {
    G24WheelButtons *instance = static_cast<G24WheelButtons*>(arg);
    while (true) {
        instance->update();
        vTaskDelay(pdMS_TO_TICKS(100));  // Update every 100 milliseconds
    }
}
