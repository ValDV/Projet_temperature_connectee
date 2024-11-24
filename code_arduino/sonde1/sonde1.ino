# include <SoftwareSerial.h>
# include <ArduinoJson.h>

const int wifiRX = 2;
const int wifiTX = 3;

const int sensorPin = A0;

const char* ssid = "ssidwifi";
const char* password = "passwordwifi";

const char* broker = "192.168.65.211";
const int port = 3000;
const char* topic = "temperature_data";

SoftwareSerial wifiSerial(wifiRX, wifiTX);

void setup() {
 Serial.begin(9600);
 wifiSerial.begin(9600);

 connectToWiFi();

 connectToMQTT();
}

void loop() {
 int sensorValue = analogRead(sensorPin);
 float voltage = sensorValue * (5.0 / 1023.0);
 float temperatureC = (voltage - 0.5) * 100.0;
 temperatureC /= 10.0;

 String payload = createJsonPayload(temperatureC);

 publishToMQTT(payload);

 delay(5000);
}

void connectToWiFi() {
 Serial.println("Connexion au WiFi...");

 sendCommand("AT");
 sendCommand("AT+RST");
 delay(1000);

 sendCommand("AT+CWMODE=1");
 sendCommand("AT+CWJAP=\"" + String(ssid) + "\",\"" + String(password) + "\"");

 delay(2000);
 Serial.println("Connecté au WiFi !");
}

void connectToMQTT() {
 Serial.println("Connexion au broker MQTT...");

 sendCommand("AT+CIPSTART=\"TCP\",\"" + String(broker) + "\"," + String(port));
 delay(2000);

 Serial.println("Connecté au broker MQTT !");
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
 int payloadSize = payload.length();

 String publishCommand = "AT+CIPSEND=" + String(payloadSize + 2);
 sendCommand(publishCommand);
 delay(500);

 wifiSerial.print(payload);
 wifiSerial.print("\r\n");
 delay(1000);

 Serial.println("Données publiées : " + payload);
}

void sendCommand(String command) {
 wifiSerial.println(command);
 delay(500);

 while (wifiSerial.available()) {
 Serial.write(wifiSerial.read());
 }
}