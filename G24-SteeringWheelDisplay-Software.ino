#include "include/data_processor.hpp"
#include "include/can.hpp"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

DataProcessor dataProcessor;
CAN canController;

void setup() {
    Serial.begin(115200);
    canController.set_data_proccessor(&dataProcessor);
}

void loop(){ 
    canController.listen();
    vTaskDelay(10);
}