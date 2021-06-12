learn form https://craftinginterpreters.com/

# A BYTECODE VIRTUAL MACHINE 字节码虚拟机

## chunks of bytecode 
`bytecode`  each instruction has a one-byte operation code
`chunk` 字节码的序列


## 20. Hash Tables

### 数据结构的层面考虑hash table
**数据结构**
成员的类型只要不是基本类型，就用*指针*，这是我观察的结果。为什么这么设计呢??? TODO:

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

## 24. Calls and Functions

**只实现函数调用过程**
```
fun test()
{
	print "test function.";
}

test();
```

**报错信息如下**
```
disassembleChunk() in endCompiler() compiler.c
== test ==
0000    3 OP_CONSTANT         0 'test function.'
0002    | OP_PRINT
0003    4 OP_NIL
0004    | OP_RETURN
disassembleChunk() in endCompiler() compiler.c
== <script> ==
0000    4 OP_CONSTANT         1 '<fn test>'
0002    | OP_DEFINE_GLOBAL    0 'test'
0004    6 OP_GET_LOCAL        2
0006    | OP_CALL             0
0008    | OP_POP
0009    7 OP_NIL
0010    | OP_RETURN

for(;;) in run() vm.c          [<script>]
0000    4 OP_CONSTANT         1 '<fn test>'
          [<script>][<fn test>]
0002    | OP_DEFINE_GLOBAL    0 'test'
          [<script>]
0004    6 OP_GET_LOCAL        2
          [<script>][false]
0006    | OP_CALL             0
Can only functions and classes.
[line 6] in script
```

callValue函数报错提示`Can only functions and classes.` 根据打印信息，报错前参数callee = {type = VAL_BOOL, as = {boolean = false, number = 0, obj = 0x0}}。传参错误,沿着这个思路先检查function's chunk填充的opcode是否正确，特别是跳转的地址。对着网页再检查了一遍代码，没发现到问题，花了一天时间，没有定位到BUG。

第二天早上，考虑从dump出的CHUNK入手，发现`OP_DEFINE_GLOBAL`之后，没有`OP_GET_GLOBAL`。然后查到`namedVariable()`中的bug。修复后正常的IR信息如下：
```
== test ==
0000    3 OP_CONSTANT         0 'test function.'
0002    | OP_PRINT
0003    4 OP_NIL
0004    | OP_RETURN
== <script> ==
0000    4 OP_CONSTANT         1 '<fn test>'
0002    | OP_DEFINE_GLOBAL    0 'test'
0004    6 OP_GET_GLOBAL       2 'test'
0006    | OP_CALL             0
0008    | OP_POP
0009    7 OP_NIL
0010    | OP_RETURN

run() in vm.c          [<script>]
0000    4 OP_CONSTANT         1 '<fn test>'
          [<script>][<fn test>]
0002    | OP_DEFINE_GLOBAL    0 'test'
          [<script>]
0004    6 OP_GET_GLOBAL       2 'test'
          [<script>][<fn test>]
0006    | OP_CALL             0
          [<script>][<fn test>]
0000    3 OP_CONSTANT         0 'test function.'
          [<script>][<fn test>][test function.]
0002    | OP_PRINT
test function.
          [<script>][<fn test>]
0003    4 OP_NIL
          [<script>][<fn test>][nil]
0004    | OP_RETURN
          [<script>][nil]
0008    | OP_POP
          [<script>]
0009    7 OP_NIL
          [<script>][nil]
0010    | OP_RETURN
```

### 给我的启示
编译器的中间表示(IR)非常重要!编译过程的前端和优化过程中每一个代码都是为了生成期望的IR，无论是神经网络编译器NNC还是传统的编译器，都是如此。正确的IR代表着正确的词法分析，语法分析以及正确的优化过程。

应该努力建立这样的能力，即**对编译器中的每行代码，能想到是如何影响IR的；反之，有了IR能想到对应的语法分析和优化过程** 。对于本项目而言，就是看`编译态`compile time生成了怎样的OP_CODE，`运行时`runtime的stack是如何被影响的。

### 实现斐波那契数列——实现参数传递，返回值，递归的功能
```
fun fib(n) {
  if (n < 2) return n;
  return fib(n - 2) + fib(n - 1);
}

var start = clock();
print fib(3);
print clock() - start;
```

dump信息如下：
```
root@debian:~/code/interpreter/build# ./clox fib.clox
== fib ==
0000    2 OP_GET_LOCAL        1
0002    | OP_CONSTANT         0 '2'
0004    | OP_LESS
0005    | OP_JUMP_IF_FALSE    5 -> 17
0008    | OP_POP
0009    | Unknown opcode 255
0010    | Unknown opcode 255
0011    | OP_GET_LOCAL        1
0013    | OP_RETURN
0014    | OP_JUMP            14 -> 20
0017    | OP_POP
0018    | Unknown opcode 255
0019    | Unknown opcode 255
0020    3 OP_GET_GLOBAL       1 'fib'
0022    | OP_GET_LOCAL        1
0024    | OP_CONSTANT         2 '2'
0026    | OP_SUBTRACT
0027    | OP_CALL             1
0029    | OP_GET_GLOBAL       3 'fib'
0031    | OP_GET_LOCAL        1
0033    | OP_CONSTANT         4 '1'
0035    | OP_SUBTRACT
0036    | OP_CALL             1
0038    | OP_ADD
0039    | OP_RETURN
0040    4 OP_NIL
0041    | OP_RETURN
== <script> ==
0000    4 OP_CONSTANT         1 '<fn fib>'
0002    | OP_DEFINE_GLOBAL    0 'fib'
0004    6 OP_GET_GLOBAL       3 'clock'
0006    | OP_CALL             0
0008    | OP_DEFINE_GLOBAL    2 'start'
0010    7 OP_GET_GLOBAL       4 'fib'
0012    | OP_CONSTANT         5 '3'
0014    | OP_CALL             1
0016    | OP_PRINT
0017    8 OP_GET_GLOBAL       6 'clock'
0019    | OP_CALL             0
0021    | OP_GET_GLOBAL       7 'start'
0023    | OP_SUBTRACT
0024    | OP_PRINT
0025   10 OP_NIL
0026    | OP_RETURN

run() in vm.c          [<script>]
0000    4 OP_CONSTANT         1 '<fn fib>'
          [<script>][<fn fib>]
0002    | OP_DEFINE_GLOBAL    0 'fib'
          [<script>]
0004    6 OP_GET_GLOBAL       3 'clock'
          [<script>][<native fn>]
0006    | OP_CALL             0
          [<script>][0.003814]
0008    | OP_DEFINE_GLOBAL    2 'start'
          [<script>]
0010    7 OP_GET_GLOBAL       4 'fib'
          [<script>][<fn fib>]
0012    | OP_CONSTANT         5 '3'
          [<script>][<fn fib>][3]
0014    | OP_CALL             1
          [<script>][<fn fib>][3]
0000    2 OP_GET_LOCAL        1
          [<script>][<fn fib>][3][3]
0002    | OP_CONSTANT         0 '2'
          [<script>][<fn fib>][3][3][2]
0004    | OP_LESS
          [<script>][<fn fib>][3][false]
0005    | OP_JUMP_IF_FALSE    5 -> 17
          [<script>][<fn fib>][3][false]
0017    | OP_POP
          [<script>][<fn fib>][3]
0018    | Unknown opcode 255
          [<script>][<fn fib>][3]
0019    | Unknown opcode 255
          [<script>][<fn fib>][3]
0020    3 OP_GET_GLOBAL       1 'fib'
          [<script>][<fn fib>][3][<fn fib>]
0022    | OP_GET_LOCAL        1
          [<script>][<fn fib>][3][<fn fib>][3]
0024    | OP_CONSTANT         2 '2'
          [<script>][<fn fib>][3][<fn fib>][3][2]
0026    | OP_SUBTRACT
          [<script>][<fn fib>][3][<fn fib>][1]
0027    | OP_CALL             1
          [<script>][<fn fib>][3][<fn fib>][1]
0000    2 OP_GET_LOCAL        1
          [<script>][<fn fib>][3][<fn fib>][1][1]
0002    | OP_CONSTANT         0 '2'
          [<script>][<fn fib>][3][<fn fib>][1][1][2]
0004    | OP_LESS
          [<script>][<fn fib>][3][<fn fib>][1][true]
0005    | OP_JUMP_IF_FALSE    5 -> 17
          [<script>][<fn fib>][3][<fn fib>][1][true]
0008    | OP_POP
          [<script>][<fn fib>][3][<fn fib>][1]
0009    | Unknown opcode 255
          [<script>][<fn fib>][3][<fn fib>][1]
0010    | Unknown opcode 255
          [<script>][<fn fib>][3][<fn fib>][1]
0011    | OP_GET_LOCAL        1
          [<script>][<fn fib>][3][<fn fib>][1][1]
0013    | OP_RETURN
          [<script>][<fn fib>][3][1]
0029    | OP_GET_GLOBAL       3 'fib'
          [<script>][<fn fib>][3][1][<fn fib>]
0031    | OP_GET_LOCAL        1
          [<script>][<fn fib>][3][1][<fn fib>][3]
0033    | OP_CONSTANT         4 '1'
          [<script>][<fn fib>][3][1][<fn fib>][3][1]
0035    | OP_SUBTRACT
          [<script>][<fn fib>][3][1][<fn fib>][2]
0036    | OP_CALL             1
          [<script>][<fn fib>][3][1][<fn fib>][2]
0000    2 OP_GET_LOCAL        1
          [<script>][<fn fib>][3][1][<fn fib>][2][2]
0002    | OP_CONSTANT         0 '2'
          [<script>][<fn fib>][3][1][<fn fib>][2][2][2]
0004    | OP_LESS
          [<script>][<fn fib>][3][1][<fn fib>][2][false]
0005    | OP_JUMP_IF_FALSE    5 -> 17
          [<script>][<fn fib>][3][1][<fn fib>][2][false]
0017    | OP_POP
          [<script>][<fn fib>][3][1][<fn fib>][2]
0018    | Unknown opcode 255
          [<script>][<fn fib>][3][1][<fn fib>][2]
0019    | Unknown opcode 255
          [<script>][<fn fib>][3][1][<fn fib>][2]
0020    3 OP_GET_GLOBAL       1 'fib'
          [<script>][<fn fib>][3][1][<fn fib>][2][<fn fib>]
0022    | OP_GET_LOCAL        1
          [<script>][<fn fib>][3][1][<fn fib>][2][<fn fib>][2]
0024    | OP_CONSTANT         2 '2'
          [<script>][<fn fib>][3][1][<fn fib>][2][<fn fib>][2][2]
0026    | OP_SUBTRACT
          [<script>][<fn fib>][3][1][<fn fib>][2][<fn fib>][0]
0027    | OP_CALL             1
          [<script>][<fn fib>][3][1][<fn fib>][2][<fn fib>][0]
0000    2 OP_GET_LOCAL        1
          [<script>][<fn fib>][3][1][<fn fib>][2][<fn fib>][0][0]
0002    | OP_CONSTANT         0 '2'
          [<script>][<fn fib>][3][1][<fn fib>][2][<fn fib>][0][0][2]
0004    | OP_LESS
          [<script>][<fn fib>][3][1][<fn fib>][2][<fn fib>][0][true]
0005    | OP_JUMP_IF_FALSE    5 -> 17
          [<script>][<fn fib>][3][1][<fn fib>][2][<fn fib>][0][true]
0008    | OP_POP
          [<script>][<fn fib>][3][1][<fn fib>][2][<fn fib>][0]
0009    | Unknown opcode 255
          [<script>][<fn fib>][3][1][<fn fib>][2][<fn fib>][0]
0010    | Unknown opcode 255
          [<script>][<fn fib>][3][1][<fn fib>][2][<fn fib>][0]
0011    | OP_GET_LOCAL        1
          [<script>][<fn fib>][3][1][<fn fib>][2][<fn fib>][0][0]
0013    | OP_RETURN
          [<script>][<fn fib>][3][1][<fn fib>][2][0]
0029    | OP_GET_GLOBAL       3 'fib'
          [<script>][<fn fib>][3][1][<fn fib>][2][0][<fn fib>]
0031    | OP_GET_LOCAL        1
          [<script>][<fn fib>][3][1][<fn fib>][2][0][<fn fib>][2]
0033    | OP_CONSTANT         4 '1'
          [<script>][<fn fib>][3][1][<fn fib>][2][0][<fn fib>][2][1]
0035    | OP_SUBTRACT
          [<script>][<fn fib>][3][1][<fn fib>][2][0][<fn fib>][1]
0036    | OP_CALL             1
          [<script>][<fn fib>][3][1][<fn fib>][2][0][<fn fib>][1]
0000    2 OP_GET_LOCAL        1
          [<script>][<fn fib>][3][1][<fn fib>][2][0][<fn fib>][1][1]
0002    | OP_CONSTANT         0 '2'
          [<script>][<fn fib>][3][1][<fn fib>][2][0][<fn fib>][1][1][2]
0004    | OP_LESS
          [<script>][<fn fib>][3][1][<fn fib>][2][0][<fn fib>][1][true]
0005    | OP_JUMP_IF_FALSE    5 -> 17
          [<script>][<fn fib>][3][1][<fn fib>][2][0][<fn fib>][1][true]
0008    | OP_POP
          [<script>][<fn fib>][3][1][<fn fib>][2][0][<fn fib>][1]
0009    | Unknown opcode 255
          [<script>][<fn fib>][3][1][<fn fib>][2][0][<fn fib>][1]
0010    | Unknown opcode 255
          [<script>][<fn fib>][3][1][<fn fib>][2][0][<fn fib>][1]
0011    | OP_GET_LOCAL        1
          [<script>][<fn fib>][3][1][<fn fib>][2][0][<fn fib>][1][1]
0013    | OP_RETURN
          [<script>][<fn fib>][3][1][<fn fib>][2][0][1]
0038    | OP_ADD
          [<script>][<fn fib>][3][1][<fn fib>][2][1]
0039    | OP_RETURN
          [<script>][<fn fib>][3][1][1]
0038    | OP_ADD
          [<script>][<fn fib>][3][2]
0039    | OP_RETURN
          [<script>][2]
0016    | OP_PRINT
2
          [<script>]
0017    8 OP_GET_GLOBAL       6 'clock'
          [<script>][<native fn>]
0019    | OP_CALL             0
          [<script>][0.013211]
0021    | OP_GET_GLOBAL       7 'start'
          [<script>][0.013211][0.003814]
0023    | OP_SUBTRACT
          [<script>][0.009397]
0024    | OP_PRINT
0.009397
          [<script>]
0025   10 OP_NIL
          [<script>][nil]
0026    | OP_RETURN
```

## 25. Closures 闭包