//-------------------StringOpenAddressHashMap.h-------------------
#ifndef OPEN_ADRESS_HASH_TREE
#define OPEN_ADRESS_HASH_TREE

#define DEFAULT_SIZE 2048
// prob function: (hash() + PROB_CONST_1*i^2 + PROB_CONST_2*i)%map_size
#define PROB_CONST_1 127
#define PROB_CONST_2 19

typedef struct __StrOAHashMap StrOAHashMap;

StrOAHashMap* str_oa_hash_map_new();
StrOAHashMap* str_oa_hash_map_new_with_size(unsigned size);

int str_oa_hash_map_set(StrOAHashMap* map, int key, char* value);
void str_oa_hash_map_remove(StrOAHashMap* map, int key);
int str_oa_hash_map_contains(StrOAHashMap* map, int key);

#endif
//---------------------------------------------------------------
//---------------------------------------------------------------


//-------------------StringOpenAddressHashMap.c-------------------
#include <stdlib.h>
#include <string.h>

typedef enum{
	EMPTY_ITEM = 0,
	DELETED_ITEM,
	FILLED_ITEM
} ItemType;

typedef struct{
	int key;
	char* value;
	ItemType type;
} StrOAHashMapItem;

struct __StrOAHashMap{
	unsigned size;
	StrOAHashMapItem* items;
};

// static unsigned __hash(char* key){
// 	unsigned hash = 0;
// 	while (*key != '\0'){
// 		hash <<= 5;
// 		hash += *(key++);
// 	}
// 	return hash;
// }

static unsigned __hash(int key){
	double hash = (unsigned)__hash;
	hash /= key;
	hash *= 5e10;
	hash += key;
	return (unsigned)hash;
}


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


static inline unsigned __prob(StrOAHashMap* map, unsigned hash, int i){
	return (hash + PROB_CONST_1 * i*i + PROB_CONST_2 * i) % map->size;
}


static StrOAHashMapItem* __check_tail_contains(StrOAHashMap* map, int key, unsigned hash, unsigned start_i){
	for (int i = start_i; i < map->size; i++){
		unsigned index = __prob(map, hash, i);
		StrOAHashMapItem* item = &map->items[index];
		
		if (item->type == FILLED_ITEM && item->key == key){
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

int str_oa_hash_map_set(StrOAHashMap* map, int key, char* value) {
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
				}
			}
			item->type = FILLED_ITEM;
			item->key = key;
			item->value = __dublicate_str(value);
			return 1;
		}
		else if (item->key == key){
			free(item->value);
			item->value = __dublicate_str(value);
			return 1;
		}
	}
	return 0;
}

void str_oa_hash_map_remove(StrOAHashMap* map, int key){
	unsigned hash = __hash(key);
	for (int i = 0; i < map->size; i++){
		
		unsigned index = __prob(map, hash, i);
		StrOAHashMapItem* item = &map->items[index];
		
		if (item->type == FILLED_ITEM && key == item->key){
			item->type = DELETED_ITEM;
			free(item->value);
			return;
		}
	}
}


int str_oa_hash_map_contains(StrOAHashMap* map, int key){
	unsigned hash = __hash(key);
	for (int i = 0; i < map->size; i++){
		
		unsigned index = __prob(map, hash, i);
		StrOAHashMapItem* item = &map->items[index];
		
		if (item->type == FILLED_ITEM && key == item->key){
			return 1;
		}
	}
	return 0;
}
//---------------------------------------------------------------
//---------------------------------------------------------------


//-----------------------------main.c----------------------------
#include <stdio.h>

int main(){
	char command;
	int key;
	
	StrOAHashMap* map = str_oa_hash_map_new();

	while (scanf("%c", &command) >= 1){
		if (command == '\n')
			continue;
		if (command != 'q'){
			scanf("%d", &key);
		}

		switch(command){
			case 'a':
				str_oa_hash_map_set(map, key, "");
				break;
			case 'r':
				str_oa_hash_map_remove(map, key);
				break;
			case 'f':
				if (str_oa_hash_map_contains(map, key))
					puts("yes");
				else
					puts("no");
				break;
			case 'q':
				return 0;
				break;
		}
	}
	return 0;
}
//---------------------------------------------------------------
//---------------------------------------------------------------
