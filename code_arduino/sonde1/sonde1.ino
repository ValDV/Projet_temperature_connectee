# include <WiFiNINA.h>
# include <PubSubClient.h>
# include <ArduinoJson.h>
//#include <EEPROM.h>

/*// Variables globales pour les paramètres WiFi
String ssid = "";
String password = "";*/
const char* ssid = "LaboCIEL2";
const char* password = "donnemoiunebrique";

const char* broker = "192.168.65.211";
const int port = 1883;
const char* topic = "temperature";

const int sensorPin = A0;

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

/*typedef struct 
{
  int id;
  char ssid[50];
  char password[50];
} Config;

#define CONFIG_ADDR 1

long lastNotify = 0;
Config config;

void loadConfigFromEEPROM()
{
  EEPROM.get(CONFIG_ADDR, config);
}

void saveConfigToEEPROM()
{
  EEPROM.put(CONFIG_ADDR, config);
}*/

void setup() {
  Serial.begin(9600);

  //loadConfigFromEEPROM();

  connectToWiFi();
  mqttClient.setServer(broker, port);
  connectToMQTT();
}

void loop() {
 /*long currentTime = millis();
 
 if(currentTime > lastNotify + 5000)
 {*/
  int sensorValue = analogRead(sensorPin);
  float voltage = sensorValue * (5.0 / 1023.0);
  float temperatureC = (voltage - 0.5) * 100.0;
  temperatureC /= 10.0;

  String payload = createJsonPayload(temperatureC);
  
  publishToMQTT(payload);

  if (!mqttClient.connected()) {
    connectToMQTT();
  }

    /*lastNotify = currentTime;
 }

 checkSerial();*/
 mqttClient.loop();
 delay(5000);
}

/*void checkSerial() {
    
    if (Serial.available()) {
      String jsonData = Serial.readStringUntil('\n');
      processWiFiConfig(jsonData);
    }
    
}

void processWiFiConfig(const String& jsonData) {
  StaticJsonDocument<128> doc;
  DeserializationError error = deserializeJson(doc, jsonData);

  if (error) {
    Serial.println("Erreur de décodage JSON");
    return;
  }

  Serial.println("Données reçues : ");
  Serial.println(jsonData);

  ssid = doc["ssid"].as<String>();
  password = doc["password"].as<String>();

  ssid.toCharArray(config.ssid, 50);
  password.toCharArray(config.password, 50);
  saveConfigToEEPROM();

  Serial.println("Paramètres WiFi reçus :");
  Serial.print("SSID : ");
  Serial.println(ssid);
  Serial.print("Mot de passe : ");
  Serial.println(password);
}*/

void connectToWiFi() {
  Serial.print("Connexion au WiFi...");
  while (WiFi.begin(ssid/*.c_str()*/, password/*.c_str()*/) != WL_CONNECTED) {
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
 doc["id"] = "Sonde_1";
 doc["name"] = "Capteur_Temperature";
 doc["type"] = "temperature";
 doc["value"] = temperature;
 doc["unit"] = "°C";

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