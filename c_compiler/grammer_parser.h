//
//  grammer_parser.h
//  c_compiler
//
//  Created by xichen on 2012-3-19.
//  Copyright (c) 2012 xichen. All rights reserved.
//

#ifndef C_COMPILER_GRAMMER_PARSER_H
#define C_COMPILER_GRAMMER_PARSER_H

#ifdef  __cplusplus
extern "C" {
#endif
   
    typedef enum
    {
        Grammer_type_nop,
        Grammer_type_declaration,
        Grammer_type_definition,
        Grammer_type_assign,
        Grammer_type_print
    }Grammer_type;
    
#ifdef  __cplusplus
}
#endif

#endif
