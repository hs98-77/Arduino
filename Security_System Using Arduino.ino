#include <Keypad.h>
#include <Password.h>

//conflict test
//JFT



const byte ROWS = 4; //four rows

const byte COLS = 4; //four columns

const char keys[ROWS][COLS] = {
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'}
  };

const byte rowPins[ROWS] = {13,12,11,10};

const byte colPins[COLS] = {9,8,7,6};

Keypad    keypad { makeKeymap(keys), rowPins, colPins, ROWS, COLS };







const byte PIR=A0;
const byte MQ2=A1;
const byte LM35=A2;
const byte LDR=A3;
const byte CZN_15E=A4;
const byte BLUE_LED=A5;                                       
const byte BUZZER=2;
const byte LAMP=3;
const byte GREEN_LED=4;
const byte RED_LED=5;



Password passwd = Password("0000");





bool ACTIVE=false;
bool ALARM=false;
int WRONG_G=0;










int PIR_STATE=LOW;
int MQ2_STATE;
float LM35_STATE;
int LDR_STATE;
int CZN_15E_STATE;


void setup() {
  
  Serial.begin(9600);
  pinMode(BLUE_LED,OUTPUT);
  pinMode(PIR,INPUT);
  pinMode(MQ2,INPUT);
  pinMode(LM35,INPUT);
  pinMode(LDR,INPUT);
  pinMode(CZN_15E,INPUT);
  pinMode(BUZZER,OUTPUT);
  pinMode(LAMP,OUTPUT);
  pinMode(GREEN_LED,OUTPUT);
  pinMode(RED_LED,OUTPUT);
  digitalWrite(BLUE_LED,HIGH);
}




void loop() {
  
  PIR_STATE=digitalRead(PIR);
  MQ2_STATE=analogRead(MQ2);
  LM35_STATE=analogRead(LM35)*0.488758;
  LDR_STATE=analogRead(LDR);
  CZN_15E_STATE=analogRead(CZN_15E);


   Serial.print("PIR=");
   Serial.println(PIR_STATE);
   Serial.print("MQ2=");
   Serial.println(MQ2_STATE);
   Serial.print("LM35=");
   Serial.println(LM35_STATE);
   Serial.print("LDR=");
   Serial.println(LDR_STATE);
   Serial.print("CZN_15E=");
   Serial.println(CZN_15E_STATE);






  
  char key=keypad.getKey();
  if(key != NO_KEY ){
    switch (key)
  {
    case '*':
    Serial.print(passwd.evaluate());
    if(passwd.evaluate())
    {
      ALARM=false;
      WRONG_G=0;
      for(int i=0;i<3;i++)
      {
        digitalWrite(GREEN_LED,HIGH);
        digitalWrite(BUZZER,HIGH);
        delay(70);
        digitalWrite(RED_LED,HIGH);
        digitalWrite(GREEN_LED,LOW);
        digitalWrite(BUZZER,LOW);
        delay(70);
        digitalWrite(RED_LED,LOW);
      }
      if(ACTIVE)
      {
        digitalWrite(BLUE_LED,HIGH);
        ACTIVE=false;
      }
      else
      {
        digitalWrite(GREEN_LED,HIGH);
        digitalWrite(BLUE_LED,LOW);
        ACTIVE=true;
      }
    }
    else
    {
      WRONG_G++;
      if(WRONG_G<3)
      {
        for(int i=0;i<WRONG_G;i++)
        {
          digitalWrite(BUZZER,HIGH);
          delay(40);
          digitalWrite(BUZZER,LOW);
          delay(100);
        }
      }
      else if(WRONG_G>=3)
      {
        for(int i=0;i<10;i++)
        {
          digitalWrite(BUZZER,HIGH);
          delay(100);
          digitalWrite(BUZZER,LOW);
          delay(900);
        }
      }
    }
    passwd.reset();
    break;
    case '#':
      passwd.reset();
      break;
    default:
      passwd.append(key);
      break;
  }
  }


  if(ACTIVE && LDR_STATE<400)
  {
    digitalWrite(LAMP,HIGH);
  }
  else if(ACTIVE && LDR_STATE>400)
  {
    digitalWrite(LAMP,LOW);
  }

  if(ALARM)
  {
    digitalWrite(RED_LED,HIGH);
    digitalWrite(GREEN_LED,LOW);
    digitalWrite(BUZZER,HIGH);
  }
  else if(ACTIVE && !ALARM)
  {
    digitalWrite(RED_LED,LOW);
    digitalWrite(GREEN_LED,HIGH);
    digitalWrite(BUZZER,LOW);
  }
  else if(!ACTIVE && !ALARM)
  {
    digitalWrite(RED_LED,LOW);
    digitalWrite(GREEN_LED,LOW);
    digitalWrite(BUZZER,LOW);
  }

  if(LM35_STATE>50 && ACTIVE)
  {
    ALARM=true;
  }

  if(MQ2_STATE>840 && ACTIVE)
  {
    ALARM=true;
  }

 
  /*if(PIR_STATE && ACTIVE)
  {
    ALARM=true;
  }*/
  
  if(CZN_15E_STATE > 90 && ACTIVE)
  {
    ALARM=true;
  }
}
