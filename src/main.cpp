#include <Arduino.h>
#include <WiFi.h>


int myFunction(int, int);
void WiFiHeath();
void setup() {
  int result = myFunction(2, 3);
  pinMode(5, INPUT); //set alarm/snooze pin
  pinMode(4, OUTPUT); //Led Pin
  pinMode(3, OUTPUT); //BUZZER PIN
  pinMode(2, OUTPUT); //SCL DISPLAY PIN
  pinMode(1, OUTPUT); //SDA DISPLAY PIN
  Serial.begin(115200);
}

void loop() {
  bool alarmOff = false;
  WiFiHealth();
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
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

  if(currentTime >= 0650 && !alarmOff) {
    alarmActivate();
  }
  delay(1000);
}

void alarmActivate() { //Alarm function
  Serial.println("Alarm Activated!");
  digitalWrite(4, HIGH); // Activate alarm
  delay(5000); // Alarm duration
}

void WiFiHealth(){ //Checks wifi status and attemps reconnect
  if(WiFi.status() != WL_CONNECTED){
    Serial.println("WiFi disabled, attempting connection.");
    int attempts;
    WiFi.begin("SSID", "Pass"); //Connect to WiFi
    while (WiFi.status() != WL_CONNECTED || attempts < 15) {
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