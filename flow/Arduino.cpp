#include <Arduino.h>

#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

static long startMillis = 0;

long millis()
{
    struct timeval  tv;
    gettimeofday(&tv, 0);
    long ms = tv.tv_sec * 1000 + tv.tv_usec / 1000 ;
    if (startMillis == 0)
        startMillis = ms;
    return ms - startMillis;
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

EEPROMClass::EEPROMClass()
{
    len = 1 * 1024;
    char dir[1024];
    char path[1024];
    sprintf(dir, "%s/.flow", getenv("HOME"));
    mkdir(dir, 0777);
    sprintf(path, "%s/eeprom", dir);
    file = fopen(path, "w+b");
    if (!file) return;
    fseek(file, 0, SEEK_END);
    long flen = ftell(file);
    fseek(file, 0, SEEK_SET);
    while (flen < len) {
        byte b = 0;
        fwrite(&b, 1, 1, file);
        flen++;
    }
    fflush(file);
}

EEPROMClass::~EEPROMClass()
{
    if (file) fclose(file);
}

void EEPROMClass::update(int idx, byte value)
{
    if (!file) return;
    fseek(file, idx, SEEK_SET);
    fwrite(&value, 1, 1, file);
    fflush(file);
}

byte EEPROMClass::read(int idx)
{
    if (!file) return 0;
    fseek(file, idx, SEEK_SET);
    byte b;
    fread(&b, 1, 1, file);
    return b;
}

StdioStream Serial;
EEPROMClass EEPROM;
