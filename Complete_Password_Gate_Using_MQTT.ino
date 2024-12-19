#include <WiFi.h>
#include <PubSubClient.h>
#include<Wire.h>
#include<Keypad.h>
#include<ESP32Servo.h>
#include <Keypad.h>

#define SERVO_PIN 15
Servo servo; // servo motor
int angle = 90;
unsigned long prev_time;
#define ROW_NUM     4
#define COLUMN_NUM  4

char key_layout[ROW_NUM][COLUMN_NUM] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte pin_rows[ROW_NUM] = {13, 12, 14, 27};
byte pin_column[COLUMN_NUM] = {26, 25, 33, 32};
Keypad keypad = Keypad( makeKeymap(key_layout), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );
const String password_1 = "ABC1234";
String input_password;
// Update these with values suitable for your network.

const char* ssid = "SifatulRafi";
const char* password = "RSI213430";
const char* mqtt_server = "broker.emqx.io"; //From comment prompt (ping test.mosquitto.org)

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("Device/For_Open_Door", "MQTT Server is connected");
      // ... and resubscribe
      //client.subscribe("device/permission_for_unlock_door");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  servo.attach(SERVO_PIN);
  servo.write(90);
  prev_time = millis();
  input_password.reserve(32);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  char key = keypad.getKey();
   if (key) {
    Serial.print(key);

    if (key == '*') {
      input_password = "";
    } else if (key == '#') {
      if (input_password == password_1) {
        Serial.println("The gate is open");
        snprintf (msg, MSG_BUFFER_SIZE, "Someone enter your house...");
        client.publish("Device/For_Open_Door", msg);
        angle = 0;
        servo.write(angle);
        prev_time = millis();
      } else {
        Serial.println("The incorrect password! try again");
        snprintf (msg, MSG_BUFFER_SIZE, "Alert! Someone type the wrong password.");
        client.publish("Device/For_Open_Door", msg);
      }

      input_password = "";
    } else {
      input_password += key;
    }
  }
  if (angle == 0 && (millis() - prev_time) > 5000) {
    angle = 90;
    servo.write(angle);
    Serial.println("Rotating Servo Motor to 90°");
  }
}
