#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "vinicius";
const char* password = "1234aaaa";
const char* mqttServer = "viniciusprojetos.duckdns.org";
const int mqttPort = 1883;
const char* mqttUser = "Vinicius";
const char* mqttPassword = "conectavinicius";
const char* mqttTopicchuveiro = "esp32/chuveiro";
const char* mqttTopictorneirac = "esp32/torneirac";
const char* mqttTopicirrigacao = "esp32/irrigacao";

const int sensorPinchuveiro = 33; // Potenciômetro da Chuveiro
const int sensorPintorneirac = 34; // Potenciômetro da Torneira Cozinha
const int sensorPinirrigacao = 35; // Potenciômetro da Irrigação do Jardim

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Conectado ao WiFi com endereço IP: ");
  Serial.println(WiFi.localIP());

  client.setServer(mqttServer, mqttPort);
  while (!client.connected()) {
    Serial.println("Conectando ao servidor MQTT...");
    if (client.connect("ESP32Client", mqttUser, mqttPassword)) {
      Serial.println("Conectado ao servidor MQTT!");
    } else {
      Serial.print("Falha na conexão - Estado: ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void loop() {
  int sensorValuechuveiro = analogRead(sensorPinchuveiro);
  int sensorValuetorneirac = analogRead(sensorPintorneirac);
  int sensorValueirrigacao = analogRead(sensorPinirrigacao);

  float chuveiro = map(sensorValuechuveiro, 0, 4095, 0, 1000) / 10.0;
  float torneirac = map(sensorValuetorneirac, 0, 4095, 0, 2000) / 10.0;
  float irrigacao = map(sensorValueirrigacao, 0, 4095, 0, 100) / 1.0;

  String chuveiroPayload = String(chuveiro);
  String torneiracPayload = String(torneirac);
  String irrigacaoPayload = String(irrigacao);

  Serial.print("Enviando dados para o Node-RED - Chuveiro: ");
  Serial.println(chuveiro);
  client.publish(mqttTopicchuveiro, chuveiroPayload.c_str());

  Serial.print("Enviando dados para o Node-RED - Torneira da Cozinha: ");
  Serial.println(torneirac);
  client.publish(mqttTopictorneirac, torneiracPayload.c_str());

  Serial.print("Enviando dados para o Node-RED - Irrigação do Jardim: ");
  Serial.println(irrigacao);
  client.publish(mqttTopicirrigacao, irrigacaoPayload.c_str());
  
  delay(1000);
}
