//Tomas Jarusek 23.11.2019
//Projekt GAL: Brute-force, k-OPT algorithm pro reseni Traveling Salesman Problem
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
	Node* firstNeighbour;
	Node* secondNeighbour;
	
	ogdf::NodeElement* originalNodePointer;
};

//obousmerne vazany seznam, ktery reprezentuje graf, nema smer - prochazime ho opacnym smerem od predchudce
class CircularLinkedList
{
    public:
		//konctruktor
		CircularLinkedList();
		//nacteni ogdf grafu ze souboru
		bool LoadOgdfGraph(string filename);
		//konvertuje k-OPT graf z nasi struktury do ogdf struktury
		void PrepareOgdfOutputKOPT();
		//konvertuje Brute-force graf z nasi struktury do ogdf struktury
		void PrepareOgdfOutputBruteForce(vector<int> &permutation);
		//ulozeni ogdf grafu do svg souboru
		bool SaveOgdfGraphSVG(string filename);
		//ulozeni ogdf grafu do gml souboru
		bool SaveOgdfGraphGML(string filename);
		//vlozeni vrcholu
		void InsertNode(ogdf::NodeElement* originalNodePointer);
		//vrati pocatecni vrchol
		void GetStartNode (Node** startNode, Node** previousNode);
		//posune ukazatele na dalsi vrchol v poradi podle predchudce
		void GetNextNode (Node** currentNode, Node** previousNode);
		//provede prohozeni hrany podle zadane permutace
		void PerformNOptSwap (vector<Node*> &swapNodes, vector<Node*> &swapNodesPredecessors, vector<int> &permutation);
		//vrati delku seznamu
		int GetLength();
		
		//destruktor
		~CircularLinkedList();
		
		//seznam ukazatelu na vsechny uzly
		vector<Node*> nodeList;
		
		//matice sousednosti
		vector<vector<double>> adjacencyMatrix;
		
    private:
		//ogdf graf
		ogdf::GraphAttributes ogdfGraphAttributes;
		ogdf::EdgeWeightedGraph<float> ogdfEdgeWeightedGraph;
		
		//pocatecni uzel
		Node* startNode;
		//delka seznamu
		int listLength = 0;	
};

//generovani permutaci prvku
class PermutationGenerator
{
	public:
		//konstruktor
		PermutationGenerator(vector<int> &symbolArray);
		//vrati dalsi eprmutaci v lexikografickem poradi
		int ReturnNextPermutation(vector<int> &permutation);
		
	private:
		//string reprezentujucic aktualni permutaci
		string symbolString;
		//delka permutace
		unsigned int symbolStringSize;
		//zarazka pro skonceni permutaci
		bool noPermutationRemaining;
};

//k-OPT

//cela implementace je postavena na naledujici strukture
//graf jsou vrcholy propojene hranama
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
//kdyz mame toto zaruceno, tak podle toho muzeme vygenerovat validni permutace(pouzivaji oznaceni symbolu - tedy 1,2,3,atd..),
//ktere budou reprezentovat ruzne prepojeni hran a vzdy zanechaji graf v celku

//volani funkce k-OPT pro nalezeni nejkratsi cesty
//parametr k urcuje, kolik hran se bude prohazovat, a graph je ukazatel na vazany seznam reprezentujici graf
bool kOPT(int k, CircularLinkedList* graph);

//-------------------------------------------------------------------------------------

//Brute-force

//vygenerujeme vsechny permutace a vypocitame jejich metriku - ta s nejmensi je vysledek
bool BruteForce(CircularLinkedList* graph);

//-------------------------------------------------------------------------------------

//Pomocne funkce

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
double GetMetricFromAdjacencyMatrix(Node* firstNode, Node* secondNode);

//zjisti, zda aktualni prohoz aktualnich hran zlepsi cenu cesty
bool IsReconnectionShorter (vector<Node*> &swapNodes ,vector<int> &permutation, double currentPathLength);

//zjisti, jestli existuje nejake mozne prohozeni aktualnich hran, ktere bude mit lepsi cenu
bool TryAllSwapsForCurrentEdgeCombination (CircularLinkedList* graph, vector<Node*> &swapNodes, vector<Node*> &swapNodesPredecessors, vector<vector<int>> &validPermutations);

//funkce nastavi hodnoty jednotlivych vnorenych cyklu na defaultni hodnoty
//pro prvni cyklus to bude startovni bod grafu, pro vnorene to bude vrchol vzdaleny o 2 hrany od aktualniho stavu prechoziho cyklu
void InitNestedLoop(CircularLinkedList* graph, vector<Node*> &swapNodes, vector<Node*> &swapNodesPredecessors, vector<int> &swapNodesPosition, int loopLevel);



















