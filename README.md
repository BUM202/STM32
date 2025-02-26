Особенности библиотеки:

Поддержка аппаратного SPI

Автоматическое определение размера памяти по JEDEC ID

Проверка границ памяти при записи

Оптимизированные методы для:

Постраничной записи

Стирания секторов (4KB)

Полного стирания чипа

Поддержка разных Flash-чипов через JEDEC ID

Простой API для основных операций

Установка:

Создать папку SPIFlash в Arduino/libraries/

Поместить файлы в соответствующие подпапки

Перезапустить Arduino IDE

Использование:

cpp
Copy
// Создание объекта
SPIFlash flash(csPin);

// Инициализация
if(!flash.begin()) {
    // Обработка ошибки
}

// Запись данных
flash.write(address, dataBuffer, dataLength);

// Чтение данных
flash.read(address, receiveBuffer, readLength);

// Стирание сектора
flash.eraseSector(sectorNumber);
