/**
    @author Stefano Nuvoli
*/
#include "graphtest.h"

#include <iostream>
#include <random>
#include <chrono>
#include <assert.h>
#include <iomanip>

#include <vector>

#include "cg3/data_structures/graphs/graph.h"
#include "cg3/utilities/utils.h"

#define ITERATION 3
#define INDENTSPACE 12

#define INPUTSIZE 1000000
#define RANDOM_MAX (INPUTSIZE*100)



namespace GraphTests {


/* ----- TYPEDEFS ----- */

typedef std::chrono::high_resolution_clock high_resolution_clock;
typedef high_resolution_clock::time_point time_point;



/* ----- FUNCTION DECLARATION ----- */


void printHeader();

void testDijkstra();


/* ----- IMPLEMENTATION ----- */

void testCorrectness() {

    //Basic usage test

    cg3::Graph<int> graph;

    cg3::Graph<int>::NodeIterator n2 = graph.nodeIteratorEnd(), n3 = graph.nodeIteratorEnd(), n4 = graph.nodeIteratorEnd(), nDuplicate = graph.nodeIteratorEnd();
    CG3_SUPPRESS_WARNING(n4);
    CG3_SUPPRESS_WARNING(nDuplicate);

    graph.addNode(2);
    assert(graph.findNode(2) != graph.nodeIteratorEnd());
    graph.deleteNode(2);
    assert(graph.findNode(2) == graph.nodeIteratorEnd());
    n2 = graph.addNode(2);
    n3 = graph.addNode(3);
    nDuplicate = graph.addNode(3);

    assert(n2 != graph.nodeIteratorEnd());
    assert(n3 != graph.nodeIteratorEnd());
    assert(nDuplicate == graph.nodeIteratorEnd());

    assert(!graph.isAdjacent(2, 3));
    assert(!graph.isAdjacent(n2, n3));

    graph.addEdge(2,3);
    assert(graph.isAdjacent(2, 3));
    assert(!graph.isAdjacent(3, 2));
    assert(graph.isAdjacent(n2, n3));
    assert(!graph.isAdjacent(n3, n2));

    graph.deleteEdge(n2,n3);
    assert(!graph.isAdjacent(2, 3));
    assert(!graph.isAdjacent(3, 2));
    assert(!graph.isAdjacent(n2, n3));
    assert(!graph.isAdjacent(n3, n2));

    graph.addEdge(n2,n3);
    assert(graph.isAdjacent(2, 3));
    assert(!graph.isAdjacent(3, 2));
    assert(graph.isAdjacent(n2, n3));
    assert(!graph.isAdjacent(n3, n2));

    graph.deleteEdge(2,3);
    assert(!graph.isAdjacent(2, 3));
    assert(!graph.isAdjacent(3, 2));
    assert(!graph.isAdjacent(n2, n3));
    assert(!graph.isAdjacent(n3, n2));

    int checkNumber = 2;
    n4 = graph.addNode(4);
    assert(*n4 == 4);
    for (const int& number : graph.nodeIterator()) {
        CG3_SUPPRESS_WARNING(number);
        assert(number == checkNumber);
        checkNumber++;
    }


    cg3::Graph<int> undirectedGraph(cg3::GraphType::UNDIRECTED);
    undirectedGraph.addNode(2);
    assert(undirectedGraph.findNode(2) != undirectedGraph.nodeIteratorEnd());
    undirectedGraph.deleteNode(2);
    assert(undirectedGraph.findNode(2) == undirectedGraph.nodeIteratorEnd());
    n2 = undirectedGraph.addNode(2);
    n3 = undirectedGraph.addNode(3);
    nDuplicate = undirectedGraph.addNode(3);
    assert(n2 != undirectedGraph.nodeIteratorEnd());
    assert(n3 != undirectedGraph.nodeIteratorEnd());
    assert(nDuplicate == undirectedGraph.nodeIteratorEnd());

    assert(!undirectedGraph.isAdjacent(2, 3));
    assert(!undirectedGraph.isAdjacent(n2, n3));

    undirectedGraph.addEdge(2,3);
    assert(undirectedGraph.isAdjacent(2, 3));
    assert(undirectedGraph.isAdjacent(3, 2));
    assert(undirectedGraph.isAdjacent(n2, n3));
    assert(undirectedGraph.isAdjacent(n3, n2));

    undirectedGraph.deleteEdge(n2,n3);
    assert(!undirectedGraph.isAdjacent(2, 3));
    assert(!undirectedGraph.isAdjacent(3, 2));
    assert(!undirectedGraph.isAdjacent(n2, n3));
    assert(!undirectedGraph.isAdjacent(n3, n2));

    undirectedGraph.addEdge(n2,n3);
    assert(undirectedGraph.isAdjacent(2, 3));
    assert(undirectedGraph.isAdjacent(3, 2));
    assert(undirectedGraph.isAdjacent(n2, n3));
    assert(undirectedGraph.isAdjacent(n3, n2));

    undirectedGraph.deleteEdge(2,3);
    assert(!undirectedGraph.isAdjacent(2, 3));
    assert(!undirectedGraph.isAdjacent(3, 2));
    assert(!undirectedGraph.isAdjacent(n2, n3));
    assert(!undirectedGraph.isAdjacent(n3, n2));

    int checkNumberUndirected = 2;
    n4 = undirectedGraph.addNode(4);
    assert(*n4 == 4);
    for (const int& number : undirectedGraph.nodeIterator()) {
        CG3_SUPPRESS_WARNING(number);
        assert(number == checkNumberUndirected);
        checkNumberUndirected++;
    }


    int number = 0;
    for (const int& n : undirectedGraph.adjacentNodeIterator(n4)) {
        CG3_SUPPRESS_WARNING(n);
        number++;
    }
    assert(number == 0);

    number = 0;
    for (std::pair<const int, const int> pair : undirectedGraph.edgeIterator()) {
        CG3_SUPPRESS_WARNING(pair);
        number++;
    }
    assert(number == 0);



    undirectedGraph.addEdge(n4, n2);


    number = 0;
    for (std::pair<const int, const int> pair : undirectedGraph.edgeIterator()) {
        CG3_SUPPRESS_WARNING(pair);
        number++;

        assert(undirectedGraph.findNode(pair.first) != undirectedGraph.nodeIteratorEnd());
        assert(undirectedGraph.findNode(pair.second) != undirectedGraph.nodeIteratorEnd());
    }
    assert(number == 2);

    number = 0;
    for (const int& n : undirectedGraph.adjacentNodeIterator(n4)) {
        CG3_SUPPRESS_WARNING(n);
        number++;

        assert(n == 2);
    }
    assert(number == 1);

    number = 0;
    for (const int& n : undirectedGraph.adjacentNodeIterator(n2)) {
        CG3_SUPPRESS_WARNING(n);
        number++;

        assert(n == 4);
    }
    assert(number == 1);
}



/* ----- FUNCTION DECLARATION ----- */

void printHeader() {
    std::cout <<
         std::setw(INDENTSPACE) << std::left << "METHOD" <<
         std::setw(INDENTSPACE) << std::left << "INPUTSIZE" <<
         std::setw(INDENTSPACE) << std::left << "ALGORITHM" <<
         std::setw(INDENTSPACE) << std::left << "TOTAL" <<
         std::endl << std::endl;
}


void testDijkstra()
{
    printHeader();

    for (int t = 0; t < ITERATION; t++) {
        //Setup random generator
        std::mt19937 rng;
        rng.seed(std::random_device()());
        std::uniform_int_distribution<std::mt19937::result_type>
                distIn(-RANDOM_MAX,RANDOM_MAX);

        std::vector<int> testNumbers;

        //Random test number generation
        for (int i = 0; i < INPUTSIZE; i++) {
            int randomValue = distIn(rng);
            testNumbers.push_back(randomValue);
        }
    }

    //TODO
}









}
