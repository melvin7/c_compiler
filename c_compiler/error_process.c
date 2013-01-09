//
//  error_process.c
//  c_compiler
//
//  Created by xichen on 2012-3-19.
//  Copyright (c) 2012 xichen. All rights reserved.
//

#include <stdio.h>
#include "error_process.h"
#include "print.h"

Error_info ErrorInfo[] = 
{
    {Error_type_oct_include_over_7_num, "oct number include over 7 num"},
    {Error_type_not_enough_mem,         "not enough memory"}
};

void
error_process(Token_state *ts, Error_type type)
{
    PRINT_STR_NL(ErrorInfo[type].info);
}