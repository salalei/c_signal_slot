# 模拟信号槽

## 简介

c_signal_slot采用C语言编写，用以实现类似QT中信号槽的通信机制。

本人是一名固件工程师，在初学QT的时候一直觉得QT的信号槽机制很好用，想着如果能移植在固件上效果应该会很好，因此，我利用工作的空余时间，尝试着用C模拟了信号槽的功能。

## 功能

* 能够定义不同参数类型的信号和槽
* 可以实现一个信号连接多个槽，一个槽连接多个信号，且数目没有限制
* 可以实现一个信号连接另一个信号
* 可以动态内存和静态内存两种方式连接信号槽
* 可以在槽中判断信号的发送者

## 使用方法

1. 修改signal_slot.h中用户接口配置部分，用户需要提供链表、动态内存和日志打印等相关接口，工程中提供的链表参考的是Linux内核的链表
2. 在自己的模块中使用SIGNAL_DEF定义信号，并在定义中调用SIGNAL_SEND，然后在你需要的位置用EMIT发射信号，最后在其头文件用SIGNAL_DECL进行申明，例如:

    demo_signal.c

        //定义一个传递字符串的信号
        SIGNAL_DEF(demo_signal, char *str)
        {
            SIGNAL_SEND(demo_signal, str);
        }

        //假设这个模块初始化的时候需要发射信号
        void init(void)
        {
            EMIT(signal1, "hello world");
        }

    demo_signal.h

        //对信号进行申明
        SIGNAL_DECL(demo_signal, char *);

3. 在另一模块中使用SLOT_DEF定义槽，在槽中执行你想做的事情，并在其头文件用SLOT_DECL进行申明，当你的槽连接了多个信号，你也可以在槽中用IS_SENDER来判断信号的发送者是谁，例如:

    demo_slot.c

        SLOT_DEF(demo_slot, char *str)
        {
            //判断信号是否是demo_signal发送的
            if(IS_SENDER(demo_signal))
                printf("%s", str);//打印接受到的字符串
        }

    demo_slot.h

        SLOT_DECL(demo_slot, char *);

4. 最后用CONNECT连接这个信号和槽，例如:

        CONNECT(demo_signal, demo_slot);

可以参考examples的例子，每个信号对应两个槽，每个槽对应两个信号，并且信号1和信号2也是相连。模块设计的时候应该给出该模块会发射的信号和该模块接受其他信号的槽。

## 注意事项

* 该工程的C标准需要为C99，编译器需要使用GCC
* 使用CONNECT连接的时候需要注意信号和槽的参数长度和类型一致，编译的时候是检查不出这个问题的
* CONNECT和DISCONNECT的时间复杂度为O(n)，所以当连接了多个信号或槽的时候使用CONNECT或DISCONNECT效率将会变低
* CONNECT_STATIC将会在每个使用它的地方静态定义一个节点，因此程序中如果有一个信号或槽会在很多地方需要连接和断开连接(虽然不太可能)，最好使用动态内存的方式
