#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "vm_interface.h"

#include "opcodes.h"

uint8_t const_pull[CONST_PULL_SIZE]; 
uint8_t stack[STACK_SIZE] = {};

uint64_t sp;

uint64_t GetSP() {
    return sp;
}

void Move_sp(int n) {
    if(n >= 0) {
    	sp = sp + (uint64_t) n;
    }
    else {
	n *= -1;
	assert(sp >= (uint64_t) n);
	sp = sp - (uint64_t) n;
    }	    
}

// TODO - dynamic frame
#define FRAME_SIZE 10

#define LOG_ON
#define ASM

// Programm counter
uint8_t pc = 0;

// Constant pull for method
// TODO Get Const pool dynamicly

// Method frame for method
uint64_t frame[FRAME_SIZE];
register uint64_t* curr_frame asm ("t1");

void Init() {
    // 1. Clean intruction stack
    sp = 0;
    memset(stack, 0, STACK_SIZE);

    // 2. TODO - fill const pull
    memset(const_pull, 0, CONST_PULL_SIZE);

    // 3. Clean Frame
    // TODO - support dynamic frames 
    curr_frame = frame;

    // 4. Initialize pc
    pc = 0;
};


// Execute bytecode sequence
void Execute(uint8_t* bc) {
    while (1) {
        uint8_t opcode = bc[pc];

        switch (opcode) {
            case(iconst_0) : {
                // Operand Stack:
                // ... ->
                // ..., <i>
#ifdef ASM
                uint8_t tmp_0;
                uint64_t tmp_1;
                asm ("addi %0, %1, 1" :"=r"(sp) :"r"(sp));
                asm ("lb  %0, 0(%1)" :"=r"(tmp_0) :"r"((uint64_t)const_pull));
                asm ("add %0, %1, %2" :"=r"(tmp_1) :"r"(sp), "r"((uint64_t)stack));
                asm ("sb  %0, 0(%1)" :  :"r"(tmp_0), "r"(tmp_1));
#else
                // C-code:
                ++sp;
                stack[sp] = const_pull[0];
#endif
                break;
            }

	    case(iadd) : {
#ifdef ASM
                uint64_t tmp_0;
                uint64_t tmp_1;
		uint64_t tmp_2;
		//printf("%u %u %u %u\n", stack[sp], stack[sp - 1], stack[sp - 2], stack[sp - 3]); 
		asm ("lw %0, 0(%1)" :"=r"(tmp_0) :"r"(sp + (uint64_t)stack));
                asm ("addi %0, %1, -4" :"=r"(sp) :"r"(sp));
                asm ("lw  %0, 0(%1)" :"=r"(tmp_1) :"r"(sp + (uint64_t)stack));
                asm ("add %0, %1, %2" :"=r"(tmp_2) :"r"(tmp_0), "r"(tmp_1));
                asm ("sw  %0, 0(%1)" :  :"r"(tmp_2), "r"(sp + (uint64_t)stack));
		//printf("%lu\n", tmp_0);
#else
                // C-code:
                sp -= 4;
                uint64_t res = 0;
	        res = res * 256 + stack[sp - 3] + stack[sp + 1];
		res = res * 256 + stack[sp - 2] + stack[sp + 2];
		res = res * 256 + stack[sp - 1] + stack[sp + 3];
		res = res * 256 + stack[sp] + stack[sp + 4];
		stack[sp] = res % 256;
		res /= 256;
		stack[sp - 1] = res % 256;
		res /= 256;
		stack[sp - 2] = res % 256;
		res /= 256;
		stack[sp - 3] = res % 256;	
#endif
                break;
            }

            case(return_) : {
                // TODO support frame removing
//                asm("ret");
                return;
                break;
            }
            default:
#ifdef LOG_ON
                printf("Unsupported instruction with bc = %d at pc = %d \n", opcode, pc);
#endif
                break;
        }
        ++pc;
    }
};



