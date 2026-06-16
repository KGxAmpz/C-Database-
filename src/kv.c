#include <kv.h>
#include <string.h>

// function hash
//params
// val: a pointer to the value to be hashed
// capacity: an integer describing db capacity
size_t hash(char *val, int capacity) {
    size_t hash = 0x13371337deadbeef;
    while(*val){
        hash ^= *val;
        hash = hash << 8;
        hash += *val;
        val++;
    }
    return hash % capacity;
}

// function kv_puts
// paramaters:
// db : a pointer to the database
// key: a pointer to the key value
// value: a pointer to the value itself 
// returns an index to the key value, 
// returns -1 
int kv_put(kv_t *db, char *key, char *value) {
    if (!db || !key || !value) return -1;
    
    size_t idx = hash(key, db->capacity);

    for (int i = 0; i < db->capacity - 1; i++) {
        size_t real_idx = (idx + i) % db->capacity;
        kv_entry_t *entry = &db->entries[real_idx]; 

        // found slot, occupied, matches key
        if (entry->key &&
            entry->key != (void*)TOMBSTONE &&
            !strcmp(entry->key, key)) {
                char *newval = strdup(value);
                if (!newval) return -1;
                entry->value = newval;
                return real_idx;
            
            }
        // found slot, empty or tombstone    
        if (!entry->key || entry->key == (void*)TOMBSTONE) {
                char *newval = strdup(value);
                char *newkey = strdup(key);
                if (!newval || !newkey) {
                    free(newkey);
                    free(newval);
                    return -1;
                }
                entry->value = newval;
                entry->key = newkey;
                db->count++;
                return real_idx;
        }   
    }
    //db is occupied 
    return -2;

};

kv_t *kv_init(size_t capacity){
    if (capacity == 0) return NULL;

    kv_t *table = malloc(sizeof(kv_t));
    if (table == NULL) {
        return NULL;
    }
    table->capacity = capacity;
    table->count = 0;

    table->entries = calloc(sizeof(kv_entry_t), capacity);
    if (table->entries == NULL) {
        free(table);
        return NULL;
    }


    return table;

}