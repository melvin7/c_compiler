//
//  lex_parser.h
//  c_compiler
//
//  Created by xichen on 2012-3-19.
//  Copyright (c) 2012 xichen. All rights reserved.
//

#ifndef C_COMPILER_LEX_PARSER_H
#define C_COMPILER_LEX_PARSER_H

#include <stdbool.h>

#ifdef  __cplusplus
extern "C" {
#endif

    typedef enum
    {
        Lex_state_begin,
        Lex_state_id,
        Lex_state_literal_num,
        Lex_state_literal_char,
        Lex_state_literal_str,
        Lex_state_op,
        Lex_state_end,
        
        Lex_state_err
    }Lex_state;
    
    typedef enum
    {
        Lex_sub_state_begin,
        Lex_sub_state_underline,
        Lex_sub_state_alpha,
        Lex_sub_state_dec_num,
        Lex_sub_state_oct_num,
        Lex_sub_state_hex_num,
        Lex_sub_state_op,
        Lex_sub_state_quot,
        Lex_sub_state_space,
        Lex_sub_state_semi,
        Lex_sub_state_literal_char_begin,
        Lex_sub_state_literal_char_end,
        Lex_sub_state_literal_str_begin,
        Lex_sub_state_literal_str_end,
        
        Lex_sub_state_end
    }Lex_sub_state;

    typedef enum
    {
        Token_type_keyword,
        Token_type_var,
        Token_type_literal,
        Token_type_operator,
        Token_type_semicolon,
        Token_type_num,
        
        Token_type_err
        
    }Token_type;

    typedef enum
    {
        Token_sub_type_dec_literal,
        Token_sub_type_dec_unsigned_literal,
        Token_sub_type_dec_long_literal,
        Token_sub_type_dec_unsigned_long_literal,
        Token_sub_type_oct_literal,
        Token_sub_type_hex_literal,
        Token_sub_type_char_literal,
        Token_sub_type_str_literal,
        Token_sub_type_float_literal,
        Token_sub_type_double_literal,
        
        Token_sub_type_err
    }Token_sub_type;
    
    extern  const char *Token_type_str[];
    extern  const char *Token_sub_type_str[];
    
#define TYPE_TO_STR(type)   Token_type_str[(type)]
#define SUB_TYPE_TO_STR(type)   \
        Token_sub_type_str[(type)]
    
    typedef struct
    {
        char            *buf;
        char            *begin;
        char            *cur;
        char            *end;
        
        Lex_state       state;
        Lex_sub_state   sub_state;
    }Token_state;

    typedef struct
    {
        char            *name;
        Token_type      type;
        Token_sub_type  sub_type;
        
        int             len;
    }Token;
    
    // contains Tokens
    typedef struct
    {
        Token       **p;
        int         size;
        int         capacity;
    }TokensTable;
    
    static  Token   *savePreviousToken(Token_state *ts, TokensTable *tt);
    
    Token_state *
        createTokenState(char *buf);
    
    bool    
    getAllTokens(Token_state *ts, TokensTable *tt);
    
    Token *
    getNextToken(Token_state *ts, TokensTable *tk);
    
    void
        freeTokenState(Token_state *ts);

    
    Token *
        createToken(const char *name);
    Token *
    createTokenByLen(const char *name, size_t len);
    void
        freeToken(Token *tk);
    void
    printTokenName(Token *tk);
    void
    printToken(Token *tk);
    Token_type  
    getTokenTypeByName(const char *name);
    Token_type  
    getTokenTypeByToken(const Token *tk);
    Token_sub_type  
    getTokenSubTypeByName(const char *name);
    Token_sub_type  
    getTokenSubTypeByToken(const Token *tk);
    
    TokensTable  *
        createTokensTable(int defaultSize);

    bool
        addTokenToTable(TokensTable *tt, const Token *token);
    void
    removeTokenAt(TokensTable *tt, int index);
    Token   *
    getTokenAt(TokensTable *tt, int index);
    // maybe not right,because the token name can be the same.
    Token   *
    getTokenByName(TokensTable *tt, const char *name);
    char    *
    getTokenNameAt(TokensTable *tt, int index);
    void
    removeLastToken(TokensTable *tt);
    void    
    freeTokensTable(TokensTable *tt);
    
    void
    showAllTokens(TokensTable *tt);
    
    static bool processNameLenIs1(const char *name, Token *tk);
    static bool processName0IsAlpha(const char *name, Token *tk);
    static bool processName0IsNum(const char *name, Token *tk);
    static bool processName0IsOp(const char *name, Token *tk);
    
    char    *findNextSingleQuto(const char *start, int len);
    char    *findNextDoubleQuto(const char *start, int len);
    char    *findNextChar(const char *start, int len, char ch);
    
    void    testLexParser();
    
#ifdef  __cplusplus
}
#endif

#endif
