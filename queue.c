#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */


/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *ret = malloc(sizeof(struct list_head));
    if (!ret)
        return NULL;

    INIT_LIST_HEAD(ret);
    return ret;
}

/* Free all storage used by queue */
void q_free(struct list_head *head)
{
    if (!head)
        return;

    element_t *current, *next;
    list_for_each_entry_safe (current, next, head, list) {
        q_release_element(current);
    }
    free(head);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head)
        return false;

    element_t *ele = malloc(sizeof(element_t));

    if (!ele)
        return false;

    ele->value = strdup(s);
    if (!ele->value) {
        free(ele);
        return false;
    }
    list_add(&ele->list, head);

    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    return q_insert_head(head->prev, s);
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;

    element_t *ele = list_first_entry(head, element_t, list);
    list_del(&ele->list);

    if (sp && bufsize) {
        strncpy(sp, ele->value, bufsize);
        sp[bufsize - 1] = '\0';
    }
    return ele;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    return q_remove_head(head->prev->prev, sp, bufsize);
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head)
        return 0;

    int len = 0;
    struct list_head *li;

    list_for_each (li, head)
        len++;
    return len;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    if (!head || list_empty(head))
        return false;

    if (!(head->next))
        return false;

    struct list_head *first = head->next, *end = head->prev;
    while (first != end && first->prev != end) {
        first = first->next;
        end = end->prev;
    }

    element_t *ele = list_entry(first, element_t, list);
    list_del(first);
    q_release_element(ele);

    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    if (!head || list_empty(head))
        return false;

    element_t *cur, *next;
    bool cur_dup = false;

    list_for_each_entry_safe (cur, next, head, list) {
        bool next_dup = &next->list != head && !strcmp(cur->value, next->value);
        if (next_dup || cur_dup) {
            list_del(&cur->list);
            q_release_element(cur);
        }
        cur_dup = next_dup;
    }

    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
    q_reverseK(head, 2);
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    struct list_head *cur, *next;
    list_for_each_safe (cur, next, head) {
        list_move(cur, head);
    }
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/

    if (!head || list_empty(head) || k <= 0)
        return;

    struct list_head *start, *tmp, *next, *ih, *it;
    int cnt = 0;
    list_for_each_safe (ih, it, head) {
        if (!cnt)
            start = ih->prev;
        if (cnt++ == k - 1) {
            tmp = start->next;
            while (cnt) {
                cnt--;
                next = tmp->next;
                list_move(tmp, start);
                tmp = next;
            }
        }
    }
}


/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    if (!head || list_empty(head))
        return 0;

    struct list_head *cur, *safe;
    element_t *max_ele = list_entry(head->next, element_t, list);
    list_for_each_safe (cur, safe, head) {
        element_t *cur_ele = list_entry(cur, element_t, list);
        if (strcmp(cur_ele->value, max_ele->value) >= 0)
            max_ele = cur_ele;
        else {
            list_del(cur);
            q_release_element(cur_ele);
        }
    }
    return q_size(head);
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    if (!head || list_empty(head))
        return 0;

    q_reverse(head);
    q_ascend(head);
    q_reverse(head);
    return q_size(head);
}

/* Merge two sorted queue */
void merge_two_list(struct list_head *L1, struct list_head *L2, bool descend)
{
    struct list_head *p1 = L1->next, *p2 = L2->next;

    if (list_empty(L1)) {
        list_splice_init(L2, L1);
        return;
    }
    /* assume ascending */
    while (!list_empty(L2)) {
        element_t *ele1 = list_entry(p1, element_t, list);
        element_t *ele2 = list_entry(p2, element_t, list);
        int cmp_result = strcmp(ele1->value, ele2->value);

        if ((!descend && cmp_result < 0) || (descend && cmp_result > 0)) {
            p1 = p1->next;
        } else {
            struct list_head *tmp = p2->next;
            list_del(p2);
            list_move(p2, p1->prev);
            p2 = tmp;
        }

        if (p1 == L1) {
            /* splice remain L2 to L1 tail */
            list_splice_tail_init(L2, L1);
            break;
        }
    }
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    if (!head || list_empty(head))
        return 0;

    struct list_head *cur, *safe;
    queue_contex_t *first = list_first_entry(head, queue_contex_t, chain);

    list_for_each_safe (cur, safe, head) {
        struct list_head *L2 = list_entry(cur, queue_contex_t, chain)->q;
        if (first->q == L2)
            continue;
        merge_two_list(first->q, L2, descend);
    }
    return q_size(list_entry(head->next, queue_contex_t, chain)->q);
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend)
{
    if (!head || list_empty(head) || list_is_singular(head))
        return;

    /* find mid point, which is first point */
    struct list_head *first = head->next, *end = head->prev;
    while (first != end && first->prev != end) {
        first = first->next;
        end = end->prev;
    }

    /* divide and conquer */
    LIST_HEAD(left);
    list_cut_position(&left, head, first->prev);

    q_sort(head, descend);
    q_sort(&left, descend);

    /* merge two list */
    if (!list_empty(&left))
        merge_two_list(head, &left, descend);
}
