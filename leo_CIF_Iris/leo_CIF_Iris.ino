#include <Servo.h>
Servo sv;
const int servoPin = 10;
const int sense = A0;
const int knobPin = A1;

void setup() {
  pinMode(knobPin, INPUT);
  pinMode(sense, INPUT);
  pinMode(servoPin, OUTPUT);
  sv.attach(servoPin, 530, 2400);
  Serial.begin(9600);
 // while(!Serial);
  Serial.println("Start");
}

int val, index, lasta, avgdistance, lastKnob = 0;
unsigned long last2, last, knobTimeout = 0;
int distances[20];
boolean reset = true;
boolean knobTurning = false;

void loop() {
  int knob = analogRead(knobPin);
  int send = map(knob, 0, 1023, 0, 103);
  int lastSend = 0;
  if(knobTurning && abs(send - lastSend) > 3){
    sv.write(send);
    lastSend = send;
  }
  if (abs(knob - lastKnob) > 70) {
    lastKnob = knob;
    knobTimeout = millis(); //reset timer
    knobTurning = true;
  }
  else {
    if (millis() - knobTimeout < 2000) {
      //Serial.print("Knob timeout: ");
      //Serial.println(millis() - knobTimeout);
      reset = true;     
    }
    
    else {     
      if(reset){
        reset = false;
        knobTurning = false;
        sv.write(0);      
      }
      if (millis() - last > 10) {//every 10 ms take a new reading for avg dist
        last = millis();
        val = analogRead(sense);        
        

        distances[index] = val;
        index++;

        if (index == 20) {
          index = 0;
        }
        for(int i = 0; i < 20; i++){
          avgdistance += distances[i];
        }
        avgdistance = constrain(avgdistance/ 20, 150, 700)/10; //15 to 70
                       
        //Serial.print("val: ");
        //Serial.println(avgdistance);
        sv.write(map(avgdistance,15,70,0,104));
      }
    }
  }
}

/*
float mapfloat(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (float)(x - in_min) * (out_max - out_min) / (float)(in_max - in_min) + out_min;
}
*/
