//
//  common.h
//  c_compiler
//
//  Created by xichen on 2012-3-19.
//  Copyright (c) 2012 xichen. All rights reserved.
//

#ifndef C_COMPILER_COMMON_H
#define C_COMPILER_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif
    
    #define TO_BOOL_STR(boolValue)      ((boolValue == 0) ? "false" : "true")
    
    void    cc_skip_space(const char **p);
    void    cc_skip_space_n(const char **p, int n);
    
#ifdef __cplusplus
}
#endif
        
#endif
