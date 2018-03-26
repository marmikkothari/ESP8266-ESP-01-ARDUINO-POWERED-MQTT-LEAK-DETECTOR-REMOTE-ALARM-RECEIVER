#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#define SEN_IN 2

const char* ssid = "YOUR-SSID-HERE";//************************************************CHANGE ME....1
const char* password = "YOUR-SSID-PASSWORD-HERE";//***********************************CHANGE ME....2
const char* mqtt_server = "broker.mqtt-dashboard.com";


WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup_wifi() {
  delay(100);
  
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length)
{
  Serial.print("Command is : [");
  Serial.print(topic);
  int p = (char)payload[0] - '0';

  if (p == 0)
  {
    Serial.println("to send MQTT Messages!]");

  }
  
  if (p == 1)
  {

  }
  
} 

void reconnect() {
  
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    
    String clientId = "YOUR-UNIQUE-RANDOM-IDENTIFIER-";//***********************************CHANGE ME...3......CAN BE ANY RANDOM-IDENTIFIER-
    clientId += String(random(0xffff), HEX);
    
    if (client.connect(clientId.c_str()))
    {
      Serial.println("connected");
      
      client.subscribe("YOUR-TOPIC");//*****************************************************CHANGE ME...4.......THIS IS YOUR MQTT TOPIC 
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      
      delay(6000);
    }
  }
} 

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
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
  long now = millis();
  // read Water sensor every 6 seconds
  if (now - lastMsg > 6000) {
    lastMsg = now;
    
    if (digitalRead(SEN_IN) == LOW) {
     
      String msg = "YOUR-MESSAGE-HERE";//***************************************************CHANGE ME...5....THIS TEXT WILL BE SENT TO RECEIVER
     
      char message[58];
      msg.toCharArray(message, 58);
      Serial.println(message);
      //publish sensor data to MQTT broker
      client.publish("YOUR-TOPIC", message);//**********************************************CHANGE ME...6....THIS IS YOUR MQTT TOPIC...SAME AS PREVIOUS TOPIC ABOVE


      delay(500);                      

    }
  }
    else{
      SEN_IN == HIGH;
    client.disconnect();
    };
  }

