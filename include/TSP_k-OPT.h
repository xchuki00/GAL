//Tomas Jarusek 23.11.2019
//Projekt GAL: k-OPT algorithm pro reseni Traveling Salesman Problem

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/layered/MedianHeuristic.h>
#include <ogdf/layered/OptimalHierarchyLayout.h>
#include <ogdf/layered/OptimalRanking.h>
#include <ogdf/layered/SugiyamaLayout.h>
#include <fstream>
#include <ogdf/graphalg/steiner_tree/common_algorithms.h>
#include <ogdf/graphalg/steiner_tree/EdgeWeightedGraph.h>
#include <ogdf/graphalg/steiner_tree/EdgeWeightedGraphCopy.h>

using namespace std;

//struktura vrcholu grafu pouzita ve vazanem seznamu
struct Node
{
	double positionX = 0;
	double positionY = 0;
	Node* firstNeighbour;
	Node* secondNeighbour;
	int id;
};

//obousmerne vazany seznam, ktery reprezentuje graf, nema smer - prochazime ho opacnym smerem od predchudce
class CircularLinkedList
{
    public:
		//konctruktor
		CircularLinkedList();
		//nacteni grafu ze souboru
		bool LoadOgdfGraph(string filename);
		//vlozeni vrcholu
		void InsertNode(double positionX, double positionY);
		//vrati pocatecni vrchol
		void GetStartNode (Node** startNode, Node** previousNode);
		//posune ukazatele na dalsi vrchol v poradi podle predchudce
		void GetNextNode (Node** currentNode, Node** previousNode);
		//provede prohozeni hrany podle zadane permutace
		void PerformNOptSwap (vector<Node*> &swapNodes, vector<Node*> &swapNodesPredecessors, vector<int> &permutation);
		//vrati delku seznamu
		int GetLength();
		//vypise obsah seznamu na vystup
		void DisplayList ();
		//destruktor
		~CircularLinkedList();
		
    private:
		ogdf::GraphAttributes ogdfGraphAttributes;
		ogdf::EdgeWeightedGraph<float> ogdfEdgeWeightedGraph;
		
		//pocatecni uzel
		Node* startNode;
		//delka seznamu
		int listLength = 0;
		
		//vypise obsah jednoho uzlu
		void DisplayNode(Node* node);
};

//generovani permutaci prvku
class PermutationGenerator
{
	public:
		//konstruktor
		PermutationGenerator(vector<int> &symbolArray);

		//funkce po zavolani vraci jednu dalsi permutaci symbolu
		//jedna se Heapuv algoritmus, ktery je modifikovan tak, aby vracel permutace po jedne a ne vsechny naraz
		//https://en.wikipedia.org/wiki/Heap%27s_algorithm
		int ReturnNextPermutation(vector<int> &permutation);
	
	private:
		//symboly pro permutovani
		vector<int> symbolArray;
		
		vector<int> symbolArrayCopy;
		int symbolArraySize;
		vector<int> stack;
		
		int state;
		int permutationI;
		
		//prohozeni intu
		void SwapInts (int* firstInt, int* secondInt);
};

//cela implementace je postavena na naledujici strukture
//graf jsou vrcholy propojeny hranama
//predpokladejme ze mame 3-opt swap
//logicky bude graf pro nas v takoveto forme

//    O(1,start)---O(2)
//   /                 \
//  O(6)               O(3)
//   \                 /
//    O(5)----------O(4)

//na oznaceni vrcholu zalezi
//hrany 12,34,56 vzdy budou ty, co budeme prehazovat
//zbytek grafu, co je mezi 23,45,61 je fixni, s tim nehybeme a vsechny nase prohozy musi zarucit, ze se graf nerozbije na vice casti
//kvuli tomu musime grafem prochazet v !jednom smeru! - to znamena ze po 23 nasleduje 45 a po 45 nasleduje 61 atd..
//kdyz mame toto zaruceno, tak podle toho muzeme vygenerovat validni permutace(pouzivaji oznaceni symbolu - tedy 1,2,3,atd..)
//ktere budou reprezentovat ruzne prepojeni hran a vzdy zanechaji graf v celku

//volani funkce k-OPT pro nalezeni nejkratsi cesty
//parametr k urcuje, kolik hran se bude prohazovat, a graph je ukazatel na vazany seznam reprezentujici graf
void kOPT(int k, CircularLinkedList* graph);

//-------------------------------------------------------------------------------------

//provede hlubokou kopii vektoru
void VectorDeepCopy (vector<int> &destination, vector<int> &source);

//zjisti, zda cyklicka posloupnost obsahuje jinou posloupnost
//prvni posloupnost musi byt delsi nez druha
bool DoesCircularSequenceContainsSubsequence(vector<int> &firstSequence, vector<int> &secondSequence);

//pomocna funkce pro urcovani validnich permutaci, ktere se pak daji pouzit pro prohazovani hran
//funkce testuje, zda permutace obsahuje vsechny fixni propojeni
//treba u 3-opt sekvence musi obsahovat posloupnosti 23,45,61 (nebo dvojice muze byt obracene - na poradi nezalezi) protoze reprezentuji tu cast grafu, ktera je mezi temi hranami,
//ktere se budou prohazovat - a ty se nemeni - my chceme prave ty permutace, ktery prohazuji jen ty 3 hrany mezi sebou
bool DoesSequenceContainsRequiredConnections(vector<int> &sequence);

//funkce vygeneruje vsechny permutace, ktere reprezentuji validni prohoz v grafu
void GenerateAllValidSwaps(vector<vector<int>> &validPermutations, int k);

//vypocet euklidovske vzdalensti mezi vrcholy
double CalculateEuclidianDistanceBetweenTwoNodes(Node* firstNode, Node* secondNode);

//zjisti, zda aktualni prohoz aktualnich hran zlepsi cenu cesty
bool IsReconnectionShorter (vector<Node*> &swapNodes ,vector<int> &permutation, double currentPathLength);

//zjisti, jestli existuje nejake mozne prohozeni aktualnich hran, ktere bude mit lepsi cenu
bool TryAllSwapsForCurrentEdgeCombination (CircularLinkedList* graph, vector<Node*> &swapNodes, vector<Node*> &swapNodesPredecessors, vector<vector<int>> &validPermutations);

//funkce nastavi hodnoty jednotlivych vnorenych cyklu na defaultni hodnoty
//pro prvni cyklus to bude startovni bod grafu, pro vnorene to bude vrchol vzdaleny o 2 hrany od aktualniho stavu prechoziho cyklu
void InitNestedLoop(CircularLinkedList* graph, vector<Node*> &swapNodes, vector<Node*> &swapNodesPredecessors, vector<int> &swapNodesPosition, int loopLevel);


















