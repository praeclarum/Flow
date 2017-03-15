#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef unsigned char byte;

typedef char __FlashStringHelper;
typedef const __FlashStringHelper *PGM_P;
#define pgm_read_byte(p) (*(p))
#define PROGMEM
#define F(s) ((const __FlashStringHelper*)(s))

typedef char prog_char;

long millis();
long micros();
void delay(unsigned long ms);

float pow(float base, float exponent);

#include <string>
class String : public std::string
{
public:
    String ();
    String (const char *rhs);
    float toFloat();
    void remove(unsigned int index, unsigned int count);
};

class Print
{
public:
    Print() {}
    virtual ~Print() {}
    virtual void print(char c) = 0;
    virtual void print(const char *text) = 0;
    virtual void println() = 0;
    virtual void println(const char *line) = 0;
    virtual void println(char c) = 0;
    virtual void println(float value) = 0;
    virtual int write(const char *buffer, int length) = 0;
    virtual void flush() = 0;
};

class Stream : public Print
{
public:
    Stream() {}
    virtual ~Stream() {}
    virtual int available() = 0;
    virtual int readBytes(char *buffer, int length) = 0;
    virtual int read() {
        char c;
        int n = readBytes(&c, 1);
        return n == 1 ? c : -1;
    }
};

class StdioStream : public Stream
{
    char inbuf[BUFSIZ];
    char outbuf[BUFSIZ];
public:
    StdioStream();
    virtual ~StdioStream();
    virtual int available();
    virtual int readBytes(char *buffer, int length);
    virtual int write(const char *buffer, int length);
    virtual void print(char c);
    virtual void print(const char *text);
    virtual void println();
    virtual void println(const char *line);
    virtual void println(char c);
    virtual void println(float value);
    virtual void flush();
};

extern StdioStream Serial;

