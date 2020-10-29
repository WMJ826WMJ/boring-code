/*
 * @Author: your name
 * @Date: 2020-10-14 21:35:27
 * @LastEditTime: 2020-10-29 12:56:48
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/container/Hashmap.h
 */
#ifndef _MAP_H_
#define _MAP_H_
#include <stdio.h>
#include <string.h>
#include "Cn.h"
#include "base/__rb_tree.h"
#include "Entity.h"

typedef Container   Map;
typedef Entity* (*Expose_Entity)(Tv);

#define Map_init(map, map_clazz, setup, conflict, ...) CN_initialize(map, map_clazz, NULL, setup, conflict, __VA_ARGS__);

#define Map_uninit(map, map_clazz, cleanup) CN_uninitialize(map, map_clazz, cleanup)

#define Map_hasx(con, x, ...)     \
    ({                            \
        Tv t[x];                  \
        Entity entity;            \
        TempEntity(&entity, x, x, t, __VA_ARGS__); \
        int ret = CN_has(con, p2t(&entity));       \
        ret; \
    })
#define Map_has(con, key) Map_hasx(con, 1, key)
#define Map_has2(con, key1, key2) Map_hasx(con, 2, key1, key2)
#define Map_has3(con, key1, key2, key3) Map_hasx(con, 3, key1, key2, key3)
 //CN_has(con, key)

#define Map_setx(con, x, y, ...) do {          \
    Tv t[x];                                   \
    Entity entity;                             \
    TempEntity(&entity, x, y, t, __VA_ARGS__); \
    CN_set(con, p2t(&entity));                 \
} while(0)

#define Map_set(con, key, value) Map_setx(con, 2, 1, key, value)
#define Map_set2(con, key1, key2, value) Map_setx(con, 3, 2, key1, key2, value)
#define Map_set3(con, key1, key2, key3, value) Map_setx(con, 4, 3, key1, key2, key3, value)

#define Map_getx(con, value, x, ...)   \
    ({                                     \
        int ret = -1;                      \
        Tv t[x];                           \
        Entity entity;                     \
        TempEntity(&entity, x, x, t, __VA_ARGS__);  \
        It it = CN_search(con, __null_iterator, p2t(&entity)); \
        if (It_valid(it)) {                         \
            Expose_Entity exposer                   \
                = (Expose_Entity) c_extra_func(con);\
            Entity* ent = exposer(It_dref(it));     \
            value = ent->tv[ent->value_index];      \
            ret = 0;                                \
        }                                           \
        ret;                                        \
    })
#define Map_get(con, key, value) Map_getx(con, value, 1, key)
#define Map_get2(con, key1, key2, value) Map_getx(con, value, 2, key1, key2)
#define Map_get3(con, key1, key2, key3, value) Map_getx(con, value, 3, key1, key2, key3)

#define Map_delx(con, rdata, x, ...)        \
    ({                                      \
        int ret = -1;                       \
        Tv t[x];                            \
        Entity entity;                      \
        Tv* prdata = rdata;                 \
        TempEntity(&entity, x, x, t, __VA_ARGS__);        \
        Tv rentity;                                       \
        ret = CN_rm_target(con, p2t(&entity), &rentity);  \
        if (ret == 0) {                                   \
            Entity* pentity = t2p(rentity);               \
            if (prdata)                                   \
                *prdata = pentity->tv[pentity->value_index]; \
            free(pentity);                              \
        }                                               \ 
        ret;\
    })

#define Map_del(con, key, rdata) Map_delx(con, rdata, 1, key)
#define Map_del2(con, key1, key2, rdata) Map_delx(con, rdata, 2, key1, key2)
#define Map_del3(con, key1, key2, key3, rdata) Map_delx(con, rdata, 3, key1, key2, key3)

static inline 
int Map_setup (Tv* v1, Tv v2) 
{
    Entity* temp = t2p(v2);
    Entity* lentity = CopyALongTimeEntity(temp);
    //printf("make a lentity %p \n", lentity);
    *v1 = p2t(lentity);
}

static inline 
int Map_conflict_fix(Tv* v1, Tv v2) 
{
    // 算出来了 key 相同。
    Entity* temp = t2p(v2);
    Entity* lentity = t2p((*v1));
    
    if (EntityValueEqual(lentity, temp) == 0) return 0;

    // 然后把 value 弄过去。
    if (temp->number == lentity->number 
    && temp->value_index == lentity->value_index) {
        CopyEntityValue(lentity, temp);
    } else {
        // 直接把旧的弄掉，换新的上去。
        free(lentity);
        Entity* nentity = CopyALongTimeEntity(temp);
        *v1 = p2t(nentity);
    } 
    return 0;
}

static inline
int Map_entity_key_equl (Tv v1, Tv v2) 
{
    Entity* e1 = t2p(v1);
    Entity* e2 = t2p(v2);
    return EntityKeyEqual(e1, e2);
}
#endif