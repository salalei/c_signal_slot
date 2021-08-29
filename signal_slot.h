/**
 * @file signal_slot.h
 * @author salalei
 * @brief 用C模拟的信号槽机制
 * @version V0.0.1
 * @date 2021-08-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef __SIGNAL_SLOT_H__
#define __SIGNAL_SLOT_H__

#ifdef __cplusplus
extern "C" {
#endif

//用户接口配置部分

//用户需要包含的头文件
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
typedef struct list_node user_list_node_t;                                             //用户提供的链表类型
#define USER_LIST_HEAD_INIT(x)                    LIST_HEAD_INIT(x)                    //用户提供链头初始化宏
#define USER_LIST_FOR_EACH_ENTRY(head, node, mem) list_for_each_entry(head, node, mem) //用户提供的遍历链表宏
#define USER_LIST_ADD(head, node)                 list_add_tail(head, node)            //用户提供的添加节点的函数
#define USER_LIST_DEL(node)                       list_del(node)                       //用户提供的删除节点的函数
#define USER_MALLOC                               malloc                               //用户提供的申请内存的函数
#define USER_FREE                                 free                                 //用户提供的释放内存的函数
#define USER_WARN                                 printf                               //用户提供的打印信息的函数

/**
 * @brief 信号或槽的节点(内部使用)
 */
struct _signal_slot_node
{
    user_list_node_t self_node;   //连接自身的节点
    user_list_node_t signal_node; //连接信号的节点
    void *func;                   //执行的函数
    void *signal;                 //归属于哪个信号
};

int _connect(struct _signal_slot_node *, struct _signal_slot_node *, struct _signal_slot_node *);
struct _signal_slot_node *_disconnect(struct _signal_slot_node *, struct _signal_slot_node *);

#define _SIGNAL_SLOT_DEF(name, ...) \
    void name(struct _signal_slot_node *sender, ##__VA_ARGS__); \
    struct _signal_slot_node name##_head = { \
        .self_node = USER_LIST_HEAD_INIT(name##_head.self_node), \
        .signal_node = USER_LIST_HEAD_INIT(name##_head.signal_node), \
        .func = name, \
        .signal = NULL, \
    }; \
    void name(struct _signal_slot_node *sender, ##__VA_ARGS__)

#define _SIGNAL_SLOT_DECL(name, ...) \
    void name(struct _signal_slot_node *sender, ##__VA_ARGS__); \
    extern struct _signal_slot_node name##_head

/**
 * @brief 定义一个信号
 * 
 * @param name 信号的名称
 * @param ... 这个信号的参数
 */
#define SIGNAL_DEF(name, ...) \
    typedef void (*name##_t)(struct _signal_slot_node * sender, ##__VA_ARGS__); \
    _SIGNAL_SLOT_DEF(name, ##__VA_ARGS__)

/**
 * @brief 发送信号(用在定义信号的函数里面)
 * 
 * @param name 信号的名称
 * @param ... 要发送的参数
 */
#define SIGNAL_SEND(name, ...) \
    struct _signal_slot_node *node; \
    name##_t func; \
    USER_LIST_FOR_EACH_ENTRY(&name##_head.signal_node, node, signal_node) \
    { \
        func = (name##_t)node->func; \
        func(&name##_head, ##__VA_ARGS__); \
    }

/**
 * @brief 将已定义的信号对外申明
 * 
 * @param name 信号的名称
 * @param ... 这个信号的参数
 */
#define SIGNAL_DECL(name, ...) \
    _SIGNAL_SLOT_DECL(name, ##__VA_ARGS__)

/**
 * @brief 定义一个槽
 * 
 * @param name 槽的名称
 * ... 这个槽的参数
 */
#define SLOT_DEF(name, ...) \
    _SIGNAL_SLOT_DEF(name, ##__VA_ARGS__)

/**
 * @brief 将已定义的槽对外申明
 * 
 * @param name 信号的名称
 * @param ... 这个槽的参数
 */
#define SLOT_DECL(name, ...) \
    _SIGNAL_SLOT_DECL(name, ##__VA_ARGS__)

/**
 * @brief 发射一个信号
 * 
 * @param name 需要发射的信号名称
 * @param ... 需要发送的数据
 */
#define EMIT(name, ...) \
    name(&name##_head, ##__VA_ARGS__)

/**
 * @brief 在槽函数中判断是谁发送的信号
 * 
 * @param name 需要判断的信号名称
 * @return 如果是name发射的信号返回非0，否则返回0
 */
#define IS_SENDER(name) \
    (sender == &name##_head)

/**
 * @brief 将指定信号与指定槽连接(使用动态内存方式)
 * 
 * @param signal_name 需要连接的信号名称
 * @param slot_name 需要连接的槽名称
 */
#define CONNECT(signal_name, slot_name) \
    do \
    { \
        struct _signal_slot_node *node; \
        node = USER_MALLOC(sizeof(struct _signal_slot_node)); \
        if (node == NULL) \
            USER_WARN("Failed to malloc %s node\n", #slot_name); \
        else \
        { \
            if (_connect(&signal_name##_head, &slot_name##_head, node)) \
            { \
                USER_WARN("%s has been connected to the %s\n", #slot_name, #signal_name); \
                USER_FREE(node); \
            } \
        } \
    } \
    while (0)

/**
 * @brief 将指定信号与指定槽断开连接(使用动态内存方式)
 * 
 * @param signal_name 需要连接的信号名称
 * @param slot_name 需要连接的槽名称
 */
#define DISCONNECT(signal_name, slot_name) \
    do \
    { \
        struct _signal_slot_node *node; \
        node = _disconnect(&signal_name##_head, &slot_name##_head); \
        if (node) \
            USER_FREE(node); \
        else \
            USER_WARN("The %s and %s have been disconnected\n", #signal_name, #slot_name); \
    } \
    while (0)

/**
 * @brief 将指定信号与指定槽连接(使用静态内存方式)
 * 
 * @param signal_name 需要连接的信号名称
 * @param slot_name 需要连接的槽名称
 */
#define CONNECT_STATIC(signal_name, slot_name) \
    do \
    { \
        static struct _signal_slot_node node; \
        if (_connect(&signal_name##_head, &slot_name##_head, &node)) \
            USER_WARN("%s has been connected to the %s\n", #slot_name, #signal_name); \
    } \
    while (0)

/**
 * @brief 将指定信号与指定槽断开连接(使用静态内存方式)
 * 
 * @param signal_name 需要连接的信号名称
 * @param slot_name 需要连接的槽名称
 */
#define DISCONNECT_STATIC(signal_name, slot_name) \
    do \
    { \
        if (_disconnect(&signal_name##_head, &slot_name##_head) == NULL) \
            USER_WARN("The %s and %s have been disconnected\n", #signal_name, #slot_name); \
    } \
    while (0)

#ifdef __cplusplus
}
#endif

#endif