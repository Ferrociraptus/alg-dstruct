#include <stdlib.h>
#include <stdio.h>
#include "StringOpenAddressHashMap.h"


int main(){
	char command;
	char key[100] = {'\0'};
	
	StrOAHashMap* map = str_oa_hash_map_new();

	while (scanf("%c", &command) >= 1){
		if (command == '\n')
			continue;
		if (command != 'q'){
			scanf("%s", key);
		}

		char* buf;
		switch(command){
			case 'a':
				str_oa_hash_map_set(map, key, "yes");
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
			case 'g':
				buf = str_oa_hash_map_get(map, key);
				if (buf){
					puts(buf);
					free(buf);
				}
				break;
			case 'q':
				return 0;
				break;
		}
	}
	str_oa_hash_map_del(map);
	return 0;
}
