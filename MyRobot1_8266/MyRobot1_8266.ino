#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "****";
char ssid[] = "***";
char pass[] = "***";
int middle = 510;
int maxim = 1020;
int delta = 170;
byte curMove=0;
byte prevMove=1;
long repete=0;


void moveControl(int x, int y)
{
 // neutral zone
 if(x <= middle+delta && x>=middle-delta  && y <= middle+delta && y>=middle-delta) curMove=13; 
   
 else if(x <= middle+delta && x>=middle-delta) 
 {
    if(y>=maxim-delta) {
         curMove=14;  //forward
      }  
    else if(y>=maxim-2*delta){
         curMove=15;  //forward slow
    }
    else if(y<=delta){
         curMove=7;  //back
    }
    else if(y<=2*delta) {
         curMove=8;  //back slow
      }  
 }
 
 else if (y <= middle+delta && y>=middle-delta)
  {
    if(x>=maxim-delta) {
         curMove=18;   // turn right
      }  
    else if(x<=delta){
         curMove=19;  // turn left
    }
  }

  else if (x>=middle+delta && y>=middle+delta) 
    { 
        curMove=16;  // move forward right
    }
  else if (x<middle-delta && y>=middle+delta) 
    { 
        curMove=17;  // move forward left
    }

  else if (x>=middle+delta && y<middle+delta)
    { 
        curMove=9;  // move back right
    }
  else if (x<middle-delta && y<middle+delta) 
    { 
        curMove=10;  // move back left
    }

  Serial.write(curMove); 
}


void getData(unsigned char info){
   float d=0; 
   if (info>=200) {d=(info-200)/10.0; Blynk.virtualWrite(V7, d); return;}
    if (info>100) {Blynk.virtualWrite(V4, info-100); return;}
     if (info>21) {Blynk.virtualWrite(V5, info-50); return;}
         if (info==21) {Blynk.virtualWrite(V6, "Movement detected!"); return;} 
          if (info==20) Blynk.virtualWrite(V6, "No one...");
}


WidgetLED led1(V1);

BLYNK_WRITE(V3)
{
  int x = param[0].asInt();
  int y = param[1].asInt();
  moveControl(x,y); 
}

BLYNK_WRITE(V2)
{
 int pinValue = param.asInt();
  if (pinValue==1) {
    led1.on();
    Serial.write(11);
  } else {
    led1.off();
    Serial.write(12);
  }
}


void setup()
{
  Serial.begin(4800);
  Blynk.begin(auth, ssid, pass);
}


void loop()
{
    unsigned char tryMe=0;
    while (Serial.available() > 0) {
       tryMe=Serial.peek();
       if (tryMe>19) getData(Serial.read()); 
       delay(2);
    }
    Blynk.run(); 
}
