#include <Arduino.h>

#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

static long startMillis = 0;
static unsigned long startMicros = 0;

long millis()
{
    struct timeval  tv;
    gettimeofday(&tv, 0);
    long ms = tv.tv_sec * 1000 + tv.tv_usec / 1000 ;
    if (startMillis == 0)
        startMillis = ms;
    return ms - startMillis;
}

long micros()
{
    struct timeval  tv;
    gettimeofday(&tv, 0);
    uint64_t ms = tv.tv_sec * 1000000ull + tv.tv_usec;
    if (startMicros == 0)
        startMicros = ms;
    return ms - startMicros;
}

void delay(unsigned long ms)
{
    usleep(ms*1000);
}

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
void StdioStream::print(char c)
{
    printf("%c", c);
}
void StdioStream::println(char c)
{
    printf("%c\n", c);
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
