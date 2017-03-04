#include <Arduino.h>

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <math.h>

float pow(float base, float exponent)
{
    return powf(base, exponent);
}

String::String() {}
String::String(const char *rhs) : std::string(rhs) {}
float String::toFloat()
{
    return atof(c_str());
}
void String::remove(unsigned int index, unsigned int count)
{
    unsigned int s = size();
    if (index >= s || count <= 0)
        return;
	if (count > s - index)
        count = s - index;
    erase(begin() + index, begin() + index + count);
}

StdioStream::StdioStream()
{
     setvbuf (stdin, inbuf, _IONBF, BUFSIZ);
     setvbuf (stdout, outbuf, _IONBF, BUFSIZ);
}
StdioStream::~StdioStream()
{
}
int StdioStream::available()
{
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(0, &rfds);
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    int r = select(1, &rfds, NULL, NULL, &tv);    
    return r > 0 ? 1 : 0;
}
int StdioStream::readBytes(char *buffer, int length)
{
    fd_set rfds;

    FD_ZERO(&rfds);
    FD_SET(0, &rfds);
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    int r = select(1, &rfds, NULL, NULL, &tv);

    int bytec = 0;
    while (r > 0 && bytec < length) {
        int b = fgetc(stdin);
        if (b < 0) break;
        buffer[bytec] = b;
        bytec++;

        FD_ZERO(&rfds);
        FD_SET(0, &rfds);
        tv.tv_sec = 0;
        tv.tv_usec = 0;
        r = select(1, &rfds, NULL, NULL, &tv);
    }
    return bytec;
}
int StdioStream::write(const char *buffer, int length)
{
    return fwrite(buffer, 1, length, stdout);
}
void StdioStream::print(const char *text)
{
    printf("%s", text);
}
void StdioStream::println(const char *line)
{
    printf("%s\n", line);
}
void StdioStream::println(float value)
{
    printf("%g\n", value);
}
void StdioStream::flush()
{
    fflush(stdout);
}

StdioStream Serial;
