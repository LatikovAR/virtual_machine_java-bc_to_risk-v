#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <assert.h>
#include "vm_interface.h"
#include "opcodes.h"
#include "test.h"

int const TEST_COUNT = OPCODE_NUM;

// Execution type and number of iterations (or test ID)
void RunTest(ExecType type, int it) {
    switch (type) {
        case (ONE_TEST) : {
            Test(it);
#ifdef LOG_ON
            printf("test %d executed\n", it);
#endif
            break;
        }
        case (DEFAULT_SEQUENCE) : {
            for(int j = 0; j < it; ++j) {
                for(int i = 0; i < TEST_COUNT; ++i) {
                    Test(i);
                }
            }
#ifdef LOG_ON
            printf("defatult sequence - done\n");
#endif
            break;
        }
        case (RANDOM_SEQUENCE) : {
            time_t t = time(NULL);
            srand(t);
            for(int i = 0; i < TEST_COUNT * it; ++i) {
                int id = rand() % TEST_COUNT;
                Test(id);
            }
#ifdef LOG_ON
            printf("randomized testing - done\n");
#endif
            break;
        }
    }
}


void Test(int id) {
    time_t t = time(NULL);
    srand(t);

    switch(id) {
        case(iconst_0) : {
            uint8_t bytecodes[2] = {iconst_0, return_};
           
            // Initialize VM
            Init();
            uint64_t prev_sp = GetSP();
            // Fill zero const pull by random value
            uint8_t rand_value = (rand());
            const_pull[0] = rand_value;
#ifdef LOG_ON
            printf("Enter to iconst_0 test\n");
            printf("  rand = %d\n", rand_value);
            printf("  sp = %ld\n", prev_sp);
#endif

            // Execute bc
            Execute(bytecodes);
#ifdef LOG_ON
            printf("Exit from execution iconst_0 test\n");
            printf("  stack[sp] = %d\n", stack[GetSP()]);
            printf("  sp = %ld\n", GetSP());
#endif
            assert(GetSP() == prev_sp + 1);
            assert(stack[GetSP()] == rand_value);
            // Create bytecode:
            break; 
        }

	case(iadd) : {
            uint8_t bytecodes[2] = {iadd, return_};
           
            // Initialize VM
            Init();
            uint64_t prev_sp, sp;
	    uint32_t num1 = 0, num2 = 0, res = 0;
	    sp = GetSP();
	    
            stack[sp] = rand() % 128;
	    num1 = (num1 * 256 + stack[sp]);
	    ++sp;
	    stack[sp] = rand();
	    num1 = (num1 * 256 + stack[sp]);
	    ++sp;
	    stack[sp] = rand();
	    num1 = (num1 * 256 + stack[sp]);
	    ++sp;	    
	    stack[sp] = rand();
	    num1 = (num1 * 256 + stack[sp]);
	    ++sp;

	    
	    stack[sp] = rand() % 128; 
	    num2 = (num2 * 256 + stack[sp]);
	    ++sp;
	    stack[sp] = rand();
	    num2 = (num2 * 256 + stack[sp]);
	    ++sp;
	    stack[sp] = rand();
	    num2 = (num2 * 256 + stack[sp]);
	    ++sp;	    
	    stack[sp] = rand();
	    num2 = (num2 * 256 + stack[sp]);
	    ++sp;

	    Move_sp(8);
#ifdef LOG_ON
	    prev_sp = sp;
            printf("Enter to iadd test\n");
            printf("  num1 = %u, num2 = %u\n", num1, num2);
            printf("  sp = %ld\n", prev_sp);
#endif

            // Execute bc
            Execute(bytecodes);
#ifdef LOG_ON
	    sp = GetSP();
	    sp -= 3;
	    res = res * 256 + stack[sp];
	    ++sp;
	    res = res * 256 + stack[sp];
	    ++sp;
	    res = res * 256 + stack[sp];
	    ++sp;
	    res = res * 256 + stack[sp];
            printf("Exit from execution iadd test\n");
            printf("  res = %u\n", res );
            printf("  sp = %ld\n", GetSP());
#endif
            assert(GetSP() == prev_sp - 4);
            assert(res == (num1 + num2));
            // Create bytecode:
            break; 
        }

    default:
#ifdef LOG_ON
        printf("unsupported opcode %d \n", id);
#endif
        break;
    }
}




