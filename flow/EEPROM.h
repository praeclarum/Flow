#pragma once

class EEPROMClass
{
    int len;
    FILE *file;
public:
    EEPROMClass();
    ~EEPROMClass();
    void update(int idx, uint8_t val);
    byte read(int idx);
    int length() { return len; }
    template <typename T>
    T &get(int idx, T &t)
    {
        byte *ptr = (byte*)&t;
        int e = idx + sizeof(T);
        for (int i = idx; i != e; i++) *ptr++ = read(i);
        return t;
    }    
    template <typename T>
    const T &put(int idx, const T &t)
    {
        const byte *ptr = (const byte*)&t;
        int e = idx + sizeof(T);
        for (int i = idx; i != e; i++) update(i, *ptr++);
        return t;
    }
};

extern EEPROMClass EEPROM;
