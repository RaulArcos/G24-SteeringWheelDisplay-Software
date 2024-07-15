#include "../include/g24_wheel_buttons.hpp"

G24WheelButtons::G24WheelButtons()
  : lastPressTimeB1(0), lastPressTimeB2(0), lastPressTimeB3(0), lastPressTimeB4(0), 
    lastPressTimeLevaIzq(0), lastPressTimeLevaDer(0),
    buttonStateB1(false), buttonStateB2(false), buttonStateB3(false), buttonStateB4(false),
    buttonStateLevaIzq(false), buttonStateLevaDer(false), canController(nullptr), lastTurnTimeE1(255), lastTurnTimeE2(0),
    encoderCounterE1(255), encoderCounterE2(0),
    lastPin_A_StateE1(255), lastPin_A_StateE2(0), displayCounter(0), lastDispayCounter(0) {}

void G24WheelButtons::begin() {
    pinMode(B1_PIN, INPUT_PULLUP);
    pinMode(B2_PIN, INPUT_PULLUP);
    pinMode(B3_PIN, INPUT_PULLUP);
    pinMode(B4_PIN, INPUT_PULLUP);
    pinMode(LEVA_IZQ_PIN, INPUT_PULLUP);
    pinMode(LEVA_DER_PIN, INPUT_PULLUP);

    pinMode(B1_LED_PIN, OUTPUT);
    pinMode(B2_LED_PIN, OUTPUT);
    pinMode(B3_LED_PIN, OUTPUT);
    pinMode(B4_LED_PIN, OUTPUT);
    pinMode(E1_PIN_A, INPUT);
    pinMode(E1_PIN_B, INPUT);
    pinMode(E2_PIN_A, INPUT);
    pinMode(E2_PIN_B, INPUT);

    attachInterruptArg(digitalPinToInterrupt(E1_PIN_A), handleEncoderInterrupt, this, CHANGE);
    attachInterruptArg(digitalPinToInterrupt(E2_PIN_A), handleEncoderInterrupt, this, CHANGE);
}

void IRAM_ATTR G24WheelButtons::handleEncoderInterrupt(void* arg) {
    G24WheelButtons* instance = static_cast<G24WheelButtons*>(arg);
    gpio_num_t encoderPin_A = E1_PIN_A;
    gpio_num_t encoderPin_B = E1_PIN_B;

    int pin_A_State = gpio_get_level(encoderPin_A);
    int pin_B_State = gpio_get_level(encoderPin_B);

    if (pin_A_State != instance->lastPin_A_StateE1) {
        if (pin_B_State != pin_A_State) {
            instance->handleClockWise(encoderPin_A);
        } else {
            instance->handleCounterClockWise(encoderPin_A);
        }
        instance->lastPin_A_StateE1 = pin_A_State;
    }
}

void G24WheelButtons::checkButtonState(gpio_num_t buttonPin, volatile bool &buttonState, volatile unsigned long &lastPressTime, int ledPin) {
    int currentState = digitalRead(buttonPin);
    unsigned long currentTime = millis();

    if (currentState == LOW && !buttonState) { // Button pressed
        if (currentTime - lastPressTime > debounceTime) {
            lastPressTime = currentTime;
            buttonState = true;
            if (ledPin != -1) {
                digitalWrite(ledPin, HIGH);
            }
            // Serial.print("Button Pressed: ");
            // Serial.println(buttonPin);
        }
    } else if (currentState == HIGH && buttonState) { // Button released
        if (currentTime - lastPressTime > debounceTime) {
            lastPressTime = currentTime;
            buttonState = false;
            if (ledPin != -1) {
                digitalWrite(ledPin, LOW);
            }
            // Serial.print("Button Released: ");
            // Serial.println(buttonPin);
        }
    }
}

void G24WheelButtons::update() {
    while (true) {
        checkButtonState(B1_PIN, buttonStateB1, lastPressTimeB1, B1_LED_PIN);
        checkButtonState(B2_PIN, buttonStateB2, lastPressTimeB2, B2_LED_PIN);
        checkButtonState(B3_PIN, buttonStateB3, lastPressTimeB3, B3_LED_PIN);
        checkButtonState(B4_PIN, buttonStateB4, lastPressTimeB4, B4_LED_PIN);
        checkButtonState(LEVA_IZQ_PIN, buttonStateLevaIzq, lastPressTimeLevaIzq, -1);
        checkButtonState(LEVA_DER_PIN, buttonStateLevaDer, lastPressTimeLevaDer, -1);

        // if (canController) {
        //     canController->send_frame(canController->createBoolMessage(buttonStateLevaIzq, buttonStateLevaDer, buttonStateB1, buttonStateB2, buttonStateB3, buttonStateB4, 0, 0));
        // }
        
        _led_strip->set_brightness(encoderCounterE1);
        // _data_processor->send_serial_change_display(displayCounter);

        // Serial.print("Encoder Counter E1: ");
        // Serial.println(encoderCounterE1);

        // Serial.print("Encoder Counter E2: ");
        // Serial.println(displayCounter);

        vTaskDelay(10); // Slightly longer delay to ensure the system is not overloaded
    }
}

void G24WheelButtons::set_can_controller(CAN *canController) {
    this->canController = canController;
}

void G24WheelButtons::handleClockWise(gpio_num_t encoderPin) {
    switch (encoderPin) {
        case E1_PIN_A:
            if(encoderCounterE1 <= 245){
                encoderCounterE1+=10;
                // _led_strip->set_brightness(encoderCounterE1);
                // Serial.print("brillo: ");
                // Serial.println(encoderCounterE1);
            }
            break;
        case E2_PIN_A:
            encoderCounterE2++;
            if( encoderCounterE2 % 5 == 0){
                if (displayCounter <= 4){
                    displayCounter++;
                }else{
                    displayCounter = 0;
                }
                // _data_processor->send_serial_change_display(displayCounter);
                // Serial.print("display: ");
                // Serial.println(displayCounter);
            }
            break;
    }
}

void G24WheelButtons::handleCounterClockWise(gpio_num_t encoderPin) {
    switch (encoderPin) {
        case E1_PIN_A:
            if(encoderCounterE1 >= 10){
                encoderCounterE1-=10;
                // _led_strip->set_brightness(encoderCounterE1);
                // Serial.print("brillo: ");
                // Serial.println(encoderCounterE1);
            }
            break;
        case E2_PIN_A:
            encoderCounterE2--;
            if( encoderCounterE2 % 5 == 0){
                if (displayCounter > 0){
                    displayCounter--;
                }else{
                    displayCounter = 4;
                }
                // _data_processor->send_serial_change_display(displayCounter);
                // Serial.print("display: ");
                // Serial.println(displayCounter);
            }
            break;
    }
}

void G24WheelButtons::updateTask(void *arg) {
    G24WheelButtons *wheelButtons = static_cast<G24WheelButtons*>(arg);
    wheelButtons->update();
    vTaskDelete(NULL);
}

void  G24WheelButtons::set_led_strip(LedStrip *ledStrip){
    _led_strip = ledStrip;
}

void  G24WheelButtons::set_data_processor(DataProcessor *dataProcessor){
    _data_processor = dataProcessor;
}