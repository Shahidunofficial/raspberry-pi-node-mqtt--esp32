#raspberry-pi-node-mqtt--esp32
An IoT demo project using ESP32 to send sensor data (humidity, temperature) to a Raspberry Pi via MQTT. Node-RED is used for data visualization and control. Secure communication is enabled with SSL, and HiveMQ Cloud serves as the MQTT broker. Includes setup for Raspberry Pi, ESP32, and Node-RED.
# Publish & subscribe for MQTTBroker (HiveMQ) using ESP32, Node-RED, on Raspberry Pi

## Description
This project demonstrates the integration of an ESP32 microcontroller with a Raspberry Pi using MQTT to publish sensor data, which is processed and visualized in Node-RED. The ESP32 connects to HiveMQ Cloud for MQTT communication over SSL, ensuring secure data transfer.

## Features
- ESP32 as an IoT sensor node.
- Secure MQTT communication using SSL.
- Node-RED UI for data visualization and control.
- Real-time simulated sensor data sending in json format(humidity and temperature).
- Integration with HiveMQ Cloud MQTT broker.

## Components
- ESP32: Collects and sends sensor data over MQTT.
- Raspberry Pi: Runs Node-RED for data visualization and MQTT broker integration.
- Node-RED: Visual interface to display sensor data and control relays.

## Technologies Used
-C++ (Arduino) for ESP32 programming.
-Node-RED for visualization and UI.
-MQTT(via HiveMQ Cloud) for message exchange.
-WiFiClientSecure for SSL/TLS communication.
-ArduinoJson for JSON handling.

## Setup Instructions

### 1. ESP32 Setup
1. Install the **Arduino IDE** and add the ESP32 board manager.
2. Install the following libraries:
   - PubSubClient
   - ArduinoJson
   - WiFiClientSecure

3. Flash the ESP32 with the code provided in `ESP32_MQTT_Sensor.ino`.

### 2. Raspberry Pi Setup
1. Install Node-RED on the Raspberry Pi.
    sudo apt update
    sudo apt install -y nodered

2. run node-red
        node-red
3. Import the Node-RED flow provided in NodeRED_Flow.json for data visualization.

### 3.HiveMQ cloud MQTT Setup
  1.  Sign up for a free HiveMQ Cloud account.
  2.  Create a cluster and use the generated URL, username, and password in the ESP32 code.
  3.  Make sure your Raspberry Pi and ESP32 are on the same network to facilitate MQTT communication.

How It Works
~ The ESP32 generates simulated sensor data (humidity and temperature) and publishes it to the sensor/data topic on the HiveMQ Cloud MQTT broker.
~ The data is visualized using Node-RED on the Raspberry Pi, and relay statuses can be controlled based on incoming messages.

MQTT TOPICS
  -Publish: sensor/data(used to send sensor data from ESP32
  -Subscribe: testTopic(used to receive control messages for realys)

Secure Communication

This project uses SSL/TLS for secuyre communication between the ESP32 and the HiveMQ MQTT broker by hardcoding the Let's encrypt CA certificate

  ![Screenshot 2024-10-16 200501](https://github.com/user-attachments/assets/ae7f2b91-22db-41d7-af3a-846aeb60706a)

![Screenshot 2024-10-16 200420](https://github.com/user-attachments/assets/db07bc89-cdc7-4975-b17a-754f97d56c1c)
![Screenshot 2024-10-16 195721](https://github.com/user-attachments/assets/153378a9-9423-41cf-9604-64f2b700e9f4)
![Screenshot 2024-10-16 200514](https://github.com/user-attachments/assets/bf10ad3d-2c05-4ce9-b1a3-9b08ea8b5957)
