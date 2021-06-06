#ifndef clox_vm_h
#define clox_vm_h

#include "chunk.h"
#include "table.h"
#include "value.h"

#define STACK_MAX (256)

typedef struct {
  Chunk* chunk;
  uint8_t* ip; //ip is next instruction
  Value stack[STACK_MAX];
  Value* stackTop;
  Table globals;
  Table strings;  //字符串都存在hash table
  Obj* objects;
} VM;

typedef enum {
  INTERPRET_OK,
  INTERPRET_COMPILE_ERROR,
  INTERPRET_RUNTIME_ERROR
} InterpretResult;

extern VM vm;

void initVM();
void freeVM();
InterpretResult interprt(const char* source);
void push(Value value);
Value pop();

#endif