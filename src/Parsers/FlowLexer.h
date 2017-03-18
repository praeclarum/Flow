#pragma once

#include "FlowParser.h"

enum FlowLexerState {
    FLS_Reset,
    FLS_Complete,
    FLS_Dot,
    FLS_InNumber,
    FLS_InNumberAfterDot,
    FLS_InName,
};

class FlowLexer
{
    static const int MAX_BUFFER_LENGTH = 32;
    char buffer[MAX_BUFFER_LENGTH];
    int bufferLength;
    int col;
    NameTable *names;
public:
    yytokentype tok;
    YYSTYPE val;
    FlowLexerState state;
    int tcol;

    FlowLexer(NameTable *names);
    bool push(int c);
    void reset();
};
