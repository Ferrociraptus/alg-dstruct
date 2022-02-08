//---------------------AABinaryTree.h---------------------
#ifndef AATREE_H
#define AATREE_H

typedef struct _AABinaryTreeNode* AABinaryTree;
void aa_binary_tree_init(AABinaryTree* tree);
void aa_binary_tree_insert(AABinaryTree* tree_p, int value);
void aa_binary_tree_remove(AABinaryTree* tree, int del_value);
int aa_binary_tree_search(AABinaryTree tree, int value);

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

static AABinaryTreeNode* bottom = NULL;
static AABinaryTreeNode* last = NULL, *deleted = NULL;

void aa_binary_tree_init(AABinaryTree* tree){
	if (bottom == NULL){
		bottom = (AABinaryTreeNode*)malloc(sizeof(AABinaryTreeNode));
		if (bottom == NULL)
			return;
		bottom->value = 0;
		bottom->depth = 0;
		bottom->left = bottom;
		bottom->right = bottom;
		deleted = bottom;
	}
	*tree = NULL;
}


static AABinaryTreeNode* __aa_binary_treeNode_new(int value){
	AABinaryTreeNode* node = (AABinaryTreeNode*)malloc(sizeof(AABinaryTreeNode));
	
	if (node == NULL)
		return NULL;
	
	node->depth = 1;
	node->left = bottom;
	node->right = bottom;
	node->value = value;
	return node;
}


static AABinaryTreeNode* __aa_binary_treeNode_split(AABinaryTree tree){
	if (tree != bottom && tree->depth == tree->right->right->depth){
	   AABinaryTreeNode* temp = tree;
	   tree = tree->right;
	   temp->right = tree->left;
	   tree->left = temp;
	   tree->depth++;
	   return tree;
	}
	return tree;
}


static AABinaryTreeNode* __aa_binary_treeNode_skew(AABinaryTree tree){
	if (tree->left->depth == tree->depth){
		AABinaryTreeNode* temp = tree;
		tree = tree->left;
		temp->left = tree->right;
		tree->right = temp;
		return tree;
	}
	else
		return tree;
}


static AABinaryTreeNode* __aa_binary_tree_insert_node(AABinaryTree tree, AABinaryTreeNode* new_node){
	if (tree == bottom || tree == NULL)
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


static AABinaryTreeNode* __aa_binary_tree_remove(AABinaryTree tree, int del_value){
	if (tree != bottom){
		last = tree;
		if (del_value < tree->value){
			tree->left = __aa_binary_tree_remove(tree->left, del_value);
		}
		else {
			deleted = tree;
			tree->right = __aa_binary_tree_remove(tree->right, del_value);
		}
		
		if ((tree == last) && (deleted != bottom) && (deleted->value == del_value)){
			deleted->value = tree->value;
			deleted = bottom;
			tree = tree->right;
			free(last);
		}
		else if ((tree->left->depth < tree->depth - 1) || 
			(tree->right->depth < tree->depth -1)){
			tree->depth--;
			if (tree->right->depth > tree->depth){
				tree->right->depth = tree->depth;
			}
			tree = __aa_binary_treeNode_skew(tree);
			tree->right = __aa_binary_treeNode_skew(tree->right);
			tree->right->right = __aa_binary_treeNode_skew(tree->right->right);
			tree = __aa_binary_treeNode_split(tree);
			tree->right = __aa_binary_treeNode_split(tree->right);
		}
	}
	return tree;
}

void aa_binary_tree_remove(AABinaryTree* tree, int del_value){
	*tree = __aa_binary_tree_remove(*tree, del_value);
}


int aa_binary_tree_search(AABinaryTree tree, int search_value){
	AABinaryTreeNode* node = tree;
	while(node != bottom){
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
	aa_binary_tree_init(&tree);
	
	while (scanf("%c %d%*c", &command, &value) >= 1){
		switch(command){
			case 'a':
				aa_binary_tree_insert(&tree, value);
				break;
			case 'r':
				aa_binary_tree_remove(&tree, value);
				break;
			case 'f':
				if (aa_binary_tree_search(tree, value))
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
