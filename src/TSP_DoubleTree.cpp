//
// Created by patrik on 22.11.19.
//

#include "../include/TSP_DoubleTree.h"

using namespace ogdf;

TSP_DoubleTree::TSP_DoubleTree() {
}

bool TSP_DoubleTree::load(string filename) {

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

bool TSP_DoubleTree::save(string filename) {
    this->GA.directed() = true;
    for (edge e : this->G.edges) {
        this->GA.arrowType(e) = EdgeArrow::Last;
    }
    std::ofstream svg(filename + ".svg");
    std::ofstream gml(filename + ".gml");

    GraphIO::writeGML(this->GA, gml);
    GraphIO::drawSVG(this->GA, svg);

    return true;
}

void TSP_DoubleTree::solveTSD() {
    this->minimumSpanningTree();
    this->doubleTree();
    this->eulerianWalk();
    auto nodeIndex = std::map<int, int>();
    for(node n:this->EC){
        try {
            int x = nodeIndex.at(n->index());
        }
        catch (const std::out_of_range& oor) {
            std::cout<<n->index()<<"->";
            nodeIndex.insert({n->index(), n->index()});
        }
    }
    std::cout<<"\n";

}

void TSP_DoubleTree::doubleTree() {
    int i = 0;
    int count = this->G.numberOfEdges();
    for (edge e:this->G.edges) {
        this->G.newEdge(e->target(), e->source(), this->G.weight(e));
        if (i >= count - 1) {
            this->MSTIndex = e->index();
            break;
        }
        i++;
    }

}

void TSP_DoubleTree::minimumSpanningTree() {
    makeMinimumSpanningTree(this->G, this->G.edgeWeights());
}

void TSP_DoubleTree::eulerianWalk() {
    auto A = std::map<int, std::vector<node>>();
    auto I = std::map<int, int>();
    for (node n:this->G.nodes) {
        A.insert({n->index(), std::vector<node>()});
        I.insert({n->index(), 0});
    }
    for(edge e:this->G.edges){
        if(e->index()<=this->MSTIndex){
            A.at(e->target()->index()).insert(A.at(e->target()->index()).end(),e->source());
        }else{
            A.at(e->target()->index()).insert(A.at(e->target()->index()).begin(),e->source());
        }
    }
    for (node n:this->G.nodes) {
        std::cout<<n->index()<<"->";
        for(node d:A.at(n->index())){
            std::cout<<d->index()<<",";
        }
        std::cout<<"\n";
    }
    this->EC = std::vector<node>();
    node u = this->G.firstNode();
    while(I.at(u->index())<u->indeg()){
        this->EC.insert(this->EC.begin(),u);
        I.at(u->index())++;
        u = A.at(u->index()).at(I.at(u->index())-1);

    }
    for(node n:this->EC){
        std::cout<<n->index()<<"->";
    }
    std::cout<<"\n";

}
