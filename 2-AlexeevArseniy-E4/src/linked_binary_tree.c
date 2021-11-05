#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ncurses.h>
#include <unistd.h>
#include "linked_binary_tree.h"


typedef struct _linked_binary_tree_node {
	struct _linked_binary_tree_node* left;
	struct _linked_binary_tree_node* right;
	void* value;
} LinkedBinaryTreeNode;

struct _linked_binary_tree {
	LinkedBinaryTreeNode* start_node;
	CompareFun cmp_fun;
};

static LinkedBinaryTreeNode* __linked_binary_tree_node_new(void* value) {
	LinkedBinaryTreeNode* node = (LinkedBinaryTreeNode*)malloc(sizeof(LinkedBinaryTreeNode));

	if (node == NULL) {
		perror("Allocating memory for new BinaryTree error");
		return NULL;
	}

	node->left = NULL;
	node->right = NULL;
	node->value = value;

	return node;
}

static void __linked_binary_tree_node_del(LinkedBinaryTreeNode* node) {
	free(node->value);
	free(node);
}

static void __linked_tree_del(LinkedBinaryTreeNode* node) {
	if (node == NULL)
		return;

	__linked_tree_del(node->left);
	__linked_tree_del(node->right);

	__linked_binary_tree_node_del(node);
}


static LinkedBinaryTreeNode* __linked_binary_tree_search_node(LinkedBinaryTree* tree, void* val) {
	if (tree->start_node != NULL) {
		LinkedBinaryTreeNode* node = tree->start_node;
		int result;

		while (node != NULL) {
			// value less current value
			if ((result = tree->cmp_fun(val, node->value)) < 0) {
				node = node->left;
			}
			// value equal current value
			else if (result == 0)
				return node;
			// value more current value
			else
				node = node->right;
		}
	}

	return NULL;
}


LinkedBinaryTree* linked_binary_tree_new(CompareFun cmp_function) {
	if (cmp_function == NULL) {
		perror("Error argument: compare function can not be NULL\n");
		return NULL;
	}

	LinkedBinaryTree* tree = (LinkedBinaryTree*)malloc(sizeof(LinkedBinaryTree));

	if (tree == NULL) {
		perror("Allocating memory for new LinkedBinaryTree error\n");
		return tree;
	}

	tree->start_node = NULL;
	tree->cmp_fun = cmp_function;

	return tree;
}


void linked_binary_tree_del(LinkedBinaryTree* tree) {
	__linked_tree_del(tree->start_node);
	free(tree);
}

void __linked_binary_tree_insert_node(LinkedBinaryTree* tree, LinkedBinaryTreeNode* insert_node) {
	if (insert_node == NULL)
		return;

	int cmp_res = 0;

	if (tree->start_node == NULL)
		tree->start_node = insert_node;
	else {
		LinkedBinaryTreeNode* node = tree->start_node;

		while (node != NULL) {
			// value less current value
			if ((cmp_res = tree->cmp_fun(insert_node->value, node->value)) < 0) {
				if (node->left != NULL)
					node = node->left;
				else {
					node->left = insert_node;
					return;
				}
			}
			// value equal current value
			else if (cmp_res == 0) {
				__linked_binary_tree_node_del(insert_node);
				return;
			}
			// value more then current value
			else if (node->right != NULL)
				node = node->right;
			else {
				node->right = insert_node;
				return;
			}
		}
	}
}

void linked_binary_tree_insert(LinkedBinaryTree* tree, void* val) {
	LinkedBinaryTreeNode* new_node = __linked_binary_tree_node_new(val);
	__linked_binary_tree_insert_node(tree, new_node);
}

void linked_binary_tree_remove(LinkedBinaryTree* tree, void* val) {
	if (tree->start_node != NULL) {
		LinkedBinaryTreeNode* parent = tree->start_node;
		LinkedBinaryTreeNode* node = tree->start_node;
		int result;
		int left_side_flag = 1;

		while (node != NULL) {
			// value less current value
			if ((result = tree->cmp_fun(val, node->value)) < 0) {
				parent = node;
				node = node->left;
				left_side_flag = 1;
			}
			// value equal current value
			else if (result == 0) {
				if (node == tree->start_node) {
					if (tree->start_node->left != NULL) {
						tree->start_node = tree->start_node->left;
						__linked_binary_tree_insert_node(tree, node->right);
					}
					else
						tree->start_node = tree->start_node->right;
				}
				else {
					if (left_side_flag)
						parent->left = NULL;
					else
						parent->right = NULL;

					__linked_binary_tree_insert_node(tree, node->left);
					__linked_binary_tree_insert_node(tree, node->right);
				}

				__linked_binary_tree_node_del(node);
				return;
			}
			// value more current value
			else {
				parent = node;
				node = node->right;
				left_side_flag = 0;
			}
		}
	}
}

int linked_binary_tree_search(LinkedBinaryTree* tree, void* val) {
	LinkedBinaryTreeNode* search_res = __linked_binary_tree_search_node(tree, val);
	
	if (search_res != NULL)
		return 1;
	return 0;
}

static int __linked_binary_tree_print(LinkedBinaryTreeNode* node, char*(*value_to_str)(void*), int is_left, int offset, int depth) {

	if (node == NULL)
		return 0;

	char* str_val = (*value_to_str)(node->value);
	int width = strlen(str_val);
	
	int left  = __linked_binary_tree_print(node->left, value_to_str, 1, offset, depth + 1);
	int right = __linked_binary_tree_print(node->right, value_to_str, 0, offset + left + width, depth + 1);

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

	free(str_val);
	return left + width + right;
}

void linked_binary_tree_print(LinkedBinaryTree* tree, char*(*value_to_str)(void*)) {
	initscr();
	curs_set(0);

	__linked_binary_tree_print(tree->start_node, value_to_str, 0, 0, 0);
	
	refresh();
	getch();
	endwin();
}

typedef struct{
	int val;
	int subtree_width;
}RecursionReturn;

static int max(int val1, int val2){
	if (val1 > val2)
		return val1;
	return val2;
}

static RecursionReturn __linked_binary_tree_print_with_width(LinkedBinaryTreeNode* node, char*(*value_to_str)(void*), int is_left, int offset, int depth, int max_len_for_subtree_width) {
	RecursionReturn ret;
	ret.subtree_width = 0;
	
	if (node == NULL){
		ret.val = 0;
		return ret;
	}
	
	
	char* str_val = (*value_to_str)(node->value);
	int str_len = strlen(str_val);
	int max_width = max(str_len, max_len_for_subtree_width);

	RecursionReturn left  = __linked_binary_tree_print_with_width(node->left,
									value_to_str,
									1, offset,
									depth + 2,
									max_len_for_subtree_width);
	
	RecursionReturn right = __linked_binary_tree_print_with_width(node->right,
									value_to_str, 0,
									offset + left.val + max_width,
									depth + 2,
									max_len_for_subtree_width);

	
	ret.subtree_width += left.subtree_width + right.subtree_width;
	char int_str[25] = {'\0'};
	sprintf(int_str, "{%d}", ret.subtree_width);
	int int_str_len = strlen(int_str);

	if (str_len < int_str_len){
		mvaddstr(depth, offset + left.val + (int_str_len - str_len) / 2, str_val);
		mvaddstr(depth + 1, offset + left.val, int_str);
	}
	else{
		mvaddstr(depth, offset + left.val, str_val);
		mvaddstr(depth + 1, offset + left.val + (str_len - int_str_len) / 2, int_str);
	}

	if (depth && is_left) {
		for (int i = 0; i < max_width + right.val; i++)
			mvaddch(depth - 1, offset + left.val + max_width / 2 + i, '-');
		mvaddch(depth - 1, offset + left.val + max_width / 2, '.');
	}
	else if (depth && !is_left) {
		for (int i = 0; i < left.val + max_width; i++)
			mvaddch(depth - 1, offset - max_width / 2 + i, '-');
		mvaddch(depth - 1, offset + left.val + max_width / 2, '.');
	}
	
	ret.subtree_width += str_len;
	ret.val = left.val + max_width + right.val;
	free(str_val);
	return ret;
}

static int __max_subtree_width(LinkedBinaryTreeNode* node, char*(*value_to_str)(void*)){
	if (node == NULL)
		return 0;
	
	char* subtree_string = value_to_str(node->value);
	int subtree_width = strlen(subtree_string);
	if (node->left == NULL && node->right == NULL){
		free(subtree_string);
		return subtree_width;
	}
	free(subtree_string);
	return __max_subtree_width(node->left, value_to_str) + __max_subtree_width(node->right, value_to_str) + subtree_width;
}

void linked_binary_tree_print_with_width(LinkedBinaryTree* tree, char*(*value_to_str)(void*)) {
	initscr();
	curs_set(0);
	clear();
	char int_str[25];
	sprintf(int_str, "(%d)", __max_subtree_width(tree->start_node, value_to_str));
	__linked_binary_tree_print_with_width(tree->start_node,
										value_to_str,
										0, 0, 0,
										strlen(int_str));
	
	refresh();
	getch();
	endwin();
}
