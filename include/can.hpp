#ifndef CAN_HPP
#define CAN_HPP

#define RX_PIN 13
#define TX_PIN 14
#define POLLING_RATE_MS 20
#define TRANSMIT_RATE_MS 200

#include "driver/twai.h"
#include "common/common_libraries.hpp"
#include "data_processor.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

class CAN {
public:
    CAN(): _mutex(xSemaphoreCreateMutex()) {}
    ~CAN();
    void start();
    void listen();
    void send_frame(twai_message_t message);
    twai_message_t createBoolMessage(bool b0, bool b1, bool b2, bool b3, bool b4, bool b5, bool b6, bool b7);

    void set_data_proccessor(DataProcessor *data_processor) {
        _data_processor = data_processor;
    }

    static void listenTask(void *arg) {
        CAN *controller = static_cast<CAN*>(arg);
        controller->listen();
    }

private:
    twai_message_t _rx_message;
    DataProcessor *_data_processor;
    SemaphoreHandle_t _mutex;
};

#endif
