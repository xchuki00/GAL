//
// Created by Patrik Chukir on 10.12.19.
// xchuki00@stud.fit.vutbr.cz
// Traveling Salesman Problem - GAL
//

#include "TSP_Exact.h"

using namespace ogdf;

bool TSP_Exact::load(string filename) {

    this->GA = GraphAttributes(
            this->G,
            GraphAttributes::nodeGraphics |
            GraphAttributes::nodeStyle |
            GraphAttributes::nodeLabel |
            GraphAttributes::edgeGraphics |
            GraphAttributes::edgeStyle |
            GraphAttributes::edgeArrow |
            GraphAttributes::edgeLabel |
            GraphAttributes::edgeArrow);

    if (!GraphIO::read(this->GA, this->G, filename, GraphIO::readGML)) {
        std::cerr << "Could not load " << filename << std::endl;
        return false;
    }
    return true;
}

bool TSP_Exact::save(string filename) {
    this->GA.directed() = true;
    for (edge e : this->G.edges) {
        this->GA.arrowType(e) = EdgeArrow::Last;
    }
    std::ofstream gml(filename );
    GraphIO::writeGML(this->GA, gml);
    return true;
}

bool TSP_Exact::saveSvg(string filename) {
    this->GA.directed() = true;
    for (edge e : this->G.edges) {
        this->GA.arrowType(e) = EdgeArrow::Last;
    }
    std::ofstream svg(filename);
    GraphIO::drawSVG(this->GA, svg);
    return true;
}
void TSP_Exact::solveTSD() {

}

TSP_Exact::TSP_Exact() {

}


