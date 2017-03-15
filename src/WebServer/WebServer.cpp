
#include "WebServer.h"
#include "../Flow.h"

char ssid[] = "yourNetwork";      // your network SSID (name)
char pass[] = "secretPassword";   // your network password

WebServer::WebServer(FlowController *flow)
    : server(80)
    , flow(flow)
{
}

void WebServer::setup()
{
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED) {
        Serial.println(ssid);
        delay(500);
    }
    server.begin();
}

void WebServer::loop()
{
    WiFiClient client = server.available();
    if (client) {
        bool currentLineIsBlank = true;
        while (client.connected()) {
            if (client.available()) {
                char c = client.read();
                if (c == '\n' && currentLineIsBlank) {
                    sendReply("", client);
                    break;
                }
                if (c == '\n') {
                    currentLineIsBlank = true;
                } else if (c != '\r') {
                    currentLineIsBlank = false;
                }
            }
        }
        delay(1); // Weird hack to do before stopping
        client.stop();
    }
}

void WebServer::sendReply(const char *url, WiFiClient &client)
{
    // send a standard http response header
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");  // the connection will be closed after completion of the response
    client.println();
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    // output the value of each analog input pin
    for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
        int sensorReading = 42;
        client.print("analog input ");
        client.print(analogChannel);
        client.print(" is ");
        client.print(sensorReading);
        client.println("<br />");
    }
    client.println("</html>");
}

