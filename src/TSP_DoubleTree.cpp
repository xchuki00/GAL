//
// Created by Patrik Chukir on 10.12.19.
// xchuki00@stud.fit.vutbr.cz
// Traveling Salesman Problem - GAL
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
    for (edge e : this->G.edges) {
        this->GA.arrowType(e) = EdgeArrow::Last;
        this->G.setWeight(e, std::stof(this->GA.label(e)));
    }
    return true;
}

bool TSP_DoubleTree::save(string filename) {
    this->GA.directed() = true;
    for (edge e : this->G.edges) {
        this->GA.label(e) = to_string((int) this->G.weight(e));
        this->GA.arrowType(e) = EdgeArrow::None;
    }
    std::ofstream gml(filename);
    GraphIO::writeGML(this->GA, gml);
    return true;
}

bool TSP_DoubleTree::saveSvg(string filename) {
    this->GA.directed() = true;
    for (edge e : this->G.edges) {
        this->GA.label(e) = to_string((int) this->G.weight(e));
        this->GA.arrowType(e) = EdgeArrow::None;
    }
    std::ofstream svg(filename);
    SugiyamaLayout SL; //Compute a hierarchical drawing of G (using SugiyamaLayout)
    SL.setRanking(new OptimalRanking);
    SL.setCrossMin(new MedianHeuristic);

    OptimalHierarchyLayout *ohl = new OptimalHierarchyLayout;

    SL.setLayout(ohl);
    SL.call(GA);
    GraphIO::drawSVG(this->GA, svg);
    return true;
}

void TSP_DoubleTree::solveTSD() {
    this->minimumSpanningTree();
    this->doubleTree();
    this->eulerianWalk();
    auto nodeIndex = std::map<int, int>();
    auto hamiltonCirculr = EdgeArray<bool>();
    hamiltonCirculr.init(this->G, false);
    node n,source=NULL,target;
    for (int i=0;i<this->EC.size();i++) {
        n = this->EC[i];
        try {
            int x = nodeIndex.at(n->index());
        }
        catch (const std::out_of_range &oor) {
            if(source == NULL){
                source = this->EC[i];
            }else{
                target = this->EC[i];
                auto e = this->G.searchEdge(source,target);
                hamiltonCirculr[e]=true;
                source = target;
            }
            std::cout << n->index() << "->";
            nodeIndex.insert({n->index(), n->index()});
        }

    }
    auto e = this->G.searchEdge(source,this->EC[0]);
    hamiltonCirculr[e]=true;
    std::cout << "\n";
    auto toDel = std::vector<edge>();
    float price = 0;
    for(auto e: this->G.edges){
        if(hamiltonCirculr[e]){
            price += this->G.weight(e);
        }else{
            toDel.push_back(e);
        }
    }
    for(auto e:toDel){
        this->G.delEdge(e);
    }
    std::cout<<"PRICE: "<<price<<std::endl;
}
/**
 * složitost mst_e
 */
void TSP_DoubleTree::doubleTree() {
    int i = 0;
    int count = this->G.numberOfEdges();
    for (edge e:this->mst) {
        auto newE = this->G.newEdge(e->target(), e->source(), this->G.weight(e));
        this->mst.push_back(newE);
        if (i >= count - 1) {
            this->MSTIndex = e->index();
            break;
        }
        i++;
    }
}
/**
 * složitost prim alg + e
 */
void TSP_DoubleTree::minimumSpanningTree() {
    auto inTree = EdgeArray<bool>();
    computeMinST(this->G, this->G.edgeWeights(), inTree);

    for (edge e:this->G.edges) {  //e
        if (inTree[e]) {
            this->mst.push_back(e);
        }
    }
}

/**
 * složitost n+m+2*(n-1)*
 */
void TSP_DoubleTree::eulerianWalk() {
    auto A = std::map<int, std::vector<node>>();
    auto I = std::map<int, int>();
    for (node n:this->G.nodes) {
        A.insert({n->index(), std::vector<node>()});
        I.insert({n->index(), 0});
    }
    for (edge e:this->mst) {
        if (e->index() <= this->MSTIndex) {
            A.at(e->target()->index()).insert(A.at(e->target()->index()).end(), e->source());
        } else {
            A.at(e->target()->index()).insert(A.at(e->target()->index()).begin(), e->source());
        }
    }
    this->EC = std::vector<node>();
    node u = this->G.firstNode();
    while (I.at(u->index()) < u->indeg()) {
        this->EC.insert(this->EC.begin(), u);
        I.at(u->index())++;
        auto newU = A.at(u->index()).at(I.at(u->index()) - 1);
        auto alreadyVisited = std::find(this->EC.begin(), this->EC.end(), newU) != this->EC.end(); // log n
        if (alreadyVisited  && A.at(u->index()).size() > I.at(u->index())) {
            auto it = A.at(u->index()).begin() + I.at(u->index()) - 1;
            std::rotate(it, it + 1, A.at(u->index()).end());
            newU = A.at(u->index()).at(I.at(u->index()) - 1);
        }
        u = newU;
    }
}


