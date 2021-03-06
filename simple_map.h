/**
 * Simple Map
 *
 * A simple header only Hash Map Library to enable for fast development.
 *
 * @author Marlon Chatman <mcchatman8009@gmail.com>
 *
 * @copyright Copyright (C) 2021 by Marlon Chatman <mcchatman8009@gmail.com>
 */
#ifndef SIMPLE_MAP_SIMPLE_MAP_H
#define SIMPLE_MAP_SIMPLE_MAP_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#define SM_GROWTH_FACTOR (4)
#define SM_MAX_LOAD_FACTOR 0.75
#define SM_INITIAL_CAPCITY 1024

#define sm_foreach(m, block) \
    assert(m != NULL); \
    for (int ___i = 0; ___i < m->cap; ++___i) { \
        h_node* ___node = m->table[___i]; \
        while (___node != NULL) { \
            void* value = ___node->val; \
            const char* key = ___node->key; \
            block \
            ___node = ___node->next; \
        } \
    } \
    NULL

typedef struct h_node h_node;

struct h_node {
    const char* key;
    void* val;
    h_node* next;
    bool alloc;
};

typedef struct smap {
    int cap;
    int size;
    h_node** table;
    double load_factor;
    int growth_factor;
} smap, simple_hashmap;

unsigned int hash_str(const char* str) {
    unsigned int hash = 0;
    int size = (int) strlen(str);
    unsigned int multiplier = 97;

    for (int i = 0; i < size; ++i) {
        hash += multiplier * ((unsigned int) str[i]);
    }

    return hash;
}

unsigned int sm_hash_str(smap* m, const char* str) {
    return hash_str(str) % m->cap;
}

smap* sm_alloc(int cap) {
    smap* map = malloc(sizeof(smap));
    h_node** table = calloc(cap, sizeof(h_node));
    map->growth_factor = SM_GROWTH_FACTOR;
    map->load_factor = SM_MAX_LOAD_FACTOR;

    for (int i = 0; i < cap; ++i) {
        table[i] = NULL;
    }

    map->table = table;
    map->cap = cap;
    map->size = 0;

    return map;
}

smap* sm_new() {
    return sm_alloc(SM_INITIAL_CAPCITY);
}

int sm_size(smap* m) {
    return m->size;
}

bool sm_empty(smap* m) {
    return m->size == 0;
}

void* sm_get(smap* m, const char* key) {
    assert(m != NULL);

    if (key == NULL) {
        return NULL;
    }

    unsigned int hash = sm_hash_str(m, key);

    h_node* node = m->table[hash];

    if (node) {
        while (node != NULL) {
            if (strcmp(key, node->key) == 0) {
                return node->val;
            }

            node = node->next;
        }
    }

    return NULL;
}


void sm_read_vals(smap* m, void* vals[], int n) {
    assert(m != NULL);

    int count = 0;

    sm_foreach(m, {
        assert(count <= m->size);

        if (n == count) {
            break;
        }
        vals[count++] = value;
    });

    assert(count == m->size || count == n);
}

void sm_read_keys(smap* m, const char* keys[], int n) {
    assert(m != NULL);

    int count = 0;
    sm_foreach(m, {
        assert(count <= m->size);

        if (n == count) {
            break;
        }
        keys[count++] = key;
    });

    assert(count == m->size || count == n);
}


void sm_free_node(h_node* node) {
    // Free the key
    free((char*) node->key);

    // Free the value if needed
    if (node->alloc) {
        free(node->val);
    }
}

h_node* sm_alloc_node(const char* key, void* value, bool alloc) {
    char* str = malloc(strlen(key) + 1);
    strcpy(str, key);

    h_node* node = malloc(sizeof(h_node));
    node->next = NULL;
    node->alloc = alloc;
    node->key = str;
    node->val = value;

    return node;
}

void sm_remove(smap* m, const char* key) {
    assert(m != NULL);

    if (key != NULL) {
        unsigned int hash = sm_hash_str(m, key);
        h_node* node = m->table[hash];
        h_node* prev = NULL;

        while (node != NULL) {
            if (strcmp(key, node->key) == 0) {
                if (node->next == NULL) {
                    //
                    // Case 1: only one node.
                    //
                    m->table[hash] = NULL;
                    // cleanup node
                    sm_free_node(node);
                    m->size--;
                    return;
                } else if (prev == NULL) {
                    //
                    // Case 2: We remove the first node in the chain/bucket.
                    //
                    m->table[hash] = node->next;
                    // cleanup node
                    sm_free_node(node);
                    m->size--;
                    return;
                } else {
                    //
                    // Case 3: Set the previous node to the current's node next node.
                    // A -> B -> C
                    // Remove B
                    // A -> C
                    //
                    prev->next = node->next;

                    // cleanup node
                    sm_free_node(node);
                    m->size--;
                    return;
                }
            }

            prev = node;
            node = node->next;
        }

    }
}

void sm_put_node(smap* m, h_node* node);

void sm_grow_map_table(smap* m) {
    const int old_cap = m->cap;
    const int new_cap = m->cap * m->growth_factor;
    int map_size = m->size;

    h_node** new_table = calloc(new_cap, sizeof(h_node));
    h_node** old_table = m->table;
    h_node* all_nodes[m->size];

    // Make sure that the table has all NULL nodes.
    for (int i = 0; i < new_cap; ++i) {
        new_table[i] = NULL;
    }

    //
    // Build out all the nodes to use in the new table.
    //
    int node_idx = 0;

    for (int i = 0; i < old_cap; ++i) {
        h_node* node = old_table[i];

        while (node != NULL) {
            all_nodes[node_idx++] = node;

            node = node->next;
        }
    }

    assert(node_idx == map_size);

    // Ensure that none of the nodes are connected.
    for (int i = 0; i < map_size; ++i) {
        all_nodes[i]->next = NULL;
    }


    // Start using the new hash table, and capacity.
    m->table = new_table;
    m->size = 0;
    m->cap = new_cap;

    // Save all tne nodes to the map
    for (int i = 0; i < map_size; ++i) {
        h_node* node = all_nodes[i];
        assert(node->next == NULL);
        sm_put_node(m, node);
    }

    assert(m->size == map_size);

    // Clean up the old hash table.
    free(old_table);
}

bool sm_contains(smap* m, const char* key) {
    unsigned int hash = sm_hash_str(m, key);

    if (m->table[hash] != NULL) {
        h_node* node = m->table[hash];
        while (node != NULL) {
            if (strcmp(key, node->key) == 0) {
                return true;
            }

            node = node->next;
        }

    }

    return false;
}

void* sm_get_or_default(smap* m, const char* key, void* default_ptr) {
    if (sm_contains(m, key)) {
        return sm_get(m, key);
    } else {
        return default_ptr;
    }
}

void sm_put_node(smap* m, h_node* node) {
    assert(m != NULL);
    assert(node->next == NULL);

    if (sm_contains(m, node->key)) {
        sm_remove(m, node->key);
    }

    unsigned int hash = sm_hash_str(m, node->key);
    //
    // Insert the node in front of the first node.
    //
    if (m->table[hash] != NULL) {
        node->next = m->table[hash];
    }

    m->table[hash] = node;
    m->size++;

    if (m->size >= (m->cap * m->load_factor)) {
        // grow the table
        sm_grow_map_table(m);
    }
}

void sm_put(smap* m, const char* key, void* ptr) {
    assert(m != NULL);

    if (key != NULL) {
        h_node* node = sm_alloc_node(key, ptr, false);
        sm_put_node(m, node);
    }
}

void sm_put_ch(smap* m, const char* key, char ch) {
    assert(m != NULL);

    if (key != NULL) {
        char* ptr = malloc(sizeof(char));
        *ptr = ch;
        h_node* node = sm_alloc_node(key, ptr, true);
        sm_put_node(m, node);
    }
}

void sm_put_float(smap* m, const char* key, float fl) {
    assert(m != NULL);

    if (key != NULL) {
        float* fl_ptr = malloc(sizeof(float));
        *fl_ptr = fl;
        h_node* node = sm_alloc_node(key, fl_ptr, true);
        sm_put_node(m, node);
    }
}

void sm_put_double(smap* m, const char* key, double d) {
    assert(m != NULL);

    if (key != NULL) {
        double* d_ptr = malloc(sizeof(double));
        *d_ptr = d;
        h_node* node = sm_alloc_node(key, d_ptr, true);
        sm_put_node(m, node);
    }
}

void sm_put_long(smap* m, const char* key, long l) {
    assert(m != NULL);

    if (key != NULL) {
        long* i_ptr = malloc(sizeof(long));
        *i_ptr = l;
        h_node* node = sm_alloc_node(key, i_ptr, true);
        sm_put_node(m, node);
    }
}

void sm_put_int(smap* m, const char* key, int i) {
    assert(m != NULL);

    if (key != NULL) {
        int* i_ptr = malloc(sizeof(int));
        *i_ptr = i;
        h_node* node = sm_alloc_node(key, i_ptr, true);
        sm_put_node(m, node);
    }
}

void sm_put_str(smap* m, const char* key, const char* val) {
    assert(m != NULL);

    if (key != NULL) {
        char* str = malloc(strlen(val) + 1);
        strcpy(str, val);
        h_node* node = sm_alloc_node(key, str, true);
        sm_put_node(m, node);
    }
}

void sm_clear(smap* m) {
    const char* keys[m->size];
    sm_read_keys(m, keys, m->size);
    int size = m->size;

    for (int i = 0; i < size; ++i) {
        sm_remove(m, keys[i]);
    }

    assert(m->size == 0);
}

void sm_free(smap* m) {
    const char* keys[m->size];

    int size = m->size;
    sm_read_keys(m, keys, size);

    for (int i = 0; i < size; ++i) {
        sm_remove(m, keys[i]);
    }

    for (int i = 0; i < m->cap; ++i) {
        assert(m->table[i] == NULL);
    }

    // Cleanup the table
    free(m->table);
    // Cleanup the map
    free(m);
}

#endif
