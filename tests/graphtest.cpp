/*
 * This file is part of cg3lib: https://github.com/cg3hci/cg3lib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "graphtest.h"

#include <iostream>
#include <random>
#include <assert.h>
#include <iomanip>

#include <vector>

#include <cg3/data_structures/graphs/graph.h>
#include <cg3/algorithms/graph_algorithms.h>

#include <cg3/utilities/utils.h>

#include <cg3/cg3lib.h>
#include <cg3/utilities/timer.h>

#define ITERATION 3
#define INDENTSPACE 12

#define INPUTSIZE 1000
#define RANDOM_MAX (INPUTSIZE*1000)
#define MAXEDGES INPUTSIZE*10
#define MAXWEIGHT 100



namespace GraphTests {


/* ----- TYPEDEFS ----- */

typedef cg3::Graph<int> IntGraph;


/* ----- FUNCTION DECLARATION ----- */


void printHeader();

void testDijkstra();


/* ----- IMPLEMENTATION ----- */

void testCorrectness() {

    //Test directed graph
    cg3::Graph<int> graph;

    cg3::Graph<int>::NodeIterator n2 = graph.nodeIteratorEnd(), n3 = graph.nodeIteratorEnd(), n4 = graph.nodeIteratorEnd(), nDuplicate = graph.nodeIteratorEnd();
    CG3_SUPPRESS_WARNING(n4);
    CG3_SUPPRESS_WARNING(nDuplicate);


    for (const int& number : graph.nodeIterator()) {
        CG3_SUPPRESS_WARNING(number);
    }
    for (const std::pair<int,int>& edge : graph.edgeIterator()) {
        CG3_SUPPRESS_WARNING(edge);
    }

    graph.addNode(2);
    assert(graph.findNode(2) != graph.nodeIteratorEnd());

    for (const int& adjNumber : graph.adjacentNodeIterator(2)) {
        CG3_SUPPRESS_WARNING(adjNumber);
    }

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


    //Test copy constructor
    cg3::Graph<int> graph2(graph);

    graph.clear();

    assert(!graph2.isAdjacent(2, 3));
    assert(!graph2.isAdjacent(3, 2));

    checkNumber = 2;
    for (const int& number : graph2.nodeIterator()) {
        CG3_SUPPRESS_WARNING(number);
        assert(number == checkNumber);
        checkNumber++;
    }


    //Test undirected graph
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
         std::setw(INDENTSPACE) << std::left << "INS-E-V" <<
         std::setw(INDENTSPACE) << std::left << "ERA-E-V" <<
         std::setw(INDENTSPACE) << std::left << "INS-E-I" <<
         std::setw(INDENTSPACE) << std::left << "ERA-E-I" <<
         std::setw(INDENTSPACE) << std::left << "IT-E" <<
         std::setw(INDENTSPACE) << std::left << "NODES" <<
         std::setw(INDENTSPACE) << std::left << "EDGES" <<
         std::setw(INDENTSPACE) << std::left << "DIJKSTRA" <<
         std::setw(INDENTSPACE) << std::left << "COPY" <<
         std::setw(INDENTSPACE) << std::left << "DIJKSTRA" <<
         std::setw(INDENTSPACE) << std::left << "ERA-N-V" <<
         std::setw(INDENTSPACE) << std::left << "NODES" <<
         std::setw(INDENTSPACE) << std::left << "EDGES" <<
         std::setw(INDENTSPACE) << std::left << "DIJKSTRA" <<
         std::setw(INDENTSPACE) << std::left << "RECOMPACT" <<
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
                distIn(0,RANDOM_MAX*2);

        std::vector<int> testNumbers;

        //Random test number generation
        for (int i = 0; i < INPUTSIZE; i++) {
            int randomValue = distIn(rng)-RANDOM_MAX;
            testNumbers.push_back(randomValue);
        }

        IntGraph graph;

        std::cout << std::setw(INDENTSPACE) << std::left;
        std::cout << testNumbers.size();


        cg3::Timer totalTimer("Total");
        cg3::Timer timer("Step");

        totalTimer.start();


        int nEdges = 0;
        int nodesToBeDeleted;



        /* Insert nodes */

        timer.start();

        for (int n : testNumbers) {
            graph.addNode(n);
        }

        timer.stop();


        std::cout << std::setw(INDENTSPACE) << std::left;
        std::cout << timer.delay();




        /* Iteration on nodes */

        timer.start();

        int numNodes = 0;
        for (int n : graph.nodeIterator()) {
            CG3_SUPPRESS_WARNING(n);
            numNodes++;
        }

        timer.stop();

        assert(numNodes == (int) graph.numNodes());


        std::cout << std::setw(INDENTSPACE) << std::left;
        std::cout << timer.delay();




        /* Insert edges (v) */

        timer.start();

        for (size_t i = 0; nEdges < MAXEDGES && i < testNumbers.size(); i++) {
            for (size_t j = 0; nEdges < MAXEDGES && j < testNumbers.size(); j++) {
                double weight = std::abs((long) distIn(rng)) % MAXWEIGHT;
                graph.addEdge(testNumbers[i], testNumbers[j], weight);
                nEdges++;
            }
        }

        timer.stop();


        std::cout << std::setw(INDENTSPACE) << std::left;
        std::cout << timer.delay();




        /* Erase edges using values */

        timer.start();

        for (size_t i = 0; nEdges > 0 && i < testNumbers.size(); i++) {
            for (size_t j = 0; nEdges > 0 && j < testNumbers.size(); j++) {
                graph.deleteEdge(testNumbers[i], testNumbers[j]);
                nEdges--;
            }
        }

        timer.stop();


        std::cout << std::setw(INDENTSPACE) << std::left;
        std::cout << timer.delay();




        /* Insert edges (it) */

        timer.start();

        for (IntGraph::NodeIterator it1 = graph.nodeIteratorBegin(); nEdges < MAXEDGES && it1 != graph.nodeIteratorEnd(); it1++) {
            for (IntGraph::NodeIterator it2 = graph.nodeIteratorBegin(); nEdges < MAXEDGES && it2 != graph.nodeIteratorEnd(); it2++) {
                double weight = std::abs((long) distIn(rng)) % MAXWEIGHT;
                graph.addEdge(it1, it2, weight);
                nEdges++;
            }
        }

        timer.stop();

        std::cout << std::setw(INDENTSPACE) << std::left;
        std::cout << timer.delay();




        /* Erase edges using iterators */

        timer.start();

        for (IntGraph::NodeIterator it1 = graph.nodeIteratorBegin(); nEdges > 0 && it1 != graph.nodeIteratorEnd(); it1++) {
            for (IntGraph::NodeIterator it2 = graph.nodeIteratorBegin(); nEdges > 0 && it2 != graph.nodeIteratorEnd(); it2++) {
                graph.deleteEdge(it1, it2);
                nEdges--;
            }
        }

        timer.stop();



        std::cout << std::setw(INDENTSPACE) << std::left;
        std::cout << timer.delay();




        //Re-add the edges
        for (size_t i = 0; nEdges < MAXEDGES && i < testNumbers.size(); i++) {
            for (size_t j = 0; nEdges < MAXEDGES && j < testNumbers.size(); j++) {
                double weight = std::abs((long) distIn(rng)) % MAXWEIGHT;
                graph.addEdge(testNumbers[i], testNumbers[j], weight);
                nEdges++;
            }
        }



        /* Iteration on edges */

        timer.start();

        int numEdges = 0;
        for (std::pair<int, int> n : graph.edgeIterator()) {
            CG3_SUPPRESS_WARNING(n);
            numEdges++;
        }

        timer.stop();

        assert(numEdges == (int) graph.numEdges());

        std::cout << std::setw(INDENTSPACE) << std::left;
        std::cout << timer.delay();



        /* Number of components */
        std::cout << std::setw(INDENTSPACE) << std::left;
        std::cout << graph.numNodes();
        std::cout << std::setw(INDENTSPACE) << std::left;
        std::cout << graph.numEdges();



        /* Dijkstra */

        timer.start();

        for (IntGraph::NodeIterator it = graph.nodeIteratorBegin(); it != graph.nodeIteratorEnd(); it++) {
            int n = *it;
            cg3::dijkstra(graph, n);
        }

        timer.stop();

        std::cout << std::setw(INDENTSPACE) << std::left;
        std::cout << timer.delay();



        /* Copy graph */

        timer.start();

        graph = IntGraph(graph);

        timer.stop();


        std::cout << std::setw(INDENTSPACE) << std::left;
        std::cout << timer.delay();



        /* Dijkstra */

        timer.start();

        for (IntGraph::NodeIterator it = graph.nodeIteratorBegin(); it != graph.nodeIteratorEnd(); it++) {
            int n = *it;
            cg3::dijkstra(graph, n);
        }

        timer.stop();

        std::cout << std::setw(INDENTSPACE) << std::left;
        std::cout << timer.delay();




        /* Delete nodes */

        nodesToBeDeleted = INPUTSIZE/2;

        timer.start();

        size_t stepDelete = testNumbers.size() / nodesToBeDeleted;
        for (size_t i = 0; i < testNumbers.size(); i += stepDelete) {
            int n = testNumbers[i];

            graph.deleteNode(n);
        }

        timer.stop();

        std::cout << std::setw(INDENTSPACE) << std::left;
        std::cout << timer.delay();




        /* Reinsert nodes and edges */

        for (int n : testNumbers) {
            graph.addNode(n);
        }
        nEdges = 0;
        for (size_t i = 0; nEdges < MAXEDGES && i < testNumbers.size(); i++) {
            for (size_t j = 0; nEdges < MAXEDGES && j < testNumbers.size(); j++) {
                double weight = std::abs((long) distIn(rng)) % MAXWEIGHT;
                graph.addEdge(testNumbers[i], testNumbers[j], weight);
                nEdges++;
            }
        }



        /* Number of components */
        std::cout << std::setw(INDENTSPACE) << std::left;
        std::cout << graph.numNodes();
        std::cout << std::setw(INDENTSPACE) << std::left;
        std::cout << graph.numEdges();



        /* Dijkstra */

        timer.start();

        for (IntGraph::NodeIterator it = graph.nodeIteratorBegin(); it != graph.nodeIteratorEnd(); it++) {
            int n = *it;
            cg3::dijkstra(graph, n);
        }

        timer.stop();

        std::cout << std::setw(INDENTSPACE) << std::left;
        std::cout << timer.delay();



        /* Recompact graph */

        timer.start();

        graph.recompact();

        timer.stop();

        std::cout << std::setw(INDENTSPACE) << std::left;
        std::cout << timer.delay();



        /* Dijkstra */

        timer.start();

        for (IntGraph::NodeIterator it = graph.nodeIteratorBegin(); it != graph.nodeIteratorEnd(); it++) {
            int n = *it;
            cg3::dijkstra(graph, n);
        }

        timer.stop();

        std::cout << std::setw(INDENTSPACE) << std::left;
        std::cout << timer.delay();


        /* Clear */

        timer.start();

        graph.clear();

        timer.stop();

        std::cout << std::setw(INDENTSPACE) << std::left;
        std::cout << timer.delay();




        /* Total */

        totalTimer.stop();

        std::cout << std::setw(INDENTSPACE) << std::left;
        std::cout << totalTimer.delay();


        std::cout << std::endl;
    }

}

}
