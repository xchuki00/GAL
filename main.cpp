#include "include/TSP_DoubleTree.h"

int main()
{
    TSP_DoubleTree *t = new TSP_DoubleTree();
    t->load("output-ERDiagram.gml");
    t->save("output-ERDiagram.gml");
    return 0;
}