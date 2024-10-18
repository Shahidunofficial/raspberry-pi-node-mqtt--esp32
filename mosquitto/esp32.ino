#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

//---- WiFi settings
const char* ssid = "your ssid";        // Replace with your WiFi SSID
const char* password = "your password"; // Replace with your WiFi password

//---- Mosquitto Broker on Raspberry Pi
const char* mqtt_server = "<replace with your ip>"; // Replace with the Raspberry Pi IP address
const int mqtt_port = 1883;              // Default MQTT port for non-SSL connections

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (500)
char msg[MSG_BUFFER_SIZE];
int value = 0;
const char* topic = "testTopic";

// Function to connect to WiFi
void setup_wifi() {
  delay(10);
  // Start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros()); // Generate random numbers

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

// Callback function to handle incoming messages
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.println("]");

  // Create a buffer to store the payload
  char message[length + 1];
  for (int i = 0; i < length; i++) {
    message[i] = (char)payload[i];
  }
  message[length] = '\0';  // Null-terminate the string

  Serial.print("Received payload: ");
  Serial.println(message);

  // Parse the incoming JSON data
  StaticJsonDocument<256> doc;
  DeserializationError error = deserializeJson(doc, message);

  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.f_str());
    return;
  }

  // Check if the "relay" object exists
  if (doc.containsKey("relay")) {
    JsonObject relay = doc["relay"];

    // Check if relay1 and relay2 exist and extract the values
    if (relay.containsKey("relay1") && relay.containsKey("relay2")) {
      int relay1 = relay["relay1"];
      int relay2 = relay["relay2"];

      // Print or process the relay values
      Serial.print("Relay 1: ");
      Serial.println(relay1);
      Serial.print("Relay 2: ");
      Serial.println(relay2);
    } else {
      Serial.println("Relay data not found.");
    }
  } else {
    Serial.println("No relay object found in the message.");
  }
}

// Function to reconnect to the MQTT broker if the connection is lost
void reconnect() {
  // Loop until we’re reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection… ");
    // MQTT client ID
    String clientId = "ESP32Client";
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected!");
      // Once connected, publish an announcement…
      // … and resubscribe
      client.subscribe(topic);
    } else {
      Serial.print("failed, rc = ");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  delay(500);
  // Start the serial communication
  Serial.begin(9600);
  delay(500);

  // Connect to WiFi
  setup_wifi();

  // Set the MQTT broker and the callback function
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  // Ensure we are connected to the MQTT broker
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > 5000) { // Send data every 5 seconds
    lastMsg = now;
    value++;

    // Create JSON object to hold the data
    StaticJsonDocument<256> doc;
    doc["nodeId"] = 1;  // Node ID (example value)

    // Create a nested "sensor" object for temperature and humidity
    JsonObject sensor = doc.createNestedObject("sensor");
    sensor["humidity"] = random(40, 60);  // Simulated humidity value
    sensor["temperature"] = random(20, 30);  // Simulated temperature value

    // Serialize JSON to a string
    char jsonBuffer[512];
    serializeJson(doc, jsonBuffer);

    // Publish the JSON message
    Serial.print("Publishing message: ");
    Serial.println(jsonBuffer);
    client.publish("sensor/data", jsonBuffer);
    delay(1000);
  }
}
