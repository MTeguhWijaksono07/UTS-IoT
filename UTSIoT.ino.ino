#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Nama_WiFi";
const char* password = "Password_WiFi";

const char* mqtt_server = "alamat_broker_MQTT";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

const int LED_PIN = 2; 

int saldoAwal = 180000;

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  
  char message[length + 1];
  for (int i = 0; i < length; i++) {
    message[i] = (char)payload[i];
  }
  message[length] = '\0';

  Serial.println(message);

  if (strcmp(topic, "saldo") == 0) {
    if (strcmp(message, "TRANSAKSI BERHASIL") == 0) {
      Serial.println("Transaksi berhasil!");
    } else if (strcmp(message, "SALDO TIDAK MENCUKUPI") == 0) {
      Serial.println("Saldo tidak mencukupi!");
    }  
  }
}

}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32Client")) {
      Serial.println("connected");
      client.subscribe("saldo");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
 
  if () {
    if (saldoAwal >= 20000) {
      digitalWrite(LED_PIN, HIGH);
      delay(5000);
      digitalWrite(LED_PIN, LOW);
      saldoAwal -= 20000;
      char saldoMessage[50];
      snprintf(saldoMessage, sizeof(saldoMessage), "TRANSAKSI BERHASIL, SISA SALDO Rp. %d", saldoAwal);
      client.publish("saldo", saldoMessage);
    } else {
      digitalWrite(LED_PIN, HIGH);
      delay(5000);
      digitalWrite(LED_PIN, LOW);
      client.publish("saldo", "SALDO TIDAK MENCUKUPI");
    }
  }
}
