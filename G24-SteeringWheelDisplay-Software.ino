#include "include/data_processor.hpp"
#include "include/can_controller.hpp"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

DataProcessor dataProcessor;
CANController canController;

void setup() {
    Serial.begin(115200);

    canController.set_data_proccessor(&dataProcessor);

    //Start CAN Controller
    xTaskCreate(
        CANController::listenTask,
        "CANController",    
        8192,              
        &canController,              
        1,                
        NULL               
    );
}

void loop(){ 
    delay(10);
}