#include <SoftwareSerial.h>
SoftwareSerial nodemcu(2,3);
int data;
//pH Sensor
#define SensorPin 0 //pH meter Analog output to Arduino Analog Input A0
float avgValue; //Store the average value of the sensor feedback
float b;
float buf[10],temp;
// for float value to string converstion
int f;
 float val; // also works with double. 
 char buff2[10];
 String valueString = "";
 String Value = "";
 float phValue;
//Ultrasonic Sensor
#define echoPin 4 // attach pin D4 Arduino to pin Echo of HC-SR04
#define trigPin 5 //attach pin D5 Arduino to pin Trig of HC-SR04
// define variables (Ultrasonic Sensor)
long duration; // variable for the duration of sound wave travel
float distance; // variable for the distance measurement
String str;
float waterlevel;
float waterlevelValue;
String waterlevelValuestr;
// Arduino Water Temperature Sensor
#include <OneWire.h>
#include <DallasTemperature.h>
//define variables (Water Temperature)
const int SENSOR_PIN = 13; // Arduino pin connected to DS18B20 sensor's DQ pin
OneWire oneWire(SENSOR_PIN); // setup a oneWire instance
DallasTemperature tempSensor(&oneWire); // pass oneWire to DallasTemperature library
float tempCelsius; // temperature in Celsius
float tempFahrenheit; // temperature in Fahrenheit
float temperatureValue;
String temperatureValuestr;
//Buzzer
const int buzzer=12;
//WaterPump
const int waterpump = A5; // the Arduino pin, which connects to the IN pin of relay
//phup
const int phup = A4;
//phdown
const int phdown = A3;
//solenoid
const int valve = A2;
//LED
const int red_led = 8;
const int green_led = 9;
const int yellow_led = 10;
const int blue_led = 11;
String cdata; //complete data, consisting of sensors value
void setup()
{
 //pinMode(13,OUTPUT); 
 Serial.begin(9600); 
 nodemcu.begin(9600);
 
 //Setup for ultrasonic
 pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
 pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
 //Setup for temp sensor
 tempSensor.begin(); // initialize the sensor
 //Setup buzzer
 pinMode(buzzer,OUTPUT);
 //Setup waterpump
 pinMode(waterpump, OUTPUT);
 //Setup phup
 pinMode(phup, OUTPUT);
 //Setup phdown
 pinMode(phdown, OUTPUT);
 //Setup valve
 pinMode(valve, OUTPUT);
 //Setup LED
 pinMode(red_led, OUTPUT);
 pinMode(green_led, OUTPUT);
 pinMode(yellow_led, OUTPUT);
 pinMode(blue_led, OUTPUT); 
}
void loop()
{
 ultra();
 ph();
 temperature();
 cdata=cdata +waterlevelValuestr+ "," +valueString+ "," +temperatureValuestr;
Serial.println(cdata);
 nodemcu.println(cdata);
 delay (1000);
 cdata = "";
 Serial.println ("");
 manual();
}
void ultra() {
 // Clears the trigPin condition
 digitalWrite(trigPin, LOW);
 delayMicroseconds(2);
 // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
 digitalWrite(trigPin, HIGH);
 delayMicroseconds(10);
 digitalWrite(trigPin, LOW);
 // Reads the echoPin, returns the sound wave travel time in microseconds
 duration = pulseIn(echoPin, HIGH);
 // Calculating the distance
 distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
 // Displays the distance on the Serial Monitor
 Serial.print("Distance: ");
 Serial.print(distance);
 Serial.print("cm");
 Serial.print(" == ");
 waterlevel = ((17.68-distance)/17.68) * 100;
 for(int i=0;i<10;i++) //Get 10 sample value from the sensor for smooth the value
 { 
 buf[i]=waterlevel; //initialize temp sensor
 delay(10);
 }
 for(int i=0;i<9;i++) //sort the analog from small to large
 {
 for(int j=i+1;j<10;j++)
 {
 if(buf[i]>buf[j])
 {
 temp=buf[i];
 buf[i]=buf[j];
buf[j]=temp;
 }
 }
 }
 avgValue=0;
 for(int i=2;i<8;i++) //take the average value of 6 center sample
 avgValue+=buf[i];
 //Serial.print(avgValue);
 waterlevelValue=avgValue/6; //convert the analog into millivolt //convert the 
millivolt into pH value
 waterlevelValuestr= dtostrf(waterlevelValue, 4, 2, buff2);
 Serial.print(waterlevelValuestr);
 Serial.println("% water level");
 if(waterlevelValue>70){
 digitalWrite(buzzer, HIGH);
 digitalWrite(valve, HIGH);
 delay(15000);
 digitalWrite(buzzer, LOW);
 digitalWrite(valve, LOW);
 digitalWrite(waterpump, LOW);
 
 }
 else if(waterlevelValue<60){
 digitalWrite(buzzer, HIGH);
 digitalWrite(valve, LOW);
 digitalWrite(waterpump, HIGH);
 delay(3000);
 digitalWrite(buzzer, LOW);
 digitalWrite(valve, LOW);
 digitalWrite(waterpump, LOW); 
 }
 else{
 digitalWrite(valve, LOW);
 digitalWrite(waterpump, LOW);
 }
}
void ph() {
 for(int i=0;i<10;i++) //Get 10 sample value from the sensor for smooth the value
 { 
 buf[i]=analogRead(SensorPin); //initialize pH sensor
 delay(10);
 }
 for(int i=0;i<9;i++) //sort the analog from small to large
 {
 for(int j=i+1;j<10;j++)
 {
 if(buf[i]>buf[j])
 {
 temp=buf[i];
 buf[i]=buf[j];
 buf[j]=temp;
 }
 }
 }
 avgValue=0;
 for(int i=2;i<8;i++) //take the average value of 6 center sample
 avgValue+=buf[i];
 phValue=(float)avgValue*5.0/1024/6; //convert the analog into millivolt
 //Serial.println(phValue);
 phValue=3.5*phValue; //convert the millivolt into pH value
 
 Value = dtostrf(phValue, 4, 2, buff2); //4 is mininum width, 6 is precision
 valueString = Value; 
 Serial.print("pH: ");
 Serial.println(valueString);
 //nodemcu.println(valueString);
 //valueString = "";
 if(phValue<7){
 digitalWrite(phup, HIGH);
 delay(300);
 digitalWrite(phup, LOW);
 }
 else if(phValue>7.8){
 digitalWrite(phdown, HIGH);
 delay(300);
 digitalWrite(phdown, LOW);
 }
 else{
 digitalWrite(phdown, LOW);
 digitalWrite(phup, LOW);
 }
//delay(5000);
}
void temperature() {
 tempSensor.requestTemperatures(); // send the command to get temperatures
 //tempCelsius = tempSensor.getTempCByIndex(0); // read temperature in Celsius
 for(int i=0;i<10;i++) //Get 10 sample value from the sensor for smooth the value
 { 
 buf[i]=tempSensor.getTempCByIndex(0); //initialize temp sensor
 delay(10);
 //Serial.println(buf[i]);
 //Serial.println(tempSensor.getTempCByIndex(0));
 }
 for(int i=0;i<9;i++) //sort the analog from small to large
 {
 for(int j=i+1;j<10;j++)
 {
 if(buf[i]>buf[j])
 {
 temp=buf[i];
 buf[i]=buf[j];
 buf[j]=temp;
 }
 }
 }
 avgValue=0;
 for(int i=2;i<8;i++) //take the average value of 6 center sample
 avgValue+=buf[i];
 //Serial.print(avgValue);
 temperatureValue=avgValue/6; //convert the analog into millivolt //convert the 
millivolt into pH value
 temperatureValuestr= dtostrf(temperatureValue, 4, 2, buff2);
 Serial.print("Temperature: ");
 Serial.print(temperatureValuestr); // print the temperature in Celsius
 Serial.print("°C");
 Serial.println("");
 if(temperatureValue<27 || temperatureValue>30){
 digitalWrite(buzzer,HIGH);
 digitalWrite(waterpump, HIGH); // turn on pump 3 seconds
 waterpumpON();
 delay(3000);
 digitalWrite(buzzer, LOW);
 digitalWrite(waterpump, LOW); // turn off pump 3 seconds
 delay(3000);
 }
 else {
 digitalWrite(buzzer,LOW);
 digitalWrite(waterpump, LOW);
 }
 
 //delay(500);
}
void manual(){
 if(nodemcu.available()>0)
 {
 data = nodemcu.parseInt();
 delay(100);
 Serial.println(data);
 if(data == 1)
 {
 waterpumpON();
 Serial.println("Waterpump ON");
 //delay(5000);
 //digitalWrite(waterpump, LOW);
 }
 else
 {
 digitalWrite(waterpump, LOW);
 }
 }
}
void waterpumpON(){
 digitalWrite(waterpump, HIGH);
}
void waterpumpOFF(){
 digitalWrite(waterpump, LOW);
}
