# LoraST1278ArduinoESP32
Lora433E6/868E6/915E6 for ARDUINO ESP32

Контакты:
i.x.c.o.n@yandex.ru
https://rutube.ru/channel/29280484/
https://www.youtube.com/@ALATIR

LoRa.begin(433E6); //433

LoRa.begin(868E6); //868

LoRa.begin(915E6); //915
![Alt text](https://github.com/DimitriiVM/LoraST1278ArduinoESP32/blob/main/%D0%97%D0%B0%D0%B2%D0%B8%D1%81%D0%B8%D0%BC%D0%BE%D1%81%D1%82%D1%8C%20%D1%87%D0%B0%D1%81%D1%82%D0%BE%D1%82%20%20%D0%BE%D1%82%20%D1%81%D0%BA%D0%BE%D1%80%D0%BE%D1%81%D1%82%D0%B8%20%D0%BF%D0%B5%D1%80%D0%B5%D0%B4%D0%B0%D1%87%D0%B8%20%D0%B4%D0%B0%D0%BB%D1%8C%D0%BD%D0%BE%D1%81%D1%82%D0%B8.png?raw=true)

Для  передачи  данных можно использовать  несколько  протоколов,  для передачи  данных от датчиков, и  для передачи  данных с полезной нагрузкой, с  информацией.

Пример протокола  для  передачи  данных датчиков:

MAC:TYPE DEV:DATA

В качестве  разделителя используется ":"

MAC - Адрес датчика 

TYPE DEV - Тип  датчика 

DATA - Данные 

Для протокола, где используется передача данных используется формат:

MAC:DATA

В качестве разделителя  используется ":"

MAC - Адрес передатчика

DATA - Полезная нагрузка

DATA - в  пакете  передается  либо в  открытом виде, либо в шифрованном.

![Alt text](https://github.com/DimitriiVM/LoraST1278ArduinoESP32/blob/main/%D0%A1%D0%BA%D0%BE%D1%80%D0%BE%D1%81%D1%82%D1%8C%20%20%D0%BF%D0%B5%D1%80%D0%B5%D0%B4%D0%B0%D1%87%D0%B8%20%D0%B4%D0%B0%D0%BB%D1%8C%D0%BD%D0%BE%D1%81%D1%82%D1%8C.png?raw=true)
