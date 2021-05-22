#include "process_data_processing.h"
#include <stddata.h>
#include "common_defs.h"

static inline unsigned int ProcessBaseRegisters(List restrict tokens) {
    assert(ListSize(tokens) >= 2);
    
    unsigned int condition;
    char func_base[MAX_FUNCTION_LENGTH + 1];
    SplitFunction(ListPopFront(tokens), func_base, &condition);

    assert(MapQuery(data_proc_opcodes, func_base));
    int opcode = MapGet(data_proc_opcodes, func_base);

    assert(IsRegister(ListFront(tokens)));
    unsigned int reg_d, reg_n, set_cspr;
    if(StringEq(func_base, "mov")) {
        reg_d = GetRegisterNum(ListPopFront(tokens));
        reg_n = 0;
        set_cspr = 0;
    } else if(SetQuery(comp_funcs, func_base)) {
        reg_d = 0;
        reg_n = GetRegisterNum(ListPopFront(tokens));
        set_cspr = 1;
    } else {
        assert(ListSize(tokens) >= 2);
        assert(IsRegister(ListGet(tokens, 1)));
        reg_d = GetRegisterNum(ListPopFront(tokens));
        reg_n = GetRegisterNum(ListPopFront(tokens));
        set_cspr = 0;
    }
    assert(reg_d <= 12 && reg_n <= 12);

    unsigned int instruction = 0;
    instruction |= condition << 28;
    instruction |= opcode << 21;
    instruction |= set_cspr << 20;
    instruction |= reg_n << 16;
    instruction |= reg_d << 12;
    return instruction;
}

static inline unsigned int ProcessImmediate(List restrict tokens) {
    assert(ListSize(tokens) == 1);
    unsigned int expr_value = GetExpressionValue(
        dummy, ListPopFront(tokens), false, true
    );

    unsigned int rotate_left = 0;
    while((expr_value & 0b11) == 0 && expr_value > 0) {
        rotate_left++;
        expr_value >>= 2;
    }
    assert(expr_value < (1<<8));

    int operand2 = 0;
    operand2 |= (rotate_left == 0 ? 0 : 16 - rotate_left) << 8;
    operand2 |= expr_value;

    return operand2;
}

static inline unsigned int ProcessImmediateShift(List restrict tokens) {
    return GetExpressionValue(
        dummy, ListPopFront(tokens), false, true
    ) << 3;
}

static inline unsigned int ProcessRegisterShift(List restrict tokens) {
    assert(IsRegister(ListFront(tokens)));
    unsigned int reg_s = GetRegisterNum(ListPopFront(tokens));
    assert(reg_s <= 12);
    
    return (reg_s << 4) | 1;
}

static inline unsigned int ProcessRegister(List restrict tokens) {
    assert(IsRegister(ListFront(tokens)));
    unsigned int reg_m = GetRegisterNum(ListPopFront(tokens));
    assert(reg_m <= 12);
    
    unsigned int shift = 0;
    if(ListSize(tokens) > 0) {
        assert(ListSize(tokens) == 2);
        assert(MapQuery(shift_codes, ListFront(tokens)));
        const unsigned int shift_code = MapGet(shift_codes, ListPopFront(tokens));
        
        shift |= shift_code << 1;

        shift |= IsImmediate(ListFront(tokens), false, true) ?
            ProcessImmediateShift(tokens) : ProcessRegisterShift(tokens);
    }

    int operand2 = 0;
    operand2 |= reg_m;
    operand2 |= shift << 4;

    return operand2;
}

void ProcessDataProcessing(
    Map restrict symbols, 
    List restrict tokens, 
    Vector restrict output, 
    int offset, 
    int instructions_num
) {
    unsigned int instruction = ProcessBaseRegisters(tokens);

    assert(!ListEmpty(tokens));
    unsigned int immediate = IsImmediate(ListFront(tokens), false, true);

    unsigned int operand2 = immediate ? ProcessImmediate(tokens) : ProcessRegister(tokens);

    instruction |= immediate << 25;
    instruction |= operand2;
    SetInstruction(output, instruction, offset);
}