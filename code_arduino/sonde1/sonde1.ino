# include <WiFiNINA.h>
# include <PubSubClient.h>
# include <ArduinoJson.h>

const char* ssid = "LaboCIEL2";
const char* password = "donnemoiunebrique";

const char* broker = "192.168.65.211";
const int port = 1883;
const char* topic = "temperature";

const int sensorPin = A0;

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

void setup() {
 Serial.begin(9600);

 connectToWiFi();

 mqttClient.setServer(broker, port);
 connectToMQTT();
}

void loop() {
 int sensorValue = analogRead(sensorPin);
 float voltage = sensorValue * (5.0 / 1023.0);
 float temperatureC = (voltage - 0.5) * 100.0;
 temperatureC /= 10.0;

 String payload = createJsonPayload(temperatureC);

 publishToMQTT(payload);

 if (!mqttClient.connected()) {
  connectToMQTT();
 }
 mqttClient.loop();

 delay(5000);
}

void connectToWiFi() {
 Serial.print("Connexion au WiFi...");
 while (WiFi.begin(ssid, password) != WL_CONNECTED) {
  Serial.print(".");
  delay(1000);
 }
 Serial.println("\nConnecté au WiFi !");
 Serial.print("Adresse IP : ");
 Serial.println(WiFi.localIP());
}

void connectToMQTT() {
 Serial.print("Connexion au broker MQTT...");
 do {
  if (mqttClient.connect("ArduinoClient")) {
   Serial.println("\nConnecté au broker MQTT !");
  } else {
   Serial.print(".");
   delay(1000);
  }
 }while (!mqttClient.connected());
}

String createJsonPayload(float temperature) {
 StaticJsonDocument<128> doc;
 doc["sonde_id"] = "Sonde_1";
 doc["sonde_name"] = "Capteur_Temperature";
 doc["type"] = "temperature";
 doc["value"] = temperature;
 doc["unit"] = "C";

 String payload;
 serializeJson(doc, payload);
 return payload;
}

void publishToMQTT(String payload) {
 if (mqttClient.publish(topic, payload.c_str())) {
  Serial.println("Données publiées : " + payload);
 } else {
  Serial.println("Échec de la publication !");
 }
}