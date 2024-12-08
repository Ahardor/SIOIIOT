#include <WiFi.h>
#include <PubSubClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>

// WiFi данные
const char* ssid = "";
const char* password = "";

// Данные сервера MQTT
const char* mqtt_server = "";
const int mqtt_port = 1883;

// Тема MQTT
const char* mqtt_topic_temp1 = "";
const char* mqtt_topic_temp2 = "";

const int tempPin = 15;

OneWire oneWire(tempPin);
DallasTemperature sensors(&oneWire);

// пример указания ID для датчиков
// DeviceAddress sensor_temp1 = {0x28, 0x44, 0xC7, 0x46, 0xD4, 0x9C, 0x3C, 0x39};
DeviceAddress sensor_temp1 = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
DeviceAddress sensor_temp2 = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};

// Клиент MQTT
WiFiClient espClient;
PubSubClient client(espClient);

int data;

void setup() {
  Serial.begin(115200);

  sensors.begin();

  Wire.begin();

  // Подключение к Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Подключено к Wi-Fi");

  // Настройка клиента MQTT
  client.setServer(mqtt_server, mqtt_port);

  // Подключение к MQTT
  while (!client.connected()) {
    Serial.print("Подключение к MQTT...");
    if (client.connect("ESP32_Client")) {
      Serial.println("Подключено");
      client.subscribe(mqtt_topic_temp1);
      client.subscribe(mqtt_topic_temp2);
    } else {
      Serial.print("Ошибка подключения: ");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

void loop() {
  // Получение данных с датчика
  sensors.requestTemperatures();
  int temp1 = sensors.getTempC(sensor_temp1);
  int temp2 = sensors.getTempC(sensor_temp2);

  // Отправка данных через MQTT
  char message[50];
  sprintf(message, "%d", temp1);
  client.publish(mqtt_topic_temp1, message, true);
  sprintf(message, "%d", temp2);
  client.publish(mqtt_topic_temp2, message, true);

  // Обновление клиента MQTT
  client.loop();

  delay(1000);
}
