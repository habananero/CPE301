#ifndef _HEADERFILE_H
#define _HEADERFILE_H

#include "addressHDR.h"

#include <DHT.h>
#include <DHT_U.h>
#include <LiquidCrystal.h>
#include <Servo.h>
#include <Wire.h>
#include <RTClib.h>

//designated pins
#define REDLED 14
#define GREENLED 2
#define BUTTON 13
#define FAN 6
#define SERVO 7
#define TRIGPIN 11
#define ECHOPIN 12
#define DHTPIN A1
#define DHTTYPE 11

//struct/class declarations
Servo servo;
LiquidCrystal  lcd(23, 22, 19, 18, 17, 16);
DHT dht(DHTPIN, DHTTYPE);
RTC_DS1307 rtc; //object

//-----machine setup functions-----//

void uSensor_setup( unsigned int tPin, unsigned int ePin)
{

  *ddr_b |= 0x20; /*pinMode(tPin, OUTPUT);*/
  *ddr_b |= 0x40; /*pinMode(ePin, INPUT);*/ 
  
}

unsigned int my_delay(unsigned int mseconds)
{

  /*calc ticks no prescaler*/
  double clk_period = 0.0000000625;
  unsigned int ticks = 0.001/clk_period;
  
  for(int i = 0; i < mseconds; i++)
  {
    *myTCCR5B &= 0xF8; /*stop timer*/
    *myTCNT5 = (unsigned int) (65536 - ticks); /*set ticks*/
    *myTCCR5B |= 0x01; /*start timer, no prescaler*/
    while((*myTIFR5 & 0x01)==0){}; 
    *myTCCR5B &= 0xF8; /*stop timer again*/
    *myTIFR5 |= 0x01;
  }
    
}

void servo_setup() 
{

  servo.attach(SERVO);
  servo.write(0);
  
}

void LED_setup()
{
  
  *ddr_j |= 0x02; /*pinMode(REDLED, OUTPUT);*/
  *ddr_e |= 0x10; /*pinMode(GREENLED, OUTPUT);*/ 
   
}

//-----functions that do things to machines-----//

void servo_rotate(unsigned int angle){

  int rt_angle;
  
  for(rt_angle = angle; rt_angle < 180; rt_angle++)
  {
    servo.write(rt_angle);
    my_delay(10);  
  }

  for(rt_angle = 180; rt_angle > angle; rt_angle--)
  {
    servo.write(rt_angle);
    my_delay(10); 
  }
   
}

void toggle_fan(unsigned int duration, unsigned int toggle)
{

  if(toggle != 0)
  {  
    *port_h |= 0x08; /*digitalWrite(FAN, HIGH);*/
    for(int i = duration; i > 0; i--) {my_delay(1000);}
  }

  else {*port_h &= ~0x80; /*digitalWrite(FAN, LOW);*/}
  
}

unsigned int uSonic_sensor(const int tPin, const int ePin)
{
  
  long duration;
  int distance;

  *ddr_b |= 0x20; /*pinMode(tPin, OUTPUT);*/
  *ddr_b |= 0x40; /*pinMode(ePin, INPUT);*/
  
  *port_b &= ~0x20; /*digitalWrite(tPin,LOW);*/
  my_delay(2);

  *port_b |= 0x20; /*digitalWrite(tPin, HIGH);*/
  my_delay(10);
  
  *port_b &= ~0x20; /*digitalWrite(tPin, LOW);*/

  duration = pulseIn(ePin, HIGH);
  distance = duration*0.034/2;

  return distance;
  
}

void display_rt(){

    DateTime now = rtc.now();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(now.month(), DEC);//Month
    lcd.print("/");
    lcd.print(now.day(), DEC);//Day
    lcd.print("/");
    lcd.print(now.year(), DEC);//Year
    lcd.print(" (");

    lcd.setCursor(0,1);
    lcd.print(now.hour(), DEC); //hour
    lcd.print(':');
    lcd.print(now.minute(), DEC);//minute
    lcd.print(':');
    lcd.print(now.second(), DEC);//second

    my_delay(1000);
}

#endif 
