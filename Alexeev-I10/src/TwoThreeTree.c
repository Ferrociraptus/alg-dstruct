//#pragma GCC optimize ("O0")

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ncurses.h>

#include "TwoThreeTree.h"

typedef enum {
	TWO_NODE_TYPE,
	THREE_NODE_TYPE
} TwoThreeTreeNodeType;

typedef struct __TwoThreeTreeNode {
	TwoThreeTreeNodeType type;
	int first_value;
	int second_value;
	struct __TwoThreeTreeNode* left;
	struct __TwoThreeTreeNode* middle; //used only in THREE_NODE_TYPE
	struct __TwoThreeTreeNode* right;
} TwoThreeTreeNode;

typedef enum {
	IN_RANGE,
	LOWER_THEN_RANGE,
	GREATER_THEN_RANGE,
	RANGE_BORDER,
	RANGE_ERR
} InRangeAns;

typedef enum {
	ERR_NODE,
	NEXT_NODE_RECIEVED,
	NODE_VALUE_EXISTS
} TwoThreeNextNodeStatus;

typedef struct __two_three_node_info{
	TwoThreeTreeNode* next_node_p;
	TwoThreeNextNodeStatus next_node_info_type;
	InRangeAns in_rage_info;
	int isleaf:1;
} __TwoThreeNodeInfo;

static inline InRangeAns __value_in_range(int value, int from, int to){
	if (from > to)
		return RANGE_ERR;
	
	if (value < from)
		return LOWER_THEN_RANGE;
	
	else if (value > to)
		return GREATER_THEN_RANGE;
	
	else if (value == from || value == to)
		return RANGE_BORDER;
	
	else
		return IN_RANGE;
}

static inline InRangeAns __value_in_node_range(int value, TwoThreeTreeNode* node){
	if (node == NULL)
		return RANGE_ERR;
	else if (node->type == TWO_NODE_TYPE)
		return __value_in_range(value, node->first_value, node->first_value);
	else
		return __value_in_range(value, node->first_value, node->second_value);
}

static inline void __swap_node_values(TwoThreeTreeNode* node){
	if (node == NULL)
		return;
	
	int tmp = node->first_value;
	node->first_value = node->second_value;
	node->second_value = tmp;
}

static inline void __swap_int_values(int* val1, int* val2){
	if (val1 == NULL || val2 == NULL)
		return;
	
	int tmp = *val1;
	*val1 = *val2;
	*val2 = tmp;
}

static inline void __swap_pointer_values(void** val1, void** val2){
	void* tmp = *val1;
	*val1 = *val2;
	*val2 = tmp;
}

static inline void __swap_branch_pointers (TwoThreeTreeNode** val1, TwoThreeTreeNode** val2){
	__swap_pointer_values((void**)val1, (void**)val2);
}

static inline int __is_two_three_node_leaf(TwoThreeTreeNode* leaf){
	if (leaf == NULL)
		return 1;
	if (leaf->left == NULL && leaf->middle == NULL && leaf->right == NULL)
		return 1;
	return 0;
}

static TwoThreeTreeNode* __two_three_node_new(int value){
	TwoThreeTreeNode* node = (TwoThreeTreeNode*)malloc(sizeof(TwoThreeTreeNode));
	if (node == NULL)
		return NULL;
	
	node->type = TWO_NODE_TYPE;
	node->left = NULL;
	node->middle = NULL;
	node->right = NULL;
	node->first_value = value;
	node->second_value = 0;
	
	return node;
}


__TwoThreeNodeInfo __two_three_node_info(TwoThreeTreeNode* node, int value){
	if (node == NULL)
		return (__TwoThreeNodeInfo){NULL, ERR_NODE, RANGE_ERR, 1};
	
	int is_leaf = __is_two_three_node_leaf(node);
	
	InRangeAns range_info;
	switch (node->type){
		case TWO_NODE_TYPE:
			if (node->first_value == value)
				return (__TwoThreeNodeInfo){node, NODE_VALUE_EXISTS, RANGE_BORDER, is_leaf};
			
			if (value < node->first_value)
				return (__TwoThreeNodeInfo){node->left, NEXT_NODE_RECIEVED, LOWER_THEN_RANGE, is_leaf};
			else
				return (__TwoThreeNodeInfo){node->right, NEXT_NODE_RECIEVED, GREATER_THEN_RANGE, is_leaf};
		break;
		
		case THREE_NODE_TYPE:
			range_info = __value_in_node_range(value, node);
			switch(range_info){
				case RANGE_BORDER:
					return (__TwoThreeNodeInfo){node, NODE_VALUE_EXISTS, range_info, is_leaf};
				case IN_RANGE:
					return (__TwoThreeNodeInfo){node->middle, NEXT_NODE_RECIEVED, range_info, is_leaf};
				case LOWER_THEN_RANGE:
					return (__TwoThreeNodeInfo){node->left, NEXT_NODE_RECIEVED, range_info, is_leaf};
				case GREATER_THEN_RANGE:
					return (__TwoThreeNodeInfo){node->right, NEXT_NODE_RECIEVED, range_info, is_leaf};
				case RANGE_ERR:
					return (__TwoThreeNodeInfo){NULL, ERR_NODE, range_info, is_leaf};
			}
	}
	return (__TwoThreeNodeInfo){NULL, ERR_NODE, RANGE_ERR, 1};
}

TwoThreeTreeNode* __two_three_tree_insert(TwoThreeTreeNode* node, int value){
	static char node_ingected_flag = 0;
	static char node_splitted = 0;
	if (node == NULL){
		node_ingected_flag = 1;
		return __two_three_node_new(value);
	}
	
	__TwoThreeNodeInfo node_info = __two_three_node_info(node, value);
	if (node->type == TWO_NODE_TYPE){
		node_splitted = 0;
		node_ingected_flag = 0;
		TwoThreeTreeNode* buf_node = NULL;
		if (node_info.next_node_p != NULL && node_info.in_rage_info != RANGE_BORDER){
			buf_node = __two_three_tree_insert(node_info.next_node_p, value);
			if (buf_node->type == TWO_NODE_TYPE && node_splitted){
				value = buf_node->first_value;
				if (node_info.in_rage_info == LOWER_THEN_RANGE){
// 					__swap_branch_pointers(&node->middle, &node->right);
					__swap_branch_pointers(&buf_node->left, &node->left);
					__swap_branch_pointers(&buf_node->right, &node->middle);
				}
				else if (node_info.in_rage_info == GREATER_THEN_RANGE){
					__swap_branch_pointers(&buf_node->left, &node->middle);
					__swap_branch_pointers(&buf_node->right, &node->right);
				}
				else{
					node_ingected_flag = 1;
					return node;
				}
				free(buf_node);
				node_ingected_flag = 0;
			}
		}
		node_splitted = 0;
		if (node_ingected_flag){
			node_info.next_node_p = buf_node;
		}
		else{
			node_ingected_flag = 1;
			node->type = THREE_NODE_TYPE;
			if (value < node->first_value){
				__swap_node_values(node);
				node->first_value = value;
			}
			else if (value > node->first_value){
				node->second_value = value;
			}
			else{
				node_ingected_flag = 1;
				node->type = TWO_NODE_TYPE;
			}
		}
		return node;
	}
	else {
		TwoThreeTreeNode* buf_node = node_info.next_node_p;
		node_ingected_flag = 0;
		node_splitted = 1;
		switch(node_info.next_node_info_type){
			case NEXT_NODE_RECIEVED:
				if (node_info.isleaf){
					TwoThreeTreeNode* new_node = __two_three_node_new(value);
					buf_node = __two_three_node_new(node->second_value);
					node->type = TWO_NODE_TYPE;
					switch (node_info.in_rage_info){
						// splitting node
						case IN_RANGE:
							// node->first_value  val node->second_value
							new_node->left = node;
							new_node->right = buf_node;
							return new_node;
							
						case LOWER_THEN_RANGE:
							// val node->first_value node->second_value
							node->left = new_node;
							node->right = buf_node;
							return node;
							
						case GREATER_THEN_RANGE:
							// node->first_value node->second_value val
							buf_node->left = node;
							buf_node->right = new_node;
							return buf_node;
							
						case RANGE_BORDER:
						case RANGE_ERR:
							node_splitted = 0;
							return node;
					}
				}
				else{
					TwoThreeTreeNode* new_node;
					if (buf_node == NULL && node->type == THREE_NODE_TYPE){
						node_info.next_node_p = __two_three_tree_insert(buf_node, value);
						return node;
					}
					
					
					buf_node = __two_three_tree_insert(buf_node, value);
					if (buf_node->type == TWO_NODE_TYPE && node_splitted){
						node_ingected_flag = 1;
						switch(node_info.in_rage_info){
							case IN_RANGE:
							// node->first_value  buf_node node->second_value
								new_node = __two_three_node_new(node->first_value);
								
								new_node->left = node->left;
								new_node->right = buf_node->left;
								node->left = buf_node->right;
								
								node->first_value = node->second_value;
								
								node->middle = NULL;
								node->type = TWO_NODE_TYPE;
								
								buf_node->left = new_node;
								buf_node->right = node;
								
								return buf_node; 
							case LOWER_THEN_RANGE:
							// buf_node node->first_value node->second_value
								new_node = __two_three_node_new(node->second_value);

								__swap_branch_pointers (&node->right, &new_node->right);
								
								__swap_branch_pointers (&node->middle, &new_node->left);
								node->left = buf_node;
								
								node->right = new_node;
								
								node->middle = NULL;
								node->type = TWO_NODE_TYPE;
								
								return node;
								
							case GREATER_THEN_RANGE:
								new_node = __two_three_node_new(node->second_value);
								new_node->left = node;
								
								new_node->right = buf_node;
								
								__swap_branch_pointers(&node->middle, &node->right);
								node->middle = NULL;
								node->type = TWO_NODE_TYPE;
								return new_node;
							case RANGE_BORDER:
							case RANGE_ERR:
								node_splitted = 0;
								node_ingected_flag = 0;
								return node;
						}
					}
					else{
						node_info.next_node_p = buf_node;
					}
				}
				break;
				
			case NODE_VALUE_EXISTS:
			case ERR_NODE:
				node_splitted = 0;
				node_ingected_flag = 1;
				return node;
		}
	}
	return node;
} 

void two_three_tree_insert(TwoThreeTree* tree_p, int value) {
	*tree_p = __two_three_tree_insert(*tree_p, value);
	
}


int two_three_tree_search (TwoThreeTree tree, int value){
	if (tree == NULL)
		return 0;
	
	TwoThreeTreeNode* node = tree;
	
	while(node != NULL){
		__TwoThreeNodeInfo node_info = __two_three_node_info(node, value);
		if (node_info.in_rage_info == RANGE_BORDER)
			return 1;
		
		node = node_info.next_node_p;
	}
	return 0;
}

static void __reconstruct_tree(TwoThreeTree* new, TwoThreeTreeNode* node){
	if (node == NULL){
		return;
	}
	
	__reconstruct_tree(new, node->middle);
	
	*new = __two_three_tree_insert(*new, node->first_value);
	if (node->type == THREE_NODE_TYPE)
		*new = __two_three_tree_insert(*new, node->second_value);

	__reconstruct_tree(new, node->right);
	__reconstruct_tree(new, node->left);
	free(node);
}

static TwoThreeTreeNode* __two_three_node_remove(TwoThreeTreeNode* node, int value){
	if (node == NULL){
		return NULL;
	}
	
	TwoThreeTreeNode* buf_node = NULL;
	__TwoThreeNodeInfo node_info = __two_three_node_info(node, value);
	if (node_info.in_rage_info == RANGE_BORDER){
		if (node_info.isleaf){
			if (node->type == THREE_NODE_TYPE){
				// if node is leaf and have 2 points remove one
				node->type = TWO_NODE_TYPE;
				if (value == node->first_value){
					__swap_node_values(node);
				}
				return node;
			}
			else{
				return NULL;
			}
		}
		else{ // if node is not leaf
			TwoThreeTreeNode* node_iter, **branch;
			int* removeble_param;
			
			if (node->first_value == value || node->type == TWO_NODE_TYPE){
				removeble_param = &node->first_value;
				
				if (node->type == TWO_NODE_TYPE){
					branch = &node->right;
				}
				else{
					branch = &node->middle;
				}
				for(node_iter = *branch; node_iter->left != NULL; node_iter = node_iter->left);
			}
			else{
				branch = &node->middle;
				removeble_param = &node->second_value;
				for(node_iter = *branch; node_iter->right != NULL; node_iter = node_iter->right);
			}
			
			
			if (node_iter->type == THREE_NODE_TYPE){
				if (node->first_value == value || node->type == TWO_NODE_TYPE){
					__swap_int_values(&node_iter->first_value, removeble_param);
					__swap_int_values(&node_iter->first_value, &node_iter->second_value);
				}
				else{
					__swap_int_values(&node_iter->second_value, removeble_param);
				}
				node_iter->type = TWO_NODE_TYPE;
				return node;
			}
			else{
				__swap_int_values(&node_iter->first_value, removeble_param);
				node_info.next_node_p = *branch;
				
			}
		}
	}
	
	if (node_info.next_node_p != NULL){
		buf_node = __two_three_node_remove(node_info.next_node_p, value);

		if (buf_node != NULL){
			node_info.next_node_p = buf_node;
		}
		else {
			// merge two nodes if it is
			if (node->left == node_info.next_node_p){
				if (node->type == TWO_NODE_TYPE){
					if (node->right->type == TWO_NODE_TYPE){
						node->right->second_value = node->right->first_value;
						node->right->first_value = node->first_value;
						
						node->right->middle = node->right->left;
						node->right->left = node->left->left;
						
						free(node->left);
						node->left = NULL;
						__swap_branch_pointers(&node->left, &node->right);
						
						node->left->type = THREE_NODE_TYPE;
						return NULL;
					}
					else{ // node->right->type == THREE_NODE_TYPE
						node->left->first_value = node->first_value;
						node->first_value = node->right->first_value;
						node->right->first_value = node->right->second_value;
						
						node->left->right = node->right->left;
						node->right->left = node->right->middle;
						node->right->middle = NULL;
						
						node->right->type = TWO_NODE_TYPE;
					}
				}
				else { // if node->type == THREE_NODE_TYPE
					if (node->middle->type == TWO_NODE_TYPE){
						node->middle->second_value = node->middle->first_value;
						node->middle->first_value = node->first_value;
						node->first_value = node->second_value;
						
						node->type = TWO_NODE_TYPE;
						node->middle->type = THREE_NODE_TYPE;
						
						node->middle->middle = node->middle->left;
						node->middle->left = node->left->left;
						
						free(node->left);
						
						node->left = node->middle;
						node->middle = NULL;
					}
					else { // node->middle->type == THREE_NODE_TYPE
						// find lowest in middle
						TwoThreeTreeNode* node_iter = node->middle;
						for (;node_iter->left != NULL; node_iter = node_iter->left);
						
						if (node->left->left != NULL){
							if (__is_two_three_node_leaf(node->left->left)){
								node->left->first_value = node->left->left->second_value;
		
								node->left->right = __two_three_node_new(node->first_value);
							
								node->left->left->type = TWO_NODE_TYPE;
							}
							else{
								TwoThreeTree subtree = NULL;
								node->left->left = __two_three_tree_insert(node->left->left, node->first_value);
								__reconstruct_tree(&subtree, node->left->left);
								node->left = subtree;
							}
						}
						else{
							node->left->first_value = node->first_value;
						}
						
						__swap_int_values(&node->first_value, &node_iter->first_value);
						
						buf_node = __two_three_node_remove(node->middle, node_iter->first_value);
						if (buf_node != NULL){
							node->middle = buf_node;
							return node;
						}
						else{
							node_info.next_node_p = node->middle;
						}
					}
				}
			}
			
			if (node->right == node_info.next_node_p){
				if (node->type == TWO_NODE_TYPE){
					if (node->left->type == TWO_NODE_TYPE){
						node->left->type = THREE_NODE_TYPE;
						node->left->second_value = node->first_value;
						
						node->left->middle = node->left->right;
						node->left->right = node->right->left;
						
						free(node->right);
						node->right = NULL;
						
						
						return NULL;
					}
					else{ // node->left->type == THREE_NODE_TYPE
						node->right->first_value = node->first_value;
						node->first_value = node->left->second_value;
						
						node->right->right = node->right->left;
						node->right->left = node->left->right;
						node->left->right = node->left->middle;
						node->left->middle = NULL;
						
						node->left->type = TWO_NODE_TYPE;
					}
				}
				else { // if node->type == THREE_NODE_TYPE
					if (node->middle->type == TWO_NODE_TYPE){
						node->middle->second_value = node->second_value;
						
						node->type = TWO_NODE_TYPE;
						node->middle->type = THREE_NODE_TYPE;
						
						node->middle->middle = node->middle->right;
						node->middle->right = node->right->left;
						
						free(node->right);
						
						node->right = node->middle;
						node->middle = NULL;
					}
					else { // node->middle->type == THREE_NODE_TYPE
						// find greatest in middle
						TwoThreeTreeNode* node_iter = node->middle;
						for (;node_iter->right != NULL; node_iter = node_iter->right);
						
						if (node->right->left != NULL){
							if (__is_two_three_node_leaf(node->right->left)){
								node->right->first_value = node->right->left->first_value;
								
								node->right->left->first_value = node->right->left->second_value;
		
								node->right->right = node->right->left;
								node->right->left = __two_three_node_new(node->second_value);
							
								node->right->right->type = TWO_NODE_TYPE;
							}
							else{
								TwoThreeTree subtree = NULL;
								node->right->left = __two_three_tree_insert(node->right->left, node->second_value);
								__reconstruct_tree(&subtree, node->right->left);
								node->right = subtree;
							}
						}
						else{
							node->right->first_value = node->second_value;
						}
						
						if (node_iter->type == TWO_NODE_TYPE){
							__swap_int_values(&node->second_value, &node_iter->first_value);
							buf_node = __two_three_node_remove(node->middle, node_iter->first_value);
						}
						else{
							__swap_int_values(&node->second_value, &node_iter->second_value);
							buf_node = __two_three_node_remove(node->middle, node_iter->second_value);
						}
						

						if (buf_node != NULL){
							node->middle = buf_node;
							return node;
						}
						else
							node_info.next_node_p = node->middle;
						
					}
				}
			}
			
			if (node->middle == node_info.next_node_p){
				
				if (node->right->type == THREE_NODE_TYPE){
					node->middle->first_value = node->second_value;
					node->second_value = node->right->first_value;
					node->right->first_value = node->right->second_value;
					
					node->middle->right = node->right->left;
					node->right->left = node->right->middle;
					node->right->middle = NULL;
					
					node->right->type = TWO_NODE_TYPE;
				}
				else if (node->left->type == THREE_NODE_TYPE){
					node->middle->first_value = node->first_value;
					node->first_value = node->left->second_value;
					
					node->middle->right = node->middle->left;
					node->middle->left = node->left->right;
					node->left->right = node->left->middle;
					node->left->middle = NULL;
					
					node->left->type = TWO_NODE_TYPE;
				}
				else{
					node->left->second_value = node->first_value;
					node->first_value = node->second_value;
					
					node->type = TWO_NODE_TYPE;
					node->left->type = THREE_NODE_TYPE;
					
					if (node->middle->left != NULL){
						node->left->middle = node->left->right;
						node->left->right = node->middle->left;
						
						
					}
					free(node->middle);
					node->middle = NULL;
				}
			}
		}
	}
	return node;
}

void two_three_tree_remove (TwoThreeTree* tree_p, int value){
	if (__is_two_three_node_leaf(*tree_p)){
		*tree_p = __two_three_node_remove(*tree_p, value);
	}
	else{
		TwoThreeTreeNode* result = __two_three_node_remove(*tree_p, value);
		if (result == NULL){
			TwoThreeTreeNode* tmp;
			if ((*tree_p)->left != NULL)
				tmp = (*tree_p)->left;
			else
				tmp = (*tree_p)->right;
			
			free(*tree_p);
			*tree_p = tmp;
		}
	}
}


static int __two_three_tree_print ( TwoThreeTreeNode* node, char side, int offset, int depth) {
	if (node == NULL)
		return 0;
	
	char str_val[50];
	if (node->type == TWO_NODE_TYPE)
		sprintf(str_val, "(%d)", node->first_value);
	else
		sprintf(str_val, "(%d | %d)", node->first_value, node->second_value);
	
	int width = strlen(str_val);
	
	int left  = __two_three_tree_print (node->left, 'l', offset, depth + 2);
	int middle = __two_three_tree_print (node->middle, 'm', offset + left + width/2, depth + 2);
	int right = __two_three_tree_print (node->right, 'r', offset + left + middle + width, depth + 2);

	mvaddstr(depth, offset + left + middle/2, str_val);

	if (depth && side == 'l') {
		for (int i = 0; i < width + left + right + middle; i++)
			mvaddch(depth - 1, offset + left + width / 2 + i, '-');
		mvaddch(depth - 1, offset  + left + width / 2 - 1, '.');
	}
	else if (depth && side == 'm'){
		mvaddch(depth - 1, offset + left + width / 2, '+');
	}
	else if (depth && side == 'r') {
		for (int i = 0; i < left + middle + width; i++)
			mvaddch(depth - 1, offset - width / 2 + i, '-');
		mvaddch(depth - 1, offset + middle + left + width / 2, '.');
	}

	return left + width + right + middle;
}


void two_three_tree_print (TwoThreeTree tree) {
	initscr();
	clear();
	refresh();
	curs_set(0);

	__two_three_tree_print (tree, 0, 5, 0);
	
	refresh();
	getch();
	endwin();
}
