
#include <Arduino.h>
#include <WiFi.h>

#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>

void WiFiClass::begin(char *ssid, const char *password)
{
}

WiFiStatus WiFiClass::status()
{
    return WL_CONNECTED;
}

WiFiClass WiFi;


WiFiClient::WiFiClient(int socket)
    : socket(socket)
{   
}

WiFiClient::operator bool ()
{
    return socket >= 0;
}

bool WiFiClient::connected()
{
    return socket >= 0;
}

int WiFiClient::available()
{
    if (socket < 0) return 0;
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(socket, &rfds);
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    int r = select(socket+1, &rfds, NULL, NULL, &tv);    
    return r > 0 ? 1 : 0;
}

int WiFiClient::read()
{
    if (socket < 0) return -1;
    byte b;
    int n = recv(socket, &b, 1, 0);
    if (n == 1) {
        return b;
    }
    else {
        return -1;
    }
}

void WiFiClient::print(const char *text)
{
    if (socket < 0) return;
    send(socket, text, strlen(text), 0);
}

void WiFiClient::println()
{
    print("\r\n");
}

void WiFiClient::println(const char *line)
{
    print(line);
    print("\r\n");
}

void WiFiClient::print(char ch)
{
    char text[2];
    text[0] = ch;
    text[1] = 0;
    print(text);
}

void WiFiClient::print(float value)
{
    char text[32];
    sprintf(text, "%g", value);
    print(text);
}

void WiFiClient::print(uintptr_t value, int base)
{
    char text[32];
    if (base == HEX)
        sprintf(text, "0x%lx", value);
    else
        sprintf(text, "%lu", value);
    print(text);
}

void WiFiClient::stop()
{
    close(socket);
    socket = -1;
}

WiFiServer::WiFiServer(int port)
    : port(port)
{
}

void WiFiServer::begin()
{
    socket = ::socket(PF_INET, SOCK_STREAM, 0);
    if (socket < 0) return;

    sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);
    if (bind(socket, (sockaddr *)&addr, sizeof(addr)) < 0) {
        close(socket);
        socket = -1;
        Serial.println(F("WiFiServer: Failed to bind"));
        return;
    }
    listen(socket, 5);
}

WiFiClient WiFiServer::available()
{
    if (socket < 0) return WiFiClient(-1);
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(socket, &rfds);
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    int r = select(socket+1, &rfds, NULL, NULL, &tv);    
    if (r > 0) {
        sockaddr_in caddr = {0};
        socklen_t len = sizeof(caddr);
        int csocket = accept(socket, (sockaddr*)&caddr, &len);
        return WiFiClient(csocket);
    }
    return WiFiClient(-1);
}
