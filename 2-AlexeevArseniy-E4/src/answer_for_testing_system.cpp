

// linked_binary_tree header
#ifndef BINARY_TREE_H
#define BINARY_TREE_H

typedef struct _linked_binary_tree LinkedBinaryTree;

// Binary tree save pointer of value, so
// if you want to save your value, you should give pointer to value copy

LinkedBinaryTree* linked_binary_tree_new();
void linked_binary_tree_del(LinkedBinaryTree* tree);
void linked_binary_tree_insert(LinkedBinaryTree* tree, int val);
void linked_binary_tree_remove(LinkedBinaryTree* tree, int val);
int linked_binary_tree_search(LinkedBinaryTree* tree, int val);

#endif
// -------------------------


// linked binary tree source
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


typedef struct _linked_binary_tree_node{
	struct _linked_binary_tree_node* left;
	struct _linked_binary_tree_node* right;
	int value;
} LinkedBinaryTreeNode;

struct _linked_binary_tree{
	LinkedBinaryTreeNode* start_node;
};

static LinkedBinaryTreeNode* __linked_binary_tree_node_new(int value){
	LinkedBinaryTreeNode* node = ( LinkedBinaryTreeNode*)malloc(sizeof( LinkedBinaryTreeNode ));
	if (node == NULL){
		perror("Allocating memory for new BinaryTree error");
		return NULL;
	}
	
	node->left = NULL;
	node->right = NULL;
	node->value = value;
	
	return node;
}

static void __linked_binary_tree_node_del (LinkedBinaryTreeNode* node){
	free(node);
}

static void __linked_tree_del (LinkedBinaryTreeNode* node){
	if (node == NULL)
		return;
	
	__linked_tree_del(node->left);
	__linked_tree_del(node->right);
	
	__linked_binary_tree_node_del(node);
}


static LinkedBinaryTreeNode* __linked_binary_tree_search_node(LinkedBinaryTree* tree, int val){
	if (tree->start_node != NULL){
		LinkedBinaryTreeNode* node = tree->start_node;
		int result;
		while (node != NULL){
			// value less current value
			if ((result = val - node->value) < 0){
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


LinkedBinaryTree* linked_binary_tree_new(){
	LinkedBinaryTree* tree = (LinkedBinaryTree*)malloc(sizeof(LinkedBinaryTree));
	if (tree == NULL){
		perror("Allocating memory for new LinkedBinaryTree error\n");
		return tree;
	}
	
	tree->start_node = NULL;
	
	return tree;
}


void linked_binary_tree_del(LinkedBinaryTree* tree){
	__linked_tree_del(tree->start_node);
	free(tree);
}

void __linked_binary_tree_insert_node(LinkedBinaryTree* tree, LinkedBinaryTreeNode* insert_node){
	if (insert_node == NULL)
		return;
	
	int cmp_res = 0;
	
	if (tree->start_node == NULL)
		tree->start_node = insert_node;
	else{
		LinkedBinaryTreeNode* node = tree->start_node;
		while (node != NULL){
			// value less current value
			if ((cmp_res = insert_node->value - node->value) < 0){
				if (node->left != NULL)
					node = node->left;
				else{
					node->left = insert_node;
					return;
				}
			}
			// value equal current value
			else if (cmp_res == 0){
				__linked_binary_tree_node_del(insert_node);
				return;
			}
			// value more then current value
			else 
				if (node->right != NULL)
					node = node->right;
				else{
					node->right = insert_node;
					return;
				}
		}
	}
}

void linked_binary_tree_insert(LinkedBinaryTree* tree, int val){
	LinkedBinaryTreeNode* new_node = __linked_binary_tree_node_new (val);
	__linked_binary_tree_insert_node(tree, new_node);
}

void linked_binary_tree_remove(LinkedBinaryTree* tree, int val){
	if (tree->start_node != NULL){
		LinkedBinaryTreeNode* parent = tree->start_node;
		LinkedBinaryTreeNode* node = tree->start_node;
		int result;
		int left_side_flag = 1;
		
		while (node != NULL){
			// value less current value
			if ((result = val - node->value) < 0){ 
				parent = node;
				node = node->left;
				left_side_flag = 1;
			}
			// value equal current value
			else if (result == 0){
				if (node == tree->start_node){
					if (tree->start_node->left != NULL){
						tree->start_node = tree->start_node->left;
						__linked_binary_tree_insert_node(tree, node->right);
					}
					else
						tree->start_node = tree->start_node->right;
				}
				else{
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
			else{
				parent = node;
				node = node->right;
				left_side_flag = 0;
			}
		}
	}
}

int linked_binary_tree_search(LinkedBinaryTree* tree, int val){
	LinkedBinaryTreeNode* search_res = __linked_binary_tree_search_node (tree, val);
	if (search_res != NULL)
		return 1;
	return 0;
}
// -------------------------



#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main(){
	int val = 0;
	char comand = '0';
	char line[100] = {'\0'};
	
	LinkedBinaryTree* tree = linked_binary_tree_new();
	
	while (fgets(line, 100, stdin) != NULL){
		if (sscanf(line ,"%c %d\n", &comand, &val) != 2)
			return 0;
		
		int* int_box = NULL;
		switch(comand){
			case 'a':
				linked_binary_tree_insert(tree, val);
				break;
			case 'r':
				linked_binary_tree_remove(tree, val);
				break;
			case 'f':
				if (linked_binary_tree_search(tree, val))
					puts("yes");
				else
					puts("no");
				break;
		}
	}
	return 0;
}
