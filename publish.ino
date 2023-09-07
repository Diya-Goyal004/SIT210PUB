#include <SPI.h>
#include <WiFiNINA.h>
#include <PubSubClient.h>

// Replace with your Wi-Fi credentials
char ssid[] = "Redmi Note 10 Pro Max";
char password[] = "123456789";

char mqttServer[] = "mqtt-dashboard.com"; // Replace with your MQTT broker's address
int mqttPort = 1883;
char mqttTopic[] = "sit210"; // MQTT topic to publish to

int pirPin = 2; // PIR sensor connected to digital pin 2
int ledPin = 13; // LED connected to digital pin 13

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(pirPin, INPUT); // Set PIR pin as input
  Serial.begin(115200);

  // Connect to Wi-Fi
  while (!WiFi.begin(ssid, password)) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connection failed. Rebooting...");
    delay(5000);
    while(1);  // Soft reset
  }
  Serial.println("Connected to WiFi");

  // Set up MQTT client
  mqttClient.setServer(mqttServer, mqttPort);
}

void loop() {
  if (!mqttClient.connected()) {
    reconnect();
  }
  mqttClient.loop();

  int pirState = digitalRead(pirPin); // Read PIR sensor state

  if (pirState == HIGH) {
    // Motion detected, publish "wave" event over MQTT
    if (mqttClient.publish(mqttTopic, "wave")) {
      Serial.println("Published 'wave' event");
    } else {
      Serial.println("Failed to publish 'wave' event");
    }
  }
}

void reconnect() {
  while (!mqttClient.connected()) {
    Serial.println("Attempting MQTT connection...");
    if (mqttClient.connect("DeviceAClient")) {
      Serial.println("Connected to MQTT broker");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" retrying in 5 seconds");
      delay(5000);
    }
  }
}
