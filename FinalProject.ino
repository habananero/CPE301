/*
 * Code Written By: Marco Habana & Jose Urrutia
 * CPE 301: Final Group Project
 * HandWashing!
 */

#include "functionHDR.h" /*machine functions*/
/*#include "addressHDR.h" contains all addresses*/

//time macros
#define WASHTIME 30
#define PUTBACK 10
#define RANGE 7


//GLOBAL VARS
unsigned int t4_counter = PUTBACK;
unsigned int t3_counter = WASHTIME;

void setup() {
  
  /*configuring the various machines*/
  /*pinMode(BUTTON, INPUT_PULLUP); start button, pullup enabled*/
  rtc.begin(); /*begin RTC*/
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); /*uses date and time of PC at complie time*/
  *ddr_b &= ~0x80; /*input enable on pin 13*/
  *port_b |= 0x80; /*pull-up enable on pin 13*/
  servo_setup(); /*motor*/
  lcd.begin(16,2); /*lcd*/
  dht.begin(); /*temp sensor*/
  uSensor_setup(TRIGPIN, ECHOPIN); /*distance sensor*/
  LED_setup; /*signal LEDs*/
  *ddr_h |= 0x08; /*pinMode(FAN, OUTPUT); drying fan*/

  /*configure timers 1, 3, 4, 5*/
  timers_setup();

  Serial.begin(9600);
  
}

void loop() {

  reboot(); /*clears LCD, LEDs off, resets counters*/

  while(*pin_b & 0x80/*digitalRead(BUTTON)*/) {lcd.setCursor(1, 0); lcd.print("PUSH TO BEGIN.");}

    lcd.clear();

  if(!(*pin_b & 0x80)/*!digitalRead(BUTTON*/)/*reads state of pullup BUTTON. if pressed, !0 = 1*/{

     wash_prompt();
     
     while(uSonic_sensor(TRIGPIN, ECHOPIN) > 10) {lcd.setCursor(4,0); lcd.print("TOO FAR!");}

     lcd.clear();
     *myTCCR1B |= 0x05; /*start temp. display ISR*/
     *myTCCR3B |= 0x05; /*start wash time countdown ISR*/

     while((t3_counter >= 1) && (t4_counter >= 1)) {servo_rotate(0); Serial.println("currently washing");}

     servo_rotate(180);

     if(t3_counter == 0) /*clear LCD, washing complete, start fan*/
     {
        
        *myTCCR1B &= ~0x07; /*stop "temp. display" ISR*/
        dry_hands(15);
        display_rt();
        restart_system(1);
    
     }

     else {restart_system(0);}  /*clear LCD, inform user reboot*/
         
  }

}

//-----interface functions-----//

void wash_prompt()
{

  Serial.println("Wash Hands");
  lcd.setCursor(3, 0);
  lcd.print("WASH HANDS");
  my_delay(2000);
  lcd.clear();

  lcd.setCursor(1, 0);
  lcd.print("PLACE HANDS IN");
  my_delay(2000);
  
  lcd.setCursor(7, 1);
  lcd.print("3...");
  my_delay(1000);

  lcd.setCursor(7, 1);
  lcd.print("2...");
  my_delay(1000);
  lcd.setCursor(7, 1);
  lcd.print("1...");
  my_delay(1000);

  lcd.clear();
  
}

void dry_hands(unsigned int drying_time) /*takes argument to specify the lenght of hand-washing*/
{
  
  *port_e |= 0x10; /*digitalWrite(GREENLED, HIGH); let user know, drying process*/
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("BEGIN DRYING");
  my_delay(5000);
  *port_e &= ~0x10; /*digitalWrite(GREENLED, LOW);*/

  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("DRYING....");
        
  toggle_fan(drying_time, 1); /*fan is on for 15 secs*/
  toggle_fan(0, 0);

  *port_e |= 0x10; /*digitalWrite(GREENLED, HIGH);*/
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("DONE!");
  my_delay(5000);
  *port_e &= ~0x10; /*digitalWrite(GREENLED, LOW);*/

}

void restart_system(unsigned int mode) {

  if(mode != 0)
  {
      *port_e |= 0x10; /*digitalWrite(GREENLED, HIGH);*/
      lcd.clear();
      lcd.setCursor(3, 0);
      lcd.print("RESTARTING");
      lcd.setCursor(3, 1);
      lcd.print("SUCCESSFUL");
      /*Serial.println("RESTARTING. SUCCESSFUL");*/
      my_delay(5000);
      *port_e &= ~0x10; /*digitalWrite(GREENLED, LOW);*/    
  }

  else
  {
      *myTCCR1B &= ~0x07;
      *port_j |= 0x02; /*digitalWrite(REDLED, HIGH);*/
      lcd.clear();
      lcd.setCursor(3, 0);
      lcd.print("RESTARTING");
      lcd.setCursor(5, 1);
      lcd.print("FAILED");
      /*Serial.println("RESTARTING. FAILED");*/
      my_delay(5000);
      *port_j &= ~0x02; /*digitalWrite(REDLED, LOW);*/
  }
  
}

void reboot()
{

  lcd.clear();

  t4_counter = PUTBACK;
  t3_counter = WASHTIME;

  *port_e &= ~0x10; /*digitalWrite(GREENLED, LOW);*/
  *port_j &= ~0x02; /*digitalWrite(REDLED, LOW);*/
  
}

unsigned int calc_ticks(unsigned int seconds)
{

  double interrupt_freq = 1/(double)seconds;
  double clk_freq = 16000000;
  unsigned int ticks = (clk_freq/(1024*interrupt_freq));
  return ticks;
  
}


//-----ISRs-----//

void timers_setup()
{

  *myTCCR1A = 0x00; // initialize timer
  *myTCCR1B = 0x00; 
  *myTCCR1C = 0x00; 

  *myTCCR3A = 0x00; 
  *myTCCR3B = 0x00;
  *myTCCR3C = 0x00;

  *myTCCR4A = 0x00; 
  *myTCCR4B = 0x00;
  *myTCCR4C = 0x00;

  *myTCCR5A = 0x00; 
  *myTCCR5B = 0x00;
  *myTCCR5C = 0x00;
 
  *myTIFR1 |= 0x01;
  *myTIFR3 |= 0x01;
  *myTIFR4 |= 0x01;
  *myTIFR5 |= 0x01;
 
  *myTIMSK1 |= 0x01; // overflow interrupt enable
  *myTIMSK3 |= 0x01;
  *myTIMSK4 |= 0x01;

  *myTCNT1 = (unsigned int) (65536 - calc_ticks(1));
  *myTCNT3 = (unsigned int) (65536 - calc_ticks(1));
  *myTCNT4 = (unsigned int) (65536 - calc_ticks(1));

  *myTCCR4B &= ~0x07;
  
}

/*Timer 1: Temperature*/
ISR(TIMER1_OVF_vect){

  /*every three seconds, display the temperature*/
  
   *myTCCR1B &= ~0x07; /*stop timer*/

   *myTCNT1 = (unsigned int) (65536 - calc_ticks(3)); /*reload counter*/
   
   float t = dht.readTemperature(); 

   if(t != 0) /*if the sensor gets a reading*/ {
    lcd.setCursor(0,0);
    lcd.print("Temp.: ");
    lcd.print(t);
   }

   else /*if the sensor does not get a reading*/
   {
    lcd.setCursor(0,0);
    lcd.print("Temp. not avail. ");
    lcd.print(t);
   }

    *myTCCR1B = 0x05; /*restart timer 1*, 1024 prescaler*/
   
}

/*Timer 3: 30s countdown*/
ISR(TIMER3_OVF_vect){

  /*executes every second*/
  
  *myTCCR3B &= ~0x07; /*stop timer*/

  *myTCNT3 = (unsigned int) (65536 - calc_ticks(1)); /*starts every second*/

  /*print time to LCD*/
  lcd.setCursor(0, 1);
  lcd.print(t3_counter);
  lcd.print("s");
  lcd.print("   ");

  /*Serial.println(t3_counter);*/

  t3_counter--; /*decrement counter*/

   /*if distance goes beyond range, start 10s timer*/
  if(uSonic_sensor(TRIGPIN, ECHOPIN) > RANGE){/**myTCCR3B &= ~0x07;*/ *myTCCR4B |= 0x05;}

  /*if the countdown hits 0, stop timer forever*/
  else if(t3_counter == 0) {*myTCCR3B &= ~0x07;}

  /*restart timer*/
  else {*myTCCR3B |= 0x05; /*start 30s timer again*/}

}   


/*Timer 4: 10s*/
ISR(TIMER4_OVF_vect) {

  *myTCCR4B &= ~0x07; /*stop timer*/

  *myTCNT4 = (unsigned int) (65536 - calc_ticks(1)); /*starts every second*/

  /*print time to LCD*/
  lcd.setCursor(7, 1);
  lcd.print("(");
  lcd.print(t4_counter);
  lcd.print("s)");
  lcd.print("     ");

  /*Serial.println(t4_counter);*/

  t4_counter--; /*decrement counter*/

  /*if the user returns hand, reset and start 30s timer*/
  if(uSonic_sensor(TRIGPIN, ECHOPIN) < RANGE)
  {
    t3_counter = WASHTIME;
    t4_counter = 10; *myTCCR3B |= 0x05;
    lcd.setCursor(7, 1);
    lcd.print("     ");
    *port_j &= ~0x02;
  }

  else if(t4_counter == 0) /*stop timer, turn LED off, clear bar*/
  {
    *myTCCR4B &= ~0x07;
    *port_j &= ~0x02;
    lcd.setCursor(7, 1);
    lcd.print("     ");
  }
  
  else {*myTCCR4B |= 0x05; *port_j ^= 0x02; /*start 10s timer again, blink led*/}
  
}
