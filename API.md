# LoRa API

## Include Library

```arduino
#include <LoRa.h>
```

## Setup

### Begin

Инициализируйте библиотеку с указанной частотой.

```arduino
LoRa.begin(frequency);
```
 * `frequency` - frequency in Hz (`433E6`, `868E6`, `915E6`)

Возвращает "1` в случае успеха и "0" в случае неудачи.

### Set pins

Переопределите используемые библиотекой пин-коды `NSS`, `NRESET` и `DIO0` по умолчанию. **Должен быть вызван перед `LoRa.begin()`.

```arduino
LoRa.setPins(ss, reset, dio0);
```
 * `ss` - выберите новый подчиненный pin-код для использования, по умолчанию используется `10`
 * `reset` - новый используемый pin-код сброса, по умолчанию используется `9`
 * `dio0` - new DIO0 pin to use, defaults to `2`.  **Must** be interrupt capable via [attachInterrupt(...)](https://www.arduino.cc/en/Reference/AttachInterrupt).

Этот вызов является необязательным и должен использоваться только в том случае, если вам нужно изменить используемые по умолчанию pin-коды.

#### No MCU controlled reset pin

Чтобы сохранить дополнительные контакты, можно было бы соединить контакт сброса микроконтроллера с контактом сброса радиоприемника, таким образом, производя сброс только при запуске.

* `reset` - set to `-1` to omit this pin

#### Pin dio0 interrupt callbacks

The dio0 pin can be used for channel activity detection callback, transmission finish callback and/or receiving callback, check `onCadDone` , `onTxDone`, and `onReceive`.

### Set SPI interface

Override the default SPI interface used by the library. **Must** be called before `LoRa.begin()`.

```arduino
LoRa.setSPI(spi);
```
 * `spi` - new SPI interface to use, defaults to `SPI`

Этот вызов является необязательным и должен использоваться только в том случае, если вам нужно изменить используемый по умолчанию SPI-интерфейс, в случае, если на вашей плате Arduino (или совместимой с ней) присутствует более одного SPI-интерфейса.

### Set SPI Frequency

Переопределите частоту SPI по умолчанию, равную 10 МГц, используемую библиотекой. **Должна быть вызвана перед `LoRa.begin()`.

```arduino
LoRa.setSPIFrequency(frequency);
```
 * `frequency` - new SPI frequency to use, defaults to `8E6`

Этот вызов является необязательным и используется только в том случае, если вам нужно изменить частоту SPI, используемую по умолчанию. Некоторые преобразователи логического уровня не могут поддерживать высокие скорости, такие как 8 МГц, поэтому с помощью "LoRa.setSPIFrequency(частота)" можно выбрать более низкую частоту вращения.

### End

Stop the library

```arduino
LoRa.end()
```

## Sending data

### Begin packet

Запустите последовательность отправки пакета.

```arduino
LoRa.beginPacket();

LoRa.beginPacket(implicitHeader);
```

 * `implicitHeader` - (optional) `true` enables implicit header mode, `false` enables explicit header mode (default)

Возвращает значение "1", если радиостанция готова к передаче, и "0", если она занята или находится в состоянии сбоя.

### Writing

Запишите данные в пакет. Каждый пакет может содержать до 255 байт.

```arduino
LoRa.write(byte);

LoRa.write(buffer, length);
```
* `byte` - single byte to write to packet

or

* `buffer` - data to write to packet
* `length` - size of data to write

Returns the number of bytes written.

** Примечание:** Для записи данных в пакет также можно использовать другие API печати Arduino

### End packet

Завершите последовательность отправки пакета.

```arduino
LoRa.endPacket();

LoRa.endPacket(async);
```
* `асинхронный" - (необязательно) "true" включает неблокирующий режим, "false" ожидает завершения передачи (по умолчанию).

Возвращает "1` в случае успеха и "0" в случае неудачи.

### Tx Done

**ВНИМАНИЕ**: TxDonecallback использует вывод прерывания в функции проверки `dio0` `setPins`!

### Register callback

Зарегистрируйте функцию обратного вызова для завершения пакетной передачи.

```arduino
LoRa.onTxDone(onTxDone);

void onTxDone() {
 // ...
}
```

* `onTxDone` - функция, вызываемая при завершении пакетной передачи.

## Получение данных

### Разбор пакета

Проверьте, был ли получен пакет.

```arduino
int packetSize = LoRa.parsePacket();

int packetSize = LoRa.parsePacket(size);
```

* `size` - (необязательно) если включен режим неявного заголовка `> 0` с ожидаемым пакетом байт `size`, режимом по умолчанию является режим явного заголовка


Возвращает размер пакета в байтах или "0", если пакет не был получен.

### Режим непрерывного приема

**ВНИМАНИЕ**: При получении обратного вызова используется вывод прерывания на `dio0`, проверьте функцию `setPins`!

#### Зарегистрировать обратный вызов

Зарегистрируйте функцию обратного вызова при получении пакета.

```arduino
LoRa.onReceive(onReceive);

void onReceive(int packetSize) {
 // ...
}
```
* `onReceive` - функция, вызываемая при получении пакета.

#### Режим приема

Переводит радиостанцию в режим непрерывного приема.

```arduino
LoRa.receive();

LoRa.receive(int size);
```

* `size` - (необязательно) если включен режим неявного заголовка `> 0` с ожидаемым пакетом байт `size`, режимом по умолчанию является режим явного заголовка

Обратный вызов onReceive будет вызван при получении пакета.

### Пакет RSSI

```arduino
int rssi = LoRa.packetRssi();
```

Возвращает усредненный RSSI последнего полученного пакета (dBm).
Чем ближе к  нулю тем сигнал мощнее!

### Пакетный SNR-сигнал

```arduino
float snr = LoRa.packetSnr();
```

Возвращает приблизительное значение SNR полученного пакета в базе данных.

## RSSI

```arduino
int rssi = LoRa.rssi();
```
Возвращает текущий RSSI радиостанции (dBm). RSSI может быть считан в любое время (как во время приема пакетов, так и без него).

### Ошибка частоты передачи пакетов

```arduino
long freqErr = LoRa.packetFrequencyError();
```

Возвращает ошибку частоты принятого пакета в Гц. Ошибка частоты - это смещение частоты между центральной частотой приемника и частотой входящего сигнала LoRa.

### Available

```arduino
int availableBytes = LoRa.available()
```

Возвращает количество байт, доступных для чтения.

### Peeking

Взгляните на следующий байт в пакете.

```arduino
byte b = LoRa.peek();
```

Возвращает следующий байт в пакете или `-1`, если доступных байтов нет.

### Reading

Считайте следующий байт из пакета.

```arduino
byte b = LoRa.read();
```

Returns the next byte in the packet or `-1` if no bytes are available.

**Note:** Other Arduino [`Stream` API's](https://www.arduino.cc/en/Reference/Stream) can also be used to read data from the packet

## Channel Activity Detection
**WARNING**: Channel activity detection callback uses the interrupt pin on the `dio0`, check `setPins` function!

### Register callback

Register a callback function for when channel activity detection has done.
```arduino
LoRa.onCadDone(onCadDone);

void onCadDone(boolean signalDetected) {
  // ...
}
```
 * `onCadDone` - function to call when channel activity detection has done.
 * `signalDetected` - if `true`, the radio detects the presence of other LoRa signals.

### Channel Activity detection mode
Puts the radio in channel activity detection mode.
```arduino
LoRa.channelActivityDetection();
```
## Other radio modes

### Idle mode

Put the radio in idle (standby) mode.

```arduino
LoRa.idle();
```

### Sleep mode

Put the radio in sleep mode.

```arduino
LoRa.sleep();
```

## Radio parameters

### TX Power

Change the TX power of the radio.

```arduino
LoRa.setTxPower(txPower);

LoRa.setTxPower(txPower, outputPin);
```
 * `txPower` - TX power in dB, defaults to `17`
 * `outputPin` - (optional) PA output pin, supported values are `PA_OUTPUT_RFO_PIN` and `PA_OUTPUT_PA_BOOST_PIN`, defaults to `PA_OUTPUT_PA_BOOST_PIN`.

Supported values are `2` to `20` for `PA_OUTPUT_PA_BOOST_PIN`, and `0` to `14` for `PA_OUTPUT_RFO_PIN`.

Most modules have the PA output pin connected to PA BOOST,

### Frequency

Change the frequency of the radio.

```arduino
LoRa.setFrequency(frequency);
```
 * `frequency` - frequency in Hz (`433E6`, `868E6`, `915E6`)

### Spreading Factor

Change the spreading factor of the radio.

```arduino
LoRa.setSpreadingFactor(spreadingFactor);
```
 * `spreadingFactor` - spreading factor, defaults to `7`

Supported values are between `6` and `12`. If a spreading factor of `6` is set, implicit header mode must be used to transmit and receive packets.

### Signal Bandwidth

Change the signal bandwidth of the radio.

```arduino
LoRa.setSignalBandwidth(signalBandwidth);
```

 * `signalBandwidth` - signal bandwidth in Hz, defaults to `125E3`.

Supported values are `7.8E3`, `10.4E3`, `15.6E3`, `20.8E3`, `31.25E3`, `41.7E3`, `62.5E3`, `125E3`, `250E3`, and `500E3`.

### Coding Rate

Change the coding rate of the radio.

```arduino
LoRa.setCodingRate4(codingRateDenominator);
```

 * `codingRateDenominator` - denominator of the coding rate, defaults to `5`

Supported values are between `5` and `8`, these correspond to coding rates of `4/5` and `4/8`. The coding rate numerator is fixed at `4`.

### Preamble Length

Change the preamble length of the radio.

```arduino
LoRa.setPreambleLength(preambleLength);
```

 * `preambleLength` - preamble length in symbols, defaults to `8`

Supported values are between `6` and `65535`.

### Sync Word

Change the sync word of the radio.

```arduino
LoRa.setSyncWord(syncWord);
```

 * `syncWord` - byte value to use as the sync word, defaults to `0x12`

### CRC

Enable or disable CRC usage, by default a CRC is not used.

```arduino
LoRa.enableCrc();

LoRa.disableCrc();
```

### Invert IQ Signals

Enable or disable Invert the LoRa I and Q signals, by default a invertIQ is not used.

```arduino
LoRa.enableInvertIQ();

LoRa.disableInvertIQ();
```
### LNA Gain

Set LNA Gain for better RX sensitivity, by default AGC (Automatic Gain Control) is used and LNA gain is not used.

```arduino
LoRa.setGain(gain);
```

 * `gain` - LNA gain

Supported values are between `0` and `6`. If gain is 0, AGC will be enabled and LNA gain will not be used. Else if gain is from 1 to 6, AGC will be disabled and LNA gain will be used.

## Other functions

### Random

Generate a random byte, based on the Wideband RSSI measurement.

```
byte b = LoRa.random();
```

Returns random byte.
