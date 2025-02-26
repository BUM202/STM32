#include "SPIFlash.h"

SPIFlash::SPIFlash(uint8_t csPin, SPIClass& spi) 
    : _csPin(csPin), _spi(spi) {}

bool SPIFlash::begin() {
    pinMode(_csPin, OUTPUT);
    digitalWrite(_csPin, HIGH);
    _spi.begin();
    return (getJEDECID() != 0);
}

uint32_t SPIFlash::getJEDECID() {
    digitalWrite(_csPin, LOW);
    _spi.transfer(READ_JEDEC_ID);
    uint32_t id = _spi.transfer(0) << 16;
    id |= _spi.transfer(0) << 8;
    id |= _spi.transfer(0);
    digitalWrite(_csPin, HIGH);
    return id;
}

void SPIFlash::read(uint32_t addr, uint8_t *buf, size_t len) {
    digitalWrite(_csPin, LOW);
    _spi.transfer(READ_DATA);
    _spi.transfer(addr >> 16);
    _spi.transfer(addr >> 8);
    _spi.transfer(addr);
    
    for(size_t i = 0; i < len; i++) {
        buf[i] = _spi.transfer(0);
    }
    digitalWrite(_csPin, HIGH);
}

bool SPIFlash::write(uint32_t addr, const uint8_t *data, size_t len) {
    if(addr + len > getCapacity()) return false;
    
    size_t written = 0;
    while(written < len) {
        size_t remaining = PAGE_SIZE - (addr % PAGE_SIZE);
        size_t toWrite = min(remaining, len - written);
        
        _writeEnable();
        _writePage(addr, data + written, toWrite);
        _waitUntilDone();
        
        addr += toWrite;
        written += toWrite;
    }
    return true;
}

bool SPIFlash::eraseSector(uint32_t sectorNumber) {
    uint32_t addr = sectorNumber * SECTOR_SIZE;
    if(addr >= getCapacity()) return false;
    
    _writeEnable();
    digitalWrite(_csPin, LOW);
    _spi.transfer(SECTOR_ERASE);
    _spi.transfer(addr >> 16);
    _spi.transfer(addr >> 8);
    _spi.transfer(addr);
    digitalWrite(_csPin, HIGH);
    _waitUntilDone();
    return true;
}

bool SPIFlash::eraseChip() {
    _writeEnable();
    digitalWrite(_csPin, LOW);
    _spi.transfer(CHIP_ERASE);
    digitalWrite(_csPin, HIGH);
    _waitUntilDone();
    return true;
}

// Private methods
void SPIFlash::_writeEnable() {
    _sendCommand(WRITE_ENABLE);
}

void SPIFlash::_waitUntilDone() {
    while(getStatus() & 0x01);
}

void SPIFlash::_sendCommand(uint8_t cmd) {
    digitalWrite(_csPin, LOW);
    _spi.transfer(cmd);
    digitalWrite(_csPin, HIGH);
}

void SPIFlash::_writePage(uint32_t addr, const uint8_t *data, size_t len) {
    digitalWrite(_csPin, LOW);
    _spi.transfer(PAGE_PROGRAM);
    _spi.transfer(addr >> 16);
    _spi.transfer(addr >> 8);
    _spi.transfer(addr);
    
    for(size_t i = 0; i < len; i++) {
        _spi.transfer(data[i]);
    }
    digitalWrite(_csPin, HIGH);
}

uint8_t SPIFlash::getStatus() {
    digitalWrite(_csPin, LOW);
    _spi.transfer(READ_STATUS_1);
    uint8_t status = _spi.transfer(0);
    digitalWrite(_csPin, HIGH);
    return status;
}

uint32_t SPIFlash::getCapacity() {
    uint32_t jedec = getJEDECID();
    uint8_t density = (jedec >> 16) & 0xFF;
    return (1UL << density);
}