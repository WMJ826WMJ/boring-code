/*
 * @Author: your name
 * @Date: 2020-10-14 21:35:27
 * @LastEditTime: 2020-10-19 17:12:57
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/container/Hashmap.h
 */
#ifndef _HASHMAP_2_H_
#define _HASHMAP_2_H_

#include "Cn.h"
#include "base/__hashmap.h"
#include "base/__entitiy.h"

typedef Container   Hashmap;
typedef entity_t    Entity;
typedef hash_node_t HashNode;
#define HASHMAP_SLOT_SIZE 10
#define Hashmap_init(hm, key_hasher, key_cmp) CN_initialize(hm, hashmap, NULL, HASHMAP_SLOT_SIZE, key_hasher, key_cmp)
#define Hashmap_uninit(hm, cleanup) CN_uninitialize(hm, hashmap, cleanup)

#define Hashmap_has(con, key) CN_has(con, key)

#define Hashmap_set(con, key, value) do { \
    Entity entity = {                     \
        .key = (key),                     \
        .value = (value)                  \
    };                                    \
    CN_insert(con, __null_iterator, p2t(&entity)); \
} while(0)

#define Hashmap_get(con, key, value)                   \
    ({                                                 \
        int ret = -1;                                  \
        It it = CN_find(con, key);                     \
        if (It_valid(it)) {                            \
            Entity *entity = (Entity*) It_getptr(it);  \
            *value = entity->value;                    \
            ret = 0;                                   \
        }                                              \
        ret;                                           \
    })
#define Hashmap_del(con, key) CN_rm_target(con, key, NULL)

#define Hashmap_table(con) (_CN( (((hashmap_t*)cc(con))->_hash_table),  (((hashmap_t*)cc(con))->key_compare) ))

#define Hashmap_keys(con, key_arr) do {     \
    int i =0;                               \
    Container table =Hashmap_table(con);    \
    for(It first=CN_first(table); !It_equal(first, CN_tail(table)); first=It_next(first)) { \
        HashNode* hash_node = (HashNode*)It_getptr(first);                                  \
        key_arr[i++] = hash_node->entity.key;                                               \
    }                                                                                       \
} while(0)

#define _Entity(key, value)      \
    ({                           \
        Entity entity = {        \
            .key = key,          \
            .value = value       \
        };                       \
        entity;                  \
    })                           

static inline 
int hashmap_ikey_hasher (Tv key, size_t slot_size) {
    int k = t2i(key);
    return (k % slot_size);
}
static inline 
int hashmap_skey_hasher (Tv key, size_t slot_size) {
    char* k = t2p(key);
    return 0;
}
#endif