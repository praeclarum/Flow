#include "FlowLexer.h"

FlowLexer::FlowLexer()
    : tok(yytokentype(0)), state(FLS_Reset), col(1)
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
                consumed = true;
                tok = (yytokentype)c;
                tcol = col;
                state = FLS_Complete;
                break;
            default:
                consumed = true;
                tok = (yytokentype)c;
                tcol = col;
                state = FLS_Complete;
                break;
            }
        }
        break;
    case FLS_InNumber:
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


