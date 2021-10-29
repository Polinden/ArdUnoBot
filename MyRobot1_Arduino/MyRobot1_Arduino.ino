#define MOTOR1PIN 9
#define MOTOR1POW 10
#define MOTOR2PIN 13
#define MOTOR2POW 11



int minSpeed = 100;
int maxSpeed = 255;
int mdlSpeedL = 120;
int mdlSpeedM = 220;
int noSpeed = 0;
unsigned char tryMe=0;



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

  if (range>29){    
     maxSpeed = 105+(range-30)*15; 
     mdlSpeedL = 70+(range-30)*5; 
     mdlSpeedM = 120+(range-30)*10;
  } else
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
      break;  
    case 12:  // normal speed        
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
      Motor2(mdlSpeedM, true);  
      Motor1(mdlSpeedM, false);
      break;
  }
}





void setup() {
    Serial.begin(9600); 
    pinMode(MOTOR1PIN, OUTPUT); 
    pinMode(MOTOR2PIN, OUTPUT); 
    pinMode(MOTOR1POW, OUTPUT); 
    pinMode(MOTOR2POW, OUTPUT); 
    speedControl(13);                     
}

     

void loop() {  
    while (Serial.available() > 0) {
        tryMe=Serial.read();                
        if (tryMe<50 and tryMe>0) speedControl(tryMe);
    }
}
