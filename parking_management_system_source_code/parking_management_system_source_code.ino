#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "HellbeE908";
const char* password = "x9pgztch";

WiFiServer server(80);

// Pins for 8 IR obstacle sensors
const int irPins[] = {D0, D1, D2, D3, D4, D5, D6, D7};
const int numSensors = 8;

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    Serial.println("New Client.");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            // Add the following line to make the page auto-refresh every 5 seconds
            client.println("<meta http-equiv=\"refresh\" content=\"5\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>");
            client.println("html { font-family: 'Arial', sans-serif; display: flex; justify-content: center; align-items: center; height: 100vh; background-color: #FFEBCD; }");
            client.println(".sensor-container { display: flex; flex-wrap: wrap; justify-content: center; gap: 20px; }");
            client.println(".sensor { width: 150px; height: 150px; display: flex; justify-content: center; align-items: center; font-size: 35px; border-radius: 20px; margin: 10px; padding: 20px; background-color: #fff; box-shadow: 0 0 10px rgba(0, 0, 0, 0.1); }");
            client.println(".sensor.high { background-color: #ff8080; }");
            client.println(".sensor.low { background-color: #80ff80; }");
            client.println("</style></head><body>");
            // client.println("<h1 style=\"text-align: center;\">Parking Management System</h1>");
            client.println("<h1 style=\"text-align: center; font-size: 39px; margin-bottom: 20px;\">Parking Management System</h1>");

            client.println("<div class=\"sensor-container\">");

            // Display the data from 8 IR sensors with colored rectangles
            for (int i = 0; i < numSensors; i++) {
              int sensorValue = digitalRead(irPins[i]);
              client.print("<div class=\"sensor\" style=\"background-color:  ");
              client.print(sensorValue == HIGH ? "green" : "red");
              client.print("\">Slot ");
              client.print(i + 1);
              client.print(": ");
              client.print(sensorValue == HIGH ? "Vacant" : "Occupied");
              client.println("</div>");
            }

            client.println("</body></html>");
            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}


