learn form https://craftinginterpreters.com/

# A BYTECODE VIRTUAL MACHINE 字节码虚拟机

## chunks of bytecode 
`bytecode`  each instruction has a one-byte operation code
`chunk` 字节码的序列


## 20. Hash Tables

### 数据结构的层面考虑hash table
**数据结构**
成员的类型只要不是基本类型，就用*指针*，这是我观察的结果。为什么这么设计呢？

**支持的操作**
基本的4种*增删改查*，区别主要是冲突发生的区别

**哈希算法**

**如何处理冲突**

**处理冲突方法对搜索的影响**
删除的键值对设置为墓碑，key为Null，但是value为true
```c
// Place a tombstone in the entry.
  entry->key = NULL;
  entry->value = BOOL_VAL(true);
```
### hash table 和 符号表的关系

? hash table 来存放什么数据