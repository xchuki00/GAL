//
// Created by patrik on 22.11.19.
//

#ifndef OGDF_USER_EXAMPLE_GRAPHGENERATOR_H
#define OGDF_USER_EXAMPLE_GRAPHGENERATOR_H

#include <ogdf/basic/Graph.h>
#include <ogdf/basic/graph_generators.h>
#include <ogdf/layered/DfsAcyclicSubgraph.h>
#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/layered/SugiyamaLayout.h>
#include <ogdf/layered/OptimalRanking.h>
#include <ogdf/layered/MedianHeuristic.h>
#include <ogdf/layered/OptimalHierarchyLayout.h>
#include <ogdf/energybased/FMMMLayout.h>

using namespace ogdf;

class GraphGenerator {
public:
    GraphGenerator();
    void generateCompleteGraph(std::string fileName,int n,int m);
};


#endif //OGDF_USER_EXAMPLE_GRAPHGENERATOR_H
