/*
 * unionFind.h
 *
 *  Created on: Jun 4, 2015
 *      Author: bassam
 */
#ifndef UNIONFIND_H_
#define UNIONFIND_H_
/******************************************************************************/
/* libraries*/
#include <stdlib.h>
#include <stdio.h>
/******************************************************************************/
/*_DEFINES_*/
const int EMPTY = 0;
const int ILLEGAL_VALUE = -1;
const int INITIAL = 1;
/******************************************************************************/
/*typedefs*/
typedef enum {
	Success, Fail, MemoryError
} UnionResult;
/******************************************************************************/
/* SUMMARY:
 * A generic Union Find(disjoint sets) data structure implementation.
 * The structure stores the 'n' elements of the universe(i.e. |U|=n) in an-
 * array and distributes them to a forest of trees which serves the purpose of-
 * union(merging) and find.
 * The amortized time complexity of both operations(union and find),
 * in this implementation is log*(n)*/
/******************************************************************************/
/* This struct represents the atomic unit of the data structure of Union Find.
 * Basically, this struct contains info regarding a single element.
 * parameters:
 * @param data: the data which the element holds
 * @param size: number of elements with this current node is their parent-
 * 	 	 (i.e. root of sub-tree).
 * @param set: the sequential number of the set to which this node(with all of-
 * 		 its children) refer to
 * @param parent: pointer to the parent node of 'this' node.*/
template<typename T>
struct DisjointNode {
	T data;
	int set;
	DisjointNode<T>* parent;
	DisjointNode(T Data);
	DisjointNode(int set1);
	DisjointNode();
	~DisjointNode();
};
/******************************************************************************/
/*implementing struct DisjointNode functions: */
/******************************************************************************/
/*constructor*/
template<typename T>
DisjointNode<T>::DisjointNode() :
		data(T()), set(ILLEGAL_VALUE), parent(NULL) {
}
template<typename T>
DisjointNode<T>::DisjointNode(T Data) :
		data(Data), set(ILLEGAL_VALUE), parent(NULL) {
	parent = (DisjointNode<T>*) malloc(sizeof(DisjointNode<T> ));
}
template<typename T>
DisjointNode<T>::DisjointNode(int set1) :
		data(T()), set(set1), parent(NULL) {
}
/******************************************************************************/
template<typename T>
DisjointNode<T>::~DisjointNode() {
	free(parent);
}
/******************************************************************************/
/*
 * Struct to represent one unit of a set of multiple elements
 * They typename of this struct is no longer 'T' as a set should contain
 * completely different type of information, thus require another custom design.
 */
template<typename T, typename D>
struct Set {
	int size;
	D department;
	Set();
};
/******************************************************************************/
template<typename T, typename D>
Set<T, D>::Set() :
		size(EMPTY), department(D()) {
}
/******************************************************************************/
/*
 * Main class.
 * Parameters:
 * @param <typename T>: type of the elements which the structure maintains.
 * @param <typename D>: type of sets(departments)
 * @param n: size of the universe - number of elements in the system.
 * @param elements: array of disjoint nodes, means, each cell has a pointer to-
 * 		   the corresponding node in the up-tree.
 */
template<typename T, typename D>
class UnionFind {
private:
	int n;
	int aux_find(DisjointNode<T>* node);
	DisjointNode<T>* getUpTreeRoot(int ele);
	DisjointNode<T>* aux_getUpTreeRoot(DisjointNode<T>* node);
public:
	DisjointNode<T>* elements;
	Set<T, D>* sets;
	/* API */
	UnionFind(int N);
	~UnionFind() {
		destroy();
	}
	UnionResult unionSets(int ele1, int ele2);
	int find(int element);
	void destroy();
};
/******************************************************************************/
/*implementing class UnionFind methods and functions: */
/******************************************************************************/
/*constructor*/
template<typename T, typename D>
UnionFind<T, D>::UnionFind(int N) :
		n(N) {
	try {
		elements = new DisjointNode<T> [N];
		sets = new Set<T, D> [N];
		for (int i = 0; i < n; ++i) {
			elements[i] = DisjointNode<T>(i);
			sets[i].size = 1;
		}
	} catch (std::bad_alloc& e) {
	}
}
/******************************************************************************/
/* Deallocates the allocated memory*/
template<typename T, typename D>
void UnionFind<T, D>::destroy() {
	delete[] elements;
	delete[] sets;
}
/******************************************************************************/
/* Unifies two sets into one.
 * The smaller set joins the larger set.
 * @param ele1, ele2: the indices in the array 'elements' of the 2 elements which-
 * 			 we want to unify between their sets.
 * The small set attaches to the large set and both are attached to-
 * the set which ele1 belongs to. */
template<typename T, typename D>
UnionResult UnionFind<T, D>::unionSets(int ele1, int ele2) {
	if ((ele1 >= n) || (ele1 < 0) || (ele2 >= n) || (ele2 < 0)
			|| (find(ele1) == find(ele2))) {
		return Fail;
	}
	DisjointNode<T>* root1 = getUpTreeRoot(ele1);
	DisjointNode<T>* root2 = getUpTreeRoot(ele2);
	int size1 = (sets + root1->set)->size;
	int size2 = (sets + root2->set)->size;
	(sets + root1->set)->size = (size1 + size2);
	(sets + root2->set)->size = EMPTY;
	root1->set = find(ele1);
	root2->set = find(ele1);
	if ((size1 == EMPTY) && (size2 == EMPTY)) {
		return Success;
	}
	if (size1 <= size2) {
		root1->parent = root2;
	} else {
		root2->parent = root1;
	}
	return Success;
}
/******************************************************************************/
/* Returns the sequential number of the set to which the item element belongs.*/
template<typename T, typename D>
int UnionFind<T, D>::find(int element) {
	DisjointNode<T>* wantedNode = getUpTreeRoot(element);
	return wantedNode->set;
}
/******************************************************************************/
template<typename T, typename D>
int UnionFind<T, D>::aux_find(DisjointNode<T>* node) {
	return node->parent ? aux_find(node->parent) : node->set;
}
/******************************************************************************/
/* Returns a pointer to the up-tree node of the required team(element)*/
template<typename T, typename D>
DisjointNode<T>* UnionFind<T, D>::getUpTreeRoot(int ele) {
	DisjointNode<T>* node = elements + ele;
	return aux_getUpTreeRoot(node);
}
/******************************************************************************/
template<typename T, typename D>
DisjointNode<T>* UnionFind<T, D>::aux_getUpTreeRoot(DisjointNode<T>* node) {
	if (node->parent == NULL) {
		return node;
	}
	return aux_getUpTreeRoot(node->parent);
}
#endif /* UNIONFIND_H_ */
