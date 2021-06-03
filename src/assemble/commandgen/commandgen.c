#include "../commandgen.h"
#include "../tokenizer.h"
<<<<<<< HEAD
#include "../error.h"
#include <stddata.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
=======
#include "instruction_layouts.h"
#include <stddata.h>
>>>>>>> assemble_tokenizer_commongen_using_decision_tree
#include <stdio.h>

bool FunctionGen(
    Map restrict symbols, 
<<<<<<< HEAD
    List restrict tokens, 
=======
    Vector restrict tokens, 
>>>>>>> assemble_tokenizer_commongen_using_decision_tree
    Vector restrict output, 
    int offset, 
    int instructions_num
) {
<<<<<<< HEAD
    if(ListSize(tokens) < 1) {
        SetErrorCode(STAGE_DISPATCH, ERROR_TOO_SHORT);
        return true;
    }
    if(TokenType(ListFront(tokens)) != TOKEN_INSTRUCTION) {
        SetErrorCode(STAGE_DISPATCH, ERROR_EXPECTED_INSTRUCTION);
        return true;
    }

    bool(*process_function)(Map, List, Vector, int, int) = 
            MapGet(func_proc, (int)TokenInstructionType(ListFront(tokens)));
    
    if(TokenType(ListFront(tokens)) == NULL) {
        SetErrorCode(STAGE_DISPATCH, ERROR_INVALID_INSTRUCTION);
        return true;
    }

=======
    bool(*process_function)(Map, Vector, Vector, int, int) = DecisionTreeQuery(instruction_layouts, tokens);
>>>>>>> assemble_tokenizer_commongen_using_decision_tree
    return process_function(symbols, tokens, output, offset, instructions_num);
}