/**
    @author Stefano Nuvoli
*/
#include "graphsample.h"

#include <iostream>

#include <vector>

#include <cg3/data_structures/graphs/graph.h>

void GraphSamples::execute()
{
    sampleDirected();
    std::cout << std::endl;
    sampleUndirected();
    std::cout << std::endl;
    sampleWeighted();
    std::cout << std::endl;
    sampleIterators();
}


/**
 * @brief Directed graph sample
 */
void GraphSamples::sampleDirected() {

    std::cout<< std::endl << " >> DIRECTED" << std::endl << std::endl;

    std::cout << "Create a directed graph..." << std::endl;
    //In constructor, cg3::GraphType::DIRECTED is the default type
    cg3::Graph<int> dGraph;
    //cg3::Graph<int> dGraph(cg3::GraphType::DIRECTED);


    //Create the nodes
    std::cout << "Create nodes 1 and 2..." << std::endl;
    //Note that if you use the iterators in the operations, then
    //you have to be sure that they are pointing to an existing node
    //(that has not been deleted for example)
    cg3::Graph<int>::NodeIterator it1 = dGraph.addNode(1);
    cg3::Graph<int>::NodeIterator it2 = dGraph.addNode(2);



    //Check adjacency with iterators (faster)
    if (dGraph.isAdjacent(it1, it2)) {
        std::cout << "Node 1 has an arc to node 2." << std::endl;
    }
    else {
        std::cout << "Node 1 has not an arc to node 2." << std::endl;
    }

    //Check adjacency with values (slower)
    if (dGraph.isAdjacent(1,2)) {
        std::cout << "Node 1 has an arc to node 2." << std::endl;
    }
    else {
        std::cout << "Node 1 has not an arc to node 2." << std::endl;
    }


    //Create arc
    std::cout << "Create arc between 1 and 2..." << std::endl;

    //Faster using iterators
    dGraph.addArc(it1, it2);

    //No changes: the graph has already that arc
    //Slow using values
    dGraph.addArc(1, 2);

    //Check adjacency (slow)
    if (dGraph.isAdjacent(1, 2))
        std::cout << "Node 1 has an arc to node 2." << std::endl;
    else
        std::cout << "Node 1 has not an arc to node 2." << std::endl;

    if (dGraph.isAdjacent(2, 1))
        std::cout << "Node 2 has an arc to node 1." << std::endl;
    else
        std::cout << "Node 2 has not an arc to node 1." << std::endl;




    std::cout << "Delete arc between node 1 and node 2" << std::endl;
    //Delete arc (fast with iterators)
    dGraph.deleteArc(it1, it2);

    //No changes: the graph has already that arc
    //Slow using values
    dGraph.deleteArc(1, 2);



    //Check adjacency (fast)
    if (dGraph.isAdjacent(it1, it2))
        std::cout << "Node 1 has an arc to node 2." << std::endl;
    else
        std::cout << "Node 1 has not an arc to node 2." << std::endl;

    //Check adjacency (slow)
    if (dGraph.isAdjacent(2, 1))
        std::cout << "Node 2 has an arc to node 1." << std::endl;
    else
        std::cout << "Node 2 has not an arc to node 1." << std::endl;



    //Find node
    cg3::Graph<int>::NodeIterator itFind = dGraph.findNode(2);
    if (itFind != dGraph.nodeIteratorEnd())
        std::cout << "Node 2 is in the graph." << std::endl;
    else
        std::cout << "Node 2 is not in the graph." << std::endl;




    //Delete node with iterator (it is not slower)
    bool deleteResult = dGraph.deleteNode(2);
    if (deleteResult)
        std::cout << "Node 2 was in the graph and it has been deleted." << std::endl;
    else
        std::cout << "Node 2 was not in the graph. No changes." << std::endl;

    //Delete node with iterator (it is not faster)
    deleteResult = dGraph.deleteNode(itFind);
    if (deleteResult)
        std::cout << "Node 2 was in the graph and it has been deleted." << std::endl;
    else
        std::cout << "Node 2 has not been found in the graph. No changes." << std::endl;


    //Find node
    cg3::Graph<int>::NodeIterator itFind2 = dGraph.findNode(2);
    if (itFind2 != dGraph.nodeIteratorEnd())
        std::cout << "Node 2 is in the graph." << std::endl;
    else
        std::cout << "Node 2 is not in the graph." << std::endl;

}

/**
 * @brief Undirected graph sample
 */
void GraphSamples::sampleUndirected() {

    std::cout << std::endl << " >> UNDIRECTED" << std::endl << std::endl;

    //Create graph
    std::cout << "Create undirected graph..." << std::endl;
    cg3::Graph<int> uGraph(cg3::GraphType::UNDIRECTED); //cg3::GraphType::DIRECTED is the default type



    //Create the nodes
    std::cout << "Create nodes 1 and 2..." << std::endl;
    //Note that if you use the iterators in the operations, then
    //you have to be sure that they are pointing to an existing node
    //(that has not been deleted for example)
    cg3::Graph<int>::NodeIterator it1 = uGraph.addNode(1);
    cg3::Graph<int>::NodeIterator it2 = uGraph.addNode(2);




    //Check adjacency with iterators (faster)
    if (uGraph.isAdjacent(it1, it2)) {
        std::cout << "Node 1 has an arc to node 2." << std::endl;
    }
    else {
        std::cout << "Node 1 has not an arc to node 2." << std::endl;
    }

    //Check adjacency with values (slower)
    if (uGraph.isAdjacent(1,2)) {
        std::cout << "Node 1 has an arc to node 2." << std::endl;
    }
    else {
        std::cout << "Node 1 has not an arc to node 2." << std::endl;
    }



    //Create arc
    std::cout << "Create arc between 1 and 2..." << std::endl;

    //Faster using iterators
    uGraph.addArc(it1, it2);

    //No changes: the graph has already that arc
    //Slow using values
    uGraph.addArc(1, 2);



    //Check adjacency (slow)
    if (uGraph.isAdjacent(1, 2))
        std::cout << "Node 1 has an arc to node 2." << std::endl;
    else
        std::cout << "Node 1 has not an arc to node 2." << std::endl;

    if (uGraph.isAdjacent(2, 1))
        std::cout << "Node 2 has an arc to node 1." << std::endl;
    else
        std::cout << "Node 2 has not an arc to node 1." << std::endl;



    std::cout << "Delete arc between node 1 and node 2" << std::endl;
    //Delete arc (fast with iterators)
    uGraph.deleteArc(it1, it2);

    //No changes: the graph has already that arc
    //Slow using values
    uGraph.deleteArc(1, 2);



    //Check adjacency (fast)
    if (uGraph.isAdjacent(it1, it2))
        std::cout << "Node 1 has an arc to node 2." << std::endl;
    else
        std::cout << "Node 1 has not an arc to node 2." << std::endl;

    //Check adjacency (slow)
    if (uGraph.isAdjacent(2, 1))
        std::cout << "Node 2 has an arc to node 1." << std::endl;
    else
        std::cout << "Node 2 has not an arc to node 1." << std::endl;



    //Find node
    cg3::Graph<int>::NodeIterator itFind = uGraph.findNode(2);
    if (itFind != uGraph.nodeIteratorEnd())
        std::cout << "Node 2 is in the graph." << std::endl;
    else
        std::cout << "Node 2 is not in the graph." << std::endl;



    //Delete node with iterator (it is not faster)
    bool deleteResult = uGraph.deleteNode(itFind);
    if (deleteResult)
        std::cout << "Node 2 was in the graph and it has been deleted." << std::endl;
    else
        std::cout << "Node 2 was not in the graph. No changes." << std::endl;

    //Delete node with iterator (it is not slower)
    deleteResult = uGraph.deleteNode(2);
    if (deleteResult)
        std::cout << "Node 2 was in the graph and it has been deleted." << std::endl;
    else
        std::cout << "Node 2 was not in the graph. No changes." << std::endl;





    //Find node
    cg3::Graph<int>::NodeIterator itFind2 = uGraph.findNode(2);
    if (itFind2 != uGraph.nodeIteratorEnd())
        std::cout << "Node 2 is in the graph." << std::endl;
    else
        std::cout << "Node 2 is not in the graph." << std::endl;

}


/**
 * @brief Undirected weight graph sample
 */
void GraphSamples::sampleWeighted() {

    std::cout << std::endl << " >> UNDIRECTED WEIGHTED" << std::endl << std::endl;

    //Create graph
    std::cout << "Create undirected graph..." << std::endl;
    cg3::Graph<int> graph(cg3::GraphType::UNDIRECTED); //cg3::GraphType::DIRECTED is the default type


    double weight;


    //Create the nodes
    std::cout << "Create nodes 1 and 2..." << std::endl;
    //Note that if you use the iterators in the operations, then
    //you have to be sure that they are pointing to an existing node
    //(that has not been deleted for example)
    cg3::Graph<int>::NodeIterator it1 = graph.addNode(1);
    cg3::Graph<int>::NodeIterator it2 = graph.addNode(2);



    //No weight
    weight = graph.getWeight(it1, it2);
    if (weight == DBL_MAX)
        std::cout << "The nodes 1 and 2 are not adjacent! Weight returned is: " << weight << std::endl;
    else
        std::cout << "(1,2): Weight is: " << weight << std::endl;


    //Add arc with undefined weight
    std::cout << "Add arc between nodes 1 and 2 (no weight defined)..." << std::endl;
    graph.addArc(1, 2); //Default weight: 0

    weight = graph.getWeight(it1, it2);
    if (weight == DBL_MAX)
        std::cout << "The nodes 1 and 2 are not adjacent! Weight returned is: " << weight << std::endl;
    else
        std::cout << "(1,2): Weight is: " << weight << std::endl;



    //Add arc with weight 10 (replacing)
    std::cout << "Add arc between nodes 1 and 2 (weight 10)... It replaces the last weight of the arc." << std::endl;
    graph.addArc(1, 2, 10); //It replaces the weight

    weight = graph.getWeight(it1, it2);
    if (weight == DBL_MAX)
        std::cout << "The nodes 1 and 2 are not adjacent! Weight returned is: " << weight << std::endl;
    else
        std::cout << "(1,2): Weight is: " << weight << std::endl;



    //Add arc with weight 10
    std::cout << "Set weight to 27..." << std::endl;
    graph.setWeight(it1, it2, 27);

    weight = graph.getWeight(it1, it2);
    if (weight == DBL_MAX)
        std::cout << "The nodes 1 and 2 are not adjacent! Weight returned is: " << weight << std::endl;
    else
        std::cout << "(1,2): Weight is: " << weight << std::endl;



    //Using no iterators (slower)
    std::cout << "Double the weight (not using the iterators)..." << std::endl;
    graph.setWeight(1, 2, graph.getWeight(1, 2) * 2); //Double the weight

    weight = graph.getWeight(1, 2);
    if (weight == DBL_MAX)
        std::cout << "The nodes 1 and 2 are not adjacent! Weight returned is: " << weight << std::endl;
    else
        std::cout << "(1,2): Weight is: " << weight << std::endl;



    std::cout << "Setting weight of an arc of the node 3 (that is not in the graph)..." << std::endl;
    //Setting the weight with a node (3) that is not in the graph
    graph.setWeight(1, 3, 10); //No effect

    //Getting the weight with a node (3) that is not in the graph
    weight = graph.getWeight(1, 3);
    if (weight == DBL_MAX)
        std::cout << "The nodes 1 and 3 are not adjacent or a node does not exist! Weight returned is: " << weight << std::endl;
    else
        std::cout << "(1,3): Weight is: " << weight << std::endl;


    // Add the node 3
    std::cout << "Add node 3..." << std::endl;
    graph.addNode(3);



    std::cout << "Setting weight of the arc (1,3) (that is not in the graph)..." << std::endl;
    //Setting the weight of an arc (1,3) that is not in the graph
    graph.setWeight(1, 3, 10); //No effect

    //Getting the weight of an arc (1,3) that is not in the graph
    weight = graph.getWeight(1, 3);
    if (weight == DBL_MAX)
        std::cout << "The nodes 1 and 3 are not adjacent or a node does not exist! Weight returned is: " << weight << std::endl;
    else
        std::cout << "(1,3): Weight is: " << weight << std::endl;



    //Add arc
    std::cout << "Add arc (3,1) with weight 20..." << std::endl;
    graph.addArc(3, 1, 20);

    //Getting the weight of an arc (1,3) that is not in the graph
    weight = graph.getWeight(1, 3);
    if (weight == DBL_MAX)
        std::cout << "The nodes 1 and 3 are not adjacent or a node does not exist! Weight returned is: " << weight << std::endl;
    else
        std::cout << "(1,3): Weight is: " << weight << std::endl;




    std::cout << "Setting weight of the arc (1,3) to 40 (the graph is undirected)..." << std::endl;
    //Setting the weight of an arc (1,3) that is not in the graph
    graph.setWeight(1, 3, 40); //No effect

    //Getting the weight of an arc (1,3) that is not in the graph
    weight = graph.getWeight(1, 3);
    if (weight == DBL_MAX)
        std::cout << "The nodes 1 and 3 are not adjacent or a node does not exist! Weight returned is: " << weight << std::endl;
    else
        std::cout << "(1,3): Weight is: " << weight << std::endl;

    //Getting the weight of an arc (1,3) that is not in the graph
    weight = graph.getWeight(3,1);
    if (weight == DBL_MAX)
        std::cout << "The nodes 3 and 1 are not adjacent or a node does not exist! Weight returned is: " << weight << std::endl;
    else
        std::cout << "(3,1): Weight is: " << weight << std::endl;


}



/**
 * @brief Directed graph iteration sample
 */
void GraphSamples::sampleIterators() {

    std::cout<< std::endl << " >> ITERATORS" << std::endl << std::endl;

    //Create directed graph
    std::cout << "Create directed graph..." << std::endl;
    cg3::Graph<int> graph;

    //Iteration on nodes of the empty graph
    std::cout << "Nodes of the graph: ";
    for (const int& node : graph.nodeIterator()) {
        std::cout << node << " ";
    }
    std::cout << std::endl;

    //Add elements to the graphs
    std::cout << "Add nodes 1, 2, 4, 3 and arcs (1,2) - (1,3) - (1,4) - (2,1)" << std::endl;
    graph.addNode(1);
    graph.addNode(2);
    graph.addNode(4);
    graph.addNode(3);
    graph.addArc(1,2);
    graph.addArc(1,3);
    graph.addArc(1,4);
    graph.addArc(2,1);

    //Iteration on graph nodes
    std::cout << "Nodes of the graph: ";
    for (const int& node : graph.nodeIterator()) {
        std::cout << node << " ";
    }
    std::cout << std::endl;

    //Iteration on graph using explicit node iterators
    std::cout << "Nodes of the graph: ";
    for (cg3::Graph<int>::NodeIterator it = graph.nodeIteratorBegin(); it != graph.nodeIteratorEnd(); it++) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;



    //TODO ARC ITERATOR

    //TODO ADJACENT NODE ITERATOR
}
