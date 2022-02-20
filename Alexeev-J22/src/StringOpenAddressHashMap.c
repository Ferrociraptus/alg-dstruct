#include <stdlib.h>
#include <string.h>
#include "StringOpenAddressHashMap.h"

typedef enum{
	EMPTY_ITEM = 0,
	DELETED_ITEM,
	FILLED_ITEM
} ItemType;

typedef struct{
	char* key;
	char* value;
	ItemType type;
} StrOAHashMapItem;

struct __StrOAHashMap{
	unsigned size;
	StrOAHashMapItem* items;
};

static char* __dublicate_str(char* str){
	// len + 1 element for '\0' symbol
	unsigned len = strlen(str) + 1;
	
	char* new = (char*)malloc(sizeof(char) * len);
	
	if (new == NULL)
		return NULL;
	
	new[len-1] = '\0';
	strcpy(new, str);
	
	return new;
}

static unsigned __hash(char* key){
	double hash = (unsigned)__hash;
	while (*key != '\0'){
		hash /= *key;
		hash *= 1e5;
		hash += *(key++);
	}
	return (unsigned)hash;
}

static inline unsigned __prob(StrOAHashMap* map, unsigned hash, int i){
	return (hash + PROB_CONST_1 * i*i + PROB_CONST_2 * i) % map->size;
}


/* 
 Check if current element exist after collision element
 (this check needed only on insertion into "deleted" blocks)
 It happend when collision element was removed.
 
 Expamle: 
	hash("hello") == hash("world")
	items array: ... ["hello"] ... ["world"] ... 
	remove "hello": ... [X] ... ["world"] ...
	adding "world": ... ["world"] ... ["world"] ...
 */ 
static StrOAHashMapItem* __check_tail_contains(StrOAHashMap* map, char* key, unsigned hash, unsigned start_i){
	for (int i = start_i; i < map->size; i++){
		unsigned index = __prob(map, hash, i);
		StrOAHashMapItem* item = &map->items[index];
		
		if (item->type == FILLED_ITEM && strcmp(item->key, key) == 0){
			return item;
		}
		else if (item->type == EMPTY_ITEM){
			return NULL;
		}
	}
	return NULL;
}

StrOAHashMap* str_oa_hash_map_new(){
	return str_oa_hash_map_new_with_size(DEFAULT_SIZE);
}

StrOAHashMap* str_oa_hash_map_new_with_size(unsigned size){
	StrOAHashMap* map = (StrOAHashMap*)malloc(sizeof(StrOAHashMap));
	if (map == NULL)
		return NULL;
	
	map->size = size;
	map->items = (StrOAHashMapItem*)malloc(sizeof(StrOAHashMapItem)*size);
	
	if (map->items == NULL){
		free(map);
		return NULL;
	}
	
	memset(map->items, 0, sizeof(StrOAHashMapItem)*size);
	
	return map;
}

void str_oa_hash_map_del(StrOAHashMap* map){
	for (int i = 0; i < map->size; i++){
		StrOAHashMapItem* item = map->items + i;
		
		if (item->type == FILLED_ITEM){
			free(item->value);
			free(item->key);
		}
	}
	free(map->items);
	free(map);
}

int str_oa_hash_map_set(StrOAHashMap* map, char* key, char* value) {
	unsigned hash = __hash(key);
	
	for (int i = 0; i < map->size; i++){
		unsigned index = __prob(map, hash, i);
		StrOAHashMapItem* item = &map->items[index];
		
		if (item->type != FILLED_ITEM){
			if (item->type == DELETED_ITEM){
				StrOAHashMapItem* check;
				if ((check = __check_tail_contains(map, key, hash, i + 1))){
					item = check;
					free(item->value);
					free(item->key);
				}
			}
			item->type = FILLED_ITEM;
			item->key = __dublicate_str(key);
			item->value = __dublicate_str(value);
			return 1;
		}
		else if (strcmp(item->key, key) == 0){
			free(item->value);
			item->value = __dublicate_str(value);
			return 1;
		}
	}
	return 0;
}

void str_oa_hash_map_remove(StrOAHashMap* map, char* key){
	unsigned hash = __hash(key);
	
	for (int i = 0; i < map->size; i++){
		unsigned index = __prob(map, hash, i);
		StrOAHashMapItem* item = &map->items[index];
		
		if (item->type == FILLED_ITEM && strcmp(key, item->key) == 0){
			item->type = DELETED_ITEM;
			free(item->key);
			free(item->value);
			return;
		}
	}
}

char* str_oa_hash_map_get(StrOAHashMap* map, char* key){
	unsigned hash = __hash(key);
	
	for (int i = 0; i < map->size; i++){
		unsigned index = __prob(map, hash, i);
		StrOAHashMapItem* item = &map->items[index];
		
		if (item->type == FILLED_ITEM && strcmp(key, item->key) == 0){
			return __dublicate_str(item->value);
		}
	}
	return NULL;
}

int str_oa_hash_map_contains(StrOAHashMap* map, char* key){
	unsigned hash = __hash(key);
	
	for (int i = 0; i < map->size; i++){
		unsigned index = __prob(map, hash, i);
		StrOAHashMapItem* item = &map->items[index];
		
		if (item->type == FILLED_ITEM && strcmp(key, item->key) == 0){
			return 1;
		}
	}
	return 0;
}
