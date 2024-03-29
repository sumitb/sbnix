#ifndef _LIST_H
#define _LIST_H

#include <sys/defs.h>

/*
 * Simple doubly linked list implementation.
 *
 * Some of the internal functions ("__xxx") are useful when
 * manipulating whole lists rather than single entries, as
 * sometimes we already know the next/prev entries and we can
 * generate better code by using them directly rather than
 * using the generic single-entry routines.
 */

#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name) \
        struct list_head name = LIST_HEAD_INIT(name)

static inline void INIT_LIST_HEAD(struct list_head *list)
{
        list->next = list;
        list->prev = list;
}

/*
 * Insert a new entry between two known consecutive entries.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline void __list_add(struct list_head *new,
                              struct list_head *prev,
                              struct list_head *next)
{
        next->prev = new;
        new->next = next;
        new->prev = prev;
        prev->next = new;
}

/**
 * list_add - add a new entry
 * @new: new entry to be added
 * @head: list head to add it after
 *
 * Insert a new entry after the specified head.
 * This is good for implementing stacks.
 */
static inline void list_add(struct list_head *new, struct list_head *head)
{
        __list_add(new, head, head->next);
}


/**
 * list_add_tail - add a new entry
 * @new: new entry to be added
 * @head: list head to add it before
 *
 * Insert a new entry before the specified head.
 * This is useful for implementing queues.
 */
static inline void list_add_tail(struct list_head *new, struct list_head *head)
{
        __list_add(new, head->prev, head);
}

/*
 * Delete a list entry by making the prev/next entries
 * point to each other.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline void __list_del(struct list_head * prev, struct list_head * next)
{
        next->prev = prev;
        prev->next = next;
}

/**
 * list_del - deletes entry from list.
 * @entry: the element to delete from the list.
 * Note: list_empty() on entry does not return true after this, the entry is
 * in an undefined state.
 */
static inline void __list_del_entry(struct list_head *entry)
{
        __list_del(entry->prev, entry->next);
}

static inline void list_del(struct list_head *entry)
{
        __list_del(entry->prev, entry->next);
        entry->next = (void *) 0;
        entry->prev = (void *) 0;
}


/**
 * list_del_init - deletes entry from list and reinitialize it.
 * @entry: the element to delete from the list.
 */
static inline void list_del_init(struct list_head *entry)
{
        __list_del_entry(entry);
        INIT_LIST_HEAD(entry);
}

/**
 * list_move - delete from one list and add as another's head
 * @list: the entry to move
 * @head: the head that will precede our entry
 */
static inline void list_move(struct list_head *list, struct list_head *head)
{
        __list_del_entry(list);
        list_add(list, head);
}

/**
 * list_move_tail - delete from one list and add as another's tail
 * @list: the entry to move
 * @head: the head that will follow our entry
 */
static inline void list_move_tail(struct list_head *list,
                          struct list_head *head)
{
        __list_del_entry(list);
        list_add_tail(list, head);
}

/**
 * list_empty - tests whether a list is empty
 * @head: the list to test.
 */
static inline int list_empty(const struct list_head *head)
{
        return head->next == head;
}

/**
 * list_is_singular - tests whether a list has just one entry.
 * @head: the list to test.
 */
static inline int list_is_singular(const struct list_head *head)
{
        return !list_empty(head) && (head->next == head->prev);
}

/**
 * container_of - cast a member of a structure out to the containing structure
 * @ptr:    the pointer to the member.
 * @type:  the type of the container struct this is embedded in.
 * @member:   the name of the member within the struct.
 *
 */
#define container_of(ptr, type, member) ({          \
            const __typeof__( ((type *)0)->member ) *__mptr = (ptr);    \
            (type *)( (char *)__mptr - offsetof(type,member) );})

/**
 * list_entry - get the struct for this entry
 * @ptr:    the &struct list_head pointer.
 * @type:  the type of the struct this is embedded in.
 * @member:   the name of the list_head within the struct.
 */
#define list_entry(ptr, type, member) \
        container_of(ptr, type, member)
/**
 * list_first_entry - get the first element from a list
 * @ptr:    the list head to take the element from.
 * @type:  the type of the struct this is embedded in.
 * @member:   the name of the list_head within the struct.
 *
 * Note, that list is expected to be not empty.
 */
#define list_first_entry(ptr, type, member) \
            list_entry((ptr)->next, type, member)

/**
 * list_next_entry - get the next element in list
 * @pos:    the type * to cursor
 * @member:    the name of the list_head within the struct.
 */
#define list_next_entry(pos, member) \
        list_entry((pos)->member.next, __typeof__(*(pos)), member)

/**
 * list_prev_entry - get the prev element in list
 * @pos:    the type * to cursor
 * @member:    the name of the list_head within the struct.
 */
#define list_prev_entry(pos, member) \
        list_entry((pos)->member.prev, __typeof__(*(pos)), member)

/**
 * list_for_each -   iterate over a list
 * @pos:    the &struct list_head to use as a loop cursor.
 * @head:  the head for your list.
 */
#define list_for_each(pos, head) \
        for (pos = (head)->next; pos != (head); pos = pos->next)

/**
 * list_for_each_entry   -   iterate over list of given type
 * @pos:    the type * to use as a loop cursor.
 * @head:  the head for your list.
 * @member:   the name of the list_head within the struct.
 */
#define list_for_each_entry(pos, head, member)              \
        for (pos = list_first_entry(head, __typeof__(*pos), member);    \
            &pos->member != (head);                    \
            pos = list_next_entry(pos, member))

#endif
