//
// Created by patrik on 22.11.19.
//

#include "../include/GraphGenerator.h"
#include <fstream>

using namespace ogdf;

GraphGenerator::GraphGenerator() {

}

void GraphGenerator::generateCompleteGraph(string fileName,int n,int m) {
    EdgeWeightedGraph<float> G;

    randomSimpleConnectedGraph(G, n, m);

    GraphAttributes GA(G,
                       GraphAttributes::nodeGraphics |
                       GraphAttributes::nodeStyle |
                       GraphAttributes::nodeLabel |
                       GraphAttributes::edgeGraphics |
                       GraphAttributes::edgeStyle |
                       GraphAttributes::edgeLabel);

    GA.directed() = false;

    for (edge e : G.edges) {
        GA.bends(e);
        int weigth = rand() % 100;
        G.setWeight(e, weigth);
        GA.strokeColor(e) = Color::Name::Black;
        GA.label(e) = to_string((int)G.weight(e));
        GA.strokeWidth(e) = 1;
    }

    for (node v : G.nodes) {
        GA.fillColor(v) = Color("#FFFF00"); // set node color to yellow

        GA.height(v) = 20.0; // set the height to 20.0
        GA.width(v) = 20.0; // set the width to 40.0
        GA.shape(v) = ogdf::Shape::Ellipse;
        GA.strokeWidth(v) = 1;
        GA.strokeColor(v) = Color::Name::Black;

        GA.label(v) = to_string(v->index());
    }

//    FMMMLayout fmmm;
//
//    fmmm.useHighLevelOptions(true);
//    fmmm.unitEdgeLength(44.0);
//    fmmm.newInitialPlacement(true);
//    fmmm.qualityVersusSpeed(FMMMOptions::QualityVsSpeed::GorgeousAndEfficient);
//
//    fmmm.call(GA);
    SugiyamaLayout SL; //Compute a hierarchical drawing of G (using SugiyamaLayout)
    SL.setRanking(new OptimalRanking);
    SL.setCrossMin(new MedianHeuristic);

    OptimalHierarchyLayout *ohl = new OptimalHierarchyLayout;

    SL.setLayout(ohl);
    SL.call(GA);
    std::ofstream gml(fileName+".gml");
    std::ofstream svg(fileName+".svg");

    GraphIO::writeGML(GA, gml);
    GraphIO::drawSVG(GA, svg);


}

