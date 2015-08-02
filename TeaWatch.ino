//TeaWatch OS
//From TeaSoftOffice ---Birkhoff Carl

//(Outside)EEPROM ADDRESS(OPINION) LIST:
//SHAKE TO WAKE UP:  0(0:off/1:on)
//POWER SAVE MODE:   1(0:off/1:on)
//12 OR 24:          2(0:24/1:12)
//ALARM HOUR1/2/3:   3/5/7(xxH)
//ALARM MINUTE1/2/3: 4/6/8(xxM)
//BIRTHDAY MONTH:    9(xxx)
//BIRTHDAY DATE:     10(xxx)
//AUTO ALARM1/2/3:   11/12/13(0:off/1:on)



#include <Adafruit_ssd1306syp.h>
#include <Enerlib.h>
#include <avr/wdt.h>
#include <EEPROM.h>
#include <DS3231.h>
#include <Wire.h>

//HARDWARE:
#define SDA 7
#define SCL 8

int wake = 2;
int lg = 3;
int rg = 4;
int buzzer = 5;
int motor = 9;
int button = 12;
int LED = 13;
bool Century=false;
bool h12;
bool PM;
int second=60;
int minute=60;
int hour=24;
int date=12;
int month=03;
int year=2001;
int temperature=15;
int DoW=1;

//VARIABLES:
int power = 0;
int LOWPOWER = 3350;
int wear = 1;

//LIBRARIES INITIALIZE:
Adafruit_ssd1306syp display(SDA,SCL);
Energy energy;
volatile byte data = 0;
DS3231 Clock;



void setup()
{
  //INITIALIZE:
  pinMode(button, INPUT_PULLUP);
  pinMode(wake, INPUT_PULLUP);
  pinMode(lg, INPUT_PULLUP);
  pinMode(rg, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);
  pinMode(motor, OUTPUT);
  pinMode(LED, OUTPUT);

  attachInterrupt(0, SHOW, LOW);
  if (EEPROM.read(11) == 1)
  {
    setup_watchdog(9);
    // 0=16ms, 1=32ms,2=64ms,3=128ms,4=250ms,5=500ms
    // 6=1 sec,7=2 sec, 8=4 sec, 9= 8sec
  }
  if (EEPROM.read(12) == 1)
  {
    setup_watchdog(9);
    // 0=16ms, 1=32ms,2=64ms,3=128ms,4=250ms,5=500ms
    // 6=1 sec,7=2 sec, 8=4 sec, 9= 8sec
  }
  if (EEPROM.read(13) == 1)
  {
    setup_watchdog(9);
    // 0=16ms, 1=32ms,2=64ms,3=128ms,4=250ms,5=500ms
    // 6=1 sec,7=2 sec, 8=4 sec, 9= 8sec
  }

  Wire.begin();
  second=Clock.getSecond();
  minute=Clock.getMinute();
  hour=Clock.getHour(h12, PM);
  date=Clock.getDate();
  month=Clock.getMonth(Century);
  year=Clock.getYear(); 
  temperature=Clock.getTemperature();
  DoW=Clock.getDoW();
  
  //BOOT LOGO:
  display.initialize();
  display.clear();
  analogWrite(motor, 128);
  digitalWrite(LED, HIGH);
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(10, 2);
  display.println("SHIELD");
  display.update();
  delay(1000);
  analogWrite(motor, 255);
  display.setCursor(19, 27);
  display.setTextSize(5);
  display.println("O S");
  display.update();
  delay(2000);
  analogWrite(motor, 0);
  digitalWrite(LED, LOW);
}



void loop()
{
  TIME();
}



void TIME()
{
  while (1)
  {
    power = readVcc();
    if (power < LOWPOWER)
    {
      delay(1000);
      BEEP(1);
    }
    
    if (digitalRead(button) == HIGH)
    {
      delay(10);
    if(digitalRead(button) == HIGH)
    {
      if (digitalRead(lg) == HIGH)
      {
      delay(10);
      if(digitalRead(lg) == HIGH)
      {
        if(digitalRead(rg) == LOW)
        {
        delay(10);
        if (digitalRead(rg) == LOW)
        {
          ALARM();
        }
        }
      }
      }
    }
    }

    second=Clock.getSecond();
    minute=Clock.getMinute();
    hour=Clock.getHour(h12, PM);
    date=Clock.getDate();
    month=Clock.getMonth(Century);
    year=Clock.getYear(); 
    temperature=Clock.getTemperature();
    DoW=Clock.getDoW();
    
    display.clear();
    display.setTextSize(5);
    display.setTextColor(BLACK, WHITE);
    display.setCursor(0, 0);
    display.println(power);
    display.update();
    delay(1000);
    display.clear();
  }
}



void ALARM()
{
  while (1)
  {
    power = readVcc();
    if (power < LOWPOWER)
    {
      BEEP(1);
    }
    
    display.clear();
    
    display.update();

if (digitalRead(button) == HIGH)
    {
      delay(10);
    if(digitalRead(button) == HIGH)
    {
      if (digitalRead(lg) == LOW)
      {
      delay(10);
      if(digitalRead(lg) == LOW)
      {
        if(digitalRead(rg) == HIGH)
        {
        delay(10);
        if (digitalRead(rg) == HIGH)
        {
          return;
        }
        }
      }
      }
    }
    }
  }
}



void BEEP(int mode)
//mode=0:none
//mode=1:sleep
{
  while (1)
  {
      digitalWrite(LED, HIGH);
      digitalWrite(buzzer, HIGH);
      analogWrite(motor, 128);
      delay(333);
      digitalWrite(LED, LOW);
      digitalWrite(buzzer, HIGH);
      analogWrite(motor, 255);
      delay(333);
      if (button == HIGH)
      {
      delay(10);
      if (button == HIGH)
      {
      digitalWrite(LED, LOW);
      digitalWrite(buzzer, LOW);
      analogWrite(motor, 0);
      return;
      }
      }
      if (mode == 1)
      {
        int i;
        for(i=0; i<5; i++)
        {
          digitalWrite(LED, HIGH);
          digitalWrite(buzzer, HIGH);
          analogWrite(motor, 128);
          delay(333);
          digitalWrite(LED, LOW);
          digitalWrite(buzzer, HIGH);
          analogWrite(motor, 255);
          delay(333);
          if(digitalRead(button)==HIGH)
          {
          if(digitalRead(button)==HIGH)
          {
            break;
          }
          }
        }
        ACSR |= _BV(ACD);
        ADCSRA = 0;
        energy.PowerDown();
        return;
      }
    }
  }



void SHOW()
{
  display.clear();
  display.update();
  if(EEPROM.read(2)==0)
  {
    display.setTextSize(5);
    display.setTextColor(WHITE);
    display.setCursor( , );
    display.print(hour);
    display.print(":");
    display.println(minute);
    display.setTextSize(3);
    display.setCursor( , );
    display.print(temprature);
    display.print("℃");
    display.update();
  }
  if(EEPROM.read(2)==1)
  {
    display.setTextColor(WHITE);
    display.setTextSize(2);
    display.setCursor( , );
    if (h12)
    {
      if (PM)
      {
      display.print("PM");
      }
      else
      {
      display.print(" AM ");
      }
    }
    display.setTextSize(5);
    display.setCursor( , );
    display.print(hour);
    display.print(":");
    display.println(minute);
    display.setTextSize(3);
    display.setCursor( , );
    display.print(temprature);
    display.print("℃");
    display.update();
  }
  delay(3000);
}



//OTHERS
long readVcc()
{
  long result;
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(2);
  ADCSRA |= _BV(ADSC);
  while (bit_is_set(ADCSRA, ADSC));
  result = ADCL;
  result |= ADCH << 8;
  result = 1126400L / result;
  return result;
}



void setup_watchdog(int ii)
{
  byte bb;
  if (ii > 9 ) ii = 9;
  bb = ii & 7;
  if (ii > 7) bb |= (1 << 5);
  bb |= (1 << WDCE);
  MCUSR &= ~(1 << WDRF);
  WDTCSR |= (1 << WDCE) | (1 << WDE);
  WDTCSR = bb;
  WDTCSR |= _BV(WDIE);
}



ISR(WDT_vect)
{
  while(1)
  {
  display.clear();
  if (EEPROM.read(3) == hour)
  {
    if (EEPROM.read(4) == minute)
    {
      BEEP(0);
      break;
    }
  }
  if (EEPROM.read(5) == hour)
  {
    if (EEPROM.read(6) == minute)
    {
      BEEP(0);
      break;
    }
  }
  if (EEPROM.read(7) == hour)
  {
    if (EEPROM.read(8) == minute)
    {
      BEEP(0);
      break;
    }
    else
    {
      energy.PowerDown();
    }
  }
  else
  {
    energy.PowerDown();
  }
  }
}

