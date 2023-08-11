#include <Ultrasonic.h>
#define Echo 10
#define Trigger 11
#define buzzer 3

Ultrasonic ultrasonic(Trigger,Echo); //Echo, Trigger

void setup(){
  Serial.begin(9600);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(buzzer,OUTPUT);
}
void loop() {
  int cm=ultrasonic.read();
  Serial.println(cm);
  if ((cm>80)and(cm<130))
  {
    tone(buzzer,3000);
    digitalWrite(4,LOW);
    digitalWrite(5,LOW);
    delay(500);
    noTone(buzzer);
    delay(500);
  }
  if ((cm>30)and (cm<50))
  {
    tone(buzzer,2000);
    digitalWrite(4,HIGH);
    digitalWrite(5,LOW);
    delay(100);
    noTone(buzzer);
    delay(100);
  }
  if (cm<30)
  {
    tone(buzzer,1000);
    digitalWrite(4,LOW);
    digitalWrite(5,HIGH);
    delay(30);
    noTone(buzzer);
    delay(30);
  }
  delay(50);
}