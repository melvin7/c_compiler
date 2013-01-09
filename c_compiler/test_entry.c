//
//  test_entry.c
//  c_compiler
//
//  Created by xichen on 2012-3-19.
//  Copyright (c) 2012 xichen. All rights reserved.
//

#include <stdio.h>
#include "test_entry.h"
#include "statement_parser.h"
#include "symbol.h"
#include "file_reader.h"
#include "lex_parser.h"

void    testEntry()
{
#if 1
    testStatementParser();
#endif
    
#if 1
    testSymbol();
#endif
    
#if 1
    testFileReader();
#endif
    
#if 1
    testLexParser();
#endif
}