/**
 * @file list.h
 * @author hz010153 (yunlei.zhou@3d-scantech.com)
 * @brief 简单的双向链表
 * @version V1.0.0
 * @date 2021-08-29
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef __LIST_H__
#define __LIST_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define MEMBER_OFFSET(type, mem)   ((size_t) & (((type *)0)->mem))
#define CONTAINER_OF(p, type, mem) ((type *)((size_t)(p)-MEMBER_OFFSET(type, mem)))

struct list_node
{
    struct list_node *prev;
    struct list_node *next;
};

#define LIST_HEAD_INIT(head) \
    { \
        .prev = &(head), \
        .next = &(head) \
    }

/**
 * @brief 在链表尾部添加一个节点
 * 
 * @param head 指向链头的指针
 * @param node 指向新节点的指针
 */
static inline void list_add_tail(struct list_node *head, struct list_node *node)
{
    node->next = head;
    node->prev = head->prev;
    head->prev->next = node;
    head->prev = node;
}

/**
 * @brief 删除指定的节点
 * 
 * @param node 指向要移除的节点的指针
 */
static inline void list_del(struct list_node *node)
{
    node->prev->next = node->next;
    node->next->prev = node->prev;
    node->prev = node;
    node->next = node;
}

/**
 * @brief 正向遍历链表
 * 
 * @param head 指向链头的指针
 * @param node 返回的当前节点
 * @param mem 链表在该结构体中的成员名
 */
#define list_for_each_entry(head, node, mem) \
    for ((node) = CONTAINER_OF((head)->next, typeof(*node), mem); \
         &(node)->mem != (head); \
         (node) = CONTAINER_OF((node)->mem.next, typeof(*node), mem))

#ifdef __cplusplus
}
#endif

#endif