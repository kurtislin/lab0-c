#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

void merge_sort(struct list_head *head, bool descend);
void split_list(struct list_head *head,
                struct list_head *list1,
                struct list_head *list2);
void merge_lists(struct list_head *result,
                 struct list_head *list1,
                 struct list_head *list2,
                 bool descend);

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
void q_sort(struct list_head *head, bool descend)
{
    if (!head)
        return;

    merge_sort(head, descend);
}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    if (!head || list_empty(head) || list_is_singular(head))
        return q_size(head);  // 返回處理後的隊列大小

    // 使用單調棧的思想，反向處理列表
    struct list_head result;
    INIT_LIST_HEAD(&result);

    struct list_head *current = head->prev;  // 從尾部開始

    // 從尾部向頭部處理
    while (current != head) {
        struct list_head *prev =
            current->prev;  // 保存前一個節點，因為當前節點可能被刪除
        element_t *elem = list_entry(current, element_t, list);

        // 如果結果鏈表為空，或者當前值小於等於結果鏈表的第一個節點的值，則保留
        if (list_empty(&result) ||
            strcmp(elem->value,
                   list_entry(result.next, element_t, list)->value) <= 0) {
            list_del(current);
            list_add(current, &result);  // 加入結果鏈表頭部
        } else {
            // 否則刪除節點
            list_del(current);
            q_release_element(elem);
        }

        current = prev;
    }

    // 將結果鏈表合併回原始頭部
    list_splice(&result, head);

    return q_size(head);  // 返回處理後的隊列大小
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    if (!head || list_empty(head) || list_is_singular(head))
        return q_size(head);

    // 使用單調棧的思想，反向處理列表
    struct list_head result;
    INIT_LIST_HEAD(&result);

    struct list_head *current = head->prev;  // 從尾部開始

    // 從尾部向頭部處理
    while (current != head) {
        struct list_head *prev =
            current->prev;  // 保存前一個節點，因為當前節點可能被刪除
        element_t *elem = list_entry(current, element_t, list);

        // 如果結果鏈表為空，或者當前值大於等於結果鏈表的第一個節點的值，則保留
        if (list_empty(&result) ||
            strcmp(elem->value,
                   list_entry(result.next, element_t, list)->value) >= 0) {
            list_del(current);
            list_add(current, &result);  // 加入結果鏈表頭部
        } else {
            // 否則刪除節點
            list_del(current);
            q_release_element(elem);
        }

        current = prev;
    }

    // 將結果鏈表合併回原始頭部
    list_splice(&result, head);

    return q_size(head);
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    if (!head || list_empty(head))
        return 0;

    // If there's only one queue, return its size
    if (list_is_singular(head)) {
        queue_contex_t *single_queue =
            list_entry(head->next, queue_contex_t, chain);
        return q_size(single_queue->q);
    }

    // Get the first queue - this will hold the final result
    queue_contex_t *first_queue = list_entry(head->next, queue_contex_t, chain);

    // Collect all elements from all queues into the first queue
    struct list_head *current = head->next->next;
    while (current != head) {
        queue_contex_t *queue_ctx = list_entry(current, queue_contex_t, chain);

        // Move all elements from current queue to first queue
        if (!list_empty(queue_ctx->q)) {
            list_splice_tail_init(queue_ctx->q, first_queue->q);
        }

        current = current->next;
    }

    // Sort the merged queue
    q_sort(first_queue->q, descend);

    return q_size(first_queue->q);
}

void split_list(struct list_head *head,
                struct list_head *list1,
                struct list_head *list2)
{
    /* Initialize the two sublists */
    INIT_LIST_HEAD(list1);
    INIT_LIST_HEAD(list2);

    /* Empty list case */
    if (list_empty(head))
        return;

    /* Single element case - move to list1 */
    if (list_is_singular(head)) {
        struct list_head *elem = head->next;
        list_del(elem);
        list_add(elem, list1);
        return;
    }

    struct list_head *slow = head->next;
    struct list_head *fast = head->next->next;

    /* Move fast pointer two steps and slow pointer one step */
    while (fast != head && fast->next != head) {
        slow = slow->next;
        fast = fast->next->next;
    }

    /* Create a temporary list to hold the second half */
    struct list_head temp_list;
    INIT_LIST_HEAD(&temp_list);

    /* Cut the list at slow position - this splits the list into two parts */
    list_cut_position(&temp_list, head, slow);

    list_splice_init(head, list2);

    list_splice_init(&temp_list, list1);
}

/* Merge two sorted lists */
void merge_lists(struct list_head *result,
                 struct list_head *list1,
                 struct list_head *list2,
                 bool descend)
{
    INIT_LIST_HEAD(result);

    /* Process elements until one list is empty */
    while (!list_empty(list1) && !list_empty(list2)) {
        const element_t *e1 = list_entry(list1->next, element_t, list);
        const element_t *e2 = list_entry(list2->next, element_t, list);
        struct list_head *node;

        /* Compare strings and pick the appropriate node */
        int cmp = strcmp(e1->value, e2->value);
        if ((!descend && cmp <= 0) || (descend && (cmp >= 0))) {
            node = list1->next;
            list_del(node);
        } else {
            node = list2->next;
            list_del(node);
        }
        /* Add the node to the end of the result list */
        list_add_tail(node, result);
    }

    /* Add any remaining nodes from list1 using list_splice_tail */
    if (!list_empty(list1))
        list_splice_tail(list1, result);

    /* Add any remaining nodes from list2 using list_splice_tail */
    if (!list_empty(list2))
        list_splice_tail(list2, result);
}

/* Recursive merge sort function */
void merge_sort(struct list_head *head, bool descend)
{
    /* Base case: empty or single node list is already sorted */
    if (list_empty(head) || list_is_singular(head))
        return;

    /* Create temporary lists for splitting */
    struct list_head list1, list2;

    /* Split the list into two parts */
    split_list(head, &list1, &list2);

    /* Recursively sort both sublists */
    merge_sort(&list1, descend);
    merge_sort(&list2, descend);

    /* Merge the sorted sublists */
    merge_lists(head, &list1, &list2, descend);
}