/*
  JANUARY 2022 TALPAY
  PROJE İSMİ: Power Failure Warning System

  LINK PINS:
  VOLTAGE INPUT A0 NUMBERED analog INPUT
  SİM800L TX PIN 2 NUMBERED DİJİTAL INPUT
  SİM800L RX PIN 3 NUMBERED DİJİTAL INPUT
*/

#include "SoftwareSerial.h"
#define SIM800L_Tx 3
#define SIM800L_Rx 2

SoftwareSerial SIM800L(SIM800L_Tx, SIM800L_Rx);

int sms_durum = 0;
String smsMetni = "";
void SMSgonder();

/********************** SİM800L KONTROL FONKSİYONU *********************/
void SMSgonder(String mesaj) {
  SIM800L.print("AT+CMGF=1\r");
  delay(100);
  SIM800L.println("AT+CMGS=\"+905377324141\"");// KULLANICI TELEFON NUMARASI
  delay(100);
  SIM800L.println(mesaj);
  delay(100);
  SIM800L.println((char)26);
  delay(100);
  SIM800L.println();
  delay(100);
  SIM800L.println("AT+CMGD=1,4");
  delay(100);
  SIM800L.println("AT+CMGF=1");
  delay(100);
  SIM800L.println("AT+CNMI=1,2,0,0,0");
  delay(200);
  smsMetni = "";
}

void setup() {
  Serial.begin(9600);
  SIM800L.begin(9600);
  smsMetni = "ERKEN UYARI SISTEMI \nTALPAY";
  SMSgonder(smsMetni);
  Serial.println("ERKEN UYARI SISTEMI \nTALPAY");
  Serial.println(" ");
}

void loop() {
  /********************** VOLTAJ ÖLÇÜMÜ *********************/
  int deger = analogRead(A0);
  float volt = (deger * 5.0) / 1023 * 2;
  if (volt < 0.09) {
    volt = 0.0;
  }

  /************ VOLTAJ DURUMUNA GÖRE SMS GÖNDERİMİ ***********/
  if (volt <= 8.5 && sms_durum == 1) {
    smsMetni = "ELEKTRIK KESINTISI";
    SMSgonder(smsMetni);
    Serial.println(" ");
    Serial.println("ELEKTRİK KESİNTİSİ");
    Serial.println(" ");
    sms_durum = 0;
  }

  if (volt > 8.5 && sms_durum == 0) {
    smsMetni = "ELEKTRIKLER GELDI";
    SMSgonder(smsMetni);
    Serial.println(" ");
    Serial.println("ELEKTRİK GELDİ");
    Serial.println(" ");
    sms_durum = 1;
  }

  Serial.print("Okunan Analog Değer: ");
  Serial.println(deger);
  Serial.print ("Voltaj(mV): ");
  Serial.print (volt);
  Serial.println (" V");
  Serial.print("sms durum: ");
  Serial.println(sms_durum);
  Serial.println ("--------------------------");
  delay(1000);
}
