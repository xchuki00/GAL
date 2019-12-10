//
// Created by Patrik Chukir on 10.12.19.
// xchuki00@stud.fit.vutbr.cz
// Traveling Salesman Problem - GAL
//

#ifndef OGDF_USER_EXAMPLE_TSP_EXACT_H
#define OGDF_USER_EXAMPLE_TSP_EXACT_H

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

class TSP_Exact {
    EdgeWeightedGraph<float> G;
    GraphAttributes GA;
    std::vector<node> EC;
    int MSTIndex = 0;
    TSP_Exact();
    bool load(string filename);
    void solveTSD();
    bool save(string filename);
    bool saveSvg(string filename);
};


#endif //OGDF_USER_EXAMPLE_TSP_EXACT_H
