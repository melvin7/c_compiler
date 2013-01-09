//
//  error_process.h
//  c_compiler
//
//  Created by xichen on 2012-3-19.
//  Copyright (c) 2012 xichen. All rights reserved.
//

#ifndef C_COMPILER_ERROR_PROCESS_H
#define C_COMPILER_ERROR_PROCESS_H

#include "lex_parser.h"

#ifdef  __cplusplus
extern "C" {
#endif
    
typedef enum
{
    Error_type_oct_include_over_7_num,
    Error_type_not_enough_mem
}Error_type;

typedef struct
{
    Error_type  type;
    char        *info;
}Error_info;

extern  Error_info ErrorInfo[];

void
error_process(Token_state *ts, Error_type type);

#ifdef  __cplusplus
}
#endif
        
#endif
