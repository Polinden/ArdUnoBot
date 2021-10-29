#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>


char auth[] = "";
char ssid[] = "";
char pass[] = "";
int middle = 510;
int maxim = 1020;
int delta =200;
byte curMove=0;
byte prevMove=1;
bool result=true;


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

  if (prevMove!=curMove) {
       Serial.write(curMove); 
       prevMove=curMove;
  }
}

void changeState()
{
  prevMove=1;
  moveControl(middle, middle);
}


BLYNK_WRITE(V3)
{
  int x = param[0].asInt();
  int y = param[1].asInt();
  moveControl(x,y); 
}


BLYNK_WRITE(V2)
{
  int z = 30 + param.asInt() / 10;    
  Serial.write(z); 
}



void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
}


void loop()
{        
    result = Blynk.connected();
    if (!result) {    
      changeState();
      Blynk.disconnect();
      Blynk.connect(15);      
    }
    Blynk.run();
}
