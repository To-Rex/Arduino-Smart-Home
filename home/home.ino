#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <Servo.h>
#define FIREBASE_HOST "https://smart-home-e9932-default-rtdb.asia-southeast1.firebasedatabase.app"
#define FIREBASE_AUTH "Dio9TOJkBY9eOKjfPPnOZXzxqFJBE3bx364upTv4"
#define WIFI_SSID ":)"
#define WIFI_PASSWORD "azamat2018$"
#define echoPin 4
#define trigPin 5
FirebaseData firebaseData;
Servo myservo;
long duration;
int distance;
int pos = 120;
int redLed = 12;
int greenLed = 14;
int buzzer = 15;
int smokeA0 = A0;
int sensorThres = 120;

void setup() {
  myservo.attach(13);
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(smokeA0, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(16, INPUT);

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
      if (Firebase.getInt(firebaseData, "home/door")) {
        if (firebaseData.intData() == 1) {  //7
          myservo.write(0);
          delay(1500);
          myservo.write(120);
          Firebase.setInt(firebaseData, "home/door", 0);
        }
      }
      int analogSensor = analogRead(smokeA0);

      Serial.println("LED is ON");
      if (analogSensor > sensorThres) {
        digitalWrite(greenLed, HIGH);
        digitalWrite(buzzer, HIGH);
        Firebase.setInt(firebaseData, "home/dim", 1);
      } else {
        digitalWrite(buzzer, LOW);
        digitalWrite(greenLed, LOW);
        Firebase.setInt(firebaseData, "home/dim", 0);
      }
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);
      duration = pulseIn(echoPin, HIGH);
      distance = duration * 0.034 / 2;
      if (digitalRead(16) == HIGH) {
        if (distance <= 30) {
          Firebase.setInt(firebaseData, "home/distance", 1);
          digitalWrite(redLed, HIGH);
          myservo.write(0);
          delay(1500);
          myservo.write(120);
        } else {
          Firebase.setInt(firebaseData, "home/distance", 0);
          digitalWrite(redLed, LOW);
        }
      } else {
        if (distance <= 30) {
          myservo.write(0);
          delay(1500);
          myservo.write(120);
        }
      }
      Serial.print(distance);
    } 
  } else {
    Serial.println("Not Connection wifi");
  }
}