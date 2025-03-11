#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *head = malloc(sizeof(struct list_head));
    if (!head)
        return NULL;
    INIT_LIST_HEAD(head);
    return head;
}

/* Free all storage used by queue */
void q_free(struct list_head *head)
{
    if (!head)
        return;

    while (!list_empty(head)) {
        struct list_head *node = head->next;
        element_t *element = list_entry(node, element_t, list);
        list_del(node);
        q_release_element(element);
    }

    free(head);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head || !s)
        return false;

    element_t *new_node = malloc(sizeof(element_t));

    if (!new_node)
        return false;

    new_node->value = strdup(s);
    if (!new_node->value) {
        free(new_node);
        return false;
    }

    list_add(&new_node->list, head);
    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head || !s)
        return false;

    element_t *new_node = malloc(sizeof(element_t));
    if (!new_node)
        return false;

    new_node->value = strdup(s);
    if (!new_node->value) {
        free(new_node);
        return false;
    }

    list_add_tail(&new_node->list, head);
    return true;
}

/* Remove an element from head of queue */
/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;

    struct list_head *first = head->next;
    element_t *elem = list_entry(first, element_t, list);

    if (sp && bufsize > 0) {
        strncpy(sp, elem->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }

    list_del(first);
    return elem;
}

/* Remove an element from tail of queue */
/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;

    struct list_head *last = head->prev;
    element_t *elem = list_entry(last, element_t, list);

    if (sp && bufsize > 0) {
        strncpy(sp, elem->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }

    list_del(last);
    return elem;
}

/* Return number of elements in queue */
/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head)
        return 0;

    int count = 0;
    struct list_head *current;
    list_for_each(current, head)
        count++;
    return count;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    if (!head || list_empty(head) || list_is_singular(head))
        return false;

    struct list_head *slow = head->next;
    struct list_head *fast = head->next->next;

    while (fast != head && fast->next != head) {
        slow = slow->next;
        fast = fast->next->next;
    }

    element_t *mid = list_entry(slow, element_t, list);
    list_del(slow);
    q_release_element(mid);

    return true;
}

/* Delete all nodes that have duplicate string */
/* Delete all nodes that have duplicate string */
/* Delete all nodes that have duplicate string */

bool q_delete_dup(struct list_head *head)
{
    if (!head || list_empty(head))
        return false;

    struct list_head *node = head->next;

    while (node != head && node->next != head) {
        const element_t *curr_elem = list_entry(node, element_t, list);
        const element_t *next_elem = list_entry(node->next, element_t, list);

        if (!strcmp(curr_elem->value, next_elem->value)) {
            char *dup_val = strdup(curr_elem->value);

            while (node != head) {
                element_t *check_elem = list_entry(node, element_t, list);
                if (!strcmp(check_elem->value, dup_val)) {
                    struct list_head *tmp = node->next;
                    list_del(node);
                    q_release_element(check_elem);
                    node = tmp;
                } else
                    break;
            }

            free(dup_val);
        } else
            node = node->next;
    }

    return true;
}


/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    if (!head || list_empty(head) || list_is_singular(head))
        return;

    struct list_head *curr = head;

    while (curr->next != head && curr->next->next != head) {
        struct list_head *first = curr->next;
        struct list_head *second = first->next;

        list_del(first);
        list_del(second);

        list_add(second, curr);
        list_add(first, second);

        curr = first;
    }
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (!head)
        return;

    struct list_head *node = head;
    do {
        struct list_head *tmp = node->next;

        node->next = node->prev;
        node->prev = tmp;

        node = tmp;
    } while (node != head);
}

/* Reverse the nodes of the list k at a time */
/* Reverse the nodes of the list k at a time */
/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    if (!head || k <= 1)
        return;

    int count = q_size(head);
    if (count < k)
        return;

    struct list_head *cur = head;
    for (int i = 0; i < count / k; i++) {
        struct list_head temp_head;
        INIT_LIST_HEAD(&temp_head);

        for (int j = 0; j < k; j++) {
            struct list_head *node = cur->next;
            list_del(node);
            list_add(node, &temp_head);
        }
        list_splice(&temp_head, cur);

        for (int j = 0; j < k; j++)
            cur = cur->next;
    }
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend) {}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}