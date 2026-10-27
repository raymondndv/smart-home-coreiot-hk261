#include <WiFi.h>
#include <PubSubClient.h>

const char* WIFI_SSID    = "Wokwi-GUEST";
const char* WIFI_PASS    = "";
const int   WIFI_CHANNEL = 6;

const char* MQTT_HOST    = "app.coreiot.io";
const int   MQTT_PORT    = 1883;
// Thay bang Access Token that khi chay. KHONG commit token that.
const char* ACCESS_TOKEN = "YOUR_ACCESS_TOKEN";

const char* TELEMETRY_TOPIC = "v1/devices/me/telemetry";

WiFiClient espClient;
PubSubClient client(espClient);

void connectWiFi() {
  Serial.print("Dang ket noi WiFi");
  WiFi.begin(WIFI_SSID, WIFI_PASS, WIFI_CHANNEL);
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }
  Serial.println("\nWiFi OK, IP: " + WiFi.localIP().toString());
}

void connectMQTT() {
  while (!client.connected()) {
    Serial.print("Dang ket noi MQTT... ");
    if (client.connect("smarthome-gw", ACCESS_TOKEN, "")) {
      Serial.println("OK");
    } else {
      Serial.print("loi, rc=");
      Serial.println(client.state());
      delay(3000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  connectWiFi();
  client.setServer(MQTT_HOST, MQTT_PORT);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) connectWiFi();
  if (!client.connected()) connectMQTT();
  client.loop();

  // Gia lap nhiet do; se thay bang BME280 o buoc tiep theo
  float temp = 25.0 + (random(-20, 21) / 10.0);
  char payload[64];
  snprintf(payload, sizeof(payload), "{\"temperature\":%.1f}", temp);

  if (client.publish(TELEMETRY_TOPIC, payload)) {
    Serial.print("Da gui: ");
    Serial.println(payload);
  } else {
    Serial.println("Gui that bai");
  }
  delay(5000);
}
