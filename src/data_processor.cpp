/**
 * @file data_processor.cpp
 * @author Raúl Arcos Herrera
 * @brief This file contains the implementation of the Data Processor class for Link G4+ ECU.
 */

#include "../include/data_processor.hpp"

void DataProcessor::send_serial(byte type, unsigned int value) {                     //Como parámetros se pasan el ID (type), que es el ID establecido al inicio del código para el dato que se quiera enviar. Ej: RPM_ID -> 0x51; y se envía el valor de dicho dato.
    byte dato[8] = { 0x5A, 0xA5, 0x05, 0x82, 0x00, 0x00, 0x00, 0x00 };  //Se establece un arreglo de bytes con los primeros datos necesarios para que la pantalla lo interprete como mensaje (En la Wiki hay tutoriales que lo explican a fondo), como ser la longitud y el tipo de mensaje.
    dato[4] = type;                                                     //Se configura en el mensaje el ID correspondiente al dato a enviar.
    dato[6] = (value >> 8) & 0xFF;                                      //Se configura el dato en los últimos 2 bytes.
    dato[7] = value & 0xFF;

    Serial.write(dato, 8);                                              //Se envía serialmente el mensaje, indicando su longituden bytes para ello.
}

void DataProcessor::send_serial_frame_0(int rpmh, int rpml, int tpsh, int tpsl, int brkh, int brkl, int gear){
    byte dato[8] = { 0x5A, 0xA5, 0x05, 0x82, 0x00, 0x00, 0x00, 0x00 };
    dato[4] = RPM_ID;
    dato[6] = rpmh & 0xFF;
    dato[7] = rpml & 0xFF;
    Serial.write(dato, 8);
    // _led_strip->set_rpm(rpmh*256 + rpml);


    dato[4] = TPS_ID;
    dato[6] = tpsh & 0xFF;
    dato[7] = tpsl & 0xFF;
    Serial.write(dato, 8);
    Serial.println(tpsh*256 + tpsl);
    _led_strip->set_rpm((tpsh*256 + tpsl)*10);


    dato[4] = BPS_ID;
    dato[6] = brkh & 0xFF;
    dato[7] = brkl & 0xFF;
    Serial.write(dato, 8);

    dato[4] = GEAR_ID;
    dato[6] = gear & 0xFF;
    dato[7] = 0;
    Serial.write(dato, 8);
}

void DataProcessor::send_serial_frame_1(int lfws, int rfws, int lrws, int rrws, int maph, int mapl, int ect){
    byte dato[8] = { 0x5A, 0xA5, 0x05, 0x82, 0x00, 0x00, 0x00, 0x00 };
    dato[4] = LF_WS_ID;
    dato[6] = lfws & 0xFF;
    dato[7] = 0;
    Serial.write(dato, 8);

    dato[4] = RF_WS_ID;
    dato[6] = rfws & 0xFF; 
    dato[7] = 0;
    Serial.write(dato, 8);

    dato[4] = LR_WS_ID;
    dato[6] = lrws & 0xFF;
    dato[7] = 0;
    Serial.write(dato, 8);

    dato[4] = RR_WS_ID;
    dato[6] = rrws & 0xFF;
    dato[7] = 0;
    Serial.write(dato, 8);

    dato[4] = MAP_ID;
    dato[6] = maph & 0xFF;
    dato[7] = mapl & 0xFF;
    Serial.write(dato, 8);

    dato[4] = ECT_IN_ID;
    dato[6] = ect & 0xFF;
    dato[7] = 0;
    Serial.write(dato, 8);
}

void DataProcessor::send_serial_frame_2(int lambh, int lambl, int lamth, int lamtl, int bvolth, int bvoltl, int iat){
    byte dato[8] = { 0x5A, 0xA5, 0x05, 0x82, 0x00, 0x00, 0x00, 0x00 };
    dato[4] = LAMBDA_ID;
    dato[6] = lambh & 0xFF;
    dato[7] = lambl & 0xFF;
    Serial.write(dato, 8);

    dato[4] = BVOLT_ID;
    dato[6] = bvolth & 0xFF;
    dato[7] = bvoltl & 0xFF;
    Serial.write(dato, 8);

    dato[4] = IAT_ID;
    dato[6] = iat & 0xFF;
    dato[7] = 0;
    Serial.write(dato, 8);
}

void DataProcessor::send_serial_change_display(int display){
    byte dato[8] = { 0x5A, 0xA5, 0x05, 0x82, 0x00, 0x00, 0x00, 0x00 };
    const int displayIDs[] = { DISPLAY_0_ID, DISPLAY_1_ID, DISPLAY_2_ID, DISPLAY_3_ID, DISPLAY_4_ID };
    dato[4] = displayIDs[display];
    dato[6] = 0;
    dato[7] = 0;
    Serial.write(dato, 8);
}