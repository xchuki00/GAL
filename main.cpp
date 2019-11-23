#include "include/TSP_DoubleTree.h"
#include "include/GraphGenerator.h"

int main()
{
//    GraphGenerator *genrator =  new GraphGenerator();
//    genrator->generateCompleteGraph("test",10,15);
    TSP_DoubleTree *t = new TSP_DoubleTree();
    t->load("test.gml");
    t->solveTSD();
    t->save("test-mst");
    return 0;
}