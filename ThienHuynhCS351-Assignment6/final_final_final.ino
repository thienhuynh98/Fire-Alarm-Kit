#include <IRremote.h>
#include "DHT.h"
#define Type DHT11
#include <LiquidCrystal.h>
int rs=7;
int en=8;
int d4=9;
int d5=10;
int d6=11;
int d7=12;
LiquidCrystal lcd(rs,en,d4,d5,d6,d7);

boolean freeze = false;

int buzzPin = 5;

int recvPin = 4;
IRrecv irrecv(recvPin);
decode_results results;

int hour = 12;
int minute = 59;
int second = 45;
int flag = 1;

int sensePin=2;
DHT HT(sensePin,Type);
float humidity;
float tempF;

void setup() {
  Serial.begin(9600);
  
  HT.begin();
  delay(500);
  lcd.begin(16,2); 

  pinMode(buzzPin , OUTPUT);

  irrecv.enableIRIn();  
}

void loop()
{
  humidity=HT.readHumidity();
  tempF=HT.readTemperature(true);

  lcd.setCursor(0,1);
  lcd.print("T=");
  lcd.print(tempF);
  lcd.setCursor(8,1);
  lcd.print("H=");
  lcd.print(humidity);
  lcd.print("%");
  if(freeze == false)
  {
    if(irrecv.decode(&results))
    {
      Serial.print(results.value, HEX);
      if(results.value == 0xFF6897)
      {
        hour = 0;
        minute = 0;
        second = 0;
      }
      if(results.value == 0xFF30CF)
      {
        hour = hour + 1;
      }
      if(results.value == 0xFF18E7)
      {
        minute = minute + 1;
      }
      irrecv.resume();
      delay(100);
    }  
    lcd.setCursor(0,0);
    lcd.print("Time ");
    if(hour < 10)
    {
      lcd.print("0");
    }
    lcd.print(hour);
    lcd.print(":");
    if(minute < 10)
    {
      lcd.print("0");
    }
    lcd.print(minute);
    lcd.print(":");
    if(second < 10)
    {
      lcd.print("0");
    }
    lcd.print(second);

    if(flag==0)
    {
      lcd.print(" AM");
    }
    if(flag==1)
    {
      lcd.print(" PM");
    }
      
    delay(1000);
 
    second = second + 1;
    if(second == 60)
    {
      second = 0;
      minute = minute +1;
    }
    if(minute == 60)
    {
      minute = 0;
      hour = hour + 1;
    }
    if(hour == 13)
    {
      hour = 1;
      flag = flag + 1;
      if(flag == 2)flag = 0;
    }
    if(humidity >= 85)
    {
      digitalWrite(buzzPin, HIGH);
    
      lcd.setCursor(0,0);
      lcd.print("Time ");
      if(hour < 10)
      {
        lcd.print("0");
      }
      lcd.print(hour);
      lcd.print(":");
      if(minute < 10)
      {
        lcd.print("0");
      }
      lcd.print(minute);
      lcd.print(":");
      if(second < 10)
      {
        lcd.print("0");
      }
      lcd.print(second);

      if(flag==0)
      {
        lcd.print(" AM");
      }
      if(flag==1)
      {
        lcd.print(" PM");
      }
      freeze = true;
    }
  }
}
