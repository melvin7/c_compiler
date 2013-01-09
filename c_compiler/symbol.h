//
//  symbol.h
//  c_compiler
//
//  Created by xichen on 2012-3-19.
//  Copyright (c) 2012 xichen. All rights reserved.
//

#ifndef C_COMPILER_SYMBOL_H
#define C_COMPILER_SYMBOL_H

#ifdef __cplusplus
extern "C" {
#endif

    #include <limits.h>
    #include <float.h>
    #include <stdbool.h>

    extern const char *Keywords[];
    extern const int KeywordsCount;
    extern  const char *Operators[];
    
    typedef enum
    {
        SYMBOL_TYPE_KEYWORD         = 0x1,
        SYMBOL_TYPE_CONST           = 0x2,
        SYMBOL_TYPE_LITERAL         = 0x4,
        SYMBOL_TYPE_STATIC          = 0x8,
        SYMBOL_TYPE_EXTERN          = 0x10,
        SYMBOL_TYPE_VAR             = 0x20,
        
        SYMBOL_TYPE_CHAR            = 0x40,
        SYMBOL_TYPE_SHORT           = 0x80,
        SYMBOL_TYPE_INT             = 0x100,
        SYMBOL_TYPE_LONG            = 0x200,
        SYMBOL_TYPE_FLOAT           = 0x400,
        SYMBOL_TYPE_DOULBE          = 0x800,
        SYMBOL_TYPE_LONGDOUBLE      = 0x1000,
        SYMBOL_TYPE_LONGLONG        = 0x2000,
        
        SYMBOL_TYPE_SEMICOLON       = UINT_MAX - 1,
        SYMBOL_TYPE_MAX             = UINT_MAX
    }SYMBOL_TYPE;

    #define IS_KEYWORD(symbolType)          (((symbolType) & SYMBOL_TYPE_KEYWORD) == SYMBOL_TYPE_KEYWORD)
    #define IS_CONST(symbolType)            (((symbolType) & SYMBOL_TYPE_CONST) == SYMBOL_TYPE_CONST)
    #define IS_LITERAL(symbolType)          (((symbolType) & SYMBOL_TYPE_LITERAL) == SYMBOL_TYPE_LITERAL)
    #define IS_STATIC(symbolType)           (((symbolType) & SYMBOL_TYPE_STATIC) == SYMBOL_TYPE_STATIC)
    #define IS_VAR(symbolType)              (((symbolType) & SYMBOL_TYPE_VAR) == SYMBOL_TYPE_VAR)
    #define IS_SEMICOLON(symbolType)        (((symbolType) & SYMBOL_TYPE_SEMICOLON) == SYMBOL_TYPE_SEMICOLON)
    #define IS_EXTERN(symbolType)           (((symbolType) & SYMBOL_TYPE_EXTERN) == SYMBOL_TYPE_EXTERN)

#define MACRO_IS_ODIGIT(ch)             ((ch) >= '0' && (ch) <= '7')
#define MACRO_IS_DIGIT(ch)              ((ch) >= '0' && (ch) <= '9')
#define MACRO_IS_XDIGIT(ch)             (((ch) >= '0' && (ch) <= '9') \
                                     || ((ch) >= 'A' && (ch) <= 'F') \
                                     || ((ch) >= 'a' && (ch) <= 'f'))
    
    bool    isKeyword(const char *str);
    
    bool    isLiteral(const char *str);
    
    bool    isVar(const char *str);
    
    bool    isSemicolon(const char *str);
    
    bool    isCharLiteral(const char *str);
    bool    isStringLiteral(const char *str);
    
    bool    isDecNumber(const char *str);
    bool    isOctNumber(const char *str);
    bool    isHexNumber(const char *str);
    bool    isFloatNumber(const char *str);
    bool    isDoubleNumber(const char *str);
    static  bool    isDoubleNumber_internal(const char *str, size_t compareLen);
    bool    isOperator(const char *str);
    bool    isOpeChar(const char *str);
    bool    isOpeChar_n(const char *str, int n);
    
    typedef struct
    {
        SYMBOL_TYPE type;
        char        *str;
    }Symbol;


    Symbol      *symbol_construct(const char *str);
    SYMBOL_TYPE symbol_getType(Symbol *sb);
    char        *symbol_getStr(Symbol *sb);
    void        symbol_deconstruct(Symbol *sb);

    
    extern void        testSymbol();
#ifdef __cplusplus
}
#endif


#endif
