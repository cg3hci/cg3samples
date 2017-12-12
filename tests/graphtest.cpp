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
#include "cg3/algorithms/graph_algorithms.h"

#include "cg3/utilities/utils.h"

#define ITERATION 3
#define INDENTSPACE 12

#define INPUTSIZE 1000
#define RANDOM_MAX (INPUTSIZE*100)
#define MAXEDGES 100000
#define MAXWEIGHT 100
#define DIJKSTRAITERATIONS 100



namespace GraphTests {


/* ----- TYPEDEFS ----- */

typedef std::chrono::high_resolution_clock high_resolution_clock;
typedef high_resolution_clock::time_point time_point;


typedef cg3::Graph<int> IntGraph;


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
         std::setw(INDENTSPACE) << std::left << "INPUTSIZE" <<
         std::setw(INDENTSPACE) << std::left << "INS-N" <<
         std::setw(INDENTSPACE) << std::left << "IT-N" <<
         std::setw(INDENTSPACE) << std::left << "NUM" <<
         std::setw(INDENTSPACE) << std::left << "INS-E-V" <<
         std::setw(INDENTSPACE) << std::left << "ERA-E-V" <<
         std::setw(INDENTSPACE) << std::left << "INS-E-I" <<
         std::setw(INDENTSPACE) << std::left << "ERA-E-I" <<
         std::setw(INDENTSPACE) << std::left << "IT-E" <<
         std::setw(INDENTSPACE) << std::left << "NUM" <<
         std::setw(INDENTSPACE) << std::left << "DIJKSTRA" <<
         std::setw(INDENTSPACE) << std::left << "CLEAR" <<
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

        IntGraph graph;

        std::cout << std::setw(INDENTSPACE) << std::left;
        std::cout << testNumbers.size();

        time_point tstart = high_resolution_clock::now();

        time_point t1;
        time_point t2;


        int nEdges = 0;



        /* Insert nodes */

        t1 = high_resolution_clock::now();

        for (int n : testNumbers) {
            graph.addNode(n);
        }

        t2 = high_resolution_clock::now();


        auto insertTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

        std::cout << std::setw(INDENTSPACE) << std::left;
        std::cout << (double) insertTime/1000;




        /* Iteration on nodes */

        t1 = high_resolution_clock::now();

        int numNodes = 0;
        for (int n : graph.nodeIterator()) {
            CG3_SUPPRESS_WARNING(n);
            numNodes++;
        }

        t2 = high_resolution_clock::now();


        auto nodeIterationTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

        std::cout << std::setw(INDENTSPACE) << std::left;
        std::cout << (double) nodeIterationTime/1000;

        std::cout << std::setw(INDENTSPACE) << std::left;
        std::cout << numNodes;




        /* Insert edges (v) */

        t1 = high_resolution_clock::now();

        for (uint i = 0; nEdges < MAXEDGES && i < testNumbers.size(); i++) {
            for (uint j = 0; nEdges < MAXEDGES && j < testNumbers.size(); j++) {
                double weight = std::abs((long) distIn(rng)) % MAXWEIGHT;
                graph.addEdge(testNumbers[i], testNumbers[j], weight);
                nEdges++;
            }
        }

        t2 = high_resolution_clock::now();


        auto edgeInsertValueTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

        std::cout << std::setw(INDENTSPACE) << std::left;
        std::cout << (double) edgeInsertValueTime/1000;




        /* Erase edges using values */

        t1 = high_resolution_clock::now();

        for (uint i = 0; nEdges > 0 && i < testNumbers.size(); i++) {
            for (uint j = 0; nEdges > 0 && j < testNumbers.size(); j++) {
                graph.deleteEdge(testNumbers[i], testNumbers[j]);
                nEdges--;
            }
        }

        t2 = high_resolution_clock::now();


        auto edgeEraseValueTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

        std::cout << std::setw(INDENTSPACE) << std::left;
        std::cout << (double) edgeEraseValueTime/1000;




        /* Insert edges (it) */

        t1 = high_resolution_clock::now();

        for (IntGraph::NodeIterator it1 = graph.nodeIteratorBegin(); nEdges < MAXEDGES && it1 != graph.nodeIteratorEnd(); it1++) {
            for (IntGraph::NodeIterator it2 = graph.nodeIteratorBegin(); nEdges < MAXEDGES && it2 != graph.nodeIteratorEnd(); it2++) {
                double weight = std::abs((long) distIn(rng)) % MAXWEIGHT;
                graph.addEdge(it1, it2, weight);
                nEdges++;
            }
        }

        t2 = high_resolution_clock::now();

        auto edgeInsertIteratorTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

        std::cout << std::setw(INDENTSPACE) << std::left;
        std::cout << (double) edgeInsertIteratorTime/1000;




        /* Erase edges using iterators */

        t1 = high_resolution_clock::now();

        for (IntGraph::NodeIterator it1 = graph.nodeIteratorBegin(); nEdges > 0 && it1 != graph.nodeIteratorEnd(); it1++) {
            for (IntGraph::NodeIterator it2 = graph.nodeIteratorBegin(); nEdges > 0 && it2 != graph.nodeIteratorEnd(); it2++) {
                graph.deleteEdge(it1, it2);
                nEdges--;
            }
        }

        t2 = high_resolution_clock::now();



        auto edgeEraseIterationTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

        std::cout << std::setw(INDENTSPACE) << std::left;
        std::cout << (double) edgeEraseIterationTime/1000;




        //Re-add the edges
        for (uint i = 0; nEdges < MAXEDGES && i < testNumbers.size(); i++) {
            for (uint j = 0; nEdges < MAXEDGES && j < testNumbers.size(); j++) {
                double weight = std::abs((long) distIn(rng)) % MAXWEIGHT;
                graph.addEdge(testNumbers[i], testNumbers[j], weight);
                nEdges++;
            }
        }



        /* Iteration on edges */

        t1 = high_resolution_clock::now();

        int numEdges = 0;
        for (std::pair<int, int> n : graph.edgeIterator()) {
            CG3_SUPPRESS_WARNING(n);
            numEdges++;
        }

        t2 = high_resolution_clock::now();


        auto edgeIterationTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

        std::cout << std::setw(INDENTSPACE) << std::left;
        std::cout << (double) edgeIterationTime/1000;

        std::cout << std::setw(INDENTSPACE) << std::left;
        std::cout << numEdges;





        /* Dijkstra */

        t1 = high_resolution_clock::now();

        int dijkstraIterations = 0;
        for (int n : testNumbers) {
            cg3::dijkstra(graph, n);
            dijkstraIterations++;

            if (dijkstraIterations >= DIJKSTRAITERATIONS)
                break;
        }

        t2 = high_resolution_clock::now();

        auto dijkstraTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

        std::cout << std::setw(INDENTSPACE) << std::left;
        std::cout << (double) dijkstraTime/1000;




        /* Clear */

        t1 = high_resolution_clock::now();

        graph.clear();

        t2 = high_resolution_clock::now();

        auto clearTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

        std::cout << std::setw(INDENTSPACE) << std::left;
        std::cout << (double) clearTime/1000;




        /* Total */

        time_point tend = high_resolution_clock::now();

        auto totalTime = std::chrono::duration_cast<std::chrono::microseconds>(tend - tstart).count();

        std::cout << std::setw(INDENTSPACE) << std::left;
        std::cout << (double) totalTime/1000;

        std::cout << std::endl;
    }

}









}
