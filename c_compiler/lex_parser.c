//
//  lex_parser.c
//  c_compiler
//
//  Created by xichen on 2012-3-19.
//  Copyright (c) 2012 xichen. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lex_parser.h"
#include "string.h"
#include "common.h"
#include <ctype.h>
#include "error_process.h"
#include "symbol.h"

const char *Token_type_str[] = 
{
    "Keyword",
    "Var",
    "Literal",
    "Operator",
    "Semicolon",
    "Num",
    "*ErrType*"
};

const char *Token_sub_type_str[] =
{
    "Dec_Literal",
    "Dec_unsigned_Literal",
    "Dec_long_Literal",
    "Dec_unsigned_long_Literal",
    "Oct_Literal",
    "Hex_Literal",
    "Char_Literal",
    "String_Literal",
    "Float_Literal",
    "Double_Literal",
    "*Sub_Type_Err*"
};

static  Token   *savePreviousToken(Token_state *ts, TokensTable *tt)
{
    // save the previous token
    Token *temp_tk = createTokenByLen(ts->begin, ts->cur - ts->begin);
    if(!temp_tk)
    {
        error_process(ts, Error_type_not_enough_mem);
        return NULL;
    }
    else
    {
        if(!addTokenToTable(tt, temp_tk))
        {
            freeToken(temp_tk);
            return NULL;
        }
        else
        {
            ts->state = Lex_state_begin;
            
            cc_skip_space((const char **)&ts->cur);
            ts->begin = ts->cur;
            return temp_tk;
        }
    }
}

Token_state *
createTokenState(char *buf)
{
    Token_state *state = (Token_state *)malloc(sizeof(Token_state));
    if(!state)
        return NULL;
    size_t len = strlen(buf);
    char    *bufCp = (char *)malloc(len + 1);
    if(!bufCp)
        goto end;
    strcpy(bufCp, buf);
    state->buf = state->begin = state->cur = bufCp;
    state->end = bufCp + len;
    state->state = Lex_state_begin;
    state->sub_state = Lex_sub_state_begin;
    return state;
end:
    free(state);
    return NULL;
}

bool    
getAllTokens(Token_state *ts, TokensTable *tt)
{
    while (*ts->cur)
    {
        if(getNextToken(ts, tt))
            continue;
        else
            return false;
    }
    return true;
}

Token *
getNextToken(Token_state *ts, TokensTable *tt)
{
    cc_skip_space((const char **)&ts->cur);
    while(*ts->cur)
    {
        // alpha
        if(isalpha(*ts->cur))
        {
            if(ts->state == Lex_state_begin || ts->state == Lex_state_id)
            {
                ts->state = Lex_state_id;
                ts->sub_state = Lex_sub_state_alpha;
                goto will_continue;
            }
            else if((*ts->cur >= 'a' && *ts->cur <= 'f')
                    || (*ts->cur >= 'A' && *ts->cur <= 'F'))
            {
                if(ts->sub_state == Lex_sub_state_hex_num)
                {
                    goto will_continue;
                }
            }
            else
            {
                
            }
        }
        else if(*ts->cur == '_')    // '_'
        {
            if(ts->state == Lex_state_begin || ts->state == Lex_state_id)
            {
                ts->state = Lex_state_id;
                ts->sub_state = Lex_sub_state_underline;
                goto will_continue;
            }
        }
        else if(isdigit(*ts->cur))  // digit
        {
            if(ts->state == Lex_state_begin)
            {
                if(*ts->cur == '0')
                {
                    ts->state = Lex_state_literal_num;
                    ts->sub_state = Lex_sub_state_oct_num;
                    goto will_continue;
                }
                else
                {
                    ts->state = Lex_state_literal_num;
                    ts->sub_state = Lex_sub_state_dec_num;
                    goto will_continue;
                }
            }
            else if(ts->sub_state == Lex_sub_state_dec_num)
            {
                ts->state = Lex_state_literal_num;
                ts->sub_state = Lex_sub_state_dec_num;
                goto will_continue;
            }
            else if(ts->sub_state == Lex_sub_state_oct_num)
            {
                if(*ts->cur >= '8')
                {
                    error_process(ts, Error_type_oct_include_over_7_num);
                    return false;
                }
                else
                {
                    ts->state = Lex_state_literal_num;
                    ts->sub_state = Lex_sub_state_oct_num;
                    goto will_continue;
                }
            }
            else if(ts->state == Lex_state_id)
            {
                goto will_continue;
            }
            else if(ts->state == Lex_sub_state_hex_num)
            {
                goto will_continue;
            }
            else
            {
                
            }
        }
        else if(isspace(*ts->cur))    // ' ', '\t', '\n', '\v',...
        {
            if(ts->state != Lex_state_begin)
            {
                return savePreviousToken(ts, tt);
            }
            else
            {
                cc_skip_space((const char **)&ts->cur);
                ts->begin = ts->cur;
                continue;
            }
        }
        else if(isOpeChar(ts->cur))        // operator , '<', '=' and so on
        {
            if(ts->state == Lex_state_begin)
            {
                ts->state = Lex_state_op;
                goto will_continue;
            }
            else if(ts->state == Lex_state_op)
            {
                if(isOpeChar_n(ts->begin, ts->cur - ts->begin + 1))
                {
                    ts->state = Lex_state_op;
                    goto will_continue;
                }
                else
                {
                    return savePreviousToken(ts, tt);
                }
            }
        }
        else if(*ts->cur == ';')
        {
            if(ts->state != Lex_state_begin)
            {
                return savePreviousToken(ts, tt);
            }
            else
            {
                ++ts->cur;
                return savePreviousToken(ts, tt);
            }
        }
        
    will_continue:
        ++ts->cur;
        continue;
    }
    if(*ts->cur == '\0' && ts->cur != ts->begin)
    {
        return savePreviousToken(ts, tt);
    }
    
    return NULL;
}

// an optimized getting token method that can get the token type after the process
Token *
getNextToken_Optimized(Token_state *ts, TokensTable *tt)
{
    cc_skip_space((const char **)&ts->cur);
    while(*ts->cur)
    {
        // alpha
        if(isalpha(*ts->cur))
        {
            if(ts->state == Lex_state_begin || ts->state == Lex_state_id)
            {
                ts->state = Lex_state_id;
                ts->sub_state = Lex_sub_state_alpha;
                goto will_continue;
            }
            else if((*ts->cur >= 'a' && *ts->cur <= 'f')
                    || (*ts->cur >= 'A' && *ts->cur <= 'F'))
            {
                if(ts->sub_state == Lex_sub_state_hex_num)
                {
                    goto will_continue;
                }
            }
            else
            {
                
            }
        }
        else if(*ts->cur == '_')    // '_'
        {
            if(ts->state == Lex_state_begin || ts->state == Lex_state_id)
            {
                ts->state = Lex_state_id;
                ts->sub_state = Lex_sub_state_underline;
                goto will_continue;
            }
        }
        else if(isdigit(*ts->cur))  // digit
        {
            if(ts->state == Lex_state_begin)
            {
                if(*ts->cur == '0')
                {
                    ts->state = Lex_state_literal_num;
                    ts->sub_state = Lex_sub_state_oct_num;
                    goto will_continue;
                }
                else
                {
                    ts->state = Lex_state_literal_num;
                    ts->sub_state = Lex_sub_state_dec_num;
                    goto will_continue;
                }
            }
            else if(ts->sub_state == Lex_sub_state_dec_num)
            {
                ts->state = Lex_state_literal_num;
                ts->sub_state = Lex_sub_state_dec_num;
                goto will_continue;
            }
            else if(ts->sub_state == Lex_sub_state_oct_num)
            {
                if(*ts->cur >= '8')
                {
                    error_process(ts, Error_type_oct_include_over_7_num);
                    return false;
                }
                else
                {
                    ts->state = Lex_state_literal_num;
                    ts->sub_state = Lex_sub_state_oct_num;
                    goto will_continue;
                }
            }
            else if(ts->state == Lex_state_id)
            {
                goto will_continue;
            }
            else if(ts->state == Lex_sub_state_hex_num)
            {
                goto will_continue;
            }
            else
            {
                
            }
        }
        else if(isspace(*ts->cur))    // ' ', '\t', '\n', '\v',...
        {
            if(ts->state != Lex_state_begin)
            {
                return savePreviousToken(ts, tt);
            }
            else
            {
                cc_skip_space((const char **)&ts->cur);
                ts->begin = ts->cur;
                continue;
            }
        }
        else if(isOpeChar(ts->cur))        // operator , '<', '=' and so on
        {
            if(ts->state == Lex_state_begin)
            {
                ts->state = Lex_state_op;
                goto will_continue;
            }
            else if(ts->state == Lex_state_op)
            {
                if(isOpeChar_n(ts->begin, ts->cur - ts->begin + 1))
                {
                    ts->state = Lex_state_op;
                    goto will_continue;
                }
                else
                {
                    return savePreviousToken(ts, tt);
                }
            }
        }
        else if(*ts->cur == ';')
        {
            if(ts->state != Lex_state_begin)
            {
                return savePreviousToken(ts, tt);
            }
            else
            {
                ++ts->cur;
                return savePreviousToken(ts, tt);
            }
        }
        
    will_continue:
        ++ts->cur;
        continue;
    }
    if(*ts->cur == '\0' && ts->cur != ts->begin)
    {
        return savePreviousToken(ts, tt);
    }
    
    return NULL;
}


void
freeTokenState(Token_state *ts)
{
    free(ts->buf);
    free(ts);
}

Token *
createToken(const char *name)
{
    Token *tk = (Token *)malloc(sizeof(Token));
    if(!tk)
        return NULL;
    size_t len = strlen(name);
    char *nameCp = (char *)malloc(len + 1);
    if(!nameCp)
        goto end;
    strcpy(nameCp, name);
    tk->name = nameCp;
    tk->type = Token_type_err;
    return tk;
end:
    free(tk);
    return NULL;
}

Token *
createTokenByLen(const char *name, size_t len)
{
    Token *tk = (Token *)malloc(sizeof(Token));
    if(!tk)
        return NULL;
    char *nameCp = (char *)malloc(len + 1);
    if(!nameCp)
        goto end;
    strncpy(nameCp, name, len);
    tk->name = nameCp;
    tk->len = len;
    tk->type = Token_type_err;
    tk->sub_type = Token_sub_type_err;
    return tk;
end:
    free(tk);
    return NULL;
}

void
printTokenName(Token *tk)
{
    printf("%p tokenName is %s\n", tk, tk->name);
}

void
printToken(Token *tk)
{
    printf("%p tokenName is %s, type:%s, sub_type is %s\n", tk, tk->name, TYPE_TO_STR(tk->type), SUB_TYPE_TO_STR(tk->sub_type));
}

Token_type  
getTokenTypeByName(const char *name)
{
    if(isKeyword(name))
        return Token_type_keyword;
    if(isCharLiteral(name))
        return Token_type_literal;
    if(isStringLiteral(name))
        return Token_type_literal;
    if(isVar(name))
        return Token_type_var;
    if(isOperator(name))
        return Token_type_operator;
    if(isDecNumber(name) 
    || isHexNumber(name) 
    || isOctNumber(name)
    || isFloatNumber(name)
    || isDoubleNumber(name))
        return Token_type_num;
    if(isSemicolon(name))
        return Token_type_semicolon;
    
    return Token_type_err;
}

Token_type  
getTokenTypeByToken(const Token *tk)
{
    return getTokenTypeByName(tk->name);
}

Token_sub_type  
getTokenSubTypeByName(const char *name)
{
    if(isCharLiteral(name))
        return Token_sub_type_char_literal;
    if(isStringLiteral(name))
        return Token_sub_type_str_literal;
    if(isOctNumber(name))
        return Token_sub_type_dec_literal;
    if(isHexNumber(name))
        return Token_sub_type_hex_literal;
    if(isOctNumber(name))
        return Token_sub_type_oct_literal;
    if(isFloatNumber(name))
        return Token_sub_type_float_literal;
    if(isDoubleNumber(name))
        return Token_sub_type_double_literal;
    
    return Token_sub_type_err;
}

Token_sub_type  
getTokenSubTypeByToken(const Token *tk)
{
    return getTokenSubTypeByName(tk->name);
}

void    analyseTokenTypes(Token *tk)
{
    char    *name = tk->name;

    if(processNameLenIs1(name, tk))
        return;
    if(processName0IsAlpha(name, tk))
        return;
    
    if(processName0IsNum(name, tk))
        return;
    if(processName0IsOp(name, tk))
        return;
    
    tk->type = Token_type_err;
    tk->sub_type = Token_sub_type_err;
}   // end of analyseTokenTypes

void
freeToken(Token *tk)
{
    free(tk->name);
    free(tk);
}


TokensTable  *
createTokensTable(int defaultSize)
{
    TokensTable *tt = (TokensTable *)malloc(sizeof(TokensTable));
    if(!tt)
        return NULL;
    
    Token    **p = (Token **)malloc(sizeof(char *) * defaultSize);
    if(!p)
        goto err_has_malloc_tt;
    tt->p = p;
    tt->capacity = defaultSize;
    tt->size = 0;
    return tt;
    
err_has_malloc_tt:
    free(tt);
    return NULL;
}


bool
addTokenToTable(TokensTable *tt, const Token *token)
{
    if(tt->size < tt->capacity)
    {
        tt->p[tt->size] = (Token *)token;
        ++tt->size;
        return true;
    }
    else
    {
        return false;
    }
}


void
removeTokenAt(TokensTable *tt, int index)
{
    Token *tk = (Token *)tt->p[index];
    freeToken(tk);
    tt->p[index] = NULL;
}

Token *
getTokenAt(TokensTable *tt, int index)
{
    return tt->p[index];
}

Token   *
getTokenByName(TokensTable *tt, const char *name)
{
    int i = 0;
    for(; i < tt->size; ++i)
    {
        if(strcmp(name, tt->p[i]->name) == 0)
            return tt->p[i];
    }
    return NULL;
}

char  *
getTokenNameAt(TokensTable *tt, int index)
{
    return tt->p[index]->name;
}

void
removeLastToken(TokensTable *tt)
{
    removeTokenAt(tt, tt->size - 1);
}

void    
freeTokensTable(TokensTable *tt)
{
    int i = 0;
    for(; i < tt->size; ++i)
    {
        if(tt->p[i])
            free(tt->p[i]);
    }
    free(tt);
}

void
showAllTokens(TokensTable *tt)
{
    int i = 0;
    printf("TokensTable:%p All tokens: ", tt);
    for(; i < tt->size; ++i)
    {
        if(tt->p[i])
        {
            printf("|%s| ", ((Token *)tt->p[i])->name);   
        }
    }
    printf("\n");
}

static bool processNameLenIs1(const char *name, Token *tk)
{
    if(tk->len > 1)
        return false;
    
    if(name[0] == ';')
    {
        tk->type = Token_type_semicolon;
        return true;
    }
    if(name[0] == '.' 
       || name[0] == '-'
       || name[0] == '*'
       || name[0] == '&'
       || name[0] == '!'
       || name[0] == '~'
       || name[0] == '/'
       || name[0] == '%'
       || name[0] == '+'
       || name[0] == '-'
       || name[0] == '>'
       || name[0] == '<'
       || name[0] == '^'
       || name[0] == '|'
       || name[0] == '='
       || name[0] == ','   
       )
    {
        tk->type = Token_type_operator;
        return true;
    }
    if(isdigit(name[0]))
    {
        tk->type = Token_type_num;
        return true;
    }
    
    if(isalpha(name[0]))
    {
        tk->type = Token_type_var;
        return true;
    }
    else
        return false;
}

static bool processName0IsAlpha(const char *name, Token *tk)
{
    if(name[0] == 'a')
    {
        if(strcmp(name, "auto") == 0)
        {
            tk->type = Token_type_keyword;
            return true;
        }
        tk->type = Token_type_var;
        return true;
    }
    
    if(name[0] == 'b')
    {
        if(strcmp(name, "break") == 0)
        {
            tk->type = Token_type_keyword;
            return true;
        }
        tk->type = Token_type_var;
        return true;
    }
    
    if(name[0] == 'c')
    {
        if(strcmp(name, "char") == 0
           || strcmp(name, "const") == 0
           || strcmp(name, "case") == 0
           || strcmp(name, "continue") == 0)
        {
            tk->type = Token_type_keyword;
            return true;
        }
        tk->type = Token_type_var;
        return true;
    }
    
    if(name[0] == 'd')
    {
        if(strcmp(name, "double") == 0
           || strcmp(name, "default") == 0
           || strcmp(name, "do") == 0)
        {
            tk->type = Token_type_keyword;
            return true;
        }
        tk->type = Token_type_var;
        return true;
    }
    
    if(name[0] == 'e')
    {
        if(strcmp(name, "extern") == 0
           || strcmp(name, "else") == 0
           || strcmp(name, "enum") == 0)
        {
            tk->type = Token_type_keyword;
            return true;
        }
        tk->type = Token_type_var;
        return true;
    }
    
    if(name[0] == 'f')
    {
        if(strcmp(name, "float") == 0
           || strcmp(name, "for") == 0)
        {
            tk->type = Token_type_keyword;
            return true;
        }
        tk->type = Token_type_var;
        return true;
    }
    
    if(name[0] == 'g')
    {
        if(strcmp(name, "goto") == 0)
        {
            tk->type = Token_type_keyword;
            return true;
        }
        tk->type = Token_type_var;
        return true;
    }
    
    if(name[0] == 'i')
    {
        if(strcmp(name, "int") == 0
        || strcmp(name, "if") == 0)
        {
            tk->type = Token_type_keyword;
            return true;
        }
        tk->type = Token_type_var;
        return true;
    }
    
    if(name[0] == 'l')
    {
        if(strcmp(name, "long") == 0)
        {
            tk->type = Token_type_keyword;
            return true;
        }
        tk->type = Token_type_var;
        return true;
    }
    
    if(name[0] == 'r')
    {
        if(strcmp(name, "register") == 0
        || strcmp(name, "return") == 0)
        {
            tk->type = Token_type_keyword;
            return true;
        }
        tk->type = Token_type_var;
        return true;
    }
    
    if(name[0] == 's')
    {
        if(   strcmp(name, "short") == 0
           || strcmp(name, "signed") == 0
           || strcmp(name, "static") == 0
           || strcmp(name, "sizeof") == 0
           || strcmp(name, "struct") == 0
           || strcmp(name, "switch") == 0)
        {
            tk->type = Token_type_keyword;
            return true;
        }
        tk->type = Token_type_var;
        return true;
    }
    
    if(name[0] == 't')
    {
        if(strcmp(name, "typedef") == 0)
        {
            tk->type = Token_type_keyword;
            return true;
        }
        tk->type = Token_type_var;
        return true;
    }
    
    if(name[0] == 'u')
    {
        if(strcmp(name, "unsigned") == 0
        || strcmp(name, "union") == 0)
        {
            tk->type = Token_type_keyword;
            return true;
        }
        tk->type = Token_type_var;
        return true;
    }
    
    if(name[0] == 'v')
    {
        if(strcmp(name, "volatile") == 0
           || strcmp(name, "void") == 0)
        {
            tk->type = Token_type_keyword;
            return true;
        }
        tk->type = Token_type_var;
        return true;
    }
    
    if(name[0] == 'w')
    {
        if(strcmp(name, "while") == 0)
        {
            tk->type = Token_type_keyword;
            return true;
        }
        tk->type = Token_type_var;
        return true;
    }
    
    if(name[0] == 'h' || name[0] == 'j' || name[0] == 'k'  || name[0] == 'm' || name[0] == 'n' || name[0] == 'o' || name[0] == 'p' || name[0] == 'q' || name[0] == 'x' || name[0] == 'y' || name[0] == 'z')
    {
        tk->type = Token_type_var;
        return true;
    }
    return false;
}

static bool processName0IsNum(const char *name, Token *tk)
{
    char    ch = name[0];
    int     len = tk->len;
    if(ch == '0')
    {
        if(len == 1)
        {
            tk->type = Token_type_num;
            tk->sub_type = Token_sub_type_dec_literal;
            return true;
        }
        else if(len > 2)
        {
            if(name[1] == 'x' || name[1] == 'X')
            {
                tk->type = Token_type_num;
                tk->sub_type = Token_sub_type_hex_literal;
                return true;
            }
            else
            {
                goto oct_process;
            }
        }
    oct_process:
        tk->type = Token_type_num;
        tk->sub_type = Token_sub_type_oct_literal;
        return true;
    }
    else if(ch >= '1' && ch <= '9')
    {
        if(is_ch_exist_in('.', name) 
        || is_ch_exist_in('e', name) 
        || is_ch_exist_in('E', name))
        {
            if(is_ch_exist_in('f', name) 
             || is_ch_exist_in('F', name))
            {
                tk->type = Token_type_num;
                tk->sub_type = Token_sub_type_float_literal;
                return true;
            }
            else
            {
                tk->type = Token_type_num;
                tk->sub_type = Token_sub_type_double_literal;
                return true;
            }
        }
        
        tk->type = Token_type_num;
        tk->sub_type = Token_sub_type_dec_literal;
        return true;
    }
    return false;
}

static bool processName0IsOp(const char *name, Token *tk)
{
    char    ch = name[0];
    int     len = tk->len;
    if(ch == '-')
    {
        if(len == 2 && (name[1] == '>' || name[1] == '-'|| name[1] == '='))
            goto type_op_process;
        else 
            goto type_err_process;
    }
    if(ch == '+')
    {
        if(len == 2 && (name[1] == '+' || name[1] == '='))
            goto type_op_process;
        else 
            goto type_err_process;
    }
    if(ch == '<')
    {
        if(name[1] == '<')
        {
            if(len == 2)
                goto type_op_process;
            else if(len == 3)
            {
                if(name[2] == '=')
                    goto type_op_process;
                else
                    goto type_err_process;
            }
            else
                goto type_err_process;
        }
        else if(name[1] == '=')
        {
            if(len == 2)
                goto type_op_process;
            else
                goto type_err_process;
        }
        else 
            goto type_err_process;
    }
    if(ch == '>')
    {
        if(name[1] == '>')
        {
            if(len == 2)
                goto type_op_process;
            else if(len == 3)
            {
                if(name[2] == '=')
                    goto type_op_process;
                else
                    goto type_err_process;
            }
            else
                goto type_err_process;
        }
        else if(name[1] == '=')
        {
            if(len == 2)
                goto type_op_process;
            else
                goto type_err_process;
        }
        else 
            goto type_err_process;
    }
    if(ch == '=')
    {
        if(len == 2 && name[1] == '=')
            goto type_op_process;
        else
            goto type_err_process;
    }
    if(ch == '!' && len == 2 && name[1] == '=')
        goto type_op_process;
    if(ch == '&' && len == 2 && (name[1] == '&' || name[1] == '='))
        goto type_op_process;
    if(ch == '|' && len == 2 && (name[1] == '|' || name[1] == '='))
        goto type_op_process;
    if(ch == '/' && len == 2 && name[1] == '=')
        goto type_op_process;
    if(ch == '*' && len == 2 && name[1] == '=')
        goto type_op_process;
    if(ch == '%' && len == 2 && name[1] == '=')
        goto type_op_process;
    if(ch == '^' && len == 2 && name[1] == '=')
        goto type_op_process;
    
    goto type_err_process;
type_op_process:
    tk->type = Token_type_operator;
    return true;
type_err_process:
    tk->type = Token_type_err;
    return false;
}

char    *findNextSingleQuto(const char *start, int len)
{
    return findNextChar(start, len, '\'');
}


char    *findNextDoubleQuto(const char *start, int len)
{
    return findNextChar(start, len, '\"');
}

char    *findNextChar(const char *start, int len, char ch)
{
    int i = 0;
    for(; i < len; ++i)
    {
        if(start[i] == ch)
            return (char *)start + i;
    }
    return NULL;
}

void    testLexParser()
{ 
#if 1
    // char    *str = "0812";
    // char    *str = "012";
    char    *str = "int i ==< 12;";
    
    Token_state *ts = createTokenState(str);
    if(!ts)
    {
        printf("create Token_state error\n");
        return;
    }
    
    TokensTable *tt = createTokensTable(16);
    if(!tt)
    {
        printf("create TokensTable error\n");
        return;
    }
#if 0
    if(getAllTokens(ts, tt))
    {
       // printTokenName(tt->p[0]);
        showAllTokens(tt);
    }
#endif
    
#if 1   // getNextToken
    {
        Token *tk;
        while((tk = getNextToken(ts, tt)))
        {
            analyseTokenTypes(tk);
            printToken(tk);
        }
    }
#endif
    
    freeTokensTable(tt);
    freeTokenState(ts);
    
#endif
}


