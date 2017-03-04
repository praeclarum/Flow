#pragma once

#include "FlowParser.h"

enum FlowLexerState {
    FLS_Reset,
    FLS_Complete,
    FLS_InNumber
};

class FlowLexer
{
    static const int MAX_BUFFER_LENGTH = 32;
    char buffer[MAX_BUFFER_LENGTH];
    int bufferLength;
public:
    yytokentype tok;
    YYSTYPE val;
    FlowLexerState state;

    FlowLexer();
    bool push(int c);
};
