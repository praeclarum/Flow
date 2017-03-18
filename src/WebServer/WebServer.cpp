
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
        String firstLine;
        bool gotFirstLine = false;
        while (client.connected()) {
            if (client.available()) {
                char c = client.read();
                if (c == '\n' && currentLineIsBlank) {
                    sendReply(firstLine.c_str(), client);
                    break;
                }
                if (c == '\n') {
                    currentLineIsBlank = true;
                    gotFirstLine = true;
                } else if (c != '\r') {
                    currentLineIsBlank = false;
                    if (!gotFirstLine) {
                        firstLine += (char)c;
                    }
                }
            }
        }
        delay(1); // Weird hack to do before stopping
        client.stop();
    }
}

static void printNode(Node *node, WiFiClient &client)
{
    client.print(F("{\"nodeType\":\""));
    switch (node->nodeType)
    {
    case NT_Null:
        client.print(F("Null"));
        break;
    case NT_Document:
        client.print(F("Document"));
        break;
    case NT_Number:
        client.print(F("Number"));
        break;
    case NT_BinaryOperator:
        client.print(F("BinaryOperator"));
        break;
    case NT_UnaryOperator:
        client.print(F("UnaryOperator"));
        break;
    case NT_Call:
        client.print(F("Call"));
        break;
    case NT_Function:
        client.print(F("Function"));
        break;
    case NT_Sub:
        client.print(F("Sub"));
        break;
    case NT_Assignment:
        client.print(F("Assignment"));
        break;
    case NT_Name:
        client.print(F("Name"));
        break;
    case NT_AssignmentReference:
        client.print(F("AssignmentReference"));
        break;
    case NT_FunctionReference:
        client.print(F("FunctionReference"));
        break;
    case NT_SwitchToSub:
        client.print(F("SwitchToSub"));
        break;
    case NT_End:
        client.print(F("End"));
        break;        
    }
    client.print(F("\",\"value\":"));
    switch (node->nodeType)
    {
    case NT_Null:
        client.print(F("null"));
        break;
    case NT_Number:
        client.print(node->value.number);
        break;
    case NT_BinaryOperator:
        client.print('"');
        client.print((char)node->value.binaryOperator);
        client.print('"');
        break;
    case NT_UnaryOperator:
        client.print('"');
        client.print((char)node->value.unaryOperator);
        client.print('"');
        break;
    case NT_Assignment:
        client.print(node->value.number);
        break;
    case NT_Name:
        client.print('"');
        client.print(node->value.name, HEX);
        client.print('"');
        break;
    case NT_AssignmentReference:
    case NT_FunctionReference:
    case NT_SwitchToSub:
    case NT_Sub:
    case NT_End:
    case NT_Document:
    case NT_Call:
    case NT_Function:
        client.print('0');
        break;
    }
    client.print(F(",\"childNodes\":["));
    Node *c = node->firstChild;
    if (c) {
        char head=' ';
        while (c) {
            client.print(head);
            printNode(c, client);
            head = ',';
            c = c->nextSibling;
        }
    }
    client.print(F("]}"));
}

void WebServer::sendReply(const char *url, WiFiClient &client)
{
    if (strstr(url, "GET / ") == url) {
        // send a standard http response header
        client.println(F("HTTP/1.1 200 OK"));
        client.println(F("Content-Type: text/html"));
        client.println(F("Connection: close"));  // the connection will be closed after completion of the response
        client.println();
        client.println(F("<!DOCTYPE html>"));
        client.println(F("<html>"));
        client.println(F("<head>"));
        client.println(F("<link rel=\"stylesheet\" href=\"https://unpkg.com/purecss@0.6.2/build/pure-min.css\"/>"));
        client.println(F("<link rel=\"stylesheet\" href=\"https://unpkg.com/purecss@0.6.2/build/grids-responsive-min.css\"/>"));
        client.println(F("<link rel=\"stylesheet\" href=\"https://praeclarum.github.io/Flow/site.css\"/>"));
        client.println(F("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"/>"));
        client.println(F("</head><body>"));
        client.println(F("<div id=\"device\"></div>"));
        client.println(F("<script src=\"https://unpkg.com/react@15/dist/react.js\"></script>"));
        client.println(F("<script src=\"https://unpkg.com/react-dom@15/dist/react-dom.js\"></script>"));
        client.println(F("<script src=\"https://praeclarum.github.io/Flow/device.js\"></script>"));
        client.println(F("</body></html>"));
    }
    else if (strstr(url, "GET /document.json ") == url) {
        client.println(F("HTTP/1.1 200 OK"));
        client.println(F("Content-Type: application/json"));
        client.println(F("Connection: close"));  // the connection will be closed after completion of the response
        client.println();
        printNode(flow->getDocument(), client);
    }
    else {
        client.println(F("HTTP/1.1 404 Not found"));
        client.println(F("Connection: close"));  // the connection will be closed after completion of the response
        client.println();
    }
}

