#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <ESP32Servo.h>

Servo myservo;

int state = 0;

// Replace with your network credentials
const char* ssid = "HP";
const char* password = "nafza9494";

// Initialize Telegram BOT
#define BOTtoken "5353052388:AAEJcTXdxKF2I2Z28IIr51X8wWJaRvCjNoY"  // your Bot Token (Get from Botfather)

// Use @myidbot to find out the chat ID of an individual or a group
// Also note that you need to click "start" on a bot before it can
// message you
#define CHAT_ID "5040790093"

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

int servoPin = 18;

int irsensor1 = 34;
int irsensor2 = 4;

int valueirsensor1 = 0;
int valueirsensor2 = 0;

void setup() {
  Serial.begin(115200);

  // PIR Motion Sensor mode INPUT_PULLUP
  pinMode(irsensor1, INPUT);
  pinMode(irsensor2, INPUT);

  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myservo.setPeriodHertz(50);// Standard 50hz servo
  myservo.attach(servoPin, 1000, 2000);

  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  bot.sendMessage(CHAT_ID, "Bot started up", "");
}

void loop() {
  valueirsensor1 = analogRead(irsensor1); // 0 - 1023
  valueirsensor2 = digitalRead(irsensor2);

  if (valueirsensor1 >= 5){
    myservo.write(90);
    delay(2000);
    myservo.write(0);
    }

  if (valueirsensor2 == LOW && state == 0){
    bot.sendMessage(CHAT_ID, "SAMPAH PENUH", "");
    state = 1;
    }
  else{
    state = 0;
    }
}
