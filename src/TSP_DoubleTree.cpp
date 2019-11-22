//
// Created by patrik on 22.11.19.
//

#include "../include/TSP_DoubleTree.h"

using namespace ogdf;

bool TSP_DoubleTree::load(string filename) {
    this->GA = GraphAttributes (this->G,
             GraphAttributes::nodeGraphics |
             GraphAttributes::edgeGraphics |
             GraphAttributes::nodeLabel |
             GraphAttributes::edgeStyle |
             GraphAttributes::nodeStyle |
             GraphAttributes::nodeTemplate
    );
    if (!GraphIO::read(this->GA, this->G, filename, GraphIO::readGML)) {
        std::cerr << "Could not load unix-history.gml" << std::endl;
        return false;
    }
    return true;
}

bool TSP_DoubleTree::save(string filename) {

    GraphIO::write(this->GA, filename, GraphIO::drawSVG);
    GraphIO::write(this->GA, "output-manual.svg", GraphIO::drawSVG);

    return true;
}

TSP_DoubleTree::TSP_DoubleTree() {

}
