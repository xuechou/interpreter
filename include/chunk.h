#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.h"
#include "value.h"

typedef enum {
  OP_CONSTANT,
  OP_RETURN,
} OpCode;

typedef struct {
  int count;//动态数组的大小
  int capacity; //动态数组分配的空间大小, count<= capacity
  uint8_t* code; //TODO: 为什么是指向uint8的指针?
  int* lines;
  ValueArray constants;
} Chunk;

void initChunk(Chunk* chunk);
void writeChunk(Chunk* chunk, uint8_t byte, int line);
int addConstant(Chunk* chunk, Value value);
void freeChunk(Chunk* chunk);

#endif