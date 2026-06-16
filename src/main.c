#include <stdio.h>
#include <kv.h>
#include <stdlib.h>
#include <string.h>

int main() {
    kv_t *table = kv_init(1024);
    //printf("%p\n", table);
    //printf("%ld \n", table->capacity);

    kv_put(table, "haha", "fafa ");
    kv_put(table, "haha", "hoho");
    kv_put(table, "lala", "lama");

    
    

    char *val = kv_get(table, "haha");
    char *val2 = kv_get(table, "lala");
    char *val3 = kv_get(table, "DNE");

    printf("%s %s %s\n", val, val2, val3);

    kv_delete(table, "haha");
    val = NULL;
    val = kv_get(table, "haha");

    printf("%s %s %s\n", val, val2, val3);
}