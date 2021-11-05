#ifndef BINARY_TREE_H
#define BINARY_TREE_H

typedef struct _linked_binary_tree LinkedBinaryTree;
typedef int(*CompareFun)(void*, void*);

// Binary tree save pointer of value, so
// if you want to save your value, you should give pointer to value copy

LinkedBinaryTree* linked_binary_tree_new(CompareFun cmp_function);
void linked_binary_tree_del(LinkedBinaryTree* tree);
void linked_binary_tree_insert(LinkedBinaryTree* tree, void* val);
void linked_binary_tree_remove(LinkedBinaryTree* tree, void* val);
int linked_binary_tree_search(LinkedBinaryTree* tree, void* val);
void linked_binary_tree_print(LinkedBinaryTree* tree, char*(*value_to_str)(void*));
void linked_binary_tree_print_with_width(LinkedBinaryTree* tree, char*(*value_to_str)(void*)) ;

#endif
