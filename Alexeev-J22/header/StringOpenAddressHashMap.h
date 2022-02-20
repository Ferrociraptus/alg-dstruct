#ifndef OPEN_ADDRESS_HASH_TREE
#define OPEN_ADDRESS_HASH_TREE

#define DEFAULT_SIZE 2048
// prob function: (hash() + PROB_CONST_1*i^2 + PROB_CONST_2*i)%map_size
#define PROB_CONST_1 13
#define PROB_CONST_2 19

typedef struct __StrOAHashMap StrOAHashMap;

StrOAHashMap* str_oa_hash_map_new();
StrOAHashMap* str_oa_hash_map_new_with_size(unsigned size);
void str_oa_hash_map_del(StrOAHashMap* map);

int str_oa_hash_map_set(StrOAHashMap* map, char* key, char* value);
void str_oa_hash_map_remove(StrOAHashMap* map, char* key);
char* str_oa_hash_map_get(StrOAHashMap* map, char* key);
int str_oa_hash_map_contains(StrOAHashMap* map, char* key);

#endif
