# 24. Calls and Functions
[ref](https://craftinginterpreters.com/calls-and-functions.html)

## 24.1 Function Objects
本章之前，局部变量和临时变量都保存在虚拟机唯一的堆栈中。从虚拟机的视角，函数是什么呢？函数是可以执行的整体，也就是一块字节码。可以考虑把包含函数在内的所有程序，编译成一整块字节码。在一整块字节码中，每个函数可以用一个指针来表示，指针指向函数第一条指令在整块字节码中的位置。
但是，本文设计的VM，每个函数拥有自己的chunk，这样模型更清晰。

LOX的函数为`一等函数`,所以函数的行为要和LOX中的对象一致。个人理解，first class就是指函数可以作为函数参数，函数返回值，要能够打印函数，就像int等基本数据类型。
所以先建立struct ObjFunction,注意ObjFunction的第一个字段是Obj obj，也就是之前提到的`struct inheritance`。这样指针就可以在struct ObjFunction与struct Obj之间类型转换。个人理解，Obj就像是ObjFunction的基类，ObjFunction就可以获取基类的数据成员。

建立的新的组合数据类型struct ObjFuction，就要添加对应的"构造函数"和"析构函数"，分别来初始化和资源释放。

## 24.2 Compiling to Function Object  把谁编译为ObjFucntion，在那个阶段?
函数内部的代码编译到ObjFunction's chunk，也就是遇到函数声明切换到函数对应的chunk,当函数编译完成则要切回到之前的chunk。为了把所有函数之外的代码编译到一个隐式的函数,建立的top-level函数。


## 24.3 Call Frames 栈帧
编译时确定函数的栈帧，即函数内部使用局部和临时变量的相对顺序。

