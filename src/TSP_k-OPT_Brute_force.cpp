//Tomas Jarusek 23.11.2019
//Projekt GAL: Brute-force, k-OPT algorithm pro reseni Traveling Salesman Problem

#include "../include/TSP_k-OPT_Brute_force.h"

//----------------------------------VAZANY-SEZNAM------------------------------------------
//konstruktor
CircularLinkedList::CircularLinkedList()
{
	startNode = NULL;
	listLength = 0;
}

//destruktor
CircularLinkedList::~CircularLinkedList()
{
	//vsechny uzly dealokujeme
	for (unsigned int i = 0; i < this->nodeList.size(); i++)
	{
		delete this->nodeList.at(i);
	}
}

bool CircularLinkedList::LoadOgdfGraph(string filename)
{
	//nacteme soubor
    this->ogdfGraphAttributes = ogdf::GraphAttributes(
											this->ogdfEdgeWeightedGraph, 
											ogdf::GraphAttributes::nodeGraphics | 
											ogdf::GraphAttributes::nodeStyle |
											ogdf::GraphAttributes::nodeLabel |
											ogdf::GraphAttributes::edgeGraphics |
											ogdf::GraphAttributes::edgeStyle |
											ogdf::GraphAttributes::edgeArrow |
											ogdf::GraphAttributes::edgeLabel |
											ogdf::GraphAttributes::edgeArrow
											);

    if (!ogdf::GraphIO::read(this->ogdfGraphAttributes, this->ogdfEdgeWeightedGraph, filename, ogdf::GraphIO::readGML)) 
	{
        std::cerr << "Error loading file: " << filename << std::endl;
        return false;
    }
	
	//vlozime vsechny body do nasi struktury
	for (ogdf::node n: this->ogdfEdgeWeightedGraph.nodes) 
	{
		InsertNode(n);
    }
	
	int listLength = GetLength();
	
	//vytvorime matici sousednosti
	adjacencyMatrix.resize(listLength);
	
	for (int i = 0; i < listLength; i++)
	{
		adjacencyMatrix.at(i).resize(listLength);
	}
	
	int sourceIndex;
	int targetIndex;
	
	//vyplnime matici sousednosti
	for (ogdf::edge e: this->ogdfEdgeWeightedGraph.edges) 
	{
		sourceIndex = e->source()->index();
		targetIndex = e->target()->index();
		
		// double deltaX = this->ogdfGraphAttributes.x(e->target())-this->ogdfGraphAttributes.x(e->source());
		// double deltaY = this->ogdfGraphAttributes.y(e->target())-this->ogdfGraphAttributes.y(e->source());
		
		// adjacencyMatrix.at(sourceIndex).at(targetIndex) = hypot(deltaX,deltaY);
		// adjacencyMatrix.at(targetIndex).at(sourceIndex) = hypot(deltaX,deltaY);
		
        adjacencyMatrix.at(sourceIndex).at(targetIndex) = stod(this->ogdfGraphAttributes.label(e));
		adjacencyMatrix.at(targetIndex).at(sourceIndex) = stod(this->ogdfGraphAttributes.label(e));
    }
	
	//debug vypis
	// for (int i = 0; i < listLength; i++)
	// {
		// for (int j = 0; j < listLength; j++)
		// {
			// cout << adjacencyMatrix.at(i).at(j) << ", ";
		// }
		// cout << "\n";
	// }
	
    return true;
}

void CircularLinkedList::PrepareOgdfOutputKOPT()
{
	//vymazeme vsechny edge puvodniho grafu
	int listLength = GetLength();
	
	for (int i = 0; i < ((listLength-1)*listLength)/2; i++)
	{
		this->ogdfEdgeWeightedGraph.delEdge(this->ogdfEdgeWeightedGraph.firstEdge());
	}
	
	//vypis debug hran
	/*for (ogdf::edge e: this->ogdfEdgeWeightedGraph.edges) 
	{
		cout << e << ", ";
    }*/
	
	//projedeme vsechny hrany z nasi struktury a vlozime je do ogdf grafu se spravnymi labely
	ogdf::edge currentEdge;
	double currentWeight;
	
	Node* startNode = NULL;
	Node* currentNode = NULL;
	Node* previousNode = NULL;
	
	//DELETE
	double currentBestPathMetric = 0;
	
	
	GetStartNode(&currentNode, &previousNode);
	startNode = currentNode;
	
	currentWeight = adjacencyMatrix.at(previousNode->originalNodePointer->index()).at(currentNode->originalNodePointer->index());
	currentEdge = this->ogdfEdgeWeightedGraph.newEdge(previousNode->originalNodePointer, currentNode->originalNodePointer, currentWeight);
	this->ogdfGraphAttributes.label(currentEdge) = std::to_string((int)round(currentWeight));
	
	currentBestPathMetric += currentWeight;
	
	GetNextNode (&currentNode, &previousNode);
	
	while (currentNode != startNode)
	{
		currentWeight = adjacencyMatrix.at(previousNode->originalNodePointer->index()).at(currentNode->originalNodePointer->index());
		currentEdge = this->ogdfEdgeWeightedGraph.newEdge(previousNode->originalNodePointer, currentNode->originalNodePointer, currentWeight);
		this->ogdfGraphAttributes.label(currentEdge) = std::to_string((int)round(currentWeight));
		
		currentBestPathMetric += currentWeight;
		
		GetNextNode (&currentNode, &previousNode);			
	}
	
	//DELETE
	std::cout<<"PRICE: "<< currentBestPathMetric <<std::endl;
	
	this->ogdfGraphAttributes.directed() = true;
   
    for (ogdf::edge e: this->ogdfEdgeWeightedGraph.edges) 
	{
        this->ogdfGraphAttributes.arrowType(e) = ogdf::EdgeArrow::Last;
    }
}

void CircularLinkedList::PrepareOgdfOutputBruteForce(vector<int> &permutation)
{
	//vymazeme vsechny edge puvodniho grafu
	int listLength = GetLength();
	
	for (int i = 0; i < ((listLength-1)*listLength)/2; i++)
	{
		this->ogdfEdgeWeightedGraph.delEdge(this->ogdfEdgeWeightedGraph.firstEdge());
	}
	
	//projedeme vsechny hrany z nasi struktury a vlozime je do ogdf grafu se spravnymi labely
	//protoze bruteforce nemeni samotnous strukturu
	ogdf::edge currentEdge;
	double currentWeight;
	
	Node* firstNode;
	Node* secondNode;
	
	unsigned int i;
	
	for (i = 0; i < permutation.size()-1; i++)
	{
		firstNode = nodeList.at((permutation.at(i)-1));
		secondNode = nodeList.at((permutation.at(i+1)-1));
		
		currentWeight = adjacencyMatrix.at(firstNode->originalNodePointer->index()).at(secondNode->originalNodePointer->index());
		currentEdge = this->ogdfEdgeWeightedGraph.newEdge(firstNode->originalNodePointer, secondNode->originalNodePointer, currentWeight);
		this->ogdfGraphAttributes.label(currentEdge) = std::to_string((int)round(currentWeight));	
	}
	
	firstNode = nodeList.at((permutation.at(i)-1));
	secondNode = nodeList.at((permutation.at(0)-1));
	
	currentWeight = adjacencyMatrix.at(firstNode->originalNodePointer->index()).at(secondNode->originalNodePointer->index());
	currentEdge = this->ogdfEdgeWeightedGraph.newEdge(firstNode->originalNodePointer, secondNode->originalNodePointer, currentWeight);
	this->ogdfGraphAttributes.label(currentEdge) = std::to_string((int)round(currentWeight));
	
	
	this->ogdfGraphAttributes.directed() = true;
   
    for (ogdf::edge e: this->ogdfEdgeWeightedGraph.edges) 
	{
        this->ogdfGraphAttributes.arrowType(e) = ogdf::EdgeArrow::Last;
    }
}

bool CircularLinkedList::SaveOgdfGraphGML(string filename)
{
	std::ofstream gml(filename); 
	
	if (!ogdf::GraphIO::writeGML(this->ogdfGraphAttributes, gml)) 
	{
        std::cerr << "Error saving file: " << filename << std::endl;
        return false;
    }
	
    return true;
}

bool CircularLinkedList::SaveOgdfGraphSVG(string filename)
{
	std::ofstream svg(filename);
	 
	if (!ogdf::GraphIO::drawSVG(this->ogdfGraphAttributes, svg)) 
	{
        std::cerr << "Error saving file: " << filename << std::endl;
        return false;
    }

    return true;
}

//vlozi vrchol do grafu
void CircularLinkedList::InsertNode(ogdf::NodeElement* originalNodePointer)
{
	//seznam je prazdny - vytvorime prvni startovni uzel
	if (listLength == 0)
	{
		startNode = new Node();
		this->nodeList.push_back(startNode);
		
		startNode->originalNodePointer = originalNodePointer;
		
		startNode->firstNeighbour = startNode;
		startNode->secondNeighbour = startNode;
	}
	//seznam ma jeden prvek - musime spravne prevazat oba ukazatele startovniho uzlu, protoze ukazuji sami na sebe
	else if (listLength == 1)
	{
		Node* newNode = new Node();	
		this->nodeList.push_back(newNode);
		
		newNode->originalNodePointer = originalNodePointer;
		newNode->firstNeighbour = startNode;
		newNode->secondNeighbour = startNode;
		
		startNode->firstNeighbour = newNode;
		startNode->secondNeighbour = newNode;
	}
	//jinak vlozime novy vrchol vedle startovniho vrcholu jako druheho souseda
	else
	{
		Node* tmpSecondNeighbour = startNode->secondNeighbour;
		
		Node* newNode = new Node();	
		this->nodeList.push_back(newNode);
		
		newNode->originalNodePointer = originalNodePointer;
		newNode->firstNeighbour = startNode;
		newNode->secondNeighbour = tmpSecondNeighbour;
		
		startNode->secondNeighbour = newNode;
		tmpSecondNeighbour->firstNeighbour = newNode;	
	}
	
	listLength++;
}

//vrati startovni vrchol a k nemu zvoli nahodneho predchudce
void CircularLinkedList::GetStartNode (Node** startNode, Node** previousNode)
{
	*startNode = this->startNode;
	*previousNode = this->startNode->secondNeighbour;
}

//prehodi ukazatele prvku a predchudce na dalsi prvek
//posouvame se tak, ze zjisitime, ktery z tech dvou ukazatelu se rovna zadanemu predchudci a
//nasledne to posuneme pres toho druheho ukazatele
void CircularLinkedList::GetNextNode (Node** currentNode, Node** previousNode)
{
	if ((*currentNode)->firstNeighbour == *previousNode)
	{
		*previousNode = *currentNode;
		*currentNode = (*currentNode)->secondNeighbour;		
	}
	else
	{
		*previousNode = *currentNode;
		*currentNode = (*currentNode)->firstNeighbour;
	}
}

//prohodime hrany podle zadane permutace
void CircularLinkedList::PerformNOptSwap (vector<Node*> &swapNodes, vector<Node*> &swapNodesPredecessors, vector<int> &permutation)
{	
	int size = swapNodes.size();
	
	//predpripravime si vrcholy jejich cile na prohozeni
	vector<Node*> swapNodesNew;
	vector<Node*> swapNodesTarget;
	//musime taky vedet, jestli prevazat prvniho nebo druheho souseda - true je prvni a false druhy
	vector<bool> swapNodesDirection;
	
	int indexOne;
	int indexTwo;
	
	Node* firstNode;
	Node* secondNode;
	
	Node* firstNodePredecessor;
	Node* secondNodePredecessor;
	
	//permutace je udelana tak, ze se prvne vygeneruje a pak se na konec da jednicka(to jde protoze se bere jako cirkularni)
	//my chceme prohazovat hrany a ty maji dva hranicni vrcholy a zaciname od vrcholu oznaceneho 1, takze musime zacit od te 1, ktera je na poslednim miste a 
	//jeji naslednik je na prvnim miste - to je prvni dvojice (treba permutace 452361 ma prvni dvojici 14 - tedy vrchol 1 se musi napojit na vrchol 4)
	//potom jdeme po dvojicich az do konce (tedy v nasem prikladu 52,36)
	//tedy zaciname od delky permutace do dvojnasobku a skaceme po dvojicich, to nam da indexy treba 6,8,10
	for (int i = size; i < size+size; i+=2)
	{
		//ty pak zmodulujeme, to nam z 6,8,10 udela
		//5-0,1-2,3-4
		//pak se podivame na permutaci, aby jsme zjistily, co je na techto indexech 
		//a to nam rekne indexy(musime odecist jednicku aby indexy zacinali od 0) do pole vrcholu
		indexOne = permutation.at((i-1)%size)-1;
		indexTwo = permutation.at((i)%size)-1;
		
		//hrana bude tedy propojovat tyto vrcholy
		firstNode = swapNodes.at(indexOne);
		secondNode = swapNodes.at(indexTwo);
		
		//tohle jsou jejich predchudci
		firstNodePredecessor = swapNodesPredecessors.at(indexOne);
		secondNodePredecessor = swapNodesPredecessors.at(indexTwo);
		
		//pushneme je do vektoru
		swapNodesNew.push_back(firstNode);
		swapNodesNew.push_back(secondNode);
		
		swapNodesTarget.push_back(secondNode);
		swapNodesTarget.push_back(firstNode);
		
		//ted musime zjistit, ktery z ukazatelu ale musime prevazat, protoze linearni seznam je obousmerny, ale nema smer
		//to udelame tak, ze z toho jak jsme zvolili strukturu prochazeni vsech n-tic v grafu a pojmenovani vrcholu(ktere vymezuji hrany, ktere se budou prohazovat) vime,
		//ze vrcholy s lichym oznacenim se musi prevezat smerem od predchudce - proto porovname predchudce s jednim ze sousedu a podle toho urcime smer prohozu
		//vrcholy se sudym oznacenim se naproti tomu musi prohodit smerem, kterych vede predchudce
		if (indexOne%2 == 0)
		{				
			if (firstNode->firstNeighbour == firstNodePredecessor)
			{
				swapNodesDirection.push_back(false);
			}
			else
			{
				swapNodesDirection.push_back(true);
			}
		}
		else
		{
			if (firstNode->firstNeighbour == firstNodePredecessor)
			{
				swapNodesDirection.push_back(true);
			}
			else
			{
				swapNodesDirection.push_back(false);
			}
		}
	
		//to stejny udelame s druhym vrcholem ve dvojici
		if (indexTwo%2 == 0)
		{
			if (secondNode->firstNeighbour == secondNodePredecessor)
			{
				swapNodesDirection.push_back(false);
			}
			else
			{
				swapNodesDirection.push_back(true);
			}
		}
		else
		{
			if (secondNode->firstNeighbour == secondNodePredecessor)
			{
				swapNodesDirection.push_back(true);
			}
			else
			{
				swapNodesDirection.push_back(false);
			}
		}
	}
	
	//ted kdyz uz vime pocatek, cil i smer, tak proste prevazeme dane ukazatele
	for (unsigned int i = 0; i < swapNodesNew.size(); i++)
	{
		if (swapNodesDirection.at(i) == true)
		{
			(swapNodesNew.at(i))->firstNeighbour = swapNodesTarget.at(i);
		}
		else
		{
			(swapNodesNew.at(i))->secondNeighbour = swapNodesTarget.at(i);
		}
	}
}

//vrati delku grafu
int CircularLinkedList::GetLength()
{
	return listLength;
}

//----------------------------------PERMUTACE------------------------------------------

PermutationGenerator::PermutationGenerator(vector<int> &symbolArray)
{
	//inicializace vsech promennych
	this->symbolString = "";
	this->symbolStringSize = symbolArray.size();
	this->noPermutationRemaining = false;
	
	for (unsigned int i = 0; i < this->symbolStringSize; i++)
	{
		this->symbolString += (char)symbolArray.at(i);
	}
}

int PermutationGenerator::ReturnNextPermutation(vector<int> &permutation)
{	
	//vycerpali jsme vsechny permutace - konec
	if (this->noPermutationRemaining == true)
	{
		return 3;
	}
	else
	{
		permutation.resize(this->symbolStringSize);
		
		//vratime aktualni stav symbolStringu
		for (unsigned int i = 0; i < this->symbolStringSize; i++)
		{
			permutation.at(i) = (int)this->symbolString.at(i);
		}
		
		//inkrementujeme na dalsi lexikografickou permutaci
		this->noPermutationRemaining = !next_permutation(this->symbolString.begin(), this->symbolString.end());
		
		return 1;
	}
}

//-------------------------------------------------------------------------------------------------------------

void VectorDeepCopy (vector<int> &destination, vector<int> &source)
{
	unsigned int sourceSize = source.size();
	destination.resize(sourceSize); 
	
	for (unsigned int i = 0; i < sourceSize; i++)
	{
		destination.at(i) = source.at(i);
	}
}

bool DoesCircularSequenceContainsSubsequence(vector<int> &firstSequence, vector<int> &secondSequence)
{
	int firstSequenceSize = firstSequence.size();
	int secondSequenceSize = secondSequence.size();
	int matchLength;
	
	//zkusime zacit od kazdeho symboly v prvni sekvenci
	for (int i = 0; i < firstSequenceSize; i++)
	{	
		matchLength = 0;

		//a zkusime zda u nej nezacina druha sekvence
		for (int j = 0; j < secondSequenceSize; j++)
		{
			//pokud neni shoda tak koncime s testovanim od aktualni pozice
			//jinak pokracujeme az do konce druhe posloupnosti
			if (firstSequence.at((i+j)%firstSequenceSize) == secondSequence.at(j))
			{
				matchLength++;
			}
			else
			{
				break;
			}
		}
		
		//pokud je delka shody rovna delce druhe sekvence, tak vime, ze jsou posoupnosti stejne a vracime true
		if (matchLength == secondSequenceSize)
		{
			return true;
		}
	}
	
	//zkusili jsme vsechno a nebyla shoda - takze false
	return false;
}

bool DoesSequenceContainsRequiredConnections(vector<int> &sequence)
{
	int sequenceSize = sequence.size();
	int numberOfRequiredConnections = sequenceSize/2;
	
	vector<int> actConnection;
	vector<int> actConnectionReversed;
	
	//postupne overime, zda nase permutace - coz je posloupnost symbolu obsahuje ty klicovy dvojice(podsekvence o delce 2)
	//pokud obsahuje vsechny, tak vracime true
	for (int i = 1; i <= numberOfRequiredConnections; i++)
	{
		actConnection.push_back(i*2);
		actConnection.push_back((i*2+1)%sequenceSize);
		
		actConnectionReversed.push_back((i*2+1)%sequenceSize);
		actConnectionReversed.push_back(i*2);
		
		//dvojice muze byt bud obracena nebo neobracena
		if (DoesCircularSequenceContainsSubsequence(sequence, actConnection) == false && DoesCircularSequenceContainsSubsequence(sequence, actConnectionReversed) == false)
		{
			return false;
		}
			
		actConnection.clear();
		actConnectionReversed.clear();
	}
	
	return true;
}

void GenerateAllValidSwaps(vector<vector<int>> &validPermutations, int k)
{
	//kdyz mame 6 vrcholu - tak ten jeden muzeme zafixovat protoze on by jen reprentoval ruzne rotace grafu a ty nas nezajimaji
	//tedy kdyz je 6 vrcholu - tak je jen (6-1!) permutaci
	//pak k ni pridame 1 nakonec a tim padem ziskavame unikatni permutace bez rotaci
	int permutationLength = k*2-1;
	
	vector<int> symbolArray;
	
	//permutujeme symboly bez 1
	for (int i = 0; i < permutationLength; i++)
	{
		symbolArray.push_back(i+2);
	}
	
	//init generatoru
	PermutationGenerator permutationGenerator(symbolArray);

	vector<int> actPermutation;
	vector<int> actPermutationReversed;
	
	int retValue = permutationGenerator.ReturnNextPermutation(actPermutation);
	
	bool safeToAdd;
	
	//3 znamena, ze uz  nejsou k dispozici dalsi
	while(retValue != 3)
	{
		if (actPermutation.at(0) < actPermutation.at(permutationLength-1))
		{
			//fixnuta 1, nejsou rotace
			actPermutation.push_back(1);

			//permutace musi obsahovat fixnute propojeni
			if (DoesSequenceContainsRequiredConnections(actPermutation) == true)
			{
				validPermutations.push_back(actPermutation);
			}
		}

		retValue = permutationGenerator.ReturnNextPermutation(actPermutation);
	}
	
	/*
	std::cout << "vypis\n";
	for (unsigned int i = 0; i < validPermutations.size(); i++)
	{
		for (unsigned int j = 0; j < validPermutations.at(i).size(); j++)
		{
			std::cout << validPermutations.at(i).at(j);	
		}
		std::cout << "\n";
	}*/
}

double GetMetricFromAdjacencyMatrix(CircularLinkedList* graph, Node* firstNode, Node* secondNode)
{
	int sourceIndex = firstNode->originalNodePointer->index();
	int targetIndex = secondNode->originalNodePointer->index();
	
	return graph->adjacencyMatrix.at(sourceIndex).at(targetIndex);
}

bool IsReconnectionShorter (CircularLinkedList* graph, vector<Node*> &swapNodes ,vector<int> &permutation, double currentPathLength)
{
	double reconnectedPathLength = 0;
	int size = swapNodes.size();

	//jak u funkce PerformNOptSwap jdeme od posledniho prvku -- tedy od 1 a pak po dvojicich, ale misto prohazovani jen spocitame jakou by to prohozeni melo vzdalenost
	for (int i = size; i < size+size; i+=2)
	{
		reconnectedPathLength += GetMetricFromAdjacencyMatrix(graph, swapNodes.at(permutation.at((i-1)%size)-1), swapNodes.at(permutation.at((i)%size)-1));
	}
	
	//porovname s aktualni - pokud lepsi - vratime true
	if (reconnectedPathLength < currentPathLength)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool TryAllSwapsForCurrentEdgeCombination (CircularLinkedList* graph, vector<Node*> &swapNodes, vector<Node*> &swapNodesPredecessors, vector<vector<int>> &validPermutations)
{
	double currentPathLength = 0;
	
	//spocitame cenu aktualni cesty
	for (unsigned int i = 0; i < swapNodes.size(); i+=2)
	{
		currentPathLength += GetMetricFromAdjacencyMatrix(graph, swapNodes.at(i), swapNodes.at(i+1));
	}

	//zkusime vsechny prohoze jestli maji lepsi cenu
	for (unsigned int i = 0; i < validPermutations.size(); i++)
	{	
		//pokud jsme nasli lepsi tak vracime true - hledani bylo uspesne
		if (IsReconnectionShorter (graph, swapNodes, validPermutations.at(i), currentPathLength) == true)
		{
			graph->PerformNOptSwap (swapNodes, swapNodesPredecessors, validPermutations.at(i));
			return true;
		}
	}
	
	//pokud jsme zkusili vsechny kombinace a nic neni lepsi - tak false - hledani neuspesne
	return false;
}

void InitNestedLoop(CircularLinkedList* graph, vector<Node*> &swapNodes, vector<Node*> &swapNodesPredecessors, vector<int> &swapNodesPosition, int loopLevel)
{	
	int firstNodeIndex = loopLevel*2;
	int secondNodeIndex = loopLevel*2+1;
	
	int previousLoopOffset = -2;
	
	//prvni cyklus - init na startovni bod grafu
	if (loopLevel == 0)
	{
		swapNodesPosition.at(loopLevel) = 0;
		
		graph->GetStartNode(&(swapNodes.at(firstNodeIndex)), &(swapNodesPredecessors.at(firstNodeIndex)));
		graph->GetStartNode(&(swapNodes.at(secondNodeIndex)), &(swapNodesPredecessors.at(secondNodeIndex)));
	
		graph->GetNextNode(&(swapNodes.at(secondNodeIndex)), &(swapNodesPredecessors.at(secondNodeIndex)));
	}
	//vnoreny cyklus - init na vzdalenost dvou hran od prechoziho cyklu
	else
	{
		//hodnota predchoziho cyklu + offset, ktery je 2
		swapNodesPosition.at(loopLevel) = swapNodesPosition.at(loopLevel-1) + 2;
		
		swapNodes.at(firstNodeIndex) = swapNodes.at(firstNodeIndex + previousLoopOffset);
		swapNodes.at(secondNodeIndex) = swapNodes.at(secondNodeIndex + previousLoopOffset);
		
		swapNodesPredecessors.at(firstNodeIndex) = swapNodesPredecessors.at(firstNodeIndex + previousLoopOffset);
		swapNodesPredecessors.at(secondNodeIndex) = swapNodesPredecessors.at(secondNodeIndex + previousLoopOffset);
		
		graph->GetNextNode(&(swapNodes.at(firstNodeIndex)), &(swapNodesPredecessors.at(firstNodeIndex)));
		graph->GetNextNode(&(swapNodes.at(firstNodeIndex)), &(swapNodesPredecessors.at(firstNodeIndex)));
		
		graph->GetNextNode(&(swapNodes.at(secondNodeIndex)), &(swapNodesPredecessors.at(secondNodeIndex)));
		graph->GetNextNode(&(swapNodes.at(secondNodeIndex)), &(swapNodesPredecessors.at(secondNodeIndex)));
	}
}

bool kOPT(int k, CircularLinkedList* graph)
{
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    //pocet vrcholu musi byt minimalne 2*k a vetsi nez 1
	if (graph->GetLength() < k*2 || k < 2) 
	{
        std::cerr << "Parameter k needs graph with at least 2*k vertices and be bigger than 1!" << std::endl;
        return false;
    }

	//vrcholy ohranicujici hrany, ktere se budou zkouset prehazovat
	vector<Node*> swapNodes;
	//jejich predchudci (vazany seznam nema smer, tedy se posunujeme na druhou stranu od predchudce)
	vector<Node*> swapNodesPredecessors;
	//citac pro kazdy vrchol, jak daleko jsme ood staru, abychom vedeli kdy skoncit
	vector<int> swapNodesPosition;
	
	//seznam vsech permutaci vrcholu, podle kterych se potom budem snazit prohozavat hrany
	vector<vector<int>> validPermutations;
	
	//kazda hrana ma dva vrcholy, takze budeme potrebovat k*2
	swapNodes.resize(k*2);
	swapNodesPredecessors.resize(k*2);
	//hrana ma jeden citac
	swapNodesPosition.resize(k);
	
	//DELETE
	std::chrono::steady_clock::time_point begin1 = std::chrono::steady_clock::now();
	
	//vygenerujeme vsechny permutace, ktere reprezentuji validni prohozeni hran
	GenerateAllValidSwaps(validPermutations, k);
	
	//DELETE
	std::chrono::steady_clock::time_point end1 = std::chrono::steady_clock::now();
	std::cout << "TIME1: " << std::chrono::duration_cast<std::chrono::nanoseconds> (end1 - begin1).count() << "[ns]" << std::endl;
	
	//zacneme generovat vsechny n-tice
	
	//aktualni zanoreni cyklu
	int currentLoopLevel = k-1;
	//maximalni index pro cyklus podle zanoreni
	int maxIndexForCurrentLoopLevel;
	//konec cyklu
	bool endOfLoop;
	
	//jendalo se o posledni kombinaci
	bool lastCombination = false;
	//cena cesty se vylepsila
	bool pathImproved;
	
	//DELETE
	std::chrono::steady_clock::time_point begin2 = std::chrono::steady_clock::now();
	int cycleCounter = 0;
	
	//budeme provadet prohozy, dokud dokazeme najit nejaky prohoz, ktery vylepsi cenu
	while(true)
	{
		//DELETE
		cycleCounter++;
		
		//nainicalizujeme vsechny cykly na pocatecni hodnoty
		for (int i = 0; i < k; i++)
		{
			InitNestedLoop(graph, swapNodes, swapNodesPredecessors, swapNodesPosition, i);
		}
	
		//zaciname s predpokladem, ze se cestu nepodarilo vylepsit a zbyvaji nam kombinacek otestovani
		pathImproved = false;
		lastCombination = false;
	
		//projdeme vsechny kombinace hran v grafu
		//tohle realizuje provedeni neurciteho poctu zanorenych cyklu podle parametru k (5-opt => 5 zanorenych cyklu v sobe)
		while (true)
		{	
			//pro kazdou kombinaci zkusime vsechny prohozy, pokud nejaky neni lepsi
			if (TryAllSwapsForCurrentEdgeCombination (graph, swapNodes, swapNodesPredecessors, validPermutations) == true)
			{
				//nase verze algoritmu funguje tak, ze pri prvnim prohozu, ktery zlepsi cenu, zacneme prohledavat kombinace uplne od znova
				//pokud jsme tady dostali true, tak to znamena, ze jsme nasli prohoz  a tim padem vyskocime z cyklu a zaciname od znova
				pathImproved = true;
				break;
			}

			//ted musime najit dalsi kombinaci
			while (true)
			{
				//posuneme hranu v aktualne zanorenem cyklu na dalsi pozici v grafu
				graph->GetNextNode(&(swapNodes.at(currentLoopLevel*2)), &(swapNodesPredecessors.at(currentLoopLevel*2)));
				graph->GetNextNode(&(swapNodes.at(currentLoopLevel*2+1)), &(swapNodesPredecessors.at(currentLoopLevel*2+1)));
				//udrzujeme si pro kazdou hranu jeji vzdalenost(pocet hran) od startu, aby jsme pak mohly cykly korektne ukoncit
				swapNodesPosition.at(currentLoopLevel)++;

				//ted musime vyresit, jestli se doslo na konec aktualniho cyklu
				endOfLoop = false;
				
				//zalezi na tom, jestli prvni hrana je nebo neni v pocatecni pozici,
				//protoze mezi hranami co se budou prohazovat musi byt mezera, takze jakmile se pocatecni hrana posune,
				//tak muzeme pouzit hranu ohranicenou predposlednim a poslednim vrcholem
				
				//maximalni index se vypocita tak, ze vezmeme delku grafu a od ni odecteme pocet pozic od konce podle urovne cyklu
				//nejvnorenejsi cyklus ma nejvyssi hodnotu a na rozdil od k se pocitaji od 0, tedy k-1-currentLoopLevel
				//tento pocet hran od konce pak musime vynasobit dvema, protoze mezi nimi musi byt alespon jedna hrana, ktera tvori mezeru
				maxIndexForCurrentLoopLevel = graph->GetLength()-(2*(k-1-currentLoopLevel));
				
				//je v pocatecni pozici
				if (swapNodesPosition.at(0) == 0)
				{
					//tedy v tomto pripade se graf jevi jako o 1 kratsi
					//pokud se aktualni index nevejde do limitu, tak jsme na konci a musime provest reset
					if (!(swapNodesPosition.at(currentLoopLevel) < maxIndexForCurrentLoopLevel-1))
					{
						endOfLoop = true;   
					}

				}
				//neni v pocatecni pozici
				else
				{
					if (!(swapNodesPosition.at(currentLoopLevel) < maxIndexForCurrentLoopLevel))
					{
						endOfLoop = true;   
					}
				}
				
				//provedeme reset
				if (endOfLoop == true)
				{
					//pokud musime resetovat nejvyssi cyklus, prosli jsme vsechny kombinace a koncime
					if (currentLoopLevel == 0)
					{
						lastCombination = true;
						break;
					}
					
					//presuneme se na mene zanoreny cyklus dekrementaci hodnoty currentLoopLevel
					currentLoopLevel--;
				}
				//neprovedeme reset
				else
				{
					//pokud nejsme na konci a jsme v nejvice zanorenem cyklu, tak je vsechno v pohode a normalne vracime kombinaci
					if (currentLoopLevel != k-1)
					{
						//protoze jsme se mohli dostat na konec vice cyklu soucasne, musime se ted postupne zanorovat dokud nedojdeme
						//do nejvice zanoreneho cyklu a pritom korektne inicializujeme cykly na danny stav
						while (currentLoopLevel < k-1)
						{
							InitNestedLoop(graph, swapNodes, swapNodesPredecessors, swapNodesPosition, currentLoopLevel+1);
							currentLoopLevel++;
						}
					}
					
					break;
				}
			}
			
			//jednalo se o posledni kombinaci - koncime
			if (lastCombination == true)
			{
				//pokud jsme dosli az sem, tak to znamena, ze jsme se pred tim nebreaknuli a tim padem nebyl improvement
				pathImproved = false;
				break;
			}
			
		}
		
		//pokud jsme prosly vsechny kombinace hran v aktualnim grafu a nenasli jsme zadny prohoz, ktery by cenu vylepsil - tak hotovo a koncime s algoritmem
		if (pathImproved == false)
		{
			break;
		}
	}
	
	//DELETE
	std::chrono::steady_clock::time_point end2 = std::chrono::steady_clock::now();
	std::cout << "TIME2: " << std::chrono::duration_cast<std::chrono::nanoseconds> (end2 - begin2).count() << "[ns]" << std::endl;
	std::cout << "TIME3: " << cycleCounter << "[ns]" << std::endl;
    std::cout << "TIME: " << std::chrono::duration_cast<std::chrono::nanoseconds>(end2 - begin).count() << "[ns]"
              << std::endl;

    //nakonec pretransformujeme graf do ogdf
	graph->PrepareOgdfOutputKOPT();
		
	return true;
}

bool BruteForce(CircularLinkedList* graph)
{
	//DELETE
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	
	//kdyz mame 6 vrcholu - tak ten jeden muzeme zafixovat protoze by jen reprezentoval ruzne rotace grafu a ty nas nezajimaji
	//tedy kdyz je 6 vrcholu - tak je jen (6-1!) permutaci
	//pak k ni pridame 1 nakonec a tim padem ziskavame unikatni permutace bez rotaci
	int permutationLength = graph->GetLength()-1;
	
	vector<int> symbolArray;
	
	//permutujeme symboly bez 1
	for (int i = 0; i < permutationLength; i++)
	{
		symbolArray.push_back(i+2);
	}
	
	//aktualne zpracovavana trasa	
	double currentPathMetric;
	vector<int> actPermutation;
	
	unsigned int i;
	Node* firstNode;
	Node* secondNode;
	
	//aktualne nelepsi trasa
	double currentBestPathMetric = numeric_limits<int>::max();
	vector<int> currentBestPermutation;
	
	//init generatoru
	PermutationGenerator permutationGenerator(symbolArray);
	
	//prvni permutace
	int retValue = permutationGenerator.ReturnNextPermutation(actPermutation);
	
	//3 znamena, ze uz  nejsou k dispozici dalsi
	while(retValue != 3)
	{
		//odfiltrujeme zpetne permutace
		if (actPermutation.at(0) < actPermutation.at(permutationLength-1))
		{
			//aktualni metrika je nula
			currentPathMetric = 0;
			
			//doplnime 1
			actPermutation.push_back(1);
			
			//spocitame delku permutace
			for (i = 0; i < permutationLength; i++)
			{
				firstNode = graph->nodeList.at((actPermutation.at(i)-1));
				secondNode = graph->nodeList.at((actPermutation.at(i+1)-1));
				
				currentPathMetric += GetMetricFromAdjacencyMatrix(graph, firstNode, secondNode);
			}
			
			firstNode = graph->nodeList.at((actPermutation.at(i)-1));
			secondNode = graph->nodeList.at((actPermutation.at(0)-1));
			
			currentPathMetric += GetMetricFromAdjacencyMatrix(graph, firstNode, secondNode);
			
			//pokud je lepsi prepiseme
			if (currentPathMetric < currentBestPathMetric)
			{
				currentBestPathMetric = currentPathMetric;
				VectorDeepCopy(currentBestPermutation, actPermutation);
			}
			
		}
		
		retValue = permutationGenerator.ReturnNextPermutation(actPermutation);
	}
	
	//DELETE
	std::cout << "PRICE: "<< currentBestPathMetric << std::endl;
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "TIME: " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;
	
	//nakonec pretransformujeme graf do ogdf
	graph->PrepareOgdfOutputBruteForce(currentBestPermutation);
	
	return true;
}








































