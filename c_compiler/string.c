//
//  string.c
//  c_compiler
//
//  Created by xichen on 2012-3-19.
//  Copyright (c) 2012 xichen. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include "string.h"

bool    is_str_startWith(const char *src, const char *pattern)
{
    while(*src && *pattern)
    {
        if(*src == *pattern)
        {
            ++src;
            ++pattern;
            continue;
        }
        return false;
    }
    return *pattern == '\0';
}

bool    is_ch_exist_in(char ch, const char *str)
{
    int i = 0;
    for(; i < strlen(str); ++i)
        if(str[i] == ch)
            return true;
    return false;
}


