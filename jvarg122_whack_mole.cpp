#include <LiquidCrystal.h> //unused. for screen
#include <SPI.h> //unused. for screen
#include <Wire.h>
#define EEPROM_i2cADDR 0x50

// ======= VARIABLES IN USE =======
int mole = -1; int light1, light2, light3; 
int HIGHSCORE = 0;
int buzzer = 10;

//moles
int mole3 = 4;
int m3 = 12;

int mole2 = 7;
int m2 = 13;

int mole1 = 6;
int m1 = 11; 

int score = 0;

typedef struct task {
  int state;
  unsigned int period;
  unsigned int elapsedTime;
  int (*TickFct)(int);
} task;

int delay_gcd;
const unsigned short tasksNum = 4; //reminder to check/change to correct #
task tasks[tasksNum];

//This is where the IR sensor had potential use
bool playing;
enum SM0_states {SM0_0, SM0_1};
int SM0_Tick(int state0) {
  switch(state0){
    /*case SM0_0:
    //mole1 == LOW;
    
    case SM0_1:
    mole1 == HIGH;
  }
  switch(state0){
   case SM0_0:
   //playing = true;
   
   case SM0_1:
   playing = false;*/
  }
}
enum SM1_states {SM1_0, SM1_1}; //ON AND OFF STATE
int SM1_Tick(int state1) {
  switch(state1){
    case SM1_0:
        state1 = SM1_0;
    }
    
  unsigned int val,n; 
  
  digitalWrite(m1, 0);
  
  digitalWrite(m2, 0);
  
  digitalWrite(m3, 0);
  
  int temp = rand() % 500 + 100;

  val = millis();
  n = millis() + temp;
  
  while(millis() < n)
  {
    light1=digitalRead(mole1); light2=digitalRead(mole2); light3=digitalRead(mole3); 

    if(light1 == HIGH)
    { 
       tone(buzzer, 100, 200);
    }

    if(light2 == HIGH)
    { 
       tone(buzzer, 100, 200);
    }

    if(light3 == HIGH)
    { 
       tone(buzzer, 100, 200);
    } 

    /*else(light3 == LOW)
    {
        //
    }*/
  }
  return state1;
}
 
bool on_off= true;
enum SM2_states {off, on}; //ON AND OFF STATE
int SM2_Tick(int state2) {
  switch(state2){
    case off:
        if(on_off == true) {
            state2= on;
        }
        else {
            state2= off;
        }
        break;

    case on:
      if(on_off == true) {
            state2= on;
        }
        else {
            state2= off;
        }
        break;
  }
  switch(state2) {
    case off:
    mole = 0;

    if(mole!=1)
    {
      on_off = false;
    }
    break;
    
    case on:
    
      mole = rand() % 3 + 1;
      
      if (mole==1)
      {
      digitalWrite(m1, HIGH); 
      }

      else if (mole==2)
      {
      digitalWrite(m2, HIGH);
      }

      else if (mole==3)
      {
      digitalWrite(m3, HIGH); 
      }

      else{
         //Serial.print("testing");
      }
      break;
  }
  return state2;
}

enum SM3_states {SM3_0}; //ON AND OFF STATE
int SM3_Tick(int state3) {
  switch(state3) {
    case SM3_0:
        state3 = SM3_0;
        
    }
    switch(state3) {

  case SM3_0:
  unsigned int val;
  unsigned int n;
  val = millis();

  n = millis() + 500;

  while(millis() < n)
  {
    light1 = digitalRead(mole1);  

// ======= WHACKING 1 =======
//if low/high -> update score
    if(light1==1)
    {         
      if(mole==1)
      {
        tone(buzzer, 1000, 10); 
        score++;
      } 
      
      else
      {
        tone(buzzer, 100, 200); 
      }  
    }
// ======= WHACKING 2 =======
//if low/high -> update score
    light2=digitalRead(mole2);
    if(light2==1)
    {        
      if(mole!=2)
      {
        tone(buzzer, 100, 200);
      } 

      else if(mole==2)
      {
        tone(buzzer, 1000, 10);
        score++;
      } 
      
      else
      {
        tone(buzzer, 100, 200);
      }  
    }

// ======= WHACKING 3 =======
//if low/high -> update score
    light3 = digitalRead(mole3);
    if(light3 == 1)
    {        
      if(mole!=3)
      {
        tone(buzzer, 100, 200);
      } 
      
      else if(mole==3)
      {
        tone(buzzer, 1000, 10);
        score++;
      } 

      else{
        tone(buzzer, 100, 200);
      }
    }    
  } 
  break;
}
return state3;
}

// ======= TIMER =======
//60 second countdown
//check complete. functioning timer.
/*void setup(){
Serial.begin(9600);
}
void loop(){
static int countDown = 60;
Serial.println(countDown);
if(countDown --== 0) countDown = 60;
delay (1000);
//delayMicroseconds(1000);

}*/

// ======= EEPROM =======

void writeEEPROM(int ADDR, byte val, int i2cADDR)
{
  Wire.beginTransmission(i2cADDR);
  Wire.write((int)(ADDR >> 8));
  Wire.write((int)(ADDR & 0xFF));

  Wire.write(val);
  Wire.endTransmission();
}
 
byte readEEPROM(int ADDR, int i2cADDR)
{
  byte DATA = 0xFF;
  Wire.beginTransmission(i2cADDR);
  Wire.write((int)(ADDR >> 8));

  Wire.write((int)(ADDR & 0xFF));
  Wire.endTransmission();
  Wire.requestFrom(i2cADDR, 1);
  DATA =  Wire.read();
  return DATA;
}

void setup()
{
  Wire.begin();
  Serial.begin(9600);
  pinMode(m1, OUTPUT); 
  pinMode(m2, OUTPUT); 
  pinMode(m3, OUTPUT); 
  pinMode(mole1, INPUT); 
  pinMode(mole2, INPUT); 
  pinMode(mole3, INPUT); 

  unsigned char i = 0;

  tasks[i].state = SM0_0;
  tasks[i].period = 100;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &SM0_Tick;
  i++; //
  
  tasks[i].state = SM1_0;
  tasks[i].period = 100;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &SM1_Tick;
  i++;

  tasks[i].state = off;
  tasks[i].period = 100;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &SM2_Tick;
  i++;

  tasks[i].state = SM3_0;
  tasks[i].period = 100;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &SM3_Tick;

  /*tasks[i].state = SM4_0;
  tasks[i].period = 100;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &SM4_Tick;*/

}

void loop() {
  unsigned char i;
  HIGHSCORE= readEEPROM(0, EEPROM_i2cADDR);
  //Serial.print(HIGHSCORE);
  Serial.println();
  
  Serial.print(score);
  Serial.println();

  //Timer
  /*static int countDown = 60;
  Serial.println(countDown);
  if(countDown --== 0) countDown = 60;*/

  for (i = 0; i < tasksNum; ++i) {
    if ( (millis() - tasks[i].elapsedTime) >= tasks[i].period) {
      tasks[i].state = tasks[i].TickFct(tasks[i].state);
      tasks[i].elapsedTime = millis();
    }
  }
}
