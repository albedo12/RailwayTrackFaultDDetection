//
// Copyright 2015 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

// FirebaseDemo_ESP8266 is a sample that demo the different functions
// of the FirebaseArduino API.

#include <ESP8266WiFi.h>
#include<SoftwareSerial.h>
#include <FirebaseArduino.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include "ESP8266WiFi.h"
static const uint8_t trigPin=12;  
static const uint8_t echoPin=13;
static const uint8_t trigPin1=2;  
static const uint8_t echoPin1=14;   
static const uint8_t m1=16;  
static const uint8_t m2=5;  
static const uint8_t m3=4;  
static const uint8_t m4=0;
long duration; 
int dist;

char myssid[] = "Ronaldo";          // your network SSID (name) 
char mypass[] = "";          // your network password

//Credentials for Google GeoLocation API...
const char* Host = "www.googleapis.com";
String thisPage = "/geolocation/v1/geolocate?key=";
String key = "AIzaSyAaWvXN7mlpkfVkJjhjCCOeI94_YrHaINg";

int status = WL_IDLE_STATUS;
String jsonString = "{\n";

float latitude    = 0.0;
float longitude   = 0.0;

int more_text = 0; // set to 1 for more debug output

// Set these to run example.
#define FIREBASE_HOST "fir-ab762.firebaseio.com"
#define FIREBASE_AUTH "ourNCkEROWID65hprLSpmZZm83WAGzGy3gjTU0o3"
#define WIFI_SSID "Ronaldo"
#define WIFI_PASSWORD ""

void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(m1, OUTPUT);
  pinMode(m2, OUTPUT);
  pinMode(m3, OUTPUT);
  pinMode(m4, OUTPUT);
  Serial.begin(115200);
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Serial.println("Start");
  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
 // WiFi.disconnect();
  delay(100);
  Serial.println("Setup done");
}

int n = 0;

void loop() {
  // set value

digitalWrite(trigPin, LOW); 
delayMicroseconds(2); 
// Sets the trigPin on HIGH state for 10 micro seconds 
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);
// Calculating the distance
dist= duration*0.034/2;
// Prints the distance on the Serial Monitor
Serial.print("Distance: ");
Serial.println(dist);
if(dist>5){
  digitalWrite(m1,LOW);
  digitalWrite(m2,LOW);
  digitalWrite(m3,LOW);
  digitalWrite(m4,LOW);
  Serial.print("Motor Stopped");
char bssid[6];
  DynamicJsonBuffer jsonBuffer;  
  Serial.println("scan start");
  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0)
    Serial.println("no networks found");
  else
  {
    Serial.print(n);
    Serial.println(" networks found...");

  if(more_text){
    // Print out the formatted json...
    Serial.println("{");
    Serial.println("\"homeMobileCountryCode\": 234,");  // this is a real UK MCC
    Serial.println("\"homeMobileNetworkCode\": 27,");   // and a real UK MNC
    Serial.println("\"radioType\": \"gsm\",");          // for gsm
    Serial.println("\"carrier\": \"Vodafone\",");       // associated with Vodafone 
    //Serial.println("\"cellTowers\": [");                // I'm not reporting any cell towers      
    //Serial.println("],");      
    Serial.println("\"wifiAccessPoints\": [");
    for (int i = 0; i < n; ++i)
    {
      Serial.println("{");
      Serial.print("\"macAddress\" : \"");    
      Serial.print(WiFi.BSSIDstr(i));
      Serial.println("\",");
      Serial.print("\"signalStrength\": ");     
      Serial.println(WiFi.RSSI(i));
      if(i<n-1)
      {
      Serial.println("},");
      }
      else
      {
      Serial.println("}");  
      } 
    }
    Serial.println("]");
    Serial.println("}");   
  }
    Serial.println(" ");
  }    
 // now build the jsonString...
  jsonString="{\n";
jsonString +="\"homeMobileCountryCode\": 234,\n";  // this is a real UK MCC
jsonString +="\"homeMobileNetworkCode\": 27,\n";   // and a real UK MNC
jsonString +="\"radioType\": \"gsm\",\n";          // for gsm
jsonString +="\"carrier\": \"Vodafone\",\n";       // associated with Vodafone 
jsonString +="\"wifiAccessPoints\": [\n";
    for (int j = 0; j < n; ++j)
    {
      jsonString +="{\n";
      jsonString +="\"macAddress\" : \"";    
      jsonString +=(WiFi.BSSIDstr(j));
      jsonString +="\",\n";
      jsonString +="\"signalStrength\": ";     
      jsonString +=WiFi.RSSI(j);
      jsonString +="\n";
      if(j<n-1)
      {
      jsonString +="},\n";
      }
      else
      {
      jsonString +="}\n";  
      }
    }
    jsonString +=("]\n");
    jsonString +=("}\n"); 
//--------------------------------------------------------------------
  
 Serial.println("");

 WiFiClientSecure client;
  // We start by connecting to a WiFi network
  Serial.print("Connecting to ");
  Serial.println(myssid);
  WiFi.begin(myssid, mypass);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
    Serial.println(".");  

  //Connect to the client and make the api call
  Serial.print("Requesting URL: ");
  Serial.println("https://" + (String)Host + thisPage + "AIzaSyAaWvXN7mlpkfVkJjhjCCOeI94_YrHaINg");
  Serial.println(" ");
  if (client.connect(Host, 443)) {
    Serial.println("Connected");    
    client.println("POST " + thisPage + key + " HTTP/1.1");    
    client.println("Host: "+ (String)Host);
    client.println("Connection: close");
    client.println("Content-Type: application/json");
    client.println("User-Agent: Arduino/1.0");
    client.print("Content-Length: ");
    client.println(jsonString.length());    
    client.println();
    client.print(jsonString);  
   delay(500);
  }

  //Read and parse all the lines of the reply from server          
  while (client.available()) {
    String line = client.readStringUntil('\r');
    if(more_text){
      Serial.print(line);
    }    
    JsonObject& root = jsonBuffer.parseObject(line);
    if(root.success()){
    latitude    = root["location"]["lat"];
    longitude   = root["location"]["lng"];
    }
  }
  

  Serial.println("closing connection");
  Serial.println();
  client.stop();

  Serial.print("Latitude = ");
  Serial.println(latitude,6);
  Serial.print("Longitude = ");
  Serial.println(longitude,6);

  // Wait for ever...
  
  
  Firebase.setFloat("latitude", latitude);
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);
  
  // update value
  Firebase.setFloat("longitude", longitude);
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);

  // get value 
  Serial.print("longitude: ");
  Serial.println(Firebase.getFloat("longitude"));
  delay(1000);
   Serial.print("latitude: ");
  Serial.println(Firebase.getFloat("latitude"));
  delay(1000);

  // remove value
  Firebase.remove("longitude");
  Firebase.remove("latitude");
  delay(1000);

  // set string value
  Firebase.setString("message", "hello world");
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /message failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);
  
  // set bool value
  Firebase.setBool("truth", false);
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /truth failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);

  // append a new value to /logs
  String name = Firebase.pushInt("logs", n++);
  // handle error
  if (Firebase.failed()) {
      Serial.print("pushing /logs failed:");
      Serial.println(Firebase.error());  
      return;
  }
  Serial.print("pushed: /logs/");
  Serial.println(name);
  //delay(1000);
}
else{
  digitalWrite(m1,HIGH);
  digitalWrite(m2,LOW);
  digitalWrite(m3,HIGH);
  digitalWrite(m4,LOW);
  Serial.print("motor started");
  }
}

