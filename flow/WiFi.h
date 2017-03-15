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
    void print(char ch);
    void print(float value);
    void print(uintptr_t, int base);
    void println();
    void println(const char *line);
    void println(char c);
};

class WiFiServer
{
    int port;
    int socket;
public:
    WiFiServer(int port);
    void begin();
    WiFiClient available();
};

class WiFiClass
{
public:
    void begin(char *ssid, const char *password);
    WiFiStatus status();
};

extern WiFiClass WiFi;

