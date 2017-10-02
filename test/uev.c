/*
 * MIT License
 *
 * Copyright (c) 2016-2017 Abel Lucas <www.github.com/uael>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <uds/map.h>

#include "uev.h"

typedef struct token {
  i8_t type;
} token_t;

VEC8_DEFINE(tokens, token_t, addrcmp)

typedef struct {
  observers_t observers;
  tokens_t tokens;
} lexer_t;

typedef enum lex_event_code {
  LEX_ON_TOK_PUSH = 0,
  LEX_ON_TOK_POP
} lex_event_code_t;

OBSERVABLE_DEFINE(lexer, lexer_t, lex_event_code_t)

typedef struct macro {
  tokens_t tokens;
} macro_t;

STR_MAP_DEFINE(macros, macro_t, addrcmp)

typedef OBSERVER(pp, lexer_t, lex_event_code_t,
  macros_t macros;
) pp_t;

void
pp_dtor(pp_t *self) {
  printf("On pp dtor !\n");
  macros_dtor(&self->macros);
}

ret_t
pp_update(pp_t *self, lexer_t *lexer, lex_event_code_t code, void *arg) {
  token_t *tok;

  (void) self;
  (void) lexer;
  switch (code) {
    case LEX_ON_TOK_PUSH:
      tok = arg;
      printf("%c\n", tok->type);
      break;
    case LEX_ON_TOK_POP:
      break;
  }
  return RET_SUCCESS;
}

i32_t
main(void) {
  lexer_t lex = {0};

  lexer_attach(&lex, init_ptr (pp_t, pp_update, pp_dtor));
  lexer_notify(&lex, LEX_ON_TOK_PUSH, init_ptr (token_t, 'H'));
  lexer_notify(&lex, LEX_ON_TOK_PUSH, init_ptr (token_t, 'e'));
  lexer_notify(&lex, LEX_ON_TOK_PUSH, init_ptr (token_t, 'l'));
  lexer_notify(&lex, LEX_ON_TOK_PUSH, init_ptr (token_t, 'l'));
  lexer_notify(&lex, LEX_ON_TOK_PUSH, init_ptr (token_t, 'o'));
  lexer_detach(&lex);
  return 0;
}