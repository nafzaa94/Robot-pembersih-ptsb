#include <WiFi.h>
#include <PubSubClient.h>
#include <ESP32Servo.h> 

Servo myservo;

// Replace the next variables with your SSID/Password combination
const char* ssid = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";

// Add your MQTT Broker IP address, example:
//const char* mqtt_server = "192.168.1.144";
const char* mqtt_server = "YOUR_MQTT_BROKER_IP_ADDRESS";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

int state = 0;

// LED Pin
const int signall = 25;
const int signal2 = 33;
const int signal3 = 32;
const int signal4 = 35;

int servoPin = 18;

int irsensor1 = 34;
int irsensor2 = 4;

int valueirsensor1 = 0;
int valueirsensor2 = 0;

void setup() {
  Serial.begin(115200);
  // default settings

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  pinMode(signall, OUTPUT);
  pinMode(signal2, OUTPUT);
  pinMode(signal3, OUTPUT);
  pinMode(signal4, OUTPUT);
  pinMode(irsensor1, INPUT);
  pinMode(irsensor2, INPUT);

  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myservo.setPeriodHertz(50);// Standard 50hz servo
  myservo.attach(servoPin, 1000, 2000);

  digitalWrite(signall, LOW);
  digitalWrite(signal2, LOW);
  digitalWrite(signal3, LOW);
  digitalWrite(signal4, LOW);

  myservo.write(0);
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
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

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off". 
  // Changes the output state according to the message
  if (String(topic) == "esp32/buttondepan") {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
      digitalWrite(signall, HIGH);
      digitalWrite(signal2, LOW);
      digitalWrite(signal3, LOW);
      digitalWrite(signal4, HIGH);
    }
  }

  if (String(topic) == "esp32/buttonbelakang") {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
      digitalWrite(signall, LOW);
      digitalWrite(signal2, HIGH);
      digitalWrite(signal3, HIGH);
      digitalWrite(signal4, LOW);
    }
  }

  if (String(topic) == "esp32/buttonkanan") {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
      digitalWrite(signall, HIGH);
      digitalWrite(signal2, LOW);
      digitalWrite(signal3, HIGH);
      digitalWrite(signal4, LOW);
    }
  }

  if (String(topic) == "esp32/buttonkiri") {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
      digitalWrite(signall, LOW);
      digitalWrite(signal2, HIGH);
      digitalWrite(signal3, LOW);
      digitalWrite(signal4, HIGH);
    }
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("esp32/buttondepan");
      client.subscribe("esp32/buttonbelakang");
      client.subscribe("esp32/buttonkanan");
      client.subscribe("esp32/buttonkiri");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  valueirsensor1 = analogRead(irsensor1); // 0 - 1023
  valueirsensor2 = digitalRead(irsensor2);

  if (valueirsensor1 >= 5){
    myservo.write(90);
    delay(2000);
    myservo.write(0);
    }

  if (valuesensor2 == LOW && state == 0){
    client.publish("esp32/notification", "notification");
    state = 1;
    }
  else{
    state = 0;
    }
  
}
