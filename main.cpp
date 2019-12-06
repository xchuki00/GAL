#include "include/TSP_DoubleTree.h"
#include "include/GraphGenerator.h"
#include "include/TSP_k-OPT.h"

int main(int argc, char ** argv)
{
    int algorithmNumber = 2;
    std::string filename(argv[2]);
    //doplnit zpracovani argumentu
    std::string s(argv[1]);
    algorithmNumber = std::stoi(argv[1]);

    if (algorithmNumber == 1)
	{
		//doplnit zpracovani a ukladani bodu za pomoci knihovny
		//kontrola minimalni delky grafu!
		//zkusit valgrind na destruktor??
		int k = 3;
		
		CircularLinkedList *graph = new CircularLinkedList();
	
		graph->InsertNode(10,60);
		graph->InsertNode(80,50);
		graph->InsertNode(100,10);
		graph->InsertNode(80,20);
		graph->InsertNode(120,30);
		graph->InsertNode(110,60);
		graph->InsertNode(20,10);
		graph->InsertNode(40,10);
		graph->InsertNode(60,10);
		graph->InsertNode(40,60);
		
		graph->InsertNode(60,70);

		std::cout << "\nInput graph:\n";
		graph->DisplayList();
		
		kOPT(k, graph);

		std::cout << "\nOutput graph:\n";
		graph->DisplayList();
		
		delete graph;
	}
	else
	{
//    GraphGenerator *genrator =  new GraphGenerator();
//    genrator->generateCompleteGraph("test",10,15);
        TSP_DoubleTree *t = new TSP_DoubleTree();
        t->load("test.gml");
        t->solveTSD();
        t->save("test-mst");
	}
	
    return 0;
}