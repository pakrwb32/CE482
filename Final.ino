#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>
#include <DHT.h>
#include <TridentTD_LineNotify.h>

#include <Wire.h>


#define FIREBASE_HOST "terrahouse-e06d8-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "4zUa9iPyKoh44HccXggcAj3QVja93CO6GAncmoXC"
#define WIFI_SSID "Mon"           //"yingpiboon_2.4G"//"Vawa"//"yingpiboon_5"
#define WIFI_PASSWORD "12345678"  //"puttrattanin"//"vawakonnarak"//"puttrattanin"

// line
// QNNMFl08oxVLaY3ezUPQMYQbJk2vPlEjc8VzIrh497J
#define LINE_TOKEN "pqRXpKVW4ZpxA328ReGt4ooKGIDaUKQeDFbho08E7GY"

#define DHTPIN 2
#define Sensor D0
#define Slight A0

int val = 0;
int fan = D6;
int fertilizer = D7;
int light = D5;
int plum = D8;

#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

FirebaseData firebaseData;
FirebaseData firebaseData2;
FirebaseData firebaseData3;

FirebaseData AIData;

void setup() {

  Serial.begin(9600);
  dht.begin();
  Serial.println(LINE.getVersion());

  pinMode(fan, OUTPUT);
  pinMode(fertilizer, OUTPUT);
  pinMode(light, OUTPUT);
  pinMode(plum, OUTPUT);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }

  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  LINE.setToken(LINE_TOKEN);

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
}

void sensorUpdate() {

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  float s = analogRead(D0);
  float l = analogRead(A0);
  float Slight = l * (3.3 / 1024.0);

  if (isnan(h) || isnan(t) || isnan(f) || isnan(s) || isnan(l)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  Serial.print(F("Humidity: "));
  Serial.print(h);

  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("C  ,"));

  Serial.print(F("Soil: "));
  Serial.print(s);

  Serial.print(F("Slight: "));
  Serial.print(l);

  Serial.print(f);
  Serial.println(F("F  "));

  if (Firebase.setFloat(firebaseData2, "/test/temperature", t)) {
    Serial.println("PASSED");
    Serial.println("PATH: " + firebaseData2.dataPath());
    Serial.println("TYPE: " + firebaseData2.dataType());
    Serial.println("ETag: " + firebaseData2.ETag());
    Serial.println("------------------------------------");
    Serial.println();
  } else {
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData2.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }

  if (Firebase.setFloat(firebaseData2, "/test/humidity", h)) {
    Serial.println("PASSED");
    Serial.println("PATH: " + firebaseData2.dataPath());
    Serial.println("TYPE: " + firebaseData2.dataType());
    Serial.println("ETag: " + firebaseData2.ETag());
    Serial.println("------------------------------------");
    Serial.println();
  } else {
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData2.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }

  if (Firebase.setFloat(firebaseData2, "/test/Soil", s)) {
    Serial.println("PASSED");
    Serial.println("PATH: " + firebaseData2.dataPath());
    Serial.println("TYPE: " + firebaseData2.dataType());
    Serial.println("ETag: " + firebaseData2.ETag());
    Serial.println("------------------------------------");
    Serial.println();
  } else {
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData2.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }

  if (Firebase.setFloat(firebaseData2, "/test/lightsensor", l)) {
    Serial.println("PASSED");
    Serial.println("PATH: " + firebaseData2.dataPath());
    Serial.println("TYPE: " + firebaseData2.dataType());
    Serial.println("ETag: " + firebaseData2.ETag());
    Serial.println("------------------------------------");
    Serial.println();
  } else {
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData2.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }
}
void loop() {
  sensorUpdate();

  delay(1000);
  if (Firebase.getString(AIData, "/test/AI")) {
    String aiMode = AIData.stringData();  // อ่านค่า AI Mode เก็บในตัวแปรชนิด String เพียงครั้งเดียว
    Serial.println("AI Mode = " + aiMode);
    if (aiMode == "0") {
      if (Firebase.getString(firebaseData3, "/test/FAN")) {
        Serial.println("FAN = " + firebaseData3.stringData());
        if (firebaseData3.stringData() == "1") {
          digitalWrite(fan, HIGH);
        } else if (firebaseData3.stringData() == "0") {
          digitalWrite(fan, LOW);
        }
      }
      delay(100);

      if (Firebase.getString(firebaseData3, "/test/Fertilizer")) {
        Serial.println("Fertilizer = " + firebaseData3.stringData());
        if (firebaseData3.stringData() == "1") {
          digitalWrite(fertilizer, HIGH);
        } else if (firebaseData3.stringData() == "0") {
          digitalWrite(fertilizer, LOW);
        }
      }
      delay(100);

      if (Firebase.getString(firebaseData3, "/test/light")) {
        Serial.println("light = " + firebaseData3.stringData());
        if (firebaseData3.stringData() == "1") {
          digitalWrite(light, HIGH);
        } else if (firebaseData3.stringData() == "0") {
          digitalWrite(light, LOW);
        }
      }
      delay(100);
      if (Firebase.getString(firebaseData3, "/test/water")) {
        Serial.println("plum = " + firebaseData3.stringData());
        if (firebaseData3.stringData() == "1") {
          digitalWrite(plum, HIGH);
        } else if (firebaseData3.stringData() == "0") {
          digitalWrite(plum, LOW);
        }
      }
      delay(100);
    }

    if (aiMode == "1") {

      Firebase.getFloat(firebaseData, "/test/temperature");  // อ่านค่าอุณหภูมิเก็บในตัวแปรชนิด float เพียงครั้งเดียว
      float temp = firebaseData.floatData();

      Firebase.getFloat(firebaseData, "/test/lightsensor");  // อ่านค่าเซนเซอร์แสงเก็บในตัวแปรชนิด float เพียงครั้งเดียว
      float Slight = firebaseData.floatData();

      Firebase.getFloat(firebaseData, "/test/Soil");  // อ่านค่าความชื้นในดินเก็บในตัวแปรชนิด int เพียงครั้งเดียว
      float soil = firebaseData.floatData();

      Serial.println(temp);
      Serial.println(Slight);
      Serial.println(soil);
      Serial.println("------------------------------------");
      Serial.println();

      // TEMP -> FAN
      if (temp >= 28) {
        Firebase.setString(firebaseData3, "/test/FAN", "1");
        Serial.println("Fan = 1");
        digitalWrite(fan, HIGH);

      } else {
        Firebase.setString(firebaseData3, "/test/FAN", "0");
        Serial.println("Fan = 0");
        digitalWrite(fan, LOW);
      }
      // SENSER -> LIGHT
      if (Slight < 120) {
        Firebase.setString(firebaseData3, "/test/light", "1");
        Serial.println("light = 1");
        digitalWrite(light, HIGH);

      } else {
        Firebase.setString(firebaseData3, "/test/light", "0");
        Serial.println("light = 0");
        digitalWrite(light, LOW);
      }
      // SOIL -> PLUM
      if (soil < 200) {
        Firebase.setString(firebaseData3, "/test/water", "1");
        Serial.println("Plum = 1");
        digitalWrite(plum, HIGH);

      } else {
        Firebase.setString(firebaseData3, "/test/water", "0");
        Serial.println("Plum = 0");
        digitalWrite(plum, LOW);
      }
    }
    Serial.println("------------------------------------");
    Serial.println();
  }
  delay(100);
}
