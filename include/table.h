#ifndef clox_table_h
#define clox_table_h

#include "common.h"
#include "value.h"

typedef struct 
{
	ObjString* key;  //??? why not just string??
	Value value;
} Entry; //一个条目就是一组键值对

typedef struct 
{
	int count;
	int capacity;
	Entry* entries;
} Table;  //数据结构与动态数组相同

void initTable(Table* table);
void freeTable(Table* table);
bool tableGet(Table* table, ObjString* key, Value* value); // get value in hash table by key
bool tableSet(Table* table, ObjString* key, Value value); // return true is add key-value, false is modify value
bool tableDelete(Table* table, ObjString* key);// delete key-value from hash table
void tableAddAll(Table* from, Table* to); // copy hash table ?? do what
ObjString* tableFindString(Table* table, const char* chars, int length, uint32_t hash); // 还提供直接通过c style的字符串来查找的接口
void tableRemoveWhite(Table* table);
void markTable(Table* table);

#endif