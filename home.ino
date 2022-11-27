MuhammadSaid Astru, [28 Nov 2022 at 00:39:00]:
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <Servo.h>
#define FIREBASE_HOST "https://smart-home-e9932-default-rtdb.asia-southeast1.firebasedatabase.app"
#define FIREBASE_AUTH "Dio9TOJkBY9eOKjfPPnOZXzxqFJBE3bx364upTv4"
#define WIFI_SSID ":)"
#define WIFI_PASSWORD "azamat2018$"
#define echoPin 4 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 5 //attach pin D3 Arduino to pin Trig of HC-SR04
FirebaseData firebaseData;
Servo myservo;  
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement
int pos = 120; 
int redLed = 12;
int greenLed = 14;
int buzzer = 15;
int smokeA0 = A0;
int sensorThres = 100;


void setup() {
myservo.attach(13);
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(smokeA0, INPUT);
    pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

 Firebase.reconnectWiFi(true);
}

void loop() {
 
  if (Firebase.getInt(firebaseData, "home/on")) {
    if (firebaseData.intData() == 1) {
      Serial.println("LED is ON");
      float h = dht.readHumidity();
      float t = dht.readTemperature();
      analogValue = analogRead(analogPin);
      Serial.println(analogValue);
      if (isnan(h) || isnan(t)) {
        Serial.println("Failed to read from DHT sensor!");
        lcd.setCursor(0, 0);
        lcd.print("Chesk sensors...");
        return;
      }
      Firebase.setInt(firebaseData, "data/soilmoisture", analogValue);
      Firebase.setFloat(firebaseData, "data/temperature", t);
      Firebase.setFloat(firebaseData, "data/humidity", h);
      
     
      if (Firebase.getInt(firebaseData, "phone/humidity")) {
        w = firebaseData.intData();
        if (Firebase.getString(firebaseData, "phone/rele2")) {
          if (firebaseData.stringData() == "auto") {
            if (h < w) {
              digitalWrite(16, LOW);
            } else {
              digitalWrite(16, HIGH);
            }
          }

          if (firebaseData.stringData() == "on") {
            digitalWrite(16, LOW);
          }
          if (firebaseData.stringData() == "off") {
            digitalWrite(16, HIGH);
          }
        }
        if (Firebase.getInt(firebaseData, "phone/soilmoisture")) {
          w = firebaseData.intData();
          if (Firebase.getString(firebaseData, "phone/rele1")) {
            if (firebaseData.stringData() == "auto") {
              if (analogValue < w) {
                digitalWrite(14, LOW);
              } else {
                digitalWrite(14, HIGH);
              }
            }
            if (firebaseData.stringData() == "on") {
              digitalWrite(14, LOW);
            }
            if (firebaseData.stringData() == "off") {
              digitalWrite(14, HIGH);
            }
          }
        }
        if (Firebase.getInt(firebaseData, "phone/temperature")) {
          w = firebaseData.intData();
          if (Firebase.getString(firebaseData, "phone/rele1")) {
            if (firebaseData.stringData() == "auto") {
              if (t > w) {
                digitalWrite(16, LOW);
              } else {
                digitalWrite(16, HIGH);
              }
            }
            if (firebaseData.stringData() == "on") {
              digitalWrite(16, LOW);
            }
            if (firebaseData.stringData() == "off") {
              digitalWrite(16, HIGH);
            }
          }
        }
      }

} else {
      lcd.clear();
      Serial.println("Program is OFF");
      lcd.setCursor(2, 0);
      lcd.print("DEVICE IS OFF");
    }
  } else {
    lcd.clear();
    Serial.println("Not Connection wifi");
    lcd.setCursor(2, 0);
    lcd.print("DEVICE IS OFF");
  }
}