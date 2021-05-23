#include <stdio.h>
#include "common.h"
#include "debug.h"
#include "vm.h"

VM vm;

static void resetStack()
{
	vm.stackTop = vm.stack;
}

void push(Value value)
{
	*vm.stackTop = value;
	vm.stackTop++;
}

Value pop()
{
	vm.stackTop--;
	return *vm.stackTop;
}

void initVM()
{
	resetStack();
}

void freeVM()
{
}

static InterpretResult run()
{
#define READ_BYTE() (*vm.ip++) //*(vm.ip++) or (*vm.ip)++ or *vm.ip vm.ip++
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()]) //values的位置也是ip表示??
#define BINARY_OP(op) \
	do { \
	  double b = pop(); \
	  double a = pop(); \
	  push(a op b); \
	}while(false)

	for (;;)
	{
#ifdef DEBUG_TRACE_EXECUTION
		printf("          ");
		for(Value* slot=vm.stack;slot<vm.stackTop;slot++)
		{
			printf("[");
			printValue(*slot);
			printf("]");
		}
		printf("\n");
		disassembleInstruction(vm.chunk,
							   (int)(vm.ip - vm.chunk->code)); //TODO:?????
#endif
		uint8_t instruction;
		switch (instruction = READ_BYTE())
		{
		case OP_CONSTANT:
		{
			Value constant = READ_CONSTANT();
			push(constant);
			break;
		}
		case OP_ADD:
		{
			BINARY_OP(+);
			break;
		}
		case OP_SUBTRACT:
		{
			BINARY_OP(-);
			break;
		}

		case OP_MULTIPLY:
		{
			BINARY_OP(*);
			break;
		}

		case OP_DIVIDE:
		{
			BINARY_OP(/);
			break;
		}
		case OP_NEGATE:
		{
			push(-pop());//??? push after pop,do what ?
			break;
		}
		case OP_RETURN:
		{
			printValue(pop());
			printf("\n");
			return INTERPRET_OK;
		}
		}
	}
#undef READ_BYTE
#undef READ_CONSTANT
#undef BINARY_OP
}

InterpretResult interprte(Chunk *chunk)
{
	vm.chunk = chunk;
	vm.ip = vm.chunk->code;

	return run();
}
