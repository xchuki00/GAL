#include "include/TSP_DoubleTree.h"
#include "include/GraphGenerator.h"
#include "include/TSP_k-OPT_Brute_force.h"
#include <chrono>

/**
 *
 * Parametrs
 * -g path - generate graph and save in file 'path'
 * -c X - generated graph will have X nodes
 *
 * -a [1,2] - solve TSP by algorithm 1(k-OPT) or 2(DoubleTree)
 * -k - unsigned int parameter needed for k-OPT algorithm
 * -i path - intput graph to solve
 * -o path - output gml file
 * -s path - output svg file
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char **argv) {
    string help(" \
            * Parametrs\n\
            * -g/--generate path - generate graph and save in file 'path'\n\
            * -c/--node-count X - generated graph will have X nodes\n\
            * -s/--output-svg path - save also as svg\
            *\n\
            * -a/--algorithm [1,2,3] - solve TSP by algorithm 1(k-OPT) or 2(DoubleTree) or 3(Brute-force)\n\
            * -k - unsigned int parameter needed for k-OPT algorithm\n\
            * -i/--input path - input graph to solve\
            * -o/--output path - output gml file\n\
            * -s/--output-svg path - output svg file");
    int algorithmNumber = 1, count = 10;
    int k = 2;
    string path;
    string output;
    string outputSvg;

    bool generate = false;
    for (int i = 0; i < argc; i++) {
        std::string s(argv[i]);
        if (s == "-g" || s == "--generate") {
            i++;
            generate = true;
            std::string value(argv[i]);
            path = value;
        }
        if (s == "-c" || s == "--node-count") {
            i++;
            std::string value(argv[i]);
            count = std::stoi(value);
        }
        if (s == "-a" || s == "--algorithm") {
            i++;
            std::string value(argv[i]);
            algorithmNumber = std::stoi(value);
        }
        if (s == "-i" || s == "--input") {
            i++;
            std::string value(argv[i]);
            path = value;
        }
        if (s == "-o" || s == "--output") {
            i++;
            std::string value(argv[i]);
            output = value;
        }
        if (s == "-s" || s == "--output-svg") {
            i++;
            std::string value(argv[i]);
            outputSvg = value;
        }
        if (s == "-k") {
            i++;
            std::string value(argv[i]);
            k = std::stoi(value);
        }
        if (s == "-h" || s == "--help") {
            cout << help << endl;
            return 0;
        }
    }


    if (generate) {
        auto g = new GraphGenerator();
        g->generateCompleteGraph(path, outputSvg, count);
        return 0;
    }


    if (algorithmNumber == 1) {
        CircularLinkedList *graph = new CircularLinkedList();

        if (!graph->LoadOgdfGraph(path)) {
            return -1;
        }
        if (!kOPT(k, graph)) {
            return -1;
        }
        if (output.length() > 0) {
            if (!graph->SaveOgdfGraphGML(output)) {
                return -1;
            }
        }
        if (outputSvg.length() > 0) {

            if (!graph->SaveOgdfGraphSVG(outputSvg)) {
                return -1;
            }
        }

        delete graph;
    } else if (algorithmNumber == 2) {
        TSP_DoubleTree *t = new TSP_DoubleTree();
        t->load(path);
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        t->solveTSD();
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        std::cout << "TIME: "<< std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count() << "[ns]"
                  << std::endl;

        if (output.length() > 0) {
            t->save(output);
        }
        if (outputSvg.length() > 0) {
            t->saveSvg(outputSvg);
        }
    } else if (algorithmNumber == 3) {
        CircularLinkedList *graph = new CircularLinkedList();

        if (!graph->LoadOgdfGraph(path)) {
            return -1;
        }

        if (!BruteForce(graph)) {
            return -1;
        }
        if (output.length() > 0) {
            if (!graph->SaveOgdfGraphGML(output)) {
                return -1;
            }
        }
        if (outputSvg.length() > 0) {

            if (!graph->SaveOgdfGraphSVG(outputSvg)) {
                return -1;
            }
        }
    }

    return 0;
}















