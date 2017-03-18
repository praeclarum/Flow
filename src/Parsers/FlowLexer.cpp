#include "FlowLexer.h"

uint32_t crc_start();
uint32_t crc_end(uint32_t crc);
uint32_t crc_update(uint32_t crc, byte data);

FlowLexer::FlowLexer(NameTable *names)
    : tok(yytokentype(0)), state(FLS_Reset)
    , col(1)
    , bufferLength(0), names(names)
{
}

bool FlowLexer::push(int c)
{
    if (state == FLS_Complete) {
        state = FLS_Reset;
    }
    bool consumed = false;
    switch (state) {
    case FLS_Reset:
        if (c == '\n') {
            consumed = true;
            tok = (yytokentype)c;
            tcol = col;
            state = FLS_Complete;
        }
        else if (c <= ' ') {
            consumed = true;
        }
        else if (c == '.') {
            consumed = true;
            buffer[0] = (char)c;
            bufferLength = 1;
            tcol = col;
            state = FLS_Dot;
        }
        else if ('0' <= c && c <= '9') {
            consumed = true;
            buffer[0] = (char)c;
            bufferLength = 1;
            tcol = col;
            state = FLS_InNumber;
        }
        else {
            switch (c) {
            case '+':
            case '-':
            case '*':
            case '/':
            case '(':
            case ')':
            case '^':
            case '=':
                consumed = true;
                tok = (yytokentype)c;
                tcol = col;
                state = FLS_Complete;
                break;
            default:
                if (c == '_' || isalpha(c)) {
                    consumed = true;
                    val.name = crc_update(crc_start(), c);
                    buffer[0] = (char)c;
                    bufferLength = 1;
                    tcol = col;
                    state = FLS_InName;
                }
                else {
                    consumed = true;
                    tok = (yytokentype)c;
                    tcol = col;
                    state = FLS_Complete;
                }
                break;
            }
        }
        break;
    case FLS_Dot:
        if ('0' <= c && c <= '9') {
            consumed = true;
            if (bufferLength + 1 < MAX_BUFFER_LENGTH) {
                buffer[bufferLength] = c;
                bufferLength++;
            }
            state = FLS_InNumberAfterDot;
        }
        else {
            consumed = false;
            tok = (yytokentype)'.';
            state = FLS_Complete;
            buffer[bufferLength] = 0;
        }
        break;
    case FLS_InNumber:
        if (c == '.' || ('0' <= c && c <= '9')) {
            consumed = true;
            if (bufferLength + 1 < MAX_BUFFER_LENGTH) {
                buffer[bufferLength] = c;
                bufferLength++;
            }
            if (c == '.')
                state = FLS_InNumberAfterDot;
        }
        else {
            tok = NUMBER;
            state = FLS_Complete;
            buffer[bufferLength] = 0;
            val.number = atof(buffer);
        }
        break;
    case FLS_InNumberAfterDot:
        if ('0' <= c && c <= '9') {
            consumed = true;
            if (bufferLength + 1 < MAX_BUFFER_LENGTH) {
                buffer[bufferLength] = c;
                bufferLength++;
            }
        }
        else {
            tok = NUMBER;
            state = FLS_Complete;
            buffer[bufferLength] = 0;
            val.number = atof(buffer);
        }
        break;
    case FLS_InName:
        if (c == '_' || isalnum(c)) {
            consumed = true;
            if (bufferLength + 2 < MAX_BUFFER_LENGTH) {
                buffer[bufferLength] = c;
                bufferLength++;
            }
            val.name = crc_update(val.name, c);
        }
        else {
            state = FLS_Complete;
            val.name = crc_end(val.name);
            switch (val.name) {
            case 0x580282dc:
                tok = SUB;
                break;
            case 0xfc33b1:
                tok = END;
                break;
            default:
                tok = NAME;
                buffer[bufferLength] = 0;
                names->put(val.name, buffer);
                break;
            }
            consumed = false;
        }
        break;
    case FLS_Complete:
        break;
    }
    if (consumed) {
        col = (c == '\n') ? 1 : (col + 1);
    }
    return consumed;
}

void FlowLexer::reset()
{
    state = FLS_Reset;
    tok = yytokentype(0);
    col = 1;
    tcol = 1;
    val.number = 0;
}



//
// CRC
// Used for names to give them a fixed size representation
// From: http://excamera.com/sphinx/article-crc.html
// Test with: >>> hex(binascii.crc32("new") & 0xffffffff)
//

static const uint32_t crc_table[16] PROGMEM = {
    0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac,
    0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
    0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
    0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c
};

uint32_t crc_start() { return ~0L; }
uint32_t crc_end(uint32_t crc) { return ~crc; }
uint32_t crc_update(uint32_t crc, byte data)
{
    byte tbl_idx;
    tbl_idx = crc ^ (data >> (0 * 4));
    crc = crc_table[tbl_idx & 0x0f] ^ (crc >> 4);
    tbl_idx = crc ^ (data >> (1 * 4));
    crc = crc_table[tbl_idx & 0x0f] ^ (crc >> 4);
    return crc;
}
uint32_t crc_string(const __FlashStringHelper *s)
{
    if (s == 0) return 0;
    PGM_P p = reinterpret_cast<PGM_P>(s);
    uint32_t crc = ~0L;
    byte b = pgm_read_byte(p);
    while (b) {
        crc = crc_update(crc, b);
        p++;
        b = pgm_read_byte(p);
    }
    return ~crc;
}
