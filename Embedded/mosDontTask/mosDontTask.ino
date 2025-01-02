#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>

#include <SoftwareSerial.h>
#include <TinyGPSPlus.h>
// #include "time.h"
#include "main.h"
#include "MPU.h"
#include <WiFi.h>
#include <WebServer.h>
#include <EEPROM.h>
#include <Update.h>
#include <ArduinoOTA.h>
#include <math.h>
#include <DNSServer.h>
#include "HTML.h"
// #include "FreeRTOS.h"
#include <Ticker.h>
#include <Update.h>
#include <ArduinoOTA.h>
#include <WiFiClientSecure.h>
#include "ADC_CalibResult.h"
#include "secrets.h"

Ticker timer;
Ticker timer1;
DNSServer dnsServer;
WebServer server(80);
// WiFi credentials
// #include "secrets.h"
// Wi-Fi credentials
// const char* ssid = "Nha 92B";
// const char* password = "888888888";

const char *ssid = "Phong Tro Co Huyen Lau 1";
const char *password = "0908877294";
const char *ntpServer = "asia.pool.ntp.org";
const long gmtOffset_sec = 25200;
const int daylightOffset_sec = 0;

double Vout, Rt = 0;
double T, Tc, Tf = 0;

double adcTemp = 0;
double adcPin = 0;
// WebServer server(80);
WiFiClientSecure wifiClient;
PubSubClient mqttClient(wifiClient);

WiFiClient espClient;
PubSubClient mosquittoClient(espClient);

TinyGPSPlus gps;
MPU6050 mpu6050;
MPU9255 mpu9255;
SoftwareSerial ss(16, 17);


void saveToEEPROM(int address, String value)
{
  Serial.println("Saving to EEPROM");
  Serial.print("Value: ");
  Serial.println(value);
  Serial.print(" Address: ");
  Serial.println(address);
  for (int i = 0; i < value.length(); ++i)
  {
    EEPROM.write(address + i, value[i]);
  }
  EEPROM.write(address + value.length(), 0); // Null terminator
  EEPROM.commit();
}

String readFromEEPROM(int address)
{
  String value = "";
  for (int i = 0; i < 50; ++i)
  {
    byte byteValue = EEPROM.read(address + i);
    if (byteValue == 0)
    {
      break;
    }
    value += char(byteValue);
  }
  Serial.println("Read from EEPROM");
  Serial.print("Value: ");
  Serial.println(value);
  Serial.print(" Address: ");
  Serial.println(address);
  return value;
}
void Read_Eui_From_EEPROM()
{
  // ===== EEPROM OTAA ====//
  for (int i = 0; i < 30; ++i)
  {
    SSID_Mosquitto += char(EEPROM.read(i));
  }
  SSID_Mosquitto.remove(SSID_Mosquitto.indexOf(0));
  Serial.print("SSID_Mosquitto: ");
  Serial.println(SSID_Mosquitto);
  for (int i = 30; i < 60; ++i)
  {
    PASSWORD_Mosquitto += char(EEPROM.read(i));
  }
  Serial.print("PASSWORD_Mosquitto: ");
  Serial.println(PASSWORD_Mosquitto);
  PASSWORD_Mosquitto.remove(PASSWORD_Mosquitto.indexOf(0));

  for (int i = 60; i < 90; ++i)
  {
    mqtt_server_s += char(EEPROM.read(i));
  }
  mqtt_server_s.remove(mqtt_server_s.indexOf(0));
  Serial.print("mqtt_server_s: ");
  Serial.println(mqtt_server_s);

  for (int i = 90; i < 120; ++i)
  {
    mqtt_topic_s += char(EEPROM.read(i));
  }
  mqtt_topic_s.remove(mqtt_topic_s.indexOf(0));
  Serial.print("mqtt_topic_s: ");
  Serial.println(mqtt_topic_s);

  for (int i = 120; i < 150; ++i)
  {
    SSID_AWS += char(EEPROM.read(i));
  }
  SSID_AWS.remove(SSID_AWS.indexOf(0));
  Serial.print("SSID_AWS: ");
  Serial.println(SSID_AWS);

  for (int i = 150; i < 180; ++i)
  {
    PASSWORD_AWS += char(EEPROM.read(i));
  }
  PASSWORD_AWS.remove(PASSWORD_AWS.indexOf(0));
  Serial.print("PASSWORD_AWS: ");
  Serial.println(PASSWORD_AWS);

  for (int i = 180; i < 210; ++i)
  {
    modeSelect += char(EEPROM.read(i));
  }
  modeSelect.remove(modeSelect.indexOf(0));
  Serial.print("modeSelect: ");
  Serial.println(modeSelect);

  for (int i = 210; i < 240; ++i)
  {
    modeWifiSelect += char(EEPROM.read(i));
  }
  modeWifiSelect.remove(modeWifiSelect.indexOf(0));
  Serial.print("modeWifiSelect: ");
  Serial.println(modeWifiSelect);

  for (int i = 240; i < 260; ++i)
  {
    weight += char(EEPROM.read(i));
  }
  weight.remove(weight.indexOf(0));
  Serial.print("weight: ");
  Serial.println(weight);
  // float m = float(weight);
}
void handleFirmwareUpdate()
{
  server.sendHeader("Connection", "close");
  server.send(200, "text/html", "<h1>OTA Firmware Update</h1>"
                                "<form method='POST' action='/update' enctype='multipart/form-data'>"
                                "  <input type='file' name='update'>"
                                "  <input type='submit' value='Upload'>"
                                "</form>");
}
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}
void handleProcess()
{
  String value1 = server.arg("value1");
  String value2 = server.arg("value2");
  String value3 = server.arg("value3");
  String value4 = server.arg("value4");
  String value5 = server.arg("value5");
  String value6 = server.arg("value6");
  String value7 = server.arg("value7");

  String modeSelect = server.arg("modeSelect");
  String modeWifiSelect = server.arg("modeWifiSelect");
  server.send(200, "text/plain", "OK");

  saveToEEPROM(0, value1);
  saveToEEPROM(30, value2);
  saveToEEPROM(60, value3);
  saveToEEPROM(90, value4);
  saveToEEPROM(120, value5);
  saveToEEPROM(150, value6);
  saveToEEPROM(240, value7);

  saveToEEPROM(180, modeSelect);
  saveToEEPROM(210, modeWifiSelect);
  Serial.println("Data saved to EEPROM successfully.");
  dataSent = true;

  webServerEnabled = false;
  
  server.stop();
  WiFi.disconnect();
  Serial.println("Disconnected from Wi-Fi_ESP32-WebServer");
  ESP.restart();
}

void buttonInterrupt()
{
  if (millis() - lastDebounceTime < debounceDelay)
  {
    return;
  }
  lastDebounceTime = millis();
  // sendWifidata = false;
  // send4Gdata = false;
  Serial.println(digitalRead(buttonPin));
  Serial.println("Interrupt");
  webServerEnabled = true;
  // digitalWrite(RS485_PIN_DIR, RS485Receive);
  // rs485Enabled = false;
  start_init = true;
}

void printLocalTime()
{
  struct tm timeinfo;
  struct timeval tv;

  // Get local time
  if (!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time");
    return;
  }
  gettimeofday(&tv, NULL);

  // Format the time string with milliseconds
  snprintf(timestamp, sizeof(timestamp), "%04d-%02d-%02d %02d:%02d:%02d.%03ld",
           timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday,
           timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec,
           tv.tv_usec / 1000); // Convert microseconds to milliseconds

  // Print the formatted timestamp
  Serial.println(timestamp);
  char sortableTimestamp[20];
  snprintf(sortableTimestamp, sizeof(sortableTimestamp), "%02d%02d%02d%02d%03ld",
           timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec,
           tv.tv_usec / 1000);

  timestamp_sort = atof(sortableTimestamp);
  Serial.print("Sortable timestamp: ");
  Serial.println(timestamp_sort);
}

void readGPS(){
  unsigned long startTime2 = millis();
  const unsigned long timeout = 1000; // 1 seconds in milliseconds

  while (ss.available() > 0 && (millis() - startTime2 < timeout)) {
    if (gps.encode(ss.read())) {
      Serial.print(F("Location: "));
      if (gps.location.isValid()) {
          Serial.print(gps.location.lat(), 6);
          latDf = gps.location.lat();
          Serial.print(F(","));
          Serial.print(gps.location.lng(), 6);
          longtitudeDf = gps.location.lng(); // Corrected variable name
          break; // Exit loop on valid fix
      } else {
          Serial.print(F("INVALID"));
          break;
      }
    }
  }
}

// void getMotion(){
//   mpu6050.get_accel_data_MPU6050(&ax1, &ay1, &az1, false);
//   mpu6050.get_gyro_data_MPU6050(&wx1, &wy1, &wz1);
//   mpu9255.get_accel_data_MPU9255(&ax2, &ay2, &az2, false);
//   mpu9255.get_gyro_data_MPU9255(&wx2, &wy2, &wz2);
//   ax2_filtered = alpha * (ax2_filtered_prev + ax2 - ax2_prev);
//   ay2_filtered = alpha * (ay2_filtered_prev + ay2 - ay2_prev);
//   az2_filtered = alpha * (az2_filtered_prev + az2 - az2_prev);

//   // Apply high-pass filter to gyroscope data
//   gx2_filtered = alpha * (gx2_filtered_prev + wx2 - gx2_prev);
//   gy2_filtered = alpha * (gy2_filtered_prev + wy2 - gy2_prev);
//   gz2_filtered = alpha * (gz2_filtered_prev + wz2 - gz2_prev);

//   // Update previous values
//   ax2_prev = ax2;
//   ay2_prev = ay2;
//   az2_prev = az2;
//   gx2_prev = wx2;
//   gy2_prev = wy2;
//   gz2_prev = wz2;

//   ax2_filtered_prev = ax2_filtered;
//   ay2_filtered_prev = ay2_filtered;
//   az2_filtered_prev = az2_filtered;
//   gx2_filtered_prev = gx2_filtered;
//   gy2_filtered_prev = gy2_filtered;
//   gz2_filtered_prev = gz2_filtered;

//   ax1_filtered = alpha * (ax1_filtered_prev + ax1 - ax1_prev);
//   ay1_filtered = alpha * (ay1_filtered_prev + ay1 - ay1_prev);
//   az1_filtered = alpha * (az1_filtered_prev + az1 - az1_prev);

//   // Apply high-pass filter to gyroscope data
//   gx1_filtered = alpha * (gx1_filtered_prev + wx1 - gx1_prev);
//   gy1_filtered = alpha * (gy1_filtered_prev + wy1 - gy1_prev);
//   gz1_filtered = alpha * (gz1_filtered_prev + wz1 - gz1_prev);

//   // Update previous values
//   ax1_prev = ax1;
//   ay1_prev = ay1;
//   az1_prev = az1;
//   gx1_prev = wx1;
//   gy1_prev = wy1;
//   gz1_prev = wz1;

//   ax1_filtered_prev = ax1_filtered;
//   ay1_filtered_prev = ay1_filtered;
//   az1_filtered_prev = az1_filtered;
//   gx1_filtered_prev = gx1_filtered;
//   gy1_filtered_prev = gy1_filtered;
//   gz1_filtered_prev = gz1_filtered;
// }
void turnOn4G()
{
  digitalWrite(PIN_PWRKEY, HIGH);
  delay(1000);
  digitalWrite(PIN_PWRKEY, LOW);
  delay(1300);
  digitalWrite(PIN_PWRKEY, HIGH);
}
void turnOff4G()
{
  digitalWrite(PIN_PWRKEY, HIGH);
  delay(1000);
  digitalWrite(PIN_PWRKEY, LOW);
  delay(1300);
  digitalWrite(PIN_PWRKEY, HIGH);
}

void setup()
{
  Serial.begin(115200);
  ss.begin(9600);
  pinMode(PIN_PWRKEY, OUTPUT);

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();
  EEPROM.begin(512);
  pinMode(buttonPin, INPUT);
  Serial.println("Start");
  Read_Eui_From_EEPROM();
  
  Serial.print("Weight m: ");
  Serial.println(m);
  // float m = float(weight.toInt()) / 1000;
  delay(1000);
  attachInterrupt(digitalPinToInterrupt(buttonPin), buttonInterrupt, FALLING);
  

  if (modeSelect == "WIFI" && modeWifiSelect == "Mosquitto"){
    while (WiFi.status() != WL_CONNECTED && millis() - startTime < 30000)
    {
      Serial.println("SSID: " + readFromEEPROM(0));
      Serial.println("PASSWORD: " + readFromEEPROM(30));
      WiFi.mode(WIFI_STA); // Optional
      WiFi.begin(SSID_Mosquitto, PASSWORD_Mosquitto);
      Serial.print(".");
      delay(500);
    }
    if (WiFi.status() == WL_CONNECTED)
    {
      Serial.println("\nConnected to the WiFi network");
      startTime = millis();
      // Serial.print("Local ESP32 IP: ");
      // Serial.println(WiFi.localIP());
      while (!mosquittoClient.connected()&& millis() - startTime < 30000)
      {
        const char *cStringServer = mqtt_server_s.c_str();
        const char *cStringTopic = mqtt_topic_s.c_str();
        mosquittoClient.setServer(cStringServer, 1883);
        // turnOffSIM();

        Serial.println("\nConnecting");
        Serial.println("Connecting to Mosquittp MQTT...");
        if (mosquittoClient.connect(cStringServer, mqtt_username, mqtt_password))
        {
          Serial.println("Connected to Mosquitto MQTT");
          dataSent = true;
        }
        else
        {
          Serial.print("Failed, rc=");
          Serial.print(mosquittoClient.state());
          Serial.println(" Retrying in 5 seconds...");
          delay(5000);
        }
      }
    }
    else
    {
      Serial.println("\nFailed to connect to the WiFi network");
      // Handle the case when WiFi connection fails
      // Add your code here
    }
  }
  if (modeSelect == "WIFI" && modeWifiSelect == "AWS")
  {
    while (WiFi.status() != WL_CONNECTED && millis() - startTime < 60000)
    {
      Serial.println("SSID: " + SSID_AWS);
      Serial.println("PASSWORD: " + PASSWORD_AWS);
      WiFi.mode(WIFI_STA); // Optional
      WiFi.begin(SSID_AWS, PASSWORD_AWS);
      Serial.print(".");
      delay(500);
    }
    // setup_wifi();
    if (WiFi.status() == WL_CONNECTED)
    {
      Serial.println("\nConnected to the WiFi network");
      wifiClient.setCACert(root);
      wifiClient.setCertificate(cert);
      wifiClient.setPrivateKey(privateKey);

      mqttClient.setServer(mqttServer, mqttPort);
      mqttClient.setCallback(callback);
      mqttClient.setBufferSize(600);
      chipId = ESP.getEfuseMac();
      startTime = millis();
      if (!mqttClient.connected()) {
        while (!mqttClient.connected()&& millis() - startTime < 30000) {
          Serial.print("Attempting MQTT connection...");
          // Create a random mqttClient ID
          String mqttClientId = "ESP32-";
          mqttClientId += String(random(0xffff), HEX);
          // Attempt to connect
          if (mqttClient.connect(mqttClientId.c_str())) {
            Serial.println("connected");
            dataSent = true;
          } else {
            Serial.print("failed, rc=");
            Serial.print(mqttClient.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
          }
        }
      }
      mqttClient.loop();
      // Serial.print("Local ESP32 IP: ");
      // Serial.println(WiFi.localIP());
    }
    else
    {
      Serial.println("\nFailed to connect to the WiFi network");
      startTime = millis();
      while (WiFi.status() != WL_CONNECTED && millis() - startTime < 30000)
      {
        Serial.println("Turn on 4G");
        
        digitalWrite(PIN_PWRKEY, HIGH);
        delay(1000);
        digitalWrite(PIN_PWRKEY, LOW);
        delay(1000);
        // Serial.println("SSID: " + SSID_AWS);
        // Serial.println("PASSWORD: " + PASSWORD_AWS);
        WiFi.mode(WIFI_STA); // Optional
        WiFi.begin("Ho", "33333333");
        Serial.print(".");
        delay(500);
      }
      // setup_wifi();
      if (WiFi.status() == WL_CONNECTED)
      {
        // Serial.println("\nConnected to the WiFi network");
        wifiClient.setCACert(root);
        wifiClient.setCertificate(cert);
        wifiClient.setPrivateKey(privateKey);

        mqttClient.setServer(mqttServer, mqttPort);
        mqttClient.setCallback(callback);
        mqttClient.setBufferSize(600);
        chipId = ESP.getEfuseMac();
        startTime = millis();
        if (!mqttClient.connected()) {
          while (!mqttClient.connected()&& millis() - startTime < 30000) {
            Serial.print("Attempting MQTT connection...");
            // Create a random mqttClient ID
            String mqttClientId = "ESP32-";
            mqttClientId += String(random(0xffff), HEX);
            // Attempt to connect
            if (mqttClient.connect(mqttClientId.c_str())) {
              Serial.println("connected");
              dataSent = true;
            } else {
              Serial.print("failed, rc=");
              Serial.print(mqttClient.state());
              Serial.println(" try again in 5 seconds");
              // Wait 5 seconds before retrying
              delay(5000);
            }
          }
        }
        mqttClient.loop();
        // Serial.print("Local ESP32 IP: ");
        // Serial.println(WiFi.localIP());
      }
      else
      {
        Serial.println("\nFailed to connect to the 4G network");
        // Handle the case when WiFi connection fails
        // Add your code here
      }
    }
  }

  if (modeSelect == "SIM4G")
  {
    while (WiFi.status() != WL_CONNECTED && millis() - startTime < 30000)
    {
      Serial.println("Turn on 4G");
      
      digitalWrite(PIN_PWRKEY, HIGH);
      delay(1000);
      digitalWrite(PIN_PWRKEY, LOW);
      delay(1000);
      // Serial.println("SSID: " + SSID_AWS);
      // Serial.println("PASSWORD: " + PASSWORD_AWS);
      WiFi.mode(WIFI_STA); // Optional
      WiFi.begin("Ho", "33333333");
      Serial.print(".");
      delay(500);
    }
    // setup_wifi();
    if (WiFi.status() == WL_CONNECTED)
    {
      // Serial.println("\nConnected to the WiFi network");
      wifiClient.setCACert(root);
      wifiClient.setCertificate(cert);
      wifiClient.setPrivateKey(privateKey);

      mqttClient.setServer(mqttServer, mqttPort);
      mqttClient.setCallback(callback);
      mqttClient.setBufferSize(600);
      chipId = ESP.getEfuseMac();
      startTime = millis();
      if (!mqttClient.connected()) {
        while (!mqttClient.connected()&& millis() - startTime < 30000) {
          Serial.print("Attempting MQTT connection...");
          // Create a random mqttClient ID
          String mqttClientId = "ESP32-";
          mqttClientId += String(random(0xffff), HEX);
          // Attempt to connect
          if (mqttClient.connect(mqttClientId.c_str())) {
            Serial.println("connected");
            modeSelect = "SIM4G";
            dataSent = true;
          } else {
            Serial.print("failed, rc=");
            Serial.print(mqttClient.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
          }
        }
      }
      mqttClient.loop();
      // Serial.print("Local ESP32 IP: ");
      // Serial.println(WiFi.localIP());
    }
    else
    {
      Serial.println("\nFailed to connect to the 4G network");
      // Handle the case when WiFi connection fails
      // Add your code here
    }
  }
  mpu6050.MPU6050_start();
  mpu9255.MPU9255_start();
  mpu6050.set_filter_range_MPU6050(FILTER_BW_5);
  mpu9255.set_filter_range_MPU9255(FILTER_BW_5);
  mpu6050.get_accel_data_MPU6050(&ax1_prev, &ay1_prev, &az1_prev, false);
  mpu6050.get_gyro_data_MPU6050(&gx1_prev, &gy1_prev, &gz1_prev);
  ax1_filtered_prev = ax1_prev;
  ay1_filtered_prev = ay1_prev;
  az1_filtered_prev = az1_prev;
  gx1_filtered_prev = gx1_prev;
  gy1_filtered_prev = gy1_prev;
  gz1_filtered_prev = gz1_prev;

  mpu9255.get_accel_data_MPU9255(&ax2_prev, &ay2_prev, &az2_prev, false);
  mpu9255.get_gyro_data_MPU9255(&gx2_prev, &gy2_prev, &gz2_prev);
  ax2_filtered_prev = ax2_prev;
  ay2_filtered_prev = ay2_prev;
  az2_filtered_prev = az2_prev;
  gx2_filtered_prev = gx2_prev;
  gy2_filtered_prev = gy2_prev;
  gz2_filtered_prev = gz2_prev;
  m = weight.toFloat();

  // Serial.print(modeSelect);
  // sendWifidata = false;
  // if (!client.connected()) {
  //   reconnect();
  // }
  // mqttClient.loop();
  // client.setServer(mqtt_server.c_str(), 1883);
  // xTaskCreatePinnedToCore(Interrupt, "Task1", 8000, NULL, 2, &Interrupt_p, 1);
  // xTaskCreatePinnedToCore(Send_Data_To_Server, "Task2", 4000, NULL, 1, &Send_Data_To_Server_p, 0);
  // disableCore0WDT();
}

void loop()
{
  if (start_init == false && dataSent == true && (millis() - lastMsg >= 20) && modeSelect == "WIFI" && modeWifiSelect == "Mosquitto")
  {
    
    long now = millis();
    if (now - lastMsg > 10) {
      mpu6050.get_accel_data_MPU6050(&ax1, &ay1, &az1, false);
      mpu6050.get_gyro_data_MPU6050(&wx1, &wy1, &wz1);
      mpu9255.get_accel_data_MPU9255(&ax2, &ay2, &az2, false);
      mpu9255.get_gyro_data_MPU9255(&wx2, &wy2, &wz2);
      ax2_filtered = alpha * (ax2_filtered_prev + ax2 - ax2_prev);
      ay2_filtered = alpha * (ay2_filtered_prev + ay2 - ay2_prev);
      az2_filtered = alpha * (az2_filtered_prev + az2 - az2_prev);

      // Apply high-pass filter to gyroscope data
      gx2_filtered = alpha * (gx2_filtered_prev + wx2 - gx2_prev);
      gy2_filtered = alpha * (gy2_filtered_prev + wy2 - gy2_prev);
      gz2_filtered = alpha * (gz2_filtered_prev + wz2 - gz2_prev);

      // Update previous values
      ax2_prev = ax2;
      ay2_prev = ay2;
      az2_prev = az2;
      gx2_prev = wx2;
      gy2_prev = wy2;
      gz2_prev = wz2;

      ax2_filtered_prev = ax2_filtered;
      ay2_filtered_prev = ay2_filtered;
      az2_filtered_prev = az2_filtered;
      gx2_filtered_prev = gx2_filtered;
      gy2_filtered_prev = gy2_filtered;
      gz2_filtered_prev = gz2_filtered;

      ax1_filtered = alpha * (ax1_filtered_prev + ax1 - ax1_prev);
      ay1_filtered = alpha * (ay1_filtered_prev + ay1 - ay1_prev);
      az1_filtered = alpha * (az1_filtered_prev + az1 - az1_prev);

      // Apply high-pass filter to gyroscope data
      gx1_filtered = alpha * (gx1_filtered_prev + wx1 - gx1_prev);
      gy1_filtered = alpha * (gy1_filtered_prev + wy1 - gy1_prev);
      gz1_filtered = alpha * (gz1_filtered_prev + wz1 - gz1_prev);

      // Update previous values
      ax1_prev = ax1;
      ay1_prev = ay1;
      az1_prev = az1;
      gx1_prev = wx1;
      gy1_prev = wy1;
      gz1_prev = wz1;

      ax1_filtered_prev = ax1_filtered;
      ay1_filtered_prev = ay1_filtered;
      az1_filtered_prev = az1_filtered;
      gx1_filtered_prev = gx1_filtered;
      gy1_filtered_prev = gy1_filtered;
      gz1_filtered_prev = gz1_filtered;
      
      if (millis() - lastMsg >= 30)
      {
        lastMsg = millis();
        count +=1;
        if (count % 20 == 0){
          
          readGPS();
          adcTemp = analogRead(ThermistorPin);
          adcTemp = ADC_LUT[(int)adcTemp];
          Vout = adcTemp * Vs / adcMax;
          Rt = R1 * Vout / (Vs - Vout);

          // T = 1 / (1 / To + log(Rt / Ro) / Beta) + 4; // Temperature in Kelvin
          // Tc = T - 273.15;
          T = 1 / (1 / To + log(Rt / Ro) / Beta)*1.2 - 51; // Temperature in Kelvin
          Tc = T - 273.15;

          adcPin = analogRead(readPin);
          adcPin = ADC_LUT[(int)adcPin];
          // battery_percent = adcPin * Vs * 2 / adcMax * 100 / 4.2;
          battery_percent = (adcPin * Vs * 2 / adcMax -3.2) * 100 / (4.2-3.2);

        }
        String payload = "";
        //    payload += "\"timestamp\";
        // String payload = "";
        // payload += "\"timestamp\";
        
        // payload += String(ax2, 3) + "," + String(ax2_filtered, 3) + "," + String(ay2, 3) + "," + String(ay2_filtered, 3) + "," + String(az2, 3) + "," + String(az2_filtered, 3) + "," + String(wx2, 3) + "," + String(gx2_filtered, 3) + "," + String(wy2, 3) + "," + String(gy2_filtered, 3) + "," + String(wz2, 3) + "," + String(gz2_filtered, 3) + ",";
        // payload += String(ax1, 3) + "," + String(ax1_filtered, 3) + "," + String(ay1, 3) + "," + String(ay1_filtered, 3) + "," + String(az1, 3) + "," + String(az1_filtered, 3) + "," + String(wx1, 3) + "," + String(gx1_filtered, 3) + "," + String(wy1, 3) + "," + String(gy1_filtered, 3) + "," + String(wz1, 3) + "," + String(gz1_filtered, 3);
        
        // payload += String(*ax2, 3) + "," + String(ax2_filtered, 3) + "," + String(*ay2, 3) + "," + String(ay2_filtered, 3) + "," + String(*az2, 3) + "," + String(az2_filtered, 3) + ","; + String(*wx2, 3) + "," + String(gx2_filtered, 3) + "," + String(*wy2, 3) + "," + String(gy2_filtered, 3) + "," + String(*wz2, 3) + "," + String(gz2_filtered, 3) + ",";
        // payload += String(*ax1, 3) + "," + String(ax1_filtered, 3) + "," + String(*ay1, 3) + "," + String(ay1_filtered, 3) + "," + String(*az1, 3) + "," + String(az1_filtered, 3) + ","; + String(*wx1, 3) + "," + String(gx1_filtered, 3) + "," + String(*wy1, 3) + "," + String(gy1_filtered, 3) + "," + String(*wz1, 3) + "," + String(gz1_filtered, 3) + ",";
        // payload += String(timestamp);
        // payload += ",";
        float m = weight.toFloat();
        payload += String(m, 3) + ",";
        payload += String(ax2_filtered, 3) + "," + String(ay2_filtered, 3) + "," + String(az2_filtered, 3) + "," + String(gx2_filtered, 3) + "," + String(gy2_filtered, 3) + "," + String(gz2_filtered, 3) + ",";
        payload += String(ax1_filtered, 3) + "," + String(ay1_filtered, 3) + "," + String(az1_filtered, 3) + "," + String(gx1_filtered, 3) + "," + String(gy1_filtered, 3) + "," + String(gz1_filtered, 3) + ",";
        payload += String(Tc, 2) + "," + String(battery_percent, 0) + ",";
        payload += String(latDf, 6) + "," + String(longtitudeDf, 6);
        // payload += "}";

        // Publish data to the MQTT topic
        Serial.print("Publishing message: ");
        Serial.println(payload);
        mosquittoClient.publish(mqtt_topic_s.c_str(), payload.c_str());
      }
      //    delay(10);
    }
  }
  if (start_init == false &&dataSent == true && (millis() - lastMsg >= 50) && modeWifiSelect == "AWS")
  {
    startTime = millis();
    while (WiFi.status() != WL_CONNECTED)
    {
      Serial.println("SSID: " + SSID_AWS);
      Serial.println("PASSWORD: " + PASSWORD_AWS);
      WiFi.mode(WIFI_STA); // Optional
      WiFi.begin(SSID_AWS, PASSWORD_AWS);
      Serial.print(".");
      delay(500);
      if (millis() - startTime > 60000) {
        Serial.println("Failed to connect to WiFi within 60 seconds. Restarting...");
        ESP.restart();
      }
    }
    if (!mqttClient.connected()) {
        while (!mqttClient.connected()&& millis() - startTime < 30000) {
          Serial.print("Attempting MQTT connection...");
          // Create a random mqttClient ID
          String mqttClientId = "ESP32-";
          mqttClientId += String(random(0xffff), HEX);
          // Attempt to connect
          if (mqttClient.connect(mqttClientId.c_str())) {
            Serial.println("connected");
            dataSent = true;
          } else {
            Serial.print("failed, rc=");
            Serial.print(mqttClient.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
          }
        }
      }
      mqttClient.loop();
      
    
    // setup_wifi();
    
    long now = millis();
    if (now - lastMsg > 10) {
      // lastMsg = now;
      mpu6050.get_accel_data_MPU6050(&ax1, &ay1, &az1, false);
      mpu6050.get_gyro_data_MPU6050(&wx1, &wy1, &wz1);
      mpu9255.get_accel_data_MPU9255(&ax2, &ay2, &az2, false);
      mpu9255.get_gyro_data_MPU9255(&wx2, &wy2, &wz2);
      // ax2_filtered = alpha * (ax2_filtered_prev + ax2 - ax2_prev);
      // ay2_filtered = alpha * (ay2_filtered_prev + ay2 - ay2_prev);
      // az2_filtered = alpha * (az2_filtered_prev + az2 - az2_prev);

      // // Apply high-pass filter to gyroscope data
      // gx2_filtered = alpha * (gx2_filtered_prev + wx2 - gx2_prev);
      // gy2_filtered = alpha * (gy2_filtered_prev + wy2 - gy2_prev);
      // gz2_filtered = alpha * (gz2_filtered_prev + wz2 - gz2_prev);

      // // Update previous values
      // ax2_prev = ax2;
      // ay2_prev = ay2;
      // az2_prev = az2;
      // gx2_prev = wx2;
      // gy2_prev = wy2;
      // gz2_prev = wz2;

      // ax2_filtered_prev = ax2_filtered;
      // ay2_filtered_prev = ay2_filtered;
      // az2_filtered_prev = az2_filtered;
      // gx2_filtered_prev = gx2_filtered;
      // gy2_filtered_prev = gy2_filtered;
      // gz2_filtered_prev = gz2_filtered;

      // ax1_filtered = alpha * (ax1_filtered_prev + ax1 - ax1_prev);
      // ay1_filtered = alpha * (ay1_filtered_prev + ay1 - ay1_prev);
      // az1_filtered = alpha * (az1_filtered_prev + az1 - az1_prev);

      // // Apply high-pass filter to gyroscope data
      // gx1_filtered = alpha * (gx1_filtered_prev + wx1 - gx1_prev);
      // gy1_filtered = alpha * (gy1_filtered_prev + wy1 - gy1_prev);
      // gz1_filtered = alpha * (gz1_filtered_prev + wz1 - gz1_prev);

      // // Update previous values
      // ax1_prev = ax1;
      // ay1_prev = ay1;
      // az1_prev = az1;
      // gx1_prev = wx1;
      // gy1_prev = wy1;
      // gz1_prev = wz1;

      // ax1_filtered_prev = ax1_filtered;
      // ay1_filtered_prev = ay1_filtered;
      // az1_filtered_prev = az1_filtered;
      // gx1_filtered_prev = gx1_filtered;
      // gy1_filtered_prev = gy1_filtered;
      // gz1_filtered_prev = gz1_filtered;
      
      // *battery_percent = battery * 100 / 4.2;
      if (now - lastMsg > 300)
      {
        lastMsg = now;
        count +=1;
        if (count % 3 == 0)
        {
          readGPS();
          adcTemp = analogRead(ThermistorPin);
          adcTemp = ADC_LUT[(int)adcTemp];
          Vout = adcTemp * Vs / adcMax;
          Rt = R1 * Vout / (Vs - Vout);

          // T = 1 / (1 / To + log(Rt / Ro) / Beta) + 4; // Temperature in Kelvin
          // Tc = T - 273.15;
          T = 1 / (1 / To + log(Rt / Ro) / Beta)*1.2 - 51; // Temperature in Kelvin
          Tc = T - 273.15;

          adcPin = analogRead(readPin);
          adcPin = ADC_LUT[(int)adcPin];
          battery_percent = (adcPin * Vs * 2 / adcMax -3.2) * 100 / (4.2-3.2);
        }
        printLocalTime();
        snprintf (msg, BUFFER_LEN, "{\"timestamp\" : \"%s\", \"id\" : \"%.0f\", \"m\" : \"%.1f\",\"timestamp_sort\" : \"%.0f\", \"ABack_x\" : \"%.3f\", \"ABack_y\" : \"%.3f\", \"ABack_z\" : \"%.3f\", \"ANeck_x\" : \"%.3f\", \"ANeck_y\" : \"%.3f\", \"ANeck_z\" : \"%.3f\", \"GBack_x\" : \"%.3f\", \"GBack_y\" : \"%.3f\", \"GBack_z\" : \"%.3f\", \"GNeck_x\" : \"%.3f\", \"GNeck_y\" : \"%.3f\", \"GNeck_z\" : \"%.3f\", \"temp\" : \"%.1f\", \"battery\" : \"%.1f\", \"lat\" : \"%.6f\", \"long\" : \"%.6f\"}", timestamp, id, m, timestamp_sort, ax2, ay2, az2, ax1, ay1, az1, wx2, wy2, wz2, wx1, wy1, wz1, Tc, battery_percent, *lat, *longtitude);

        // snprintf (msg, BUFFER_LEN, "{\"timestamp\" : \"%s\", \"id\" : \"%.0f\", \"m\" : \"%.1f\",\"timestamp_sort\" : \"%.0f\", \"ABack_x\" : \"%.3f\", \"ABack_y\" : \"%.3f\", \"ABack_z\" : \"%.3f\", \"ANeck_x\" : \"%.3f\", \"ANeck_y\" : \"%.3f\", \"ANeck_z\" : \"%.3f\", \"GBack_x\" : \"%.3f\", \"GBack_y\" : \"%.3f\", \"GBack_z\" : \"%.3f\", \"GNeck_x\" : \"%.3f\", \"GNeck_y\" : \"%.3f\", \"GNeck_z\" : \"%.3f\", \"temp\" : \"%.1f\", \"battery\" : \"%.1f\", \"lat\" : \"%.6f\", \"long\" : \"%.6f\"}", timestamp, id, m, timestamp_sort, ax2_filtered, ay2_filtered, az2_filtered, ax1_filtered, ay1_filtered, az1_filtered, gx2_filtered, gy2_filtered, gz2_filtered, gx1_filtered, gy1_filtered, gz1_filtered, Tc, battery_percent, *lat, *longtitude);
        
        Serial.print("Publish message: ");
        Serial.println(msg);
        // mqttClient.publish("raspi/data", msg);
        if (mqttClient.connected()) {
            if (mqttClient.publish("raspi/data", msg, plength)) {
                Serial.println("Publish succeeded");
            } else {
                Serial.println("Publish failed");
            }
        } else {
            Serial.println("MQTT client not connected");

        }
        // lastMsg = millis();
      }
    }
  // }
  }
  if (start_init == false && dataSent == true && (millis() - lastMsg >= 1000) && modeSelect == "SIM4G")
  {
    long now = millis();
    if (now - lastMsg > 20) {
      // lastMsg = now;
      mpu6050.get_accel_data_MPU6050(&ax1, &ay1, &az1, false);
      mpu6050.get_gyro_data_MPU6050(&wx1, &wy1, &wz1);
      mpu9255.get_accel_data_MPU9255(&ax2, &ay2, &az2, false);
      mpu9255.get_gyro_data_MPU9255(&wx2, &wy2, &wz2);
      // ax2_filtered = alpha * (ax2_filtered_prev + ax2 - ax2_prev);
      // ay2_filtered = alpha * (ay2_filtered_prev + ay2 - ay2_prev);
      // az2_filtered = alpha * (az2_filtered_prev + az2 - az2_prev);

      // // Apply high-pass filter to gyroscope data
      // gx2_filtered = alpha * (gx2_filtered_prev + wx2 - gx2_prev);
      // gy2_filtered = alpha * (gy2_filtered_prev + wy2 - gy2_prev);
      // gz2_filtered = alpha * (gz2_filtered_prev + wz2 - gz2_prev);

      // // Update previous values
      // ax2_prev = ax2;
      // ay2_prev = ay2;
      // az2_prev = az2;
      // gx2_prev = wx2;
      // gy2_prev = wy2;
      // gz2_prev = wz2;

      // ax2_filtered_prev = ax2_filtered;
      // ay2_filtered_prev = ay2_filtered;
      // az2_filtered_prev = az2_filtered;
      // gx2_filtered_prev = gx2_filtered;
      // gy2_filtered_prev = gy2_filtered;
      // gz2_filtered_prev = gz2_filtered;

      // ax1_filtered = alpha * (ax1_filtered_prev + ax1 - ax1_prev);
      // ay1_filtered = alpha * (ay1_filtered_prev + ay1 - ay1_prev);
      // az1_filtered = alpha * (az1_filtered_prev + az1 - az1_prev);

      // // Apply high-pass filter to gyroscope data
      // gx1_filtered = alpha * (gx1_filtered_prev + wx1 - gx1_prev);
      // gy1_filtered = alpha * (gy1_filtered_prev + wy1 - gy1_prev);
      // gz1_filtered = alpha * (gz1_filtered_prev + wz1 - gz1_prev);

      // // Update previous values
      // ax1_prev = ax1;
      // ay1_prev = ay1;
      // az1_prev = az1;
      // gx1_prev = wx1;
      // gy1_prev = wy1;
      // gz1_prev = wz1;

      // ax1_filtered_prev = ax1_filtered;
      // ay1_filtered_prev = ay1_filtered;
      // az1_filtered_prev = az1_filtered;
      // gx1_filtered_prev = gx1_filtered;
      // gy1_filtered_prev = gy1_filtered;
      // gz1_filtered_prev = gz1_filtered;
      
      // *battery_percent = battery * 100 / 4.2;
      if (now - lastMsg > 1000)
      {
        lastMsg = now;
        count +=1;
        if (count % 2 == 0)
        {
          readGPS();
          adcTemp = analogRead(ThermistorPin);
          adcTemp = ADC_LUT[(int)adcTemp];
          Vout = adcTemp * Vs / adcMax;
          Rt = R1 * Vout / (Vs - Vout);

          // T = 1 / (1 / To + log(Rt / Ro) / Beta) + 4; // Temperature in Kelvin
          // Tc = T - 273.15;
          T = 1 / (1 / To + log(Rt / Ro) / Beta)*1.2 - 51; // Temperature in Kelvin
          Tc = T - 273.15;

          adcPin = analogRead(readPin);
          adcPin = ADC_LUT[(int)adcPin];
          // battery_percent = adcPin * Vs * 2 / adcMax * 100 / 4.2;
          battery_percent = (adcPin * Vs * 2 / adcMax -3.2) * 100 / (4.2-3.2);
        }
        printLocalTime();
        snprintf (msg, BUFFER_LEN, "{\"timestamp\" : \"%s\", \"id\" : \"%.0f\", \"m\" : \"%.1f\",\"timestamp_sort\" : \"%.0f\", \"ABack_x\" : \"%.3f\", \"ABack_y\" : \"%.3f\", \"ABack_z\" : \"%.3f\", \"ANeck_x\" : \"%.3f\", \"ANeck_y\" : \"%.3f\", \"ANeck_z\" : \"%.3f\", \"GBack_x\" : \"%.3f\", \"GBack_y\" : \"%.3f\", \"GBack_z\" : \"%.3f\", \"GNeck_x\" : \"%.3f\", \"GNeck_y\" : \"%.3f\", \"GNeck_z\" : \"%.3f\", \"temp\" : \"%.1f\", \"battery\" : \"%.1f\", \"lat\" : \"%.6f\", \"long\" : \"%.6f\"}", timestamp, id, m, timestamp_sort, ax2, ay2, az2, ax1, ay1, az1, wx2, wy2, wz2, wx1, wy1, wz1, Tc, battery_percent, *lat, *longtitude);

        // snprintf (msg, BUFFER_LEN, "{\"timestamp\" : \"%s\", \"id\" : \"%.0f\", \"m\" : \"%.1f\",\"timestamp_sort\" : \"%.0f\", \"ABack_x\" : \"%.3f\", \"ABack_y\" : \"%.3f\", \"ABack_z\" : \"%.3f\", \"ANeck_x\" : \"%.3f\", \"ANeck_y\" : \"%.3f\", \"ANeck_z\" : \"%.3f\", \"GBack_x\" : \"%.3f\", \"GBack_y\" : \"%.3f\", \"GBack_z\" : \"%.3f\", \"GNeck_x\" : \"%.3f\", \"GNeck_y\" : \"%.3f\", \"GNeck_z\" : \"%.3f\", \"temp\" : \"%.1f\", \"battery\" : \"%.1f\", \"lat\" : \"%.6f\", \"long\" : \"%.6f\"}", timestamp, id, m, timestamp_sort, ax2_filtered, ay2_filtered, az2_filtered, ax1_filtered, ay1_filtered, az1_filtered, gx2_filtered, gy2_filtered, gz2_filtered, gx1_filtered, gy1_filtered, gz1_filtered, Tc, battery_percent, *lat, *longtitude);

        Serial.print("Publish message: ");
        Serial.println(msg);
        // mqttClient.publish("raspi/data", msg);
        if (mqttClient.connected()) {
            if (mqttClient.publish("raspi/data", msg, plength)) {
                Serial.println("Publish succeeded");
            } else {
                Serial.println("Publish failed");
            }
        } else {
            Serial.println("MQTT client not connected");
        }
        // lastMsg = millis();
      }
    }
  // }
  }
  // *battery_percent = battery
  if (start_init == true)
  {
    WiFi.mode(WIFI_AP);
    IPAddress localIP(192, 168, 1, 1);
    IPAddress gateway(192, 168, 1, 1);
    IPAddress subnet(255, 255, 255, 0);
    WiFi.softAPConfig(localIP, gateway, subnet);
    WiFi.softAP("ESP32-WebServer", "12345678");
    ArduinoOTA.setHostname("ESP32-OTA");
    ArduinoOTA.begin();

    // Setup web server
    String html = String(htmlContent); // Copy the HTML content to a String
    // Read values from EEPROM
    String value1 = readFromEEPROM(0);
    String value2 = readFromEEPROM(30);
    String value3 = readFromEEPROM(60);
    String value4 = readFromEEPROM(90);
    String value5 = readFromEEPROM(120);
    String value6 = readFromEEPROM(150);
    String value7 = readFromEEPROM(240);
    String modeSelect = readFromEEPROM(180);
    String modeWifiSelect = readFromEEPROM(210);

    html.replace("id=\"value1\"", "id=\"value1\" value=\"" + value1 + "\"");
    html.replace("id=\"value2\"", "id=\"value2\" value=\"" + value2 + "\"");
    html.replace("id=\"value3\"", "id=\"value3\" value=\"" + value3 + "\"");
    html.replace("id=\"value4\"", "id=\"value4\" value=\"" + value4 + "\"");
    html.replace("id=\"value5\"", "id=\"value5\" value=\"" + value5 + "\"");
    html.replace("id=\"value6\"", "id=\"value6\" value=\"" + value6 + "\"");
    html.replace("id=\"value7\"", "id=\"value7\" value=\"" + value7 + "\"");
    html.replace("id=\"modeSelect\"", "id=\"modeSelect\" value=\"" + modeSelect + "\"");
    html.replace("id=\"modeWifiSelect\"", "id=\"modeWifiSelect\" value=\"" + modeWifiSelect + "\"");
    dnsServer.setTTL(3600);
    dnsServer.start(DNS_PORT, "*", localIP);
    server.on("/process", HTTP_POST, handleProcess);
    // Serial.println("HTTP server started 3");
    // server.on("/update", HTTP_GET, handleFirmwareUpdate);
    // Serial.println("HTTP server started 4");
    server.on("/wpad.dat", []()
              { server.send(404); });

    server.on("/update", HTTP_POST, []()
              {
    server.sendHeader("Connection", "close");
    server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    // Serial.println("HTTP server started 5");
    ESP.restart(); });
    server.onNotFound([html]()
                      {
    server.send(200, "text/html", htmlContent);
    Serial.println("Server not found"); });
    server.on("/process", HTTP_POST, handleProcess);
    // Serial.println("HTTP server started 3");
    server.on("/update", HTTP_GET, handleFirmwareUpdate);
    // Serial.println("HTTP server started 4");
    server.on("/wpad.dat", []()
              { server.send(404); });

    // server.on("/update", HTTP_POST, []() {
    //   server.sendHeader("Connection", "close");
    //   server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    //   // Serial.println("HTTP server started 5");
    //   ESP.restart();
    // });
    if (dataSent)
    {
      dataSent = false;
    }

    server.begin();
    start_init = false;
  }

  ArduinoOTA.handle();
  if (webServerEnabled)
  {
    dnsServer.processNextRequest();
    server.handleClient();
  }
  // if (dataSent)
  // {
  //   dataSent = false;
  // }
}
