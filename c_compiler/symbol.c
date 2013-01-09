//
//  symbol.c
//  c_compiler
//
//  Created by xichen on 2012-3-19.
//  Copyright (c) 2012 xichen. All rights reserved.
//

#include <stdio.h>
#include "symbol.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "common.h"
#include "string.h"

const char *Keywords[] =  {
    "char",         
    "short", 
    "int", 
    "long",
    "float", 
    "double", 
    "long long", 
    "long double", 
    "unsigned", 
    "signed",
    "const",
    "extern",
    "volatile",
    "auto",
    "static",
    "break",
    "case",
    "continue",
    "default",
    "do",
    "else",
    "enum",
    "for",
    "goto",
    "if",
    "register",
    "return",
    "sizeof",
    "struct",
    "switch",
    "typedef",
    "union",
    "void",
    "while"
                    };

const char *Operators[] = 
{
    ".",
    "->",
    "-",
    "++",
    "--",
    "*",
    "&",
    "!",
    "~",
    "sizeof",
    "/",
    "*",
    "%",
    "+",
    "-",
    "<<",
    ">>",
    ">",
    ">=",
    "<",
    "<=",
    "==",
    "!=",
    "^",
    "|",
    "&&",
    "||",
    "=",
    "/=",
    "*=",
    "%=",
    "+=",
    "-=",
    "<<=",
    ">>=",
    "&=",
    "^=",
    "|=",
    ","
};

const int KeywordsCount = sizeof(Keywords) / sizeof(Keywords[0]);

bool    isKeyword(const char *str)
{
    int i = 0;
    for(; i < KeywordsCount; ++i)
    {
        if(strcmp(str, Keywords[i]) == 0)
            return true;
    }
    return false;
}

// just for hex num, char, char * style string, float or double number
bool    isLiteral(const char *str)
{
    size_t len;
    int i;

    if(isKeyword(str))
        return false;
    
    len = strlen(str);
    if(len == 3 && str[0] == '\'' && str[2] == '\'')
        return true;
    if(len >= 3 && str[0] == '\"' && str[len - 1] == '\"')
        return true;
    
    for(i = 0; i < len; ++i)
    {
        if(isnumber(str[i]))
            continue;
        if(i == len - 1 && (str[i] == 'u' || str[i] == 'U' || str[i] == 'l' || str[i] == 'L'))
            return true;
        if(i == len - 2)
        {
            if((str[i] == 'u' || str[i] == 'U') && (str[i + 1] == 'l' || str[i + 1] == 'L'))
                return true;
            if((str[i + 1] == 'u' || str[i + 1] == 'U') && (str[i] == 'l' || str[i] == 'L'))
                return true;
        }
        if(str[i] == '.')
        {
            int j = i + 1;
            for(; j < len; ++j)
            {
                if(isnumber(str[j]))
                    continue;
                
                break;
            }
            if(j == len - 1)
            {
                if(str[j] == 'f' || str[j] == 'F')
                    return true;
            }
            if(j == len)
                return true;
            return false;
        }
        if(str[i] == 'e' || str[i] == 'E')
        {
            int j = i + 1;
            for(; j < len; ++j)
            {
                if(isnumber(str[j]))
                    continue;
                return false;
            }

            return true;
        }
        return false;
    }
    
    return true;
}

bool    isVar(const char *str)
{
    int i = 1;
    if(isKeyword(str) || isSemicolon(str))
        return false;
    
    if(str[0] != '_' && !isalpha(str[0]))
        return false;
    
    for(; i < strlen(str); ++i)
        if(!isalpha(str[i]))
            return false;
    
    return true;
}


inline bool    isSemicolon(const char *str)
{
    return strcmp(str, ";") == 0;
}

inline bool    isCharLiteral(const char *str)
{
    return strlen(str) == 3 && str[0] == '\'' && str[2] == '\'';
}

inline  bool    isStringLiteral(const char *str)
{
    size_t len = strlen(str);
    return len >= 2 && str[0] == '\"' && str[len - 1] == '\"';
}

bool    isDecNumber(const char *str)
{
    if(str[0] == '\0')
        return false;
    if(str[0] == '0')
        return false;
    
    while(*str)
    {
        if (!MACRO_IS_DIGIT(*str)) 
            return false;
        ++str;
    }
    return true;
}

bool    isOctNumber(const char *str)
{
    if(str[0] == '\0')
        return false;
    if(str[0] != '0')
        return false;
    
    ++str;
    while(*str)
    {
        if (!MACRO_IS_ODIGIT(*str)) 
            return false;
        ++str;
    }
    return true;
}

bool    isHexNumber(const char *str)
{
    if(str[0] == '\0')
        return false;
    if(is_str_startWith(str, "0x") || is_str_startWith(str, "0X"))
        str += 2;
    else
        return false;
    
    while(*str)
    {
        if (!MACRO_IS_XDIGIT(*str)) 
            return false;
        ++str;
    }
    return true;
}

bool    isFloatNumber(const char *str)
{
    size_t len = strlen(str);
    if(str[len - 1] != 'f' && str[len - 1] != 'F')
        return false;
        
    return isDoubleNumber_internal(str, len - 1);
}

bool    isDoubleNumber(const char *str)
{
    return isDoubleNumber_internal(str, strlen(str));
}

bool    isDoubleNumber_internal(const char *str, size_t compareLen)
{
    bool meetDot = false;
    bool meetE = false;
    int i;
    if(compareLen < 1)
        return false;
    
    i = 0;
    while(*str && i < compareLen)
    {
        if(*str == '.')
        {
            if(meetDot)
                return false;
            meetDot = true;
            ++str;
            ++i;
            continue;
        }
        if(*str == 'e' || *str == 'E')
        {
            if(meetE)
                return false;
            meetE = true;
            ++str;
            ++i;
            if(*str == '+' || *str == '-')
            {
                ++str;
                ++i;
                if(*str == '\0')
                    return false;
            }
            if(*str == '\0' || !MACRO_IS_DIGIT(*str))
                return false;
            
            ++str;
            ++i;
            while(*str)
            {
                if(!MACRO_IS_DIGIT(*str))
                    return false;
                ++str;
                ++i;
            }
            break;
        }
        if(!MACRO_IS_DIGIT(*str))
            return false;
        ++str;
        ++i;
    }
    
    return true;
}

bool    isOperator(const char *str)
{
    int i = 0;
    for(; i < sizeof(Operators) / sizeof(Operators[0]); ++i)
    {
        if(strcmp(str, Operators[i]) == 0)
            return true;
    }
    return false;
}

bool    isOpeChar(const char *str)
{
    return isOpeChar_n(str, 1);
}

bool    isOpeChar_n(const char *str, int n)
{
    int i = 0;
    for(; i < sizeof(Operators) / sizeof(Operators[0]); ++i)
    {
        if(strncmp(str, Operators[i], n) == 0)
            return true;
    }
    return false;
}

Symbol      *symbol_construct(const char *str)
{
    SYMBOL_TYPE type;
    Symbol *sb = (Symbol *)malloc(sizeof(Symbol));
    if(!sb)
        return NULL;
    char *sbStr = (char *)malloc(strlen(str) + 1);
    if(!sbStr)
        return NULL;
    
    type = 0;
    if(isKeyword(str))
        type |= SYMBOL_TYPE_KEYWORD;
    if(isVar(str))
        type |= SYMBOL_TYPE_VAR;
    if(isSemicolon(str))
        type |= SYMBOL_TYPE_SEMICOLON;
    if(isLiteral(str))
        type |= SYMBOL_TYPE_LITERAL;
    sb->type = type;
    
    strcpy(sbStr, str);
    sb->str = sbStr;
    return sb;
}

inline SYMBOL_TYPE symbol_getType(Symbol *sb)
{
    return sb->type;
}

inline char        *symbol_getStr(Symbol *sb)
{
    return sb->str;
}

void        symbol_deconstruct(Symbol *sb)
{
    free(sb->str);
    free(sb);
}


void        testSymbol()
{
#if 0   // Operator
    int i = 0;
    for(; i < KeywordsCount; ++i)
    {
        printf("%s\n", Operators[i]);
    }
#endif
    
// test: isKeyword  isLiteral  isVar   isSemicolon  symbol_construct  symbol_deconstruct
#if 0   // not ok
    char *symbol[] = {"int", "i", "=", "12", ";"};
    for(int i = 0; i < sizeof(symbol) / sizeof(symbol[0]); ++i)
    {
        // ok
        printf("symbol %s : isKeyword:%s isLiteral:%s isVar:%s isSemicolon:%s\n", 
               symbol[i], TO_BOOL_STR(isKeyword(symbol[i])), TO_BOOL_STR(isLiteral(symbol[i])),
               TO_BOOL_STR(isVar(symbol[i])), TO_BOOL_STR(isSemicolon(symbol[i])));
        Symbol *sb = symbol_construct(symbol[i]);
        if(sb)
        {
            // not ok
            printf("%x %x %x %x \n",  IS_KEYWORD(sb->type), 
                                IS_LITERAL(sb->type), 
                                IS_VAR(sb->type),
                                IS_SEMICOLON(sb->type));
            symbol_deconstruct(sb);
        }
    }
#endif
    
// test: isCharLiteral isStringLiteral isDecNumber isOctNumber isHexNumber isFloatNumer
// tes:  isDoubleNumber
#if 0   // ok
    int i = 0;
    const char *strArr[] = {"\'c\'", "\"abc\"", "453", "0453", "781", "a90", "0x34", "0X56",
                            "9.34", "9.4e2", "9.5E5", "9e+2", "9e-3", "9.34f", "9.34F"
                            };

    for(; i < sizeof(strArr) / sizeof(strArr[0]); ++i)
    {
        printf("%s: isCharLiteral(%s)\n\t", strArr[i], TO_BOOL_STR(isCharLiteral(strArr[i])));
        printf("isStringLiteral(%s)\n\t", TO_BOOL_STR(isStringLiteral(strArr[i])));
        printf("isDecNumber(%s)\n\t", TO_BOOL_STR(isDecNumber(strArr[i])));
        printf("isOctNumber(%s)\n\t", TO_BOOL_STR(isOctNumber(strArr[i])));
        printf("isHexNumber(%s)\n\t", TO_BOOL_STR(isHexNumber(strArr[i])));
        printf("isFloatNumber(%s)\n\t", TO_BOOL_STR(isFloatNumber(strArr[i])));
        printf("isDoubleNumber(%s)\n", TO_BOOL_STR(isDoubleNumber(strArr[i])));
    }
    
    /*  // I don't know why, but it can't output all strings
    for(; i < sizeof(strArr) / sizeof(strArr[0]); ++i)
    {
        printf("%s: isCharLiteral(%s)\n\t isStringLiteral(%s)\n\t isDecNumber(%s)\n\t isOctNumber(%s)\n\t isHexNumber(%s)\n\t isFloatNumber(%s)\n\t isDoubleNumber(%s)\n",
               strArr[i], TO_BOOL_STR(isCharLiteral(strArr[i])), TO_BOOL_STR(isStringLiteral(strArr[i])), TO_BOOL_STR(isDecNumber(strArr[i])), TO_BOOL_STR(isOctNumber(strArr[i])), TO_BOOL_STR(isHexNumber(strArr[i])), TO_BOOL_STR(isFloatNumber(strArr[i])), TO_BOOL_STR(isDoubleNumber(strArr[i])));
    }
     */
#endif
}
