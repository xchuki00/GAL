//
// Created by Patrik Chukir on 10.12.19.
// xchuki00@stud.fit.vutbr.cz
// Traveling Salesman Problem - GAL
//

#include "../include/GraphGenerator.h"
#include <fstream>

using namespace ogdf;

GraphGenerator::GraphGenerator() {

}

void GraphGenerator::generateCompleteGraph(string fileName, string svgFile, int n) {
    EdgeWeightedGraph<float> G;

    completeGraph(G, n);
    GraphAttributes GA(G,
                       GraphAttributes::nodeGraphics |
                       GraphAttributes::nodeStyle |
                       GraphAttributes::nodeLabel |
                       GraphAttributes::edgeGraphics |
                       GraphAttributes::edgeStyle |
                       GraphAttributes::edgeArrow |
                       GraphAttributes::edgeLabel |
                       GraphAttributes::edgeArrow);

    GA.directed() = true;
    for (edge e : G.edges) {
        GA.bends(e);
        int weigth = rand() % 100+100;
        G.setWeight(e, weigth);
        GA.strokeColor(e) = Color::Name::Black;
        GA.label(e) = to_string((int) G.weight(e));
        GA.strokeWidth(e) = 1;
        GA.arrowType(e) = EdgeArrow::Last;
    }

    for (node v : G.nodes) {

        GA.shape(v) = ogdf::Shape::Ellipse;
        GA.strokeWidth(v) = 1;
        GA.strokeColor(v) = Color::Name::Black;
        GA.fillColor(v) = Color::Name::Yellow;

        GA.label(v) = to_string(v->index());
    }

    SugiyamaLayout SL; //Compute a hierarchical drawing of G (using SugiyamaLayout)
    SL.setRanking(new OptimalRanking);
    SL.setCrossMin(new MedianHeuristic);

    OptimalHierarchyLayout *ohl = new OptimalHierarchyLayout;

    SL.setLayout(ohl);
    SL.call(GA);
    std::ofstream gml(fileName);
    GraphIO::writeGML(GA, gml);
    if (svgFile.length() > 0) {
        std::ofstream svg(svgFile);
        GraphIO::drawSVG(GA, svg);
    }
}

