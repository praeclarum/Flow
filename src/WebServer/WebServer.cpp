
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

static void printNode(Node *node, WiFiClient &client)
{
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
    client.print(F(" "));
    client.print((uintptr_t)node, HEX);
    client.print(F(" "));
    switch (node->nodeType)
    {
    case NT_Null:
        break;
    case NT_Document:
        break;
    case NT_Number:
        client.print(node->value.number);
        break;
    case NT_BinaryOperator:
        client.print((char)node->value.binaryOperator);
        break;
    case NT_UnaryOperator:
        client.print((char)node->value.unaryOperator);
        break;
    case NT_Call:
        break;
    case NT_Function:
        client.print((uintptr_t)node->value.function, HEX);
        break;
    case NT_Sub:
        break;
    case NT_Assignment:
        client.print(node->value.number);
        break;
    case NT_Name:
        client.print(node->value.name, HEX);
        break;
    case NT_AssignmentReference:
        client.print((uintptr_t)node->value.assignmentReference, HEX);
        break;
    case NT_FunctionReference:
        client.print((uintptr_t)node->value.functionReference->function, HEX);
        break;
    case NT_SwitchToSub:
        client.print(F("SwitchToSub"));
        break;
    case NT_End:
        client.print(F("End"));
        break;        
    }
    Node *c = node->firstChild;
    if (c) {
        client.println(F("<ul>"));
        while (c) {
            client.println(F("<li>"));
            printNode(c, client);
            client.println(F("</li>"));
            c = c->nextSibling;
        }
        client.println(F("</ul>"));
    }
}

void WebServer::sendReply(const char *url, WiFiClient &client)
{
    // send a standard http response header
    client.println(F("HTTP/1.1 200 OK"));
    client.println(F("Content-Type: text/html"));
    client.println(F("Connection: close"));  // the connection will be closed after completion of the response
    client.println();
    client.println(F("<!DOCTYPE html>"));
    client.println(F("<html>"));
    client.println(F("<head>"));
    client.println(F("<link href=\"http://fonts.googleapis.com/icon?family=Material+Icons\" rel=\"stylesheet\">"));
    client.println(F("<link type=\"text/css\" rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/materialize/0.98.0/css/materialize.min.css\" media=\"screen,projection\"/>"));
    client.println(F("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"/>"));
    client.println(F("</head><body>"));
    client.println(F("<div id=\"device\"></div>"));
    client.println(F("<script src=\"https://unpkg.com/react@15/dist/react.js\"></script>"));
    client.println(F("<script src=\"https://unpkg.com/react-dom@15/dist/react-dom.js\"></script>"));
    client.println(F("<script src=\"https://code.jquery.com/jquery-2.1.1.min.js\"></script>"));
    client.println(F("<script src=\"https://cdnjs.cloudflare.com/ajax/libs/materialize/0.98.0/js/materialize.min.js\"></script>"));
    client.println(F("<script src=\"https://praeclarum.github.io/Flow/device.js\"></script>"));
    client.println(F("</body></html>"));
}

