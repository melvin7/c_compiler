//
//  string.h
//  c_compiler
//
//  Created by xichen on 2012-3-19.
//  Copyright (c) 2012 xichen. All rights reserved.
//

#ifndef C_COMPILER_STRING_H
#define C_COMPILER_STRING_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

    bool    is_str_startWith(const char *src, const char *pattern);

    bool    is_ch_exist_in(char ch, const char *str);
    
#ifdef __cplusplus
}
#endif
        
#endif
