//
//  common.c
//  c_compiler
//
//  Created by xichen on 2012-3-19.
//  Copyright (c) 2012 xichen. All rights reserved.
//

#include <stdio.h>
#include <ctype.h>
#include "common.h"

void    cc_skip_space(const char **p)
{
    while (isspace(**p))
    {
        (*p)++;
    }
}

void    cc_skip_space_n(const char **p, int n)
{
    int index = 0;
    while (isspace(**p) && index < n)
    {
        (*p)++;
        ++index;
    }
}
