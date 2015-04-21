#ifndef _DEFS_H
#define _DEFS_H

#undef NULL
#define NULL ((void *)0)
#define EOF  -1

typedef enum {
        false   = 0,
        true    = 1
} bool;

typedef unsigned long  uint64_t;
typedef          long   int64_t;
typedef unsigned int   uint32_t;
typedef          int    int32_t;
typedef unsigned short uint16_t;
typedef          short  int16_t;
typedef unsigned char   uint8_t;

typedef unsigned long  size_t;
#undef offsetof
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

struct list_head {
        struct list_head *next, *prev;
};

struct hlist_head {
        struct hlist_node *first;
};

struct hlist_node {
        struct hlist_node *next, **pprev;
};
#endif
