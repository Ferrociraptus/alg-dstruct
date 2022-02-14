#include <stdlib.h>
#include <stdio.h>
#include "TwoThreeTree.h"


int main(){
	char command;
	int value;

	TwoThreeTree tree = NULL;

	while (scanf("%c", &command) >= 1){
		if (command != 'p' && command != 'q'){
			scanf("%d", &value);
		}

		switch(command){
			case 'a':
				two_three_tree_insert(&tree, value);
				break;
			case 'r':
				two_three_tree_remove(&tree, value);
				break;
			case 'f':
				if (two_three_tree_search(tree, value))
					puts("yes");
				else
					puts("no");
				break;
			case 'p':
				two_three_tree_print(tree);
				break;
			case 'q':
				return 0;
				break;
		}
	}
	return 0;
}
