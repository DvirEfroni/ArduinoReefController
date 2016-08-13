
#include <Time.h>
#include <TimeAlarms.h>


#include <OneWire.h>
#include <DallasTemperature.h>
//#include <LiquidCrystal.h>
 
// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS 2
 
// Setup a oneWire instance to communicate with any OneWire devices 
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
 
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);


const int CHL_1 = 5;
const int CHL_2 = 6;
const int CHL_3 = 9;
const int CHL_4 = 10;
const int CHL_5 = 11;

const byte buttonStateSwitch = 2;
const byte topstateSwitch = 4;

int AtoPump = 7;

int DoisingPump1 = 8;
int DoisingPump2 =12 ;
int DoisingPump3 = 13;

//LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
long int Temp = 22;


void setup() {
  // put your setup code here, to run once:

pinMode(CHL_1, OUTPUT);
pinMode(CHL_2, OUTPUT);
pinMode(CHL_3, OUTPUT);
pinMode(CHL_4, OUTPUT);
pinMode(CHL_5, OUTPUT);
pinMode(topstateSwitch,INPUT);
pinMode(buttonStateSwitch,INPUT);


 Serial.begin(9600);
  // Start up the library
  sensors.begin();
  setTime(22,35,00,8,4,16); // set time to Saturday 8:29:00am Jan 1 2011
 
  // create the alarms for Lights
  Alarm.alarmRepeat(7,00,00, Lights_SunRaise);  // 7:00am  every day
  Alarm.alarmRepeat(8,00,00,Lights_Day);        // 8:00am  every day
  Alarm.alarmRepeat(13,00,00,Lights_Night);     // 13:00am every day
  Alarm.alarmRepeat(20,30,10,Lights_Evening);   // 20:30pm every day
  Alarm.alarmRepeat(22,00,00,Lights_Night);     // 22:00pm every day 

 // create the alarms for Dosing Pumps 
  Alarm.alarmRepeat(8,32,0,DosingA);
  Alarm.alarmRepeat(8,32,10,DosingB);

//  Alarm.alarmRepeat(dowSaturday,8,30,30,WeeklyAlarm);  // 8:30:30 every Saturday
  Alarm.timerRepeat(15, Repeats);            // timer for every 15 seconds    
  Alarm.timerOnce(10, OnceOnly); 
}

void loop() {
  // put your main code here, to run repeatedly:
 
digitalClockDisplay();
  Alarm.delay(1000); // wait one second between clock display

 ATO_fill();
 ATO_stop();
 Temprature();
 //DosingA();
 //DosingB();
 //DosingC(); 
}


void Lights_SunRaise(){
  analogWrite(CHL_1, 16); //Ch1- Red 
  analogWrite(CHL_2, 16); //Ch3- llue1
  analogWrite(CHL_3, 16); //Ch3- llue2
  analogWrite(CHL_4, 255); //Ch4- UV
  analogWrite(CHL_5, 16); //Ch5- White
  delay(1000);
}

void Lights_Day(){
  analogWrite(CHL_1, 64);  //Ch1- Red 
  analogWrite(CHL_2, 200); //Ch2- Blue1
  analogWrite(CHL_3, 200); //Ch3- llue2
  analogWrite(CHL_4, 255); //Ch4- UV
  analogWrite(CHL_5, 200); //Ch5- White
  delay(1000);
}
 
void Lights_Evening(){
  analogWrite(CHL_1, 16); //Ch1- Red 
  analogWrite(CHL_2, 16); //Ch2- Blue1
  analogWrite(CHL_3, 16); //Ch2- Blue2
  analogWrite(CHL_4, 255); //Ch4- UV
  analogWrite(CHL_5, 00); //Ch5- White
  delay(1000);
}

void Lights_Night(){
  analogWrite(CHL_1, 0); //Ch1- Red 
  analogWrite(CHL_2, 0); //Ch3- llue1
  analogWrite(CHL_3, 0); //Ch3- llue2
  analogWrite(CHL_4, 0); //Ch4- UV
  analogWrite(CHL_5, 0); //Ch5- White
  delay(1000); 
}


void Repeats(){
  Serial.println("15 second timer");         
}

void OnceOnly(){
  Serial.println("This timer only triggers once");  
}


void digitalClockDisplay()
{
  // digital clock display of the time
  Serial.print("Time is : ");
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.println();
 // lcd.print(hour()+ ":" + minute()+ ":" + second());
// Serial.print("Time is: "+hour()+":"+minute()+":"+second()); 
   
}

void printDigits(int digits)
{
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

void ATO_fill()
{
 //pinMode(topstateSwitch,INPUT);
 //pinMode(buttonStateSwitch,INPUT);
 // if(digitalRead(topstateSwitch)==1&digitalRead(buttonStateSwitch)==0||digitalRead(topstateSwitch)==1&digitalRead(buttonStateSwitch)==1)
 if(digitalRead(topstateSwitch)==HIGH&digitalRead(buttonStateSwitch)==LOW||digitalRead(topstateSwitch)==HIGH&digitalRead(buttonStateSwitch)==HIGH)
  {
    digitalWrite(AtoPump,HIGH);
    Serial.println ("ATO is ON");
   // lcd.print("ATO is ON");
  } 
  
}

void ATO_stop()
{
  //if(digitalRead(topstateSwitch)==0&digitalRead(buttonStateSwitch)==0||digitalRead(topstateSwitch)==0&digitalRead(buttonStateSwitch)==1)
  if(digitalRead(topstateSwitch)==LOW&digitalRead(buttonStateSwitch)==LOW||digitalRead(topstateSwitch)==LOW&digitalRead(buttonStateSwitch)==HIGH)
  {
    digitalWrite(AtoPump,LOW);
    Serial.println ("ATO is OFF");
   // lcd.print("ATO is OFF");
  }
    
}

void Temprature()
{
  // call sensors.requestTemperatures() to issue a global temperature
  // request to all devices on the bus
  Serial.print(" Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");

  Serial.print("Temperature for Device 1 is: ");
  Serial.print(sensors.getTempCByIndex(0)); // Why "byIndex"? 
    // You can have more than one IC on the same bus. 
    // 0 refers to the first IC on the wire
  Temp =sensors.getTempCByIndex(0);
 // lcd.print("T= "+ Temp);
}

void DosingA()
{
    digitalWrite(DoisingPump1,HIGH);
    Serial.println ("DoisingPump1 is ON");
    //lcd.print("DoisingPump1 is ON");
       delay(1000);                  // wait for a second 
    digitalWrite(DoisingPump1,LOW);
    Serial.println ("DoisingPump1 is OFF");
  //  lcd.print("DoisingPump1 is OFF");
}


void DosingB()
{
    digitalWrite(DoisingPump2,HIGH);
    Serial.println ("DoisingPump2 is ON");
  //  lcd.print("DoisingPump2 is ON"); 
        delay(1000);                  // waits for a second 
    digitalWrite(DoisingPump2,LOW);
    Serial.println ("DoisingPump2 is OFF");
   // lcd.print("DoisingPump2 is OFF");

}
 
 void DosingC()
 {
    digitalWrite(DoisingPump3,HIGH);
    Serial.println ("DoisingPump3 is ON");
   // lcd.print("DoisingPump3 is ON"); 
       delay(1000);                  // waits for a second 
    digitalWrite(DoisingPump3,LOW);
    Serial.println ("DoisingPump3 is OFF");
   // lcd.print("DoisingPump3 is OFF");
 }


