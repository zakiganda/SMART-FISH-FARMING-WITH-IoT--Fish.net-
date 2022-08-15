#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPLeWyarDgM"
#define BLYNK_DEVICE_NAME "Fishnet"
#define BLYNK_AUTH_TOKEN "DYr7wWxW7G_NJozpn6eQN20ZxYkLrjPI"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SoftwareSerial.h>
SoftwareSerial arduinoUno(2,3);
//#include <BlynkTimer.h>
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).

//WidgetLCD lcd(V2);
String data;
String I;
char auth[] = "DYr7wWxW7G_NJozpn6eQN20ZxYkLrjPI";
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Hotspot@PUTRA-2.4G";
char pass[] = "";
BlynkTimer timer;
String myString;
char rdata;
float firstVal, secondVal, thirdVal;
int pinValue1;
BLYNK_CONNECTED(){
 Blynk.syncVirtual(V5);
}
BLYNK_WRITE(V5){
 pinValue1 = param.asInt();
 Serial.print("Waterpump");
 Serial.println(pinValue1);
 arduinoUno.print(pinValue1);
}
void myTimerEvent()
{
 
 Blynk.virtualWrite(V1, millis() / 1000); //ph
 //Blynk.virtualWrite(V2, millis() / 1000); //temp
 //Blynk.virtualWrite(V3, millis() / 1000); //water level
 //Blynk.virtualWrite(V4, millis() / 1000); //
 //Blynk.virtualWrite(V5, millis() / 1000); //
 
}
void setup()
{
 // Debug console
 Serial.begin(9600);
 Blynk.begin(auth, ssid, pass);
 timer.setInterval(500L,sensorvalue1);
 timer.setInterval(500L,sensorvalue2);
 timer.setInterval(500L,sensorvalue3);
}
void loop()
{
 if (Serial.available() == 0 ) 
 {
 Blynk.run();
 timer.run(); // Initiates BlynkTimer
 }
 
 if (Serial.available() > 0 ) 
 {
 rdata = Serial.read(); 
 myString = myString+ rdata; 
 Blynk.virtualWrite(V4, myString);
 //Serial.print(rdata);
 if( rdata == '\n')
 {
 String l = getValue(myString, ',', 0);
 String m = getValue(myString, ',', 1);
 String n = getValue(myString, ',', 2);
 
 firstVal = l.toFloat();
 secondVal = m.toFloat();
 thirdVal = n.toFloat();
 myString = "";
 }
 }
}
String getValue(String data, char separator, int index)
{
 int found = 0;
 int strIndex[] = { 0, -1 };
 int maxIndex = data.length() - 1;
 for (int i = 0; i <= maxIndex && found <= index; i++) {
 if (data.charAt(i) == separator || i == maxIndex) {
 found++;
 strIndex[0] = strIndex[1] + 1;
 strIndex[1] = (i == maxIndex) ? i+1 : i;
 }
 }
 return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
void sensorvalue1()
{
//data = data + I; 
//lcd.print(0,0,"pH Value:");
//lcd.print(0,1,data);
// data = "";
int sdata = firstVal;
Blynk.virtualWrite(V9, sdata);
}
void sensorvalue2()
{
float sdata = secondVal;
Blynk.virtualWrite(V1, sdata);
}
void sensorvalue3()
{
float sdata = thirdVal;
Blynk.virtualWrite(V2, sdata);
}
