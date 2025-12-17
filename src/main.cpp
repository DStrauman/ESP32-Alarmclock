#include <Arduino.h>
#include <WiFi.h>

//Project: ESP32-Alarmclock
//Version: Pre-Alpha (Not tested)

int myFunction(int, int);
void WiFiHealth();
void alarmActivate();
void setup() {
  int result = myFunction(2, 3);
  pinMode(5, INPUT); //set alarm/snooze pin
  pinMode(4, OUTPUT); //Led Pin
  pinMode(3, OUTPUT); //BUZZER PIN
  pinMode(2, OUTPUT); //SCL DISPLAY PIN
  pinMode(1, OUTPUT); //SDA DISPLAY PIN
  Serial.begin(115200);
}

String WiFiSSID = "SSID"; //WiFi SSID
String WiFiPassword = "Pass"; //WiFi Password
int alarmTime = 650; //Alarm time in HHMM format
void loop() {
  String timeZone = "PST8PDT,M3.2.0,M11.1.0"; //Set timezone to PST
  setenv("TZ", timeZone.c_str(), 1);
  bool alarmOff = false;
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    WiFiHealth();
    return;
  }

  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  int currentTime = timeinfo.tm_hour * 100 + timeinfo.tm_min;
  if (digitalRead(5) == HIGH) { //check for button press on pin 5
    Serial.println("Pin 5 is HIGH");
    alarmOff = true;
  } else {
    Serial.println("Pin 5 is LOW");
  }

  if (currentTime == 0000 && alarmOff) { //checks if time is 12AM, if it is it will turn on the alarm
    Serial.println("It's noon and alarm is not off!");
    Serial.println("Setting alarm!");
    alarmOff = false; // prevent multiple alarms
  } else {
    Serial.println("No alarm condition met.");
  }

  if(currentTime >= alarmTime && !alarmOff) {
    alarmActivate();
  }
  delay(1000);
}

void alarmActivate() { //Alarm function
  while (digitalRead(5) != HIGH) //Blinks LED and Buzzer untill button is pressed
  {
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
    delay(100);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
  }
  
}

void WiFiHealth(){ //Checks wifi status and attemps reconnect
  if(WiFi.status() != WL_CONNECTED){
    Serial.println("WiFi disconnected, attempting connection.");
    int attempts = 0;
    WiFi.begin(WiFiSSID.c_str(), WiFiPassword.c_str()); //Connect to WiFi
    while (WiFi.status() != WL_CONNECTED && attempts < 15) {
      delay(500);
      attempts++;
      Serial.print(".");
    }
    if (attempts >= 15) {
      Serial.println("\nFailed to connect to WiFi. Try changing WiFi credentials.");
      return;
    }
    Serial.println("\nWiFi connected");
  }
  else{
    Serial.println("WiFi OK!");
  }
}


// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}