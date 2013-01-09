//
//  statement_parser.c
//  c_compiler
//
//  Created by xichen on 2012-3-19.
//  Copyright (c) 2012 xichen. All rights reserved.
//

#include <stdio.h>
#include "statement_parser.h"
#include <ctype.h>
#include <string.h>
#include "common.h"
#include <stdlib.h>
#include <stdbool.h>

int    cc_sp_to_symbols(const char *statement, char *symbolArr[], size_t symbolArrCnt)
{
    int len = 0;
    int currIndex = 0;
    bool hasProcessLast = false;
    const char *start;
    
    cc_skip_space(&statement);
    
    start = statement;
    while(*statement)
    {
        if(isspace(*statement) && currIndex < symbolArrCnt)
        {
            memcpy(symbolArr[currIndex], start, len);
            len = 0;
            ++currIndex;
            hasProcessLast = true;
            
            cc_skip_space(&statement);
            start = statement;
            continue;
        }
        ++len;
        ++statement;
        hasProcessLast = false;
    }
    if(!hasProcessLast)
    {
        memcpy(symbolArr[currIndex], start, len);
        ++currIndex;
    }
    
    return currIndex;
}

void   testStatementParser()
{
#if 0   // cc_sp_to_symbols 
#define SYMBOL_COUNT    20
    int i = 0;
    int ret;
    
    const char *s = "int i = 12;";
    // char result[20][128];
    char **result = (char **)malloc(SYMBOL_COUNT * sizeof(char *));
    if(!result)
        return;
    for(i = 0; i < SYMBOL_COUNT; ++i)
    {
        result[i] = (char *)malloc(128);
        if(!result[i])
            return;
    }
    
    
    ret = cc_sp_to_symbols(s, (char **)result, SYMBOL_COUNT);
    printf("symbol count is %d\n", ret);
    
    for(i = 0; i < ret; ++i)
    {
        printf("symbol %d is \"%s\"\n", i + 1, result[i]);
    }
    
    
#undef SYMBOL_COUNT
#endif
    
#if 1
    
#endif
}