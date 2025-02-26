#ifndef SPIFlash_h
#define SPIFlash_h

#include <Arduino.h>
#include <SPI.h>

class SPIFlash {
public:
    // Конструктор
    SPIFlash(uint8_t csPin, SPIClass& spi = SPI);
    
    // Основные методы
    bool begin();
    void read(uint32_t addr, uint8_t *buf, size_t len);
    bool write(uint32_t addr, const uint8_t *data, size_t len);
    bool eraseSector(uint32_t sectorNumber);
    bool eraseChip();
    
    // Информационные методы
    uint32_t getJEDECID();
    uint8_t getStatus();
    uint32_t getCapacity();
    
    // Утилиты
    static uint32_t sectorToAddr(uint16_t sector) {
        return sector * 4096UL;
    }

private:
    uint8_t _csPin;
    SPIClass& _spi;
    
    void _writeEnable();
    void _waitUntilDone();
    void _sendCommand(uint8_t cmd);
    void _writePage(uint32_t addr, const uint8_t *data, size_t len);
    
    enum Commands {
        WRITE_ENABLE = 0x06,
        READ_DATA = 0x03,
        PAGE_PROGRAM = 0x02,
        SECTOR_ERASE = 0x20,
        CHIP_ERASE = 0xC7,
        READ_STATUS_1 = 0x05,
        READ_JEDEC_ID = 0x9F
    };
    
    enum {
        PAGE_SIZE = 256,
        SECTOR_SIZE = 4096
    };
};

#endif