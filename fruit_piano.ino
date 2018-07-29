/*
  Keyboard Message test

  For the Arduino Leonardo and Micro.

  Sends a text string when a button is pressed.

  The circuit:
  - pushbutton attached from pin 4 to +5V
  - 10 kilohm resistor attached from pin 4 to ground

  created 24 Oct 2011
  modified 27 Mar 2012
  by Tom Igoe
  modified 11 Nov 2013
  by Scott Fitzgerald

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/KeyboardMessage
*/

#include "Keyboard.h"


int buttons[]= {A9, A0, A1, A2, A3 };
int buttonsCount,buttonState;

int referenceAnalogIn=A7, difference;

int measurements[]={0,0,0,0,0,0,0,0,0,0}; //array for couting keyups
int maxdif[10];

      
int flagpress;   // flag of press any key

int selectButton=8, sbState, sbState_old, led = 10;


const int Mode_GH=0 , Mode_1234=1, Mode_ASDF=2, Mode_ZXCV=3;
int mode=0;

char keys[4][5] = {"12345","12345","asdfg","zxcvb"};

void setup() {
  for(int i=0; i< buttonsCount; i++){
  pinMode(buttons[i], INPUT);
  }
  pinMode(referenceAnalogIn, INPUT);

  pinMode(led, OUTPUT);
  pinMode(selectButton, OUTPUT);
  
  while(millis()<10000) // time for upload new sketch
  {}
  
  Keyboard.begin();
  Serial.begin(9600);
  
  buttonsCount = sizeof( buttons)/ sizeof( buttons[0] );
  
  digitalWrite(led,HIGH);
  delay(800);
  digitalWrite(led,LOW);
}

void loop() {
  sbState = digitalRead(selectButton);
  if(sbState > sbState_old){
    mode++;
    if(mode>=4) mode=1;
    for(int i=1; i<= mode; i++)
    {
      digitalWrite(led,HIGH);
      delay(800);
      digitalWrite(led,LOW);
      delay(800);
      }
    
    }
  sbState = sbState_old;

  
  flagpress=0;

  for(int i=0; i< buttonsCount; i++){
    
    buttonState = analogRead(buttons[i]);
    difference = analogRead(referenceAnalogIn)-buttonState;
 
    if (difference > 200 ||  (measurements[i]>0 && buttonState>1000))
      {
        if(difference > maxdif[i]){ maxdif[i]= difference;
            measurements[i]=12;
        }
        if(difference<=100) maxdif[i]=0; // new cycle
        if(measurements[i]>0) measurements[i]--;
          
          
          flagpress=1;
          Keyboard.press(keys[mode][i]);
          delay(20);
          Keyboard.press(176);
          delay(50);
          Serial.print(buttons[i]);
          Serial.print("=");
          Serial.print(buttonState);
          Serial.print("\t ");
      }else{
        measurements[i]=0;
        maxdif[i]=0;
         Keyboard.release(keys[mode][i]);
         }    
    }

  if(flagpress==1){

    Serial.println(" ");
    }
 else{
    if(mode == Mode_GH) Keyboard.releaseAll(); 
  
  }
delay(120);
}
