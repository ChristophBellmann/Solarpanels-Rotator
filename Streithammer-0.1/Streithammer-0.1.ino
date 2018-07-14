//self explaining
#include <NTPtimeESP32.h>

//display stuff
#include "SSD1306.h" // alias for `#include "SSD1306Wire.h"'

//mqtt stuff
//#include <PubSubClient.h>

// Motor Stuff

// Motor A
int motor1Pin1 = 0; 
int motor1Pin2 = 2; 
int enable1Pin = 14;

// Motor B
int motor2Pin1 = 12; 
int motor2Pin2 = 13; 
int enable2Pin = 15;

// Setting PWM properties
const int freq = 30000;
const int pwmChannel = 0;
const int resolution = 8;
int dutyCycle = 200;

// Initialize the OLED display using Wire library
SSD1306  display(0x3c, 5, 4);

//setting up wifi & ntp
NTPtime NTPch("ch.pool.ntp.org");   // Choose server pool as required
char *ssid      = "*****";               // Set you WiFi SSID
char *password  = "*****";               // Set you WiFi password

/*
// mqtt stuff
const char* mqtt_server = "iot.eclipse.org";
#define mqtt_port 1883
#define MQTT_USER "mqtt username"
#define MQTT_PASSWORD "mqtt password"
#define MQTT_SERIAL_PUBLISH_CH "/ic/esp32/serialdata/uno/"

WiFiClient wifiClient;

PubSubClient client(wifiClient);

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
    client.publish("outTopic", "hello world");
    // Create a random client ID
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(),MQTT_USER,MQTT_PASSWORD)) {
      Serial.println("connected");
      //Once connected, publish an announcement...
      client.publish("/ic/presence/esp32/", "hello world");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

*/

strDateTime dateTime;

void setup() {
  display.init();
  display.flipScreenVertically();
  display.setColor(WHITE);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_24);

// Motor Stuff

// sets the pins as outputs:
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  pinMode(enable2Pin, OUTPUT);

  
  // configure LED PWM functionalitites
  ledcSetup(pwmChannel, freq, resolution);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(enable1Pin, pwmChannel);
  ledcAttachPin(enable2Pin, pwmChannel);

  
  Serial.begin(115200);
  Serial.println();
   // testing
  Serial.print("Testing DC Motor...");
  //********************
  display.display();
  display.clear();
  display.drawString(64, 20, String("Booted"));
  delay(2000);
  display.clear();
  display.setFont(ArialMT_Plain_16);
  display.drawString(64, 20, String("Connecting to Wi-Fi"));
  delay(2000);
 
  display.display();
  Serial.println("Booted");
  Serial.println("Connecting to Wi-Fi");

  WiFi.mode(WIFI_STA);
  WiFi.begin (ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
/* mqtt stuff in setup     
  
  Serial.setTimeout(500);// Set time out for 
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  reconnect();
*/

}

/*
  void publishSerialData(char *serialData){
  if (!client.connected()) {
    reconnect();
  }
  client.publish(MQTT_SERIAL_PUBLISH_CH, serialData);
  }
 
 */

void loop() {

  // first parameter: Time zone in floating point (for India); second parameter: 1 for European summer time; 2 for US daylight saving time (contributed by viewwer, not tested by me)
  dateTime = NTPch.getNTPtime(+1.0, 2); //CURRENTLY SET FOR US EST
  
  // check dateTime.valid before using the returned time
  // Use "setSendInterval" or "setRecvTimeout" if required
  
  if(dateTime.valid){
    NTPch.printDateTime(dateTime);

    byte actualHour = dateTime.hour;
    byte actualMinute = dateTime.minute;
    byte actualsecond = dateTime.second;
    int actualyear = dateTime.year;
    byte actualMonth = dateTime.month;
    byte actualday =dateTime.day;
    byte actualdayofWeek = dateTime.dayofWeek;
    String ts = String(dateTime.hour) + ":" + String(actualMinute / 10) + String(actualMinute % 10);

display.clear();
  Serial.print(actualHour);
display.setFont(ArialMT_Plain_24);
  display.drawString(64, 20, String(ts));
 
  display.display();

  delay(10);

    
  }

//serial stuff
   if (Serial.available() > 0) {
     char bfr[501];
     memset(bfr,0, 501);
    Serial.println(bfr);
   }

/*

// Motor Stuff
 // Move DC motor A backwards at maximum speed
  Serial.println("Moving A Backwards");
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  ledcWrite(pwmChannel, 255);
 // feedback driver pin state
  Serial.println(digitalRead(motor1Pin1));
  Serial.println(digitalRead(motor1Pin2));
  
  delay(2000);
  
 // Stop the DC motor A
  Serial.println("Motor A stopped");
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
 // feedback driver pin state
   Serial.println(digitalRead(motor1Pin1));
  Serial.println(digitalRead(motor1Pin2));
  delay(2000);
  
    Serial.println("Moving B Backwards");
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
  ledcWrite(pwmChannel, 255);
 // feedback driver pin state
  Serial.println(digitalRead(motor2Pin1));
  Serial.println(digitalRead(motor2Pin2));
  
  delay(2000);

   // Stop the DC motor B
  Serial.println("Motor B stopped");
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
 // feedback driver pin state
  Serial.println(digitalRead(motor2Pin1));
  Serial.println(digitalRead(motor2Pin2));
  delay(2000);
*/

}


void testmotorA() {

 // Move DC motor A backwards at maximum speed
  Serial.println("Moving A Backwards");
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH); 
  delay(4000);
  
 // Stop the DC motor A
  Serial.println("Motor A stopped");
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  delay(1000);

  // Move DC motor A forward with increasing speed
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  while (dutyCycle <= 255){
    ledcWrite(pwmChannel, dutyCycle);   
    Serial.print("Forward A with duty cycle: ");
    Serial.println(dutyCycle);
    dutyCycle = dutyCycle + 5;
    delay(500);
  }
  dutyCycle = 200;
  
 // Move DC motor A forward at maximum speed
  Serial.println("Moving A Forward");
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW); 
  delay(4000);

 // Stop the DC motor A
  Serial.println("Motor A stopped");
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  delay(1000);

}

void testmotorB() {

 // Move DC motor A backwards at maximum speed
  Serial.println("Moving A Backwards");
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH); 
  delay(4000);
  
 // Stop the DC motor A
  Serial.println("Motor A stopped");
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  delay(1000);

  // Move DC motor B forward with increasing speed
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
  while (dutyCycle <= 255){
    ledcWrite(pwmChannel, dutyCycle);   
    Serial.print("Forward A with duty cycle: ");
    Serial.println(dutyCycle);
    dutyCycle = dutyCycle + 5;
    delay(500);
  }
  dutyCycle = 200;
  
 // Move DC motor B forward at maximum speed
  Serial.println("Moving A Forward");
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW); 
  delay(4000);

 // Stop the DC motor B
  Serial.println("Motor A stopped");
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
  delay(1000);

}


