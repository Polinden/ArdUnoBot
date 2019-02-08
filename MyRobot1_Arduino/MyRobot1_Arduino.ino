#include <DHT.h>
#include <NewPing.h>
#define MOTOR1PIN 9
#define MOTOR1POW 10
#define MOTOR2PIN 13
#define MOTOR2POW 11
#define DHTPIN 8
#define SOUNDPIN 2
#define DHTTYPE DHT11
#define TRIGGER_PIN  6
#define ECHO_PIN     7
#define MOVE_PIN     3
#define MAX_DISTANCE 600
#define LEDPIN1 4
#define LEDPIN2 5
#define LEDPIN3 12


DHT dht(DHTPIN, DHTTYPE);
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
int minSpeed = 90;
int maxSpeed = 160;
int mdlSpeedL=minSpeed/1.4;
int mdlSpeedM=maxSpeed/1.2;
int noSpeed = 0;
int mindistance=100;
boolean toclose=false;
int distance = 600;
int temp = 100;
int humyd = 99;
int host = 0;
long ms1=0;
long ms2=0;
long ms3=0;
long ms4=0;
long ms5=0;
long ms6=0;
long ms7=0;
unsigned char tryMe=0;
unsigned char tryBefore=0;
boolean pick=true;
boolean sqweek=false;
boolean ledoff=true;




void Motor1(int pwm, boolean reverse) {
  analogWrite(MOTOR1POW, pwm);
  if (reverse)  {
    digitalWrite(MOTOR1PIN, HIGH);
  }
  else  {
    digitalWrite(MOTOR1PIN, LOW);
  }
}

void Motor2(int pwm, boolean reverse) {
  analogWrite(MOTOR2POW, pwm);
  if (reverse)  {
    digitalWrite(MOTOR2PIN, LOW);
  }
  else  {
    digitalWrite(MOTOR2PIN, HIGH);
  }
}


void speedControl(unsigned char range)
{
  switch (range) {
    case 7: // move back  
      Motor1(maxSpeed, true);  
      Motor2(maxSpeed, true);
      break;
    case 8: // move back slow
      Motor1(minSpeed, true);  
      Motor2(minSpeed, true);
      break;
    case 9: // move back and right 
      Motor1(mdlSpeedL, true);  
      Motor2(mdlSpeedM, true);
      break;  
    case 10: // move back and left  
      Motor1(mdlSpeedM, true);  
      Motor2(mdlSpeedL, true);
      break;  
    case 11:  // deccreaced speed  
      sqweek=true;
      break;  
    case 12:  // normal speed  
      sqweek=false;
     break;  
    case 13: // stop  
      Motor1(noSpeed, true);  
      Motor2(noSpeed, true);
      break;  
    case 14:  // move forward
      Motor1(maxSpeed, false);  
      Motor2(maxSpeed, false); 
      break;
    case 15:  // move forward slow
      Motor1(minSpeed, false);  
      Motor2(minSpeed, false); 
      break;
    case 16: // move forward right  
      Motor1(mdlSpeedL, false);  
      Motor2(mdlSpeedM, false); 
      break;
    case 17: // move forward left 
      Motor1(mdlSpeedM, false);  
      Motor2(mdlSpeedL, false); 
      break;
    case 18:  // turn right  
      Motor1(mdlSpeedM, true);  
      Motor2(mdlSpeedM, false); 
      break;
    case 19:  // turn left  
      Motor1(mdlSpeedM, false);  
      Motor2(mdlSpeedM, true);
      break;
  }
}


void megureSensor1(){ 
     distance=sonar.ping_cm();
     if (distance<5) distance=600;
     /*
     long duration1=0, duration2=0;
     int dist=0;
     digitalWrite(TRIGGER_PIN, LOW);
     delayMicroseconds(2);
     digitalWrite(TRIGGER_PIN, HIGH);
     delayMicroseconds(5);
     digitalWrite(TRIGGER_PIN, LOW);  
     duration1 = pulseIn(ECHO_PIN, HIGH);
     delayMicroseconds(2);
     digitalWrite(TRIGGER_PIN, HIGH);
     delayMicroseconds(5);
     digitalWrite(TRIGGER_PIN, LOW);  
     duration2 = pulseIn(ECHO_PIN, HIGH);
     duration1=(duration1+duration2)/2;
     distance=duration1*0.034/2;
     if (distance>500) distance=500;
     if (distance<1) distance=1;
     */
     if (distance<mindistance) {speedControl(13); toclose=true;}
        else toclose=false;
        
}

void megureSensor2(){
     if (digitalRead(MOVE_PIN)==HIGH) {host++;}
}

void megureSensor3(){
     temp=round(dht.readTemperature());
     humyd=round(dht.readHumidity());
}


void sendData1(){
        if (distance/10>50) Serial.write(250); else Serial.write(200+(byte)round(distance/10));
}

void sendData2(){
        if (host>0) {Serial.write(21);  host=0;} else Serial.write(20);
}

void sendData3(){
        Serial.write(50+temp);
        Serial.write(100+humyd);
}

void setup() {
    Serial.begin(4800); 
    pinMode(MOTOR1PIN, OUTPUT); 
    pinMode(MOTOR2PIN, OUTPUT); 
    pinMode(MOTOR1POW, OUTPUT); 
    pinMode(MOTOR2POW, OUTPUT); 
    pinMode(TRIGGER_PIN, OUTPUT); 
    pinMode(ECHO_PIN, INPUT);
    pinMode(MOVE_PIN, INPUT); 
    pinMode(DHTPIN, INPUT); 
    pinMode(SOUNDPIN, OUTPUT); 
    pinMode(LEDPIN1, OUTPUT); 
    pinMode(LEDPIN2, OUTPUT); 
    pinMode(LEDPIN3, OUTPUT); 
    dht.begin();
    speedControl(13);                     
}

     

void loop() {  
    while (Serial.available() > 0) {
        tryMe=Serial.peek();
        if (toclose and tryMe>13 and tryMe<18) {Serial.read();break;}
        tryMe=Serial.read();
        if (tryMe<20 and tryMe>0) 
          if (tryMe==tryBefore) {speedControl(tryMe); break;}
          else {speedControl(tryMe); tryBefore=tryMe;}
        delay(3);
    }
    if (ms1++>30000) {megureSensor1(); ms1=0;}
    if (ms2++>70000) {sendData1(); ms2=0;}
    if (ms3++>100000) {megureSensor2(); ms3=0;}
    if (ms4++>200000) {sendData2(); ms4=0;}
    if (ms5++>2000000) {megureSensor3(); sendData3(); ms5=0;}      
    if (sqweek) {  
      if (pick && ms6++>60) {digitalWrite(SOUNDPIN, HIGH);digitalWrite(LED_BUILTIN, HIGH); pick=false;}
      if (not pick && ms6++>120) {digitalWrite(SOUNDPIN, LOW); digitalWrite(LED_BUILTIN, LOW); pick=true; ms6=0;}       
    }
    if (host>0 and ledoff) {
        ledoff=false;
        digitalWrite(LEDPIN1, HIGH);
        digitalWrite(LEDPIN2, HIGH);
        digitalWrite(LEDPIN3, HIGH);
    }
    if (host==0 and (not ledoff))
    {
        ledoff=true;
        digitalWrite(LEDPIN1, LOW);
        digitalWrite(LEDPIN2, LOW);
        digitalWrite(LEDPIN3, LOW);
      
    }
}
