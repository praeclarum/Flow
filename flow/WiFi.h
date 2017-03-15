#pragma once

enum WiFiStatus
{
    WL_CONNECTED = 1,
};


class WiFiClient
{
    int socket;
public:
    WiFiClient(int socket);
    operator bool();
    bool connected();
    void stop();

    int available();    
    int read();

    void print(const char *text);
    void println();
    void println(const char *line);
    void println(char c);
    void print(float value);
};

class WiFiServer
{
    int port;
public:
    WiFiServer(int port);
    void begin();
    WiFiClient available();
};

class WiFiClass
{
public:
    void begin(const char *ssid, const char *password);
    WiFiStatus status();
};

extern WiFiClass WiFi;

