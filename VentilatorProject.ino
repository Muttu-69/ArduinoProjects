#include <SoftwareSerial.h>     // LIBERY
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);  
#include <Wire.h> 

#define USE_ARDUINO_INTERRUPTS true
#include <PulseSensorPlayground.h>

#define lm35 A3              
    int sensorPin = A1;
    int sensorValue;  
    
#define relay 12            
#define limit_s A2

String apiKey = "6HSP32519VFPCNJE";    // Edit this API key according to your Account
String Host_Name = "sachinsachin";         // WIFA Host_Name
String Password = "sachinsachin";          // WIFA Password

SoftwareSerial ser(8, 9);              // RX, TX    ESP8266 PIN 

int i = 1;
String cmd;


const int PulseWire = A0;
int Threshold = 550;
PulseSensorPlayground pulseSensor;


void setup()
{

  Serial.begin(115200);                  // enable software serial
  ser.begin(115200);
  ser.println("AT+RST");               // Resetting ESP8266

    Serial.begin(115200);
  
  char inv = '"';
  String cmd = "AT+CWJAP";

  cmd += "=";
  cmd += inv;
  cmd += Host_Name;
  cmd += inv;
  cmd += ",";
  cmd += inv;
  cmd += Password;
  cmd += inv;
  ser.println(cmd);                    // Connecting ESP8266 to your WiFi Router


  // set up the LCD's number of columns and rows:
  lcd.backlight();
  lcd.init(); 

 
  lcd.clear();
  
  lcd.print("    JAIN");           //FIRST LINE
  lcd.setCursor(0, 1);
  lcd.print("    COLLEGE");                //SECOND LINE
  delay(3000);
  lcd.clear();
 
  lcd.print("    E C ");           //FIRST LINE
  lcd.setCursor(0, 1);
  lcd.print("    DEPARTMENT");   
  delay(3000);
  lcd.clear();
  
    lcd.print("THE PEOPLE'S");           //FIRST LINE
  lcd.setCursor(0, 1);
  lcd.print("VENTILATOR");                //SECOND LINE
  delay(3000);
  lcd.clear();

  lcd.print("ELECTRONICS &");           //FIRST LINE
  lcd.setCursor(0, 1);
  lcd.print("COMMUNICATION DEPARTMENT");   
  delay(3000);
  lcd.clear();
  
    lcd.print("HOD");           //FIRST LINE
  lcd.setCursor(0, 1);
  lcd.print("DR KRUPA RASANE");                //SECOND LINE
  delay(3000);
  lcd.clear();

    lcd.print("GUIDE");           //FIRST LINE
  lcd.setCursor(0, 1);
  lcd.print("PROF RAVINDRA M S");                //SECOND LINE
  delay(3000);
  lcd.clear();

    pulseSensor.analogInput(PulseWire);   
  pulseSensor.setThreshold(Threshold);

  if (pulseSensor.begin())
  { 
    Serial.println("PulseSensor object created!");

}


pinMode(limit_s,INPUT_PULLUP);
pinMode(relay,OUTPUT);

}

void loop()
{
   temp();
    heartrate();
  vent();
    moisture();
  EstablishTCPconnection();
  UpdatetoTS();
  
}

void temp()
      {
          int temp=analogRead(lm35)*0.488;
             lcd.print("Temp: ");
            lcd.print(temp);
            lcd.print(" C");
            delay(1000);
             lcd.clear();
      } 

void heartrate()
{
int myBPM = pulseSensor.getBeatsPerMinute();     
       
 // if (pulseSensor.sawStartOfBeat()) 
                   
                 lcd.setCursor(1, 0);
                 lcd.print("HEART RATE");
                 lcd.setCursor(0, 1);
                 lcd.print(myBPM);
                 delay(1000);
                   lcd.clear();
          
          
                    if(myBPM>80)  
                                                                                         
                                    {
                                      lcd.setCursor(1, 0);
                                      lcd.print("BP HIGH"); 
                                      digitalWrite(relay, HIGH);
                                      delay(2000);
                                      lcd.clear();
                                    }
                                    else
                                    {  
                                      digitalWrite(relay, LOW);
                                    }
                          
                    
              }
  void vent()
 {
          int limit_state=digitalRead(limit_s);  
          if(limit_state==0)
          {
          digitalWrite(relay, HIGH);
          delay(10000);
          }
          else
          {
            digitalWrite(relay, LOW);
          }
        }


         
 void moisture()
 {
      lcd.clear();
       lcd.setCursor(0, 0);
      lcd.print("moisture val");
       sensorValue = analogRead(sensorPin);                                       
       lcd.setCursor(0, 1);
      lcd.print(sensorValue);
      delay(2000);
       lcd.clear();
 }
 
void EstablishTCPconnection()
{
  String cmd = "AT+CIPSTART=\"TCP\",\"";          // Establishing TCP connection
  cmd += "184.106.153.149";                       // api.thingspeak.com
  cmd += "\",80";                                 // port 80

  ser.println(cmd);
  Serial.println(cmd);
  if (ser.find("Error")) 
  {
    Serial.println("AT+CIPSTART error");
    return;
  }
}

void UpdatetoTS()
{
  int temp=analogRead(lm35)*0.488;
  int myBPM = pulseSensor.getBeatsPerMinute(); 
  sensorValue = analogRead(sensorPin);

 String state1 = String(temp);               // Converting them to string as to send it through URL
  String state2 = String(myBPM);
String state3 = String(sensorValue);

  String getStr = "GET /update?api_key=";         // prepare GET string
  getStr += apiKey;
  getStr += "&field1=";
  getStr += String(state1);
  getStr += "&field2=";
  getStr += String(state2);
    getStr += "&field3=";
  getStr += String(state3);
  
  getStr += "\r\n\r\n";
  
  cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());                // Total Length of data

  ser.println(cmd);
  Serial.println(cmd);

  if (ser.find(">")) 
  {
    ser.print(getStr);
    Serial.print(getStr);
  }
  else 
  {
    ser.println("AT+CIPCLOSE");                  // closing connection
    // alert user
    Serial.println("AT+CIPCLOSE");
  }
  
}