/*
 * graph.h
 *
 *  Created on: May 9, 2016
 *      Author: dan
 */

#ifndef GRAPH_H_
#define GRAPH_H_


typedef int TCost; // Cost(weight) of path to a specific node (NOT USED IN THIS LAB)

typedef struct TNode{
	int v; // Destination node index
	struct TNode *next; // Link to the next node in the list
}TNode, *ATNode;

typedef struct{
	int nn; // Number of nodes in the graph
	ATNode *adl; // Adjancey list (TNode**)
}TGraphL;

#endif /* GRAPH_H_ */
