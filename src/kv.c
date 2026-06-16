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

// function kv_get
// paramaters:
// db : a pointer to the database
// returns 0 on completion
// or NULL if not found 
int kv_free(kv_t *db) {
    if (!db) return -1;
    for (int i = 0; i < db->capacity-1; i++) {
        kv_entry_t *e = &db->entries[i];
        
        if (e->key && e->key != (void *)TOMBSTONE) {
            free(e->key);
            free(e->value);
            e->key = NULL;
            e->value = NULL;
            db->count--;
        }

    }

    free(db->entries);
    free(db);

    return 0;
}
// function kv_delete
// paramaters:
// db : a pointer to the database
// key: a pointer to the key value
// returns the index of deleted 
int kv_delete(kv_t *db, char *key) {
      if (!db || !key) return -1;
    
    size_t idx = hash(key, db->capacity);    
    for (int i = 0; i < db->capacity - 1; i++) {
        size_t real_idx = (idx + i) % db->capacity;

        kv_entry_t *entry = &db->entries[real_idx]; 
         //no key
        if (entry->key == NULL) {
            return -1;
        }
        //find entry if key matches
        if (entry->key && 
            entry->key != (void*)TOMBSTONE &&
            !strcmp(entry->key, key)) {
            free(entry->key);
            free(entry->value);
            db->count--;
            entry->key = TOMBSTONE;
            entry->value = NULL;

            return real_idx;
        }

    }
    return -1;
} 

// function kv_get
// paramaters:
// db : a pointer to the database
// key: a pointer to the key value
// returns pounter to the key
// or NULL if not found 
char *kv_get(kv_t *db, char *key) {
      if (!db || !key) return NULL;
    
    size_t idx = hash(key, db->capacity);    
    for (int i = 0; i < db->capacity - 1; i++) {
        size_t real_idx = (idx + i) % db->capacity;

        kv_entry_t *entry = &db->entries[real_idx]; 
         //no key
        if (entry->key == NULL) {
            return NULL;
        }
        //find entry if key matches
        if (entry->key && 
            entry->key != (void*)TOMBSTONE &&
            !strcmp(entry->key, key)) {
            return entry->value;

        }

    }
    return NULL;
} 

// function kv_put
// paramaters:
// db : a pointer to the database
// key: a pointer to the key value
// value: a pointer to the value itself 
// returns 0 on succsess
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
                free(entry->value);
                entry->value = newval;
                return 0;
            
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
                return 0;
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