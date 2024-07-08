#include "include/data_processor.hpp"
#include "include/can.hpp"
#include "include/g24_wheel_buttons.hpp"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

DataProcessor dataProcessor;
CAN canController;
G24WheelButtons wheelButtons;

void setup() {
    Serial.begin(115200);
    canController.set_data_proccessor(&dataProcessor);
    wheelButtons.set_can_controller(&canController);
    canController.start();
    
    wheelButtons.begin();
    xTaskCreate(G24WheelButtons::updateTask, "Wheel Buttons", 4096, &wheelButtons, 1, NULL);
}

void loop(){ 
    canController.listen();
    vTaskDelay(10);
}