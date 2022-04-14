///// vaqt fayllarini kiritish///////
#include <DS3231.h>
DS3231  rtc(SDA, SCL);
/////////////////////////////////////

#include <EEPROM.h>

#include <dht.h>

#include <ds3231.h>

#define dht_apin A0
////// temperatura olchgichni nomlash ////
dht DHT;
//////////////////////////////////////////


//////////////// leds ///////////////////
const int LS = 4;
const int LP = 5;
const int RLedH = 7;
const int GLedH = 6;
const int RLedT = 9;
const int GLedT = 8;
//////////////////////////////////////////

// buzzer 
const int Buzz = 10;
//////////////////////////////////////////

///////// knopkalar raqamini nomlash ////////////////
const int StartB = 3; 
const int PauseB = 2;
///////////////////////////////////

//// lampani raqamini nomlash /////
const int Lampa = 11;
///////////////////////////////////

//// vaqtni nomlash //////////////
Time vaqt;
//////////////////////////////////

//// eeprom manzillarni nomlash //
const uint8_t addressMuddatDate = 105;
const uint8_t addressRuxsat = 103;
const uint8_t addressMuddatMin = 112;
//const uint8_t address = 115;
//////////////////////////////////
int in;
Time Muddat;
bool Ruxsat = false;
bool chiyilloq = false;


//// harorat raqamini nomlash/////

//////////////////////////////////

//// aylantirish vaqtlari ////////
Time ertalab;//= 8;
Time peshin;//= 13;
Time kech;// = 20;
//////////////////////////////////
 
void setup() {
  // put your setup code here, to run once

ertalab.hour = 4;
peshin.hour = 13;
kech.hour = 20;
ertalab.min = 30;
////////// chiroqlarga ulanishni boshlash ///

pinMode(LS, OUTPUT);
pinMode(LP, OUTPUT);
pinMode(RLedH, OUTPUT);
pinMode(GLedH, OUTPUT);
pinMode(RLedT, OUTPUT);
pinMode(GLedT, OUTPUT);
///////////////////////////

////// knopkalar bilan ishlash ////
pinMode(StartB, INPUT);
pinMode(PauseB, INPUT);
///////////////////////////////////

////// chiyilloqni ulash //////////
pinMode(Buzz, OUTPUT);
///////////////////////////////////

////// lampani boshqarish punkti //
pinMode(Lampa, OUTPUT);
///////////////////////////////////

////// vaqtni boshlash ////////////
rtc.begin();
///////////////////////////////////
delay(200);

if (EEPROM.read(addressRuxsat) == true)
{
Ruxsat = true;

Muddat.date = EEPROM.read(addressMuddatDate);

Muddat.min = EEPROM.read(addressMuddatMin);
digitalWrite(LS, HIGH);
vaqt = rtc.getTime();
delay(100);
        
//////////////////////////////////////////
 in = vaqt.sec + 10;
  
  if (in >=60)
  {
            if (in == 60)
            {
                in = 0;
            }
    in = (in - 60);
    
    }
//////////////////////////////////////////
}

Serial.begin(9600);


}






////////////////////////////////////
void loop() {///////////////////////
 int tin =0; 
vaqt = rtc.getTime();
tin = vaqt.sec;
unsigned char i;



if (digitalRead(StartB) == HIGH && Ruxsat == true)
{ int hold =0;
   
  while (digitalRead(StartB) == HIGH)
  {
    delay(20);
   hold++;
    }
  if (hold >= 90)
{
  Ruxsat = false;
  digitalWrite(LS, LOW);

  EEPROM.write(addressRuxsat, false);
  delay(50); 
  
  }
}  else { if (digitalRead(StartB) == HIGH){ 
  
  Ruxsat = true; digitalWrite(LS, HIGH); Muddat.date = 0;
Muddat.min = vaqt.date; in = tin + 10; 
  if (in >=60)
  {
               if (in == 60)
            {
                in = 0;
            }
    in = (in - 60);
  }
    }
    
  EEPROM.write(addressRuxsat, true);
    delay(50); 
     EEPROM.write(addressMuddatDate, Muddat.date);
       delay(50); 
  EEPROM.write(addressMuddatMin, Muddat.min);
  delay(50);  
    }
  


if (tin == in && Ruxsat == true)
{
  in = tin + 10;
  
  if (in >=60)
  {
            if (in == 60)
            {
                in = 0;
            }
    in = (in - 60);
    
    }
    
  if (Muddat.min != vaqt.date)
  {
    Muddat.date++;
    Muddat.min = vaqt.date;

      EEPROM.write(addressMuddatDate, Muddat.date);
  delay(10);
  EEPROM.write(addressMuddatMin, Muddat.min);
    delay(10);
    }
 
  DHT.read11(dht_apin);

 Serial.print("namlik: ");
  Serial.print(DHT.humidity);
  Serial.print("  harorat: "); Serial.print(DHT.temperature); Serial.print("  ");
  Serial.print(vaqt.hour); Serial.print(vaqt.min); Serial.println(vaqt.sec);
  Serial.print("tuxum qo'yilganiga"); 
  Serial.print( Muddat.date);
  Serial.println(" kun bo'ldi");
  
  
  if (( DHT.humidity >= 45 && DHT.humidity <= 60 && Muddat.date <= 18)||
  (DHT.humidity >= 65 && DHT.humidity <= 80 && Muddat.date > 18))
  {


    digitalWrite(RLedH, LOW); 
    digitalWrite(GLedH, HIGH);
            
    
    }else{  
      
      
      digitalWrite(GLedH, LOW); digitalWrite(RLedH, HIGH);

      if (DHT.humidity >= 60 && Muddat.date <= 18)
      {
        delay(150);
        digitalWrite(RLedH, LOW);
        delay(150);
        digitalWrite(RLedH, HIGH);
                delay(150);
        digitalWrite(RLedH, LOW);
        delay(150);
        digitalWrite(RLedH, HIGH);
                delay(150);
        digitalWrite(RLedH, LOW);
        delay(150);
        digitalWrite(RLedH, HIGH);
        
        }
        if (DHT.humidity >= 80 && Muddat.date >=18)
        {
        delay(150);
        digitalWrite(RLedH, LOW);
        delay(150);
        digitalWrite(RLedH, HIGH);
                delay(150);
        digitalWrite(RLedH, LOW);
        delay(150);
        digitalWrite(RLedH, HIGH);
                delay(150);
        digitalWrite(RLedH, LOW);
        delay(150);
        digitalWrite(RLedH, HIGH);
          }

       
      }



    
 /////////// temperaturaga aloqador punkt ////////////////////////
  if (DHT.temperature >= 37 && DHT.temperature <= 41)
  {
    digitalWrite(RLedT, LOW);
    digitalWrite(GLedT, HIGH);
    }else{ digitalWrite(GLedT, LOW); digitalWrite(RLedT, HIGH); }

  //if (DHT.temperature) 
  
  if (DHT.temperature <= 38 )
  {
    digitalWrite(Lampa, HIGH);
    }
    
  if (DHT.temperature >= 39)
  {
    digitalWrite(Lampa, LOW);
    }  
 ///////////////////////////////////////////////////////// 
  
  }

///// Serial aloqa qilish punkti /////////////////////////
   
   
//////////////////////////////////////////////////////////

if (digitalRead(PauseB) == HIGH)
{
    while (digitalRead(PauseB) == HIGH)
  {
    delay(15);
    } 
 chiyilloq = false;

  digitalWrite(LP, LOW);
  
  }
  


if (Ruxsat == false)
{
  digitalWrite(RLedT, LOW);
  digitalWrite(Lampa, LOW);
  digitalWrite(GLedH, LOW);
  digitalWrite(GLedT, LOW);
  digitalWrite(RLedH, LOW);
 }


if (vaqt.hour == ertalab.hour && vaqt.min == ertalab.min && ertalab.sec == vaqt.sec && Ruxsat == true)
{ 
    digitalWrite(LP, HIGH);

    chiyilloq = true;

  }

if (vaqt.hour == peshin.hour && vaqt.min == peshin.min && vaqt.sec == peshin.sec && Ruxsat == true)
{ 
    digitalWrite(LP, HIGH);

    chiyilloq = true;

  }
  
if (vaqt.hour == kech.hour && vaqt.min == kech.min && vaqt.sec == kech.sec && Ruxsat == true)
{ 
    digitalWrite(LP, HIGH);

    chiyilloq = true;

  }
  
//////////////////////////////////


if (chiyilloq == true)
{
  
  //////// chiyillash kodi /////////
    for (i = 0; i < 80; i++){
    digitalWrite(Buzz, HIGH);

  delay(2);
  digitalWrite(Buzz, LOW);
  delay(2);
    }
//////////////////////////////////

 }
delay(50);
}
