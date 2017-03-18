#pragma once

#include <Arduino.h>
#include <WiFi.h>

class FlowController;

class WebServer
{
    FlowController *flow;
    WiFiServer server;

    void sendReply(const char *url, const char *content, WiFiClient &client);

public:
    WebServer(FlowController *flow, int port);
    void setup();
    void loop();
};

