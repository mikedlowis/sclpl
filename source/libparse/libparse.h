/**
  @file libparse.h
  @brief TODO: Describe this file
  $Revision$
  $HeadURL$
  */
#ifndef LIBPARSE_H
#define LIBPARSE_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <errno.h>
#include "mem.h"
#include "vec.h"
#include "exn.h"

typedef struct {
    char* p_line;
    size_t index;
    size_t lineno;
    FILE* p_input;
    char* p_prompt;
} Lexer;

typedef enum {
    T_ID, T_CHAR, T_INT, T_FLOAT, T_BOOL, T_STRING, T_LBRACE, T_RBRACE, T_LBRACK,
    T_RBRACK, T_LPAR, T_RPAR, T_COMMA, T_SQUOTE, T_DQUOTE, T_END, T_END_FILE
} TokenType;

typedef struct {
    TokenType type;
    const char* file;
    size_t line;
    size_t col;
    void* value;
} Token;

DECLARE_EXCEPTION(ParseException);

typedef struct {
    Lexer* p_lexer;
    Token* p_tok;
    vec_t* p_tok_buf;
} Parser;

typedef enum {
    ATOM,
    TREE
} ASTTag;

typedef struct {
    ASTTag tag;
    union {
        Token* tok;
        vec_t* vec;
    } ptr;
} AST;

// Lexer routines
Lexer* lexer_new(char* p_prompt, FILE* p_input);
Token* lex_tok_new(TokenType type, void* val);
Token* lexer_read(Lexer* p_lexer);
void lexer_skipline(Lexer* p_lexer);
char* lexer_dup(const char* p_old);

// Parser routines
Parser* parser_new(char* p_prompt, FILE* input);
void fetch(Parser* p_parser);
Token* peek(Parser* p_parser);
bool parser_eof(Parser* p_parser);
void parser_resume(Parser* p_parser);
void error(Parser* p_parser, const char* p_text);
bool accept(Parser* p_parser, TokenType type);
bool accept_str(Parser* p_parser, TokenType type, const char* p_text);
bool expect(Parser* p_parser, TokenType type);
bool expect_str(Parser* p_parser, TokenType type, const char* p_text);
size_t mark(Parser* p_parser);
void reduce(Parser* p_parser, size_t mark);
AST* get_tree(Parser* p_parser);
void insert(Parser* p_parser, TokenType type, void* value);

// AST Routines
AST* tree_convert(AST* p_tree);
AST* tree_new(ASTTag tag, void* p_obj);
AST* tree_get_child(AST* p_tree, size_t idx);
void* tree_get_val(AST* p_tree);
void* tree_get_child_val(AST* p_tree, size_t idx);
bool tree_is_formtype(AST* p_tree, const char* val);

// Grammar Routines
AST* grammar_toplevel(Parser* p);
void grammar_require(Parser* p);
void grammar_type_annotation(Parser* p);
void grammar_type_definition(Parser* p);
void grammar_type(Parser* p);
void grammar_tuple(Parser* p);
void grammar_function(Parser* p);
void grammar_definition(Parser* p);
void grammar_expression(Parser* p);
void grammar_literal(Parser* p);
void grammar_arglist(Parser* p);
void grammar_if_stmnt(Parser* p);
void grammar_fn_stmnt(Parser* p);

#endif /* LIBPARSE_H */
