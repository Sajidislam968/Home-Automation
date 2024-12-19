#include <WiFi.h>
#include <PubSubClient.h>
#include<Wire.h>


int trigPin = 21;
int echoPin = 18;
int fan = 5;
long duration;
float distance;
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

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(fan, HIGH);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  }
  else if((char)payload[0]=='0'){
    digitalWrite(fan, LOW);
  }

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
      client.publish("device/tank", "MQTT Server is connected");
      // ... and resubscribe
      client.subscribe("device/manual/on_off");
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
  pinMode(fan,OUTPUT);
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  // put your main code here, to run repeatedly:
  digitalWrite(trigPin,LOW);
  delay(10);
  digitalWrite(trigPin,HIGH);
  delay(10);
  digitalWrite(trigPin,LOW);
  duration = pulseIn(echoPin,HIGH);
  distance = duration*0.034/2;
  Serial.print("\n");
  
  
  
  if(distance<6){
    Serial.print("Water Full: ");
    Serial.print(distance);
    digitalWrite(fan,LOW);
    snprintf (msg, MSG_BUFFER_SIZE, "Water Full.");
    client.publish("device/tank", msg);
    }
    else if(distance>7 && distance<8){
      Serial.print("Tank Level: ");
      Serial.print(distance);
      //delay(2000);
      snprintf (msg, MSG_BUFFER_SIZE, "Medium Level");
      client.publish("device/tank", msg);
    }
    else if(distance>12 && distance<20){
      Serial.print("Tank Empty: ");
      Serial.print(distance);
      snprintf (msg, MSG_BUFFER_SIZE, "Tank Empty. Turning on the Water-Motor");
      client.publish("device/tank", msg);
      digitalWrite(fan, HIGH);
      delay(1500);
      
      while(distance<15 && distance>6){
        
        Serial.print("Tank is loading: ");
        Serial.println(distance);
        snprintf (msg, MSG_BUFFER_SIZE, "Tank is loading...");
        client.publish("device/tank", msg);
        digitalWrite(trigPin,LOW);
        delay(10);
        digitalWrite(trigPin,HIGH);
        delay(10);
        digitalWrite(trigPin,LOW);
        duration = pulseIn(echoPin,HIGH);
        distance = duration*0.034/2;
        delay(2000);
      }
    }else{
      Serial.print("Tank lavel : ");
      Serial.print(distance);
    }
  delay(2000);

}
