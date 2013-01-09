//
//  statement_parser.h
//  c_compiler
//
//  Created by xichen on 2012-3-19.
//  Copyright (c) 2012 xichen. All rights reserved.
//

#ifndef C_COMPILER_STATEMENT_PARSER_H
#define C_COMPILER_STATEMENT_PARSER_H

#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif
    
// parse the statement to symbols, stores in the symbolArr
// returns the parsed symbols count
// just a simple parser, it just skips blanks: "int i= 2;",   "i=" will be parsed to one token
int    cc_sp_to_symbols(const char *statement, char *symbolArr[], size_t symbolArrCnt);

void   testStatementParser();

#ifdef __cplusplus
}
#endif
        
#endif
