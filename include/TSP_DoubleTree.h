//
// Created by patrik on 22.11.19.
//

#ifndef GAL_TSP_DOUBLETREE_H
#define GAL_TSP_DOUBLETREE_H

#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/layered/MedianHeuristic.h>
#include <ogdf/layered/OptimalHierarchyLayout.h>
#include <ogdf/layered/OptimalRanking.h>
#include <ogdf/layered/SugiyamaLayout.h>

using namespace ogdf;

class TSP_DoubleTree {
public:
    Graph G;
    GraphAttributes GA;
    TSP_DoubleTree();
    bool load(string filename);
    bool save(string filename);
};


#endif //GAL_TSP_DOUBLETREE_H
