#include <SPIFlash.h>

// CS pin, SPI instance
SPIFlash flash(10);

void setup() {
    Serial.begin(115200);
    while(!Serial);
    
    if(!flash.begin()) {
        Serial.println("Flash memory not found!");
        while(1);
    }
    
    Serial.print("JEDEC ID: 0x");
    Serial.println(flash.getJEDECID(), HEX);
    
    // Пример записи и чтения
    uint8_t data[4] = {0xDE, 0xAD, 0xBE, 0xEF};
    uint8_t buffer[4];
    
    flash.eraseSector(0);
    flash.write(0, data, sizeof(data));
    flash.read(0, buffer, sizeof(buffer));
    
    Serial.print("Read data: ");
    for(auto b : buffer) {
        Serial.print(b, HEX);
        Serial.print(" ");
    }
    Serial.println();
}

void loop() {
}