#include <Arduino.h>
#include <EEPROM.h>

#include <sys/stat.h>

EEPROMClass EEPROM;

EEPROMClass::EEPROMClass()
{
    len = 1 * 1024;
    char dir[1024];
    char path[1024];
    sprintf(dir, "%s/.flow", getenv("HOME"));
    mkdir(dir, 0777);
    sprintf(path, "%s/eeprom", dir);
    file = fopen(path, "r+b");
    if (!file) file = fopen(path, "w+b");
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

