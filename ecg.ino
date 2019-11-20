#include <PubSubClient.h>
#include <WiFi.h>

// variaveis 
const char* ssid = "rede";
const char* password = "senha";
const char* mqttServer = "m15.cloudmqtt.com";
const int mqttPort = 16836;
const char* mqttUser = "usuario";
const char* mqttPassword = "senha";
char msg[50];
long sinal = 0;

WiFiClient espClient;
PubSubClient client(espClient);

// funcao para conectar com o wifi
void conexaoWifi() {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print("Connecting to WiFi:");
        Serial.println(ssid);
    }

    Serial.println("Connected to the WiFi network");
    Serial.println("");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

// funcao para conectar com o servidor
void conexaoMQTT() {
    client.setServer(mqttServer, mqttPort);
    client.setCallback(callback);

    while (!client.connected()) {
        Serial.println("Connecting to MQTT…");
        String clientId = "ESP32Client-ECG";
        clientId += String(random(0xffff), HEX);
        if (client.connect(clientId.c_str(), mqttUser, mqttPassword )) {
            Serial.println("connected");
        } else {
            Serial.print("failed with state ");
            Serial.print(client.state());
            delay(2000);
        }
    }
}


// funcao de callback
void callback(char* topic, byte* payload, unsigned int length) {
 
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
 
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
 
  Serial.println();
  Serial.println("-----------------------");
 
}

void setup() {
    // inicia a serial com baudrate: 115200
    Serial.begin(115200);
    // faz conexão com o wifi/internet
    conexaoWifi();
    // faz conexão com o servidor MQTT
    conexaoMQTT();
}

void loop() {
  // chama a funcao para a aquisicao do sinal
  sinal = analogRead(A0);

  // transforma o sinal em int
  sprintf(msg, "%ld", sinal );

  // envia para o servidro MQTT o sinal para gerar o grafico online
  client.publish("ecg", msg);
  client.loop();
}


















