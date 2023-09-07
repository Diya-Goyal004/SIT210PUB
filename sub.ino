#include <SPI.h>
#include <WiFiNINA.h>
#include <PubSubClient.h>

// Replace with your Wi-Fi credentials
char ssid[] = "Redmi Note 10 Pro Max";
char password[] = "123456789";

char mqttServer[] = "mqtt-dashboard.com"; // Replace with your MQTT broker's address
int mqttPort = 1883;
char mqttTopic[] = "sit210"; // MQTT topic to subscribe to

int ledPin = 13; // LED connected to digital pin 13

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

void setup() {
  pinMode(ledPin, OUTPUT);
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
  mqttClient.setCallback(callback);
}

void loop() {
  if (!mqttClient.connected()) {
    reconnect();
  }
  mqttClient.loop();
}

void callback(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  if (strcmp(topic, mqttTopic) == 0 && message == "wave") {
    // Received a "wave" message, flash the LED 3 times
    for (int i = 0; i < 3; i++) {
      digitalWrite(ledPin, HIGH); // Turn the LED on
      delay(500); // Wait for 0.5 seconds
      digitalWrite(ledPin, LOW); // Turn the LED off
      delay(500); // Wait for 0.5 seconds
    }
  }
}

void reconnect() {
  while (!mqttClient.connected()) {
    Serial.println("Attempting MQTT connection...");
    if (mqttClient.connect("DeviceBClient")) {
      Serial.println("Connected to MQTT broker");
      mqttClient.subscribe(mqttTopic); // Subscribe to the same topic as Device A
    } else {
      Serial.print("Failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" retrying in 5 seconds");
      delay(5000);
    }
  }
}
