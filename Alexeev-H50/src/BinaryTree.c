#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ncurses.h>

#include "BinaryTree.h"

#define swap_value(val1, val2) {int buff = val1; val1 = val2; val2 = buff;}


typedef struct __BinaryTreeNode {
	int value;
	struct __BinaryTreeNode* left;
	struct __BinaryTreeNode* right;
} BinaryTreeNode;

static void __binary_tree_insert_node(BinaryTree* tree_p, BinaryTreeNode* new_node){
	if (new_node == NULL)
		return;
	
	if (*tree_p == NULL){
		*tree_p = new_node;
		return;
	}
	
	BinaryTree tree = *tree_p;
	
	BinaryTreeNode* node = tree;
	while(node != NULL){
		if (node->value > new_node->value){
			if (node->left == NULL){
				node->left = new_node;
				return;
			}
			else{
				node = node->left;
			}
		}
		else if (node->value == new_node->value){
			free(new_node);
			return;
		}
		else{
			if (node->right == NULL){
				node->right = new_node;
				return;
			}
			else{
				node = node->right;
			}
		}
	}
}


void binary_tree_insert(BinaryTree* tree, int value) {
	
	BinaryTreeNode* new_node = (BinaryTreeNode*)malloc(sizeof(BinaryTreeNode));
	if (new_node == NULL)
			return;
		
	(new_node)->value = value;
	(new_node)->left = NULL;
	(new_node)->right = NULL;
	
	
	__binary_tree_insert_node(tree, new_node);
}


int binary_tree_search(BinaryTree tree, int value){
	BinaryTreeNode* node = tree;
	
	while(node != NULL){
		if (node->value > value){
			node = node->left;
		}
		else if (node->value == value){
			return 1;
		}
		else{
			node = node->right;
		}
	}
	return 0;
}


void binary_tree_remove(BinaryTree* tree_p, int value){
	BinaryTreeNode* node = *tree_p;

	while(node != NULL){
		if (node->value > value){
			node = node->left;
		}
		else if (node->value == value){
			if (node->right != NULL){
				swap_value(node->value, node->right->value);
				if (node->right->left == NULL && node->right->right == NULL){
					free(node->right);
					node->right = NULL;
					return;
				}
				node = node->right;
				continue;
			}
			else if (node->left != NULL){
				swap_value(node->value, node->left->value);
				if (node->left->left == NULL && node->left->right == NULL){
					free(node->left);
					node->left = NULL;
					return;
				}
				node = node->left;
				continue;
			}
			
			if (node == *tree_p && node->left == NULL && node->right == NULL){
				free(node);
				*tree_p = NULL;
				return;
			}
			
		}
		else{
			node = node->right;
		}
	}
}


static int __binary_tree_print(BinaryTreeNode* node, int is_left, int offset, int depth) {
	if (node == NULL)
		return 0;
	
	char str_val[50];
	sprintf(str_val, "(%d)", node->value);
	int width = strlen(str_val);
	
	int left  = __binary_tree_print(node->left, 1, offset, depth + 1);
	int right = __binary_tree_print(node->right, 0, offset + left + width, depth + 1);

	mvaddstr(depth, offset + left, str_val);

	if (depth && is_left) {
		for (int i = 0; i < width + right; i++)
			mvaddch(depth - 1, offset + left + width / 2 + i, '-');
		mvaddch(depth - 1, offset + left + width / 2, '.');
	}
	else if (depth && !is_left) {
		for (int i = 0; i < left + width; i++)
			mvaddch(depth - 1, offset - width / 2 + i, '-');
		mvaddch(depth - 1, offset + left + width / 2, '.');
	}

	return left + width + right;
}


void binary_tree_print(BinaryTree tree) {
	initscr();
	clear();
	refresh();
	curs_set(0);

	__binary_tree_print(tree, 0, 5, 0);
	
	refresh();
	getch();
	endwin();
}


static void __binary_tree_clear(BinaryTreeNode* node){
	if (node == NULL)
		return;
	
	__binary_tree_clear(node->left);
	__binary_tree_clear(node->right);
	
	free(node);
}

void binary_tree_clear(BinaryTree* tree) {
	__binary_tree_clear(*tree);
	*tree = NULL;
}
