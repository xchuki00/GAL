#include "include/TSP_DoubleTree.h"
#include "include/GraphGenerator.h"
#include "include/TSP_k-OPT.h"

/**
 *
 * Parametrs
 * -g path - generate graph and save in file 'path'
 * -c X - generated graph will have X nodes
 *
 * -a [1,2] - solve TSP by algorithm 1(k-OPT) or 2(DoubleTree)
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
            * -a/--algorithm [1,2] - solve TSP by algorithm 1(k-OPT) or 2(DoubleTree)\n\
            * -i/--input path - input graph to solve\
            * -o/--output path - output gml file\n\
            * -s/--output-svg path - output svg file");
    int algorithmNumber = 1, count = 10;
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
        if (s == "-h" || s == "--help") {
            cout << help << endl;
            return 0;
        }

    }
    if (generate) {
        auto g = new GraphGenerator();
        g->generateCompleteGraph(path,outputSvg, count);
        return 0;
    }
   if (algorithmNumber == 1)
	{
		//doplnit zpracovani a ukladani bodu za pomoci knihovny
		//kontrola minimalni delky grafu!
		//zkusit valgrind na destruktor??
		int k = 3;
		
		CircularLinkedList *graph = new CircularLinkedList();
	
		graph->LoadOgdfGraph("src/10-nodes.gml");
	
		// graph->InsertNode(10,60);
		// graph->InsertNode(80,50);
		// graph->InsertNode(100,10);
		// graph->InsertNode(80,20);
		// graph->InsertNode(120,30);
		// graph->InsertNode(110,60);
		// graph->InsertNode(20,10);
		// graph->InsertNode(40,10);
		// graph->InsertNode(60,10);
		// graph->InsertNode(40,60);
		// graph->InsertNode(60,70);

		// std::cout << "\nInput graph:\n";
		// graph->DisplayList();
		
		// kOPT(k, graph);

		// std::cout << "\nOutput graph:\n";
		// graph->DisplayList();
		
		delete graph;
	} else {
        TSP_DoubleTree *t = new TSP_DoubleTree();
        t->load(path);
        t->solveTSD();
        if (output.length() > 0) {
            t->save(output);
        }
        if (outputSvg.length() > 0) {
            t->saveSvg(outputSvg);
        }
    }
    return 0;
}