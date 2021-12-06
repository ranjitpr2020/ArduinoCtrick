#include "DHT.h"
#include <LiquidCrystal_I2C.h>

#define DHTPIN 5 
#define DHTTYPE DHT11   // DHT 11
#define echoPin 7
#define trigPin 8
 
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
int interruptPin = 2;
int mode=0;
int state=LOW;
int refr=0;
int smokeA0 = A0;
long duration; 
int distance;
DHT dht(DHTPIN, DHTTYPE);
typedef struct dhtreturn{
float tempr, hum;
}DHTRETURN;
void setup()
{
  dht.begin();
  
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  pinMode(smokeA0, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin), chgOutput, FALLING);
  lcd.init();                      // initialize the lcd 
  lcd.clear();
  // Print a message to the LCD.
  lcd.backlight();
  //lcd.setCursor(3,0);
  lcd.print("Hello, world!");
  
}


void loop()
{
  
  String msg="";
  DHTRETURN rv;
  //lcd.clear();
  lcd.setCursor(1,0);
  digitalWrite(LED_BUILTIN, state);
  if(!refr)
  {
    lcd.clear();
    if (mode==0)
    {
      rv = readDHT();
      msg = String(String(rv.tempr)+", "+String(rv.hum));
      lcd.print(msg);
    }
    else if (mode==1)
    {
      // Clears the trigPin condition
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);
      // Reads the echoPin, returns the sound wave travel time in microseconds
      duration = pulseIn(echoPin, HIGH);
      // Calculating the distance
      distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
      // Displays the distance on the Serial Monitor
      lcd.print(distance);
    }
    else if(mode==2)
      lcd.print(analogRead(smokeA0));
    refr=1;
  }
  delay(1000);
  refr=0;
}

void chgOutput()
{
  
  state = !state;
  refr=0;
  
  mode = (mode+1)%3;
}

DHTRETURN readDHT()
{
  DHTRETURN rv;
  rv.hum = dht.readHumidity();
  rv.tempr = dht.readTemperature();
  return rv;
}
