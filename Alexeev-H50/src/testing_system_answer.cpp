//---------------------AABinaryTree.h---------------------
#ifndef AATREE_H
#define AATREE_H

typedef struct _AABinaryTreeNode* AABinaryTree;
void aa_binary_tree_insert(AABinaryTree* tree_p, int value);
void aa_binary_tree_remove(AABinaryTree* tree, int del_value);
int aa_binary_tree_search(AABinaryTree tree, int value);
void aa_binary_tree_print(AABinaryTree tree);

#endif
//--------------------------------------------------------
//--------------------------------------------------------



//---------------------AABinaryTree.c---------------------
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct _AABinaryTreeNode {
	int value;
	struct _AABinaryTreeNode* left;
	struct _AABinaryTreeNode* right;
	int depth;
};

typedef struct _AABinaryTreeNode AABinaryTreeNode;


static AABinaryTreeNode* __aa_binary_treeNode_new(int value){
	AABinaryTreeNode* node = (AABinaryTreeNode*)malloc(sizeof(AABinaryTreeNode));
	
	if (node == NULL)
		return NULL;
	
	node->depth = 0;
	node->left = NULL;
	node->right = NULL;
	node->value = value;
	return node;
}


static AABinaryTreeNode* __aa_binary_treeNode_split(AABinaryTree tree){
	if (tree == NULL)
		return NULL;
	else if (tree->right == NULL || tree->right->right == NULL)
		return tree;
	else if (tree->depth == tree->right->right->depth){
       // Существует два правых горизонтальных ребра. Берем центральную вершину, «поднимаем» ее и возвращаем указатель на нее
	   
	   AABinaryTreeNode* right = tree->right;
	   tree->right = right->left;
	   right->left = tree;
	   right->depth++;
	   
	   return right;
	}
	return tree;
}


static AABinaryTreeNode* __aa_binary_treeNode_skew(AABinaryTree tree){
	if (tree == NULL)
		return NULL;
	else if (tree->left == NULL)
		return tree;
	else if (tree->left->depth == tree->depth){
		AABinaryTreeNode* left = tree->left;
		tree->left = left->right;
		left->right = tree;
		return left;
	}
	else
		return tree;
}


static AABinaryTreeNode* __aa_binary_tree_insert_node(AABinaryTree tree, AABinaryTreeNode* new_node){
	if (tree == NULL)
		return new_node;
	
	if (new_node->value > tree->value){
		tree->right = __aa_binary_tree_insert_node(tree->right, new_node);
	}
	else if (new_node->value == tree->value){
		free(new_node);
		return tree;
	}
	else {
		tree->left =  __aa_binary_tree_insert_node(tree->left, new_node);
	}
	tree = __aa_binary_treeNode_skew(tree);
	tree = __aa_binary_treeNode_split(tree);
	return tree;
}


void aa_binary_tree_insert(AABinaryTree* tree_p, int value){
	*tree_p = __aa_binary_tree_insert_node(*tree_p, __aa_binary_treeNode_new(value));
}


static AABinaryTreeNode* __aa_binary_tree_decrease_level(AABinaryTree tree){
	int left_depth = 0;
	int right_depth = 0;
	int need_depth;
	
	if (tree == NULL)
		return tree;
	
	if (tree->left != NULL)
		left_depth = tree->left->depth;
	if (tree->right != NULL)
		right_depth = tree->right->depth;

	need_depth = ((left_depth <= right_depth) ? left_depth : right_depth);
	if (need_depth < tree->depth){
		tree->depth = need_depth;
		if (need_depth < right_depth && tree->right != NULL)
			tree->right->depth = need_depth;
	}
           
	return tree;
}

static AABinaryTreeNode* __aa_binary_tree_get_predecessor(AABinaryTree tree){
	if (tree == NULL)
		return NULL;
	while (tree->right != NULL){
		tree = tree->right;
	}
	return tree;
}

static AABinaryTreeNode* __aa_binary_tree_get_successor(AABinaryTree tree){
	if (tree == NULL)
		return NULL;
	while (tree->left != NULL){
		tree = tree->left;
	}
	return tree;
}



static AABinaryTreeNode* __aa_binary_tree_remove(AABinaryTree tree, int del_value){
	if (tree == NULL)
		return NULL;
	else if (del_value > tree->value)
		tree->right = __aa_binary_tree_remove(tree->right, del_value);
	else if (del_value < tree->value)
		tree->left = __aa_binary_tree_remove(tree->left, del_value);
	else{
		if (tree->left == NULL && tree->right == NULL){
			free(tree);
			return NULL;
		}
		else if (tree->left == NULL){
			int successor_value = __aa_binary_tree_get_successor(tree->right)->value;
			tree->right = __aa_binary_tree_remove(tree->right, successor_value);
			tree->value = successor_value;
		}
		else{
			int predecessor_value = __aa_binary_tree_get_predecessor(tree->left)->value;
			tree->left = __aa_binary_tree_remove(tree->left, predecessor_value);
			tree->value = predecessor_value;
		}
	}
	
	tree = __aa_binary_tree_decrease_level(tree);
	tree = __aa_binary_treeNode_skew(tree);
	tree->right = __aa_binary_treeNode_skew(tree->right);
	if (tree->right != NULL){
		tree->right->right = __aa_binary_treeNode_skew(tree->right->right);
	}
	tree = __aa_binary_treeNode_split(tree);
	tree->right = __aa_binary_treeNode_split(tree->right);
	return tree;
}

void aa_binary_tree_remove(AABinaryTree* tree, int del_value){
	*tree = __aa_binary_tree_remove(*tree, del_value);
}


int aa_binary_tree_search(AABinaryTree tree, int search_value){
	AABinaryTreeNode* node = tree;
	while(node != NULL){
		if (node->value < search_value){
			node = node->right;
		}
		else if (node->value == search_value){
			return 1;
		}
		else {
			node = node->left;
		}
	}
	return 0;
}

//--------------------------------------------------------
//--------------------------------------------------------



//-------------------------main.c-------------------------
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


int main(){
	char command;
	int value;
	
	AABinaryTree tree = NULL;
	
	while (scanf("%c %d%*c", &command, &value) >= 1){
		switch(command){
			case 'a':
				aa_binary_tree_insert(&tree, value);
				break;
			case 'r':
				aa_binary_tree_remove (&tree, value);
				break;
			case 'f':
				if ( aa_binary_tree_search (tree, value))
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
//--------------------------------------------------------
//--------------------------------------------------------
