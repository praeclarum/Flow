
#include <Arduino.h>
#include <WiFi.h>

#include <sys/socket.h>
#include <unistd.h>

void WiFiClass::begin(const char *ssid, const char *password)
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
    return socket != 0;
}

bool WiFiClient::connected()
{
    return socket != 0;
}

int WiFiClient::available()
{
    if (socket == 0)
        return 0;
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(socket, &rfds);
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    int r = select(1, &rfds, NULL, NULL, &tv);    
    return r > 0 ? 1 : 0;
}

int WiFiClient::read()
{
    if (socket == 0)
        return -1;
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
    if (socket == 0) return;
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

void WiFiClient::print(float value)
{
    char text[32];
    sprintf(text, "%g", value);
    print(text);
}

void WiFiClient::stop()
{
    close(socket);
}

WiFiServer::WiFiServer(int port)
    : port(port)
{
}

void WiFiServer::begin()
{
}

WiFiClient WiFiServer::available()
{
    return WiFiClient(0);
}
