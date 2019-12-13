//
// Created by Patrik Chukir on 10.12.19.
// xchuki00@stud.fit.vutbr.cz
// Traveling Salesman Problem - GAL
//

#ifndef GAL_TSP_DOUBLETREE_H
#define GAL_TSP_DOUBLETREE_H

#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/layered/MedianHeuristic.h>
#include <ogdf/layered/OptimalHierarchyLayout.h>
#include <ogdf/layered/OptimalRanking.h>
#include <ogdf/layered/SugiyamaLayout.h>
#include <fstream>
#include <ogdf/graphalg/steiner_tree/common_algorithms.h>
#include <ogdf/graphalg/steiner_tree/EdgeWeightedGraph.h>
#include <ogdf/graphalg/steiner_tree/EdgeWeightedGraphCopy.h>
#include <cstdio>
using namespace ogdf;

class TSP_DoubleTree {
public:
    EdgeWeightedGraph<float> G;
    GraphAttributes GA;
    std::vector<node> EC;
    std::vector<edge> mst;
    int MSTIndex = 0;
    TSP_DoubleTree();
    bool load(string filename);
    void eulerianWalk();
    void minimumSpanningTree();
    void doubleTree();
    void solveTSD();
    bool save(string filename);
    bool saveSvg(string filename);
};


#endif //GAL_TSP_DOUBLETREE_H
