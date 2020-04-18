#include <PubSubClient.h>
#include <WiFi.h>

// variaveis 
//const char* ssid = "ECG ROUTER";
//const char* password = "ecg010203";
const char* ssid = "Nintendo";
const char* password = "luisea7x";
const char* mqttServer = "tailor.cloudmqtt.com";
const int mqttPort = 16494;
const char* mqttUser = "fffdpzks";
const char* mqttPassword = "mJwNquIkLedX";
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
     pinMode(12,INPUT);
    // inicia a serial com baudrate: 115200
    Serial.begin(115200);
    // faz conexão com o wifi/internet
    conexaoWifi();
    // faz conexão com o servidor MQTT
    conexaoMQTT();
}

void loop() {
  // chama a funcao para a aquisicao do sinal
  sinal = analogRead(12);

  // transforma o sinal em const char
  sprintf(msg, "%ld", sinal );

  // envia para o servidro MQTT o sinal para gerar o grafico online
  client.publish("ecg", msg);
  Serial.println(sinal);
  client.loop();
}





