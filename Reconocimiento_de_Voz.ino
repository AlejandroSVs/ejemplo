#include "VoiceRecognitionV3.h"

/**
  ******************************************************************************
    @file    vr_sample_control_led.ino
    @author  JAlejaS
    @brief   This file provides a demostration on
              how to control led by using VoiceRecognitionModule
  ******************************************************************************
    @nota:
        control de voz led
  ******************************************************************************
    @section  HISTORY

    version inicial
*/

#include <SoftwareSerial.h>


/**
  Connection
  Arduino    VoiceRecognitionModule
   2   ------->     TX
   3   ------->     RX
*/
VR myVR(2, 3);   // 2:RX 3:TX, you can choose your favourite pins.

uint8_t records[7]; // save record
uint8_t buf[64];

int ledBano = 10;
int ledComedor = 11;
int ledRecamara = 12;
int ledSala = 13;

#define  Encender    (0)
#define  Apagar  (1)
#define Bano    (2)
#define  Comedor  (3)
#define  Recamara  (4)
#define  Sala   (5)

/**
  @brief   Print signature, if the character is invisible,
           print hexible value instead.
  @param   buf     --> command length
           len     --> number of parameters
*/
void printSignature(uint8_t *buf, int len)
{
  int i;
  for (i = 0; i < len; i++) {
    if (buf[i] > 0x19 && buf[i] < 0x7F) {
      Serial.write(buf[i]);
    }
    else {
      Serial.print("[");
      Serial.print(buf[i], HEX);
      Serial.print("]");
    }
  }
}

/**
  @brief   Print signature, if the character is invisible,
           print hexible value instead.
  @param   buf  -->  VR module return value when voice is recognized.
             buf[0]  -->  Group mode(FF: None Group, 0x8n: User, 0x0n:System
             buf[1]  -->  number of record which is recognized.
             buf[2]  -->  Recognizer index(position) value of the recognized record.
             buf[3]  -->  Signature length
             buf[4]~buf[n] --> Signature
*/
void printVR(uint8_t *buf)
{
  Serial.println("VR Index\tGroup\tRecordNum\tSignature");

  Serial.print(buf[2], DEC);
  Serial.print("\t\t");

  if (buf[0] == 0xFF) {
    Serial.print("NONE");
  }
  else if (buf[0] & 0x80) {
    Serial.print("UG ");
    Serial.print(buf[0] & (~0x80), DEC);
  }
  else {
    Serial.print("SG ");
    Serial.print(buf[0], DEC);
  }
  Serial.print("\t");

  Serial.print(buf[1], DEC);
  Serial.print("\t\t");
  if (buf[3] > 0) {
    printSignature(buf + 4, buf[3]);
  }
  else {
    Serial.print("NONE");
  }
  Serial.println("\r\n");
}

void setup()
{
  /** initialize */
  myVR.begin(9600);

  Serial.begin(115200);
  Serial.println("Elechouse Voice Recognition V3 Prueba Prometec");

  pinMode(ledBano, OUTPUT);
  pinMode(ledComedor, OUTPUT);
  pinMode(ledRecamara, OUTPUT);
  pinMode(ledSala, OUTPUT);

  digitalWrite(ledBano, LOW);
  digitalWrite(ledComedor, LOW);
  digitalWrite(ledRecamara, LOW);
  digitalWrite(ledSala, LOW);

  if (myVR.clear() == 0) {
    Serial.println("Recognizer cleared.");
  } else {
    Serial.println("Not find VoiceRecognitionModule.");
    Serial.println("Please check connection and restart Arduino.");
    while (1);
  }
  if (myVR.load((uint8_t)Encender) >= 0) {
    Serial.println("Encender loaded");
  }

  if (myVR.load((uint8_t)Apagar) >= 0) {
    Serial.println("Apagar loaded");
  }

  if (myVR.load((uint8_t)Bano) >= 0) {
    Serial.println("Bano loaded");
  }

  if (myVR.load((uint8_t)Comedor) >= 0) {
    Serial.println("Comedor loaded");
  }

  if (myVR.load((uint8_t)Recamara) >= 0) {
    Serial.println("Recamara loaded");
  }

  if (myVR.load((uint8_t)Sala) >= 0) {
    Serial.println("Sala loaded");
  }

}

void loop()
{
  int ret;
  ret = myVR.recognize(buf, 50);
  if (ret > 0) {
    switch (buf[1]) {
      case Bano:
        digitalWrite(ledBano, !digitalRead(ledBano));
        break;
      case Comedor:
        digitalWrite(ledComedor, !digitalRead(ledComedor));
        break;
      case Recamara:
        digitalWrite(ledRecamara, !digitalRead(ledRecamara));
        break;
      case Sala:
        digitalWrite(ledSala, !digitalRead(ledSala));
        break;
      case Encender:
        digitalWrite(ledBano, HIGH);
        digitalWrite(ledComedor, HIGH);
        digitalWrite(ledRecamara, HIGH);
        digitalWrite(ledSala, HIGH);
        break;
      case Apagar:
        digitalWrite(ledBano, LOW);
        digitalWrite(ledComedor, LOW);
        digitalWrite(ledRecamara, LOW);
        digitalWrite(ledSala, LOW);
        break;
      default:
        Serial.println("Record function undefined");
        break;
    }
    /** voice recognized */
    printVR(buf);
  }
}
