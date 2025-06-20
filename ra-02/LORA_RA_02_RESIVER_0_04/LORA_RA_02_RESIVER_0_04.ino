#include <SPI.h>
#include <LoRa.h>
#include <EEPROM.h>

#define VERSION 0.04
//#define BAUDLORA 433E6 //868E6 //915E6
//Лучше  не отправлять по лоре  более 25 байт
//Максимальный размер  отправляемого пакета 255 байт

char MAC[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
char TYPEDEV[12] = "";
char DATA[12] = "";
char Message[64];
char SETOK[] = "SET OK";
char ERROR[] = "ERROR";

byte BAUDLORA;
byte txPower;
byte signalBandwidth;
uint32_t Timer = 0;
uint32_t Timer1 = 0;

void setup() 
{
  //EEPROM.begin(512);
  Configuration();
}

void loop() {

  if(millis() - Timer > 50)
  {
    Timer =millis();
    USB();
    LoraReceiver();
  }

  if(millis() - Timer1 > 3000)
  {
    Timer1 =millis();
  }

}

void LoraReceiver()
{
  int packetSize = LoRa.parsePacket();
  if (packetSize) 
  {
    // received a packet
    Serial.print("Received packet '");
    // read packet
    while (LoRa.available()) 
    {
      Serial.print((char)LoRa.read());
    }
    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }
}

void USB()
{
  if(Serial.available() > 0)
  {
    int Buf = Serial.readBytes(Message, 64);
    for(int  i = 0; i < Buf; i++)//Принудительно переводим все в нижний регистр
    {
      if(Message[i]<= 0x5a && Message[i] >= 0x41)
      { //Проверяем большая ли буква
        Message[i] = Message[i] + 32; //Переводим  в нижний  регистр
      }
    }
    if(strncmp(Message, "freq=433e6", 10)==0)
    {
      byte TempBAUDLORA;
      TempBAUDLORA = EEPROM.read(0);
      if(TempBAUDLORA != 0)
      {
        BAUDLORA = 0;
        EEPROM.write(0, BAUDLORA);
      }
      Serial.println(SETOK);
    }
    else if(strncmp(Message, "freq=868e6", 10)==0)
    {
      byte TempBAUDLORA;
      TempBAUDLORA = EEPROM.read(0);
      if(TempBAUDLORA != 1)
      {
        BAUDLORA = 1;
        EEPROM.write(0, BAUDLORA);
      }
      Serial.println(SETOK);
    }
    else if(strncmp(Message, "freq=915e6", 10)==0)
    {
      byte TempBAUDLORA;
      TempBAUDLORA = EEPROM.read(0);
      if(TempBAUDLORA != 2)
      {
        BAUDLORA = 2;
        EEPROM.write(0, BAUDLORA);
      }
      Serial.println(SETOK);
    }
    else if(strncmp(Message, "freq?", 5)==0)
    {
      switch(BAUDLORA)
      {
        case 0:
          Serial.println("433E6");
          break;
        case 1:
          Serial.println("868E6");
          break;
        case 2:
          Serial.println("915E6");
          break;
        default:
          Serial.println("433E6");
          break;
      }
    }
    else if(strncmp(Message, "mac", 3)==0)
    {
      Serial.print("MAC=");
      for(int i = 0; i < 6; i++)
      {
        Serial.print("0x");
        Serial.print((uint8_t)MAC[i], HEX);
        if(i < 5)
        {
        Serial.print(":");
        }
      }
      Serial.println();
    }
    else if(strncmp(Message, "version", 7)==0)
    {
      Serial.print("VERSION:");
      Serial.println(VERSION);
    }
    else if(strncmp(Message, "data?", 5)==0)
    {
      Serial.print("DATA:");
      Serial.println(DATA);
    }
    else if(strncmp(Message, "typedev?", 8)==0)
    {
      Serial.print("TYPEDEV:");
      Serial.println(TYPEDEV);
    }
    else if(strncmp(Message, "power=", 6)==0)
    {
      char CharTemp[3] = {0x00, 0x00, 0x00};
      for(int i = 0; i < 2; i++)
      {
        if(Message[6 + i] <= 0x39 && Message[6 + i] >= 0x30)
        {
          CharTemp[i] = Message[6 + i];
        }
        else
        {
          break;
        }
      }
      int  temp = atoi(CharTemp);
      if(temp < 1 || temp > 20)
      {
        temp = 17;
      }
      txPower = temp;
      Serial.println(txPower);
      EEPROM.write(1, txPower);
    }
    else if(strncmp(Message, "hz=7.8e3", 8)==0)
    {
      signalBandwidth = 0;
      EEPROM.write(2, signalBandwidth);
      Serial.print("HZ=7.8E3");
    }
    else if(strncmp(Message, "hz=10.4e3", 8)==0)
    {
      signalBandwidth = 1;
      EEPROM.write(2, signalBandwidth);
      Serial.print("HZ=10.4E3");
    }
    else if(strncmp(Message, "hz=15.6e3", 8)==0)
    {
      signalBandwidth = 2;
      EEPROM.write(2, signalBandwidth);
      Serial.print("HZ=15.6E3");
    }
    else if(strncmp(Message, "hz=20.8e3", 8)==0)
    {
      signalBandwidth = 3;
      EEPROM.write(2, signalBandwidth);
      Serial.print("HZ=20.8E3");
    }
    else if(strncmp(Message, "hz=31.25e3", 8)==0)
    {
      signalBandwidth = 4;
      EEPROM.write(2, signalBandwidth);
      Serial.print("HZ=31.25E3");
    }
    else if(strncmp(Message, "hz=41.7e3", 8)==0)
    {
      signalBandwidth = 5;
      EEPROM.write(2, signalBandwidth);
      Serial.print("HZ=41.7E3");
    }
    else if(strncmp(Message, "hz=762.5e3", 8)==0)
    {
      signalBandwidth = 6;
      EEPROM.write(2, signalBandwidth);
      Serial.print("HZ=62.5E3");
    }
    else if(strncmp(Message, "hz=125e3", 8)==0)
    {
      signalBandwidth = 7;
      EEPROM.write(2, signalBandwidth);
      Serial.print("HZ=125E3");
    }
    else if(strncmp(Message, "hz=250e3", 8)==0)
    {
      signalBandwidth = 8;
      EEPROM.write(2, signalBandwidth);
      Serial.print("HZ=250E3");
    }
    else if(strncmp(Message, "hz=500e3", 8)==0)
    {
      signalBandwidth = 9;
      EEPROM.write(2, signalBandwidth);
      Serial.print("HZ=500E3");
    }
    else if(strncmp(Message, "send=", 5)==0)
    {
      char LoraMessage[64];
      for(int i = 5; i < Buf; i++)
      {
        LoraMessage[i-5] = Message[i];
      }
      Serial.print("SEND:");Serial.print(LoraMessage);
      SendLora(LoraMessage);
    }
    else
    {
      Serial.println(ERROR);
    }
  }
}

void Configuration()
{
  Serial.begin(115200);
  Serial.setTimeout(50);
  Serial.println("START OK!");
  while (!Serial);
  BAUDLORA = EEPROM.read(0); //Считываем частоту
  if(BAUDLORA == 255)
  {
    BAUDLORA = 0;
    EEPROM.write(0, BAUDLORA);
  }
  txPower = EEPROM.read(1); //Считываем мощность передачи
  if(txPower > 20)
  {
    txPower = 17;
    EEPROM.write(1, txPower);
  }
  switch(BAUDLORA)
  {
    case 0:
      LoRa.begin(433E6);
      Serial.println("LORA FREG:433E6");
      break;
    case 1:
      LoRa.begin(868E6);
      Serial.println("LORA FREG:868E6");
      break;
    case 2:
      LoRa.begin(915E6);
      Serial.println("LORA FREG:915E6");
      break;
    default:
      LoRa.begin(433E6);
      Serial.println("LORA FREG:433E6");
      break;
  }
  LoRa.setTxPower(txPower);
  signalBandwidth = EEPROM.read(2); //полоса пропускания сигнала в Гц, по умолчанию равна 125E3
  //7.8E3, 10.4E3, 15.6E3, 20.8E3, 31.25E3, 41.7E3, 62.5E3, 125E3, 250E3, 500E3
  if(signalBandwidth > 10)
  {
    signalBandwidth = 7;
    EEPROM.write(2, signalBandwidth);
  }
  switch(signalBandwidth)
  {
    case 0:
      LoRa.setSignalBandwidth(7.8E3);
      Serial.println("Hz:7.8E3");
      break;
    case 1:
      LoRa.setSignalBandwidth(10.4E3);
      Serial.println("Hz:10.4E3");
      break;
    case 2:
      LoRa.setSignalBandwidth(15.6E3);
      Serial.println("Hz:15.6E3");
      break;
    case 3:
      LoRa.setSignalBandwidth(20.8E3);
      Serial.println("Hz:20.8E3");
      break;
    case 4:
      LoRa.setSignalBandwidth(31.25E3);
      Serial.println("Hz:31.25E3");
      break;
    case 5:
      LoRa.setSignalBandwidth(41.7E3);
      Serial.println("Hz:41.7E3");
      break;
    case 6:
      LoRa.setSignalBandwidth(62.5E3);
      Serial.println("Hz:62.5E3");
      break;
    case 7:
      LoRa.setSignalBandwidth(125E3);
      Serial.println("Hz:125E3");
      break;
    case 8:
      LoRa.setSignalBandwidth(250E3);
      Serial.println("Hz:250E3");
      break;
    case 9:
      LoRa.setSignalBandwidth(500E3);
      Serial.println("Hz:500E3");
      break;
    default:
      LoRa.setSignalBandwidth(125E3);
      Serial.println("Hz:125E3");
      break;
  }
  Serial.print("POWER LORA:");
  Serial.println(txPower);
}

void SendLora(char LoraMessage[])
{
  Serial.println("START SENDER LORA");
  LoRa.beginPacket();
  LoRa.print(LoraMessage);
  LoRa.endPacket();
  Serial.println("FINISH SENDER LORA");
}
