//
//  print.h
//  c_compiler
//
//  Created by xichen on 2012-3-19.
//  Copyright (c) 2012 xichen. All rights reserved.
//

#ifndef C_COMPILER_PRINT_H
#define C_COMPILER_PRINT_H

#include <stdio.h> 

#ifdef  __cplusplus
extern "C" {
#endif
    
#ifndef PRINT_NL
#define PRINT_NL    printf("\n")
#endif

#ifndef PRINT_INT
#define PRINT_INT(value)   printf("%d", (value))
#endif

#ifndef PRINT_INT_NL
#define PRINT_INT_NL(value)   printf("%d\n", (value))
#endif

#ifndef PRINT_STR_NL
#define PRINT_STR_NL(str)   printf("%s\n", (str))
#endif
    
#ifdef  __cplusplus
}
#endif
        
#endif
