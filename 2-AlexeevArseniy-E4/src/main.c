#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "linked_binary_tree.h"

char* dublicate_str(void* val){
	char* str = (char*)val;
	char* dublicate = (char*)(malloc(sizeof(char)*(strlen(str)+1)));
	strcpy(dublicate, str);
	return dublicate;
}


int main(){
	char command = '0';
	char line[503] = {'\0'};
	char* string;
	
	LinkedBinaryTree* tree = linked_binary_tree_new((CompareFun)strcmp);
	
	while (fgets(line, 503, stdin) != NULL){
		command = line[0];
		string = line + 2;
		string[strlen(string) - 1] = '\0';

		switch(command){
			case 'a':
				linked_binary_tree_insert(tree, dublicate_str(string));
				break;
			case 'r':
				linked_binary_tree_remove(tree, string);
				break;
			case 'f':
				if (linked_binary_tree_search(tree, string))
					puts("yes");
				else
					puts("no");
				break;
			case 'p':
				linked_binary_tree_print_with_width(tree, dublicate_str);
				break;
			case 'q':
				return 0;
				break;
		}
	}
	return 0;
}

