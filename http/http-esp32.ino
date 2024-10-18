#include <WiFi.h>
#include <HTTPClient.h>
#include <WebServer.h>
#include <ArduinoJson.h>

// WiFi settings
const char* ssid = "###########";   //  replace with your ssid
const char* password = "##########";   //  replace with your password

// Server settings for sending data
const char* postServerName = "http://1<replace-with-your-ip-address>/sensor-data";  

// Create web server to receive POST requests from Node-RED
WebServer server(80);

unsigned long lastMsg = 0;

// Function to handle incoming POST requests (to receive relay data)
void handleRelayData() {
  if (server.hasArg("plain")) {  // Check if there's a POST body
    String body = server.arg("plain");
    Serial.println("Received POST data:");
    Serial.println(body);

    // Parse the JSON data
    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, body);

    if (error) {
      Serial.print("JSON parsing failed: ");
      Serial.println(error.f_str());
      server.send(400, "text/plain", "Invalid JSON");
      return;
    }

    // Extract relay values from JSON
    if (doc.containsKey("relay")) {
      int relay1 = doc["relay"]["relay1"];
      int relay2 = doc["relay"]["relay2"];

      // Print or process the relay values
      Serial.print("Relay 1: ");
      Serial.println(relay1);
      Serial.print("Relay 2: ");
      Serial.println(relay2);
    } else {
      Serial.println("No relay data found.");
    }

    // Respond back to Node-RED (200 OK)
    server.send(200, "application/json", "{\"status\":\"received\"}");
  } else {
    server.send(400, "text/plain", "No POST body received");
  }
}

void setup() {
  Serial.begin(115200);
  delay(500);

  // Connect to WiFi
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Setup server to listen for POST requests on `/receive-relay-data`
  server.on("/relay-data", HTTP_POST, handleRelayData);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  // Listen for incoming POST requests
  server.handleClient();

  unsigned long now = millis();
  
  // Send sensor data to Node-RED every 5 seconds
  if (now - lastMsg > 5000) { 
    lastMsg = now;

    // Step 1: POST sensor data
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      StaticJsonDocument<256> sensorData;

      // Prepare the sensor data (simulated)
      sensorData["nodeId"] = 1;
      JsonObject sensor = sensorData.createNestedObject("sensor");
      sensor["humidity"] = random(40, 60);  // Simulated humidity value
      sensor["temperature"] = random(20, 30);  // Simulated temperature value

      // Convert the JSON object to a string
      String postData;
      serializeJson(sensorData, postData);

      // Send the POST request
      http.begin(postServerName);  
      http.addHeader("Content-Type", "application/json");
      int httpResponseCode = http.POST(postData);

      if (httpResponseCode > 0) {
        String response = http.getString();
        Serial.print("POST Response code: ");
        Serial.println(httpResponseCode);
        Serial.println("POST Response: ");
        Serial.println(response);
      } else {
        Serial.print("Error on sending POST: ");
        Serial.println(httpResponseCode);
      }

      http.end();
    }
  }
}
