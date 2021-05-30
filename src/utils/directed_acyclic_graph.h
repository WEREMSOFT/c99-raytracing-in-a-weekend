#ifndef __DIRECTED_ACYCLIC_GRAPH_H__
#define __DIRECTED_ACYCLIC_GRAPH_H__

#include <memory.h>
#include "array_header.h"

struct Node;

typedef struct Node {
    int parent;
    void(*update)(struct Node*);
    int value;
} Node;

typedef struct DAG {
    ArrayHeader header;
    Node nodes[1];
} DAG;

void nodeUpdate(Node* self){
    printf("updating node\n");
    self->value++;
}

Node nodeCreate(){
    Node self = {0};
    self.update = nodeUpdate;
    return self;
}

// recomended initial capacity 10
DAG* DAGInit(int initialCapacity){
    size_t size = sizeof(Node) * initialCapacity + sizeof(ArrayHeader);
    DAG* dag = (DAG*)malloc(size);

    if(!dag){
        printf("Error allocation memory for DAG %s::%d\n", __FILE__, __LINE__);
        exit(-1);
    }

    memset(dag, 0, size);

    dag->header.capacity = initialCapacity;
    dag->header.elementSize = sizeof(Node);
    dag->header.length = 0;
    return dag;
}

void DAGInsertNode(DAG** self, Node element){
    if((*self)->header.length + 1 == (*self)->header.capacity){
        *self = realloc(*self, (*self)->header.capacity * (*self)->header.elementSize * 2 + sizeof(ArrayHeader));
        if(*self == NULL) {
            printf("Error reallocating GameObjectDAG\n");
            exit(-1);
        } else {
            (*self)->header.capacity *= 2;
        }
    }
    if((*self)->header.length == 0) element.parent = -1;
    (*self)->nodes[(*self)->header.length++] = element;
}

int DAGAddChild(DAG* self, uint64_t parent, uint64_t child){
    if(parent <= child){
        self->nodes[child].parent = parent;
        return 0;
    } else {
        return -1;
    }
}

void DAGFini(DAG* self) {
    free(self);
}

static void nodePrint(Node nodes[], int nodeCount, int node, int indent) {
    if(node >= nodeCount) return;

    for(int i = 0; i < indent; i++) printf("\t");

    printf("%d\n", nodes[node].value);

    for(int i = node + 1; i < nodeCount; i++) {
        if(nodes[i].parent == node) nodePrint(nodes, nodeCount, i, indent + 1);
    }
}

void DAGPrint(DAG* self) {
    nodePrint(self->nodes, self->header.length, 0, 0);
}

#endif