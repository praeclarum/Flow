#include "FlowLexer.h"

FlowLexer::FlowLexer()
    : tok(yytokentype(0)), state(FLS_Reset)
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
            state = FLS_Complete;
        }
        else if (c <= ' ') {
            consumed = true;
        }
        else if ('0' <= c && c <= '9') {
            consumed = true;
            buffer[0] = (char)c;
            bufferLength = 1;
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
                consumed = true;
                tok = (yytokentype)c;
                state = FLS_Complete;
                break;
            default:
                consumed = true;
                tok = (yytokentype)9999;
                state = FLS_Complete;
                break;
            }
        }
        break;
    case FLS_InNumber:
        if ('0' <= c && c <= '9') {
            consumed = true;
            if (bufferLength < MAX_BUFFER_LENGTH) {
                buffer[bufferLength] = c;
                bufferLength++;
            }
        }
        else {
            tok = NUM;
            state = FLS_Complete;
            consumed = false;
        }
        break;
    case FLS_Complete:
        break;
    }
    return consumed;
}




