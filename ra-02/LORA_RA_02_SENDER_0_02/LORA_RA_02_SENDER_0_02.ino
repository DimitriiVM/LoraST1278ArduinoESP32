#include <SPI.h>
#include <LoRa.h>
#include <EEPROM.h>

#define VERSION 0.02
//#define BAUDLORA 433E6 //868E6 //915E6

char MAC[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
char TYPEDEV[12] = "DS18B20";
char DATA[12];
char Message[64];
char SETOK[] = "SET OK";
char ERROR[] = "ERROR";

byte BAUDLORA;
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
  }

  if(millis() - Timer1 > 3000)
  {
    Timer1 =millis();
    SendLora();
  }

}

void SendLora()
{
  Serial.println("START SENDER LORA");
  //Просто складывайте в DATA в формате UTF-8
  strcat(Message, MAC);
  strcat(Message, ":");
  strcat(Message, TYPEDEV);
  strcat(Message, ":");
  strcat(Message, DATA);
  //LoRa.beginPacket();
  //LoRa.print(Message);
  //LoRa.endPacket();
  Serial.println("FINISH SENDER LORA");
  for(int  i = 0; i < 64; i++)
  {
    Message[i] = 0x00;
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
  BAUDLORA = EEPROM.read(0);
  if(BAUDLORA == 255)
  {
    BAUDLORA = 0;
    EEPROM.write(0, BAUDLORA);
  }
  switch(BAUDLORA)
  {
    case 0:
      LoRa.begin(433E6);
      break;
    case 1:
      LoRa.begin(868E6);
      break;
    case 2:
      LoRa.begin(915E6);
      break;
    default:
      LoRa.begin(433E6);
      break;
  }
}
