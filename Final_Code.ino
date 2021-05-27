#include <Ultrasonic.h>
#include <SoftwareSerial.h>

#define _SS_MAX_RX_BUFF 256
int trigPin=12;                      //Pin on which Trig Pin of HC-SR04 is connected
int echoPin=13;                      //Pin on which Echo Pin of HC-SR04 is connected
int trigPin2=10;                      //Pin on which Trig Pin of HC-SR04 is connected
int echoPin2=11;                      //Pin on which Echo Pin of HC-SR04 is connected
#define EspRxPIN 8                      //Pin on which Rx of ESP Module is connected
#define EspTxPIN 9                      //Pin on which Tx of ESP Module is connected
#define EspBaudRate 9600                //Baud rate of communication with ESP Module
long duration,duration2,cm,cm2;

String domain = "api.thingspeak.com";
String url = "/update?";
String apiKey = "RFNIM41UJTMDBZMN";

SoftwareSerial mySerial(EspTxPIN, EspRxPIN); 

void setup() {
  Serial.begin(9600);
  mySerial.begin(EspBaudRate);
  mySerial.println("AT+CWJAP=\"amisha\",\"look@moon\"");
  delay(5000);
}

void loop() {
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(10);
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
  cm = (duration/2) / 29.1;  

  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(10);
  pinMode(echoPin2, INPUT);
  duration2 = pulseIn(echoPin2, HIGH);
  cm2 = (duration2/2) / 29.1;
  
  Serial.print(cm);
  Serial.print("cm, ");
  Serial.print(cm2);
  Serial.print("cm2, ");
  Serial.println();

  
  Serial.print("Sending : ");
  Serial.println("AT+CIPCLOSE");
  mySerial.println("AT+CIPCLOSE");
  delay(500);
  reader();

  String startString = "AT+CIPSTART=\"TCP\",\"";
  startString += domain;
  startString += "\",80";
  
  Serial.print("Sending : ");
  Serial.println(startString);
  mySerial.println(startString);
  delay(1500);
  reader();

  String msg = "GET ";
  msg += url + "api_key="  + apiKey + "&field1=" + String(cm)+ "&field2=" + String(cm2);    
  int len = msg.length();
  len += 2;
  Serial.print("Sending : ");
  Serial.println("AT+CIPSEND=" + String(len));
  mySerial.println("AT+CIPSEND=" + String(len));
  delay(500);
  reader();
  
  Serial.print("Sending : ");
  Serial.println(msg);
  mySerial.println(msg);
  delay(500);
  reader();
  long tym = millis();
  while(!mySerial.available())
  {
    if((millis() - tym) >5000)
    {
       // Serial.println("Timeout");
        break;
    }
  }
  if(mySerial.available())
  {
    while(mySerial.available())
    {
      delay(1);
      Serial.write(mySerial.read());
    }
  }
  
  Serial.println();
}

void reader()
{
  delay(500);
  if(mySerial.available()>0)
  {
      //Serial.println("Available");
      while(mySerial.available())
      {
        delay(20);
        Serial.write(mySerial.read());
      }
  }
}
