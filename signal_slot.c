/**
 * @file signal_slot.c
 * @author salalei
 * @brief 用C模拟的信号槽机制
 * @version V0.0.1
 * @date 2021-08-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "signal_slot.h"

/**
 * @brief 连接信号槽
 * 
 * @param signal_head 信号的链头
 * @param head 被连接的信号或槽的链头
 * @param node 新的信号或槽的节点
 * @return int 成功返回0，若已有节点存在则返回-1
 */
int _connect(struct _signal_slot_node *signal_head, struct _signal_slot_node *head, struct _signal_slot_node *new_node)
{
    struct _signal_slot_node *node;

    USER_LIST_FOR_EACH_ENTRY(&head->self_node, node, self_node)
    {
        if (node->signal == signal_head)
            return -1;
    }
    new_node->signal = signal_head;
    new_node->func = head->func;
    USER_LIST_ADD(&head->self_node, &new_node->self_node);
    USER_LIST_ADD(&signal_head->signal_node, &new_node->signal_node);
    return 0;
}

/**
 * @brief 将指定信号和槽的连接断开
 * 
 * @param signal_head 信号的链头
 * @param slot_head 槽的链头
 * @return struct _signal_slot_node* 成功返回被删除的节点，失败返回NULL
 */
struct _signal_slot_node *_disconnect(struct _signal_slot_node *signal_head, struct _signal_slot_node *slot_head)
{
    struct _signal_slot_node *node;

    USER_LIST_FOR_EACH_ENTRY(&slot_head->self_node, node, self_node)
    {
        if (node->signal == signal_head)
        {
            USER_LIST_DEL(&node->signal_node);
            USER_LIST_DEL(&node->self_node);
            return node;
        }
    }
    return NULL;
}