
#include "WebServer.h"
#include "../Flow.h"

char ssid[] = "ssid";
char pass[] = "pwd";

WebServer::WebServer(FlowController *flow, int port)
    : server(port)
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
    client.println(F("HTTP/1.1 200 OK"));
    client.println(F("Content-Type: text/html"));
    client.println(F("Connection: close"));  // the connection will be closed after completion of the response
    client.println();
    client.println(F("<!DOCTYPE HTML>"));
    client.println(F("<html>"));
    // output the value of each analog input pin
    for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
        int sensorReading = 42;
        client.print(F("analog input "));
        client.print(analogChannel);
        client.print(F(" is "));
        client.print(sensorReading);
        client.println(F("<br />"));
    }
    client.println(F("</html>"));
}

