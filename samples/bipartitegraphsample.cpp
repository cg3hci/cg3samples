#include "bipartitegraphsample.h"

void BipartiteGraphSample::execute() {

    //this bipartite graph connects strings to integers
    //every string represent an integer.
    cg3::BipartiteGraph<std::string, int> bigraph;
    bigraph.addUNode("2");
    bigraph.addUNode("3");
    bigraph.addUNode("4");
    bigraph.addUNode("5");
    bigraph.addVNode(10);
    bigraph.addVNode(12);
    bigraph.addVNode(15);
    bigraph.addVNode(17);
    bigraph.addVNode(20);
    bigraph.addVNode(24);

    //every arc between u and v means that u is divisor of v
    bigraph.addArc("2", 10);
    bigraph.addArc("2", 12);
    bigraph.addArc("2", 20);
    bigraph.addArc("2", 24);
    bigraph.addArc("3", 12);
    bigraph.addArc("3", 15);
    bigraph.addArc("3", 24);
    bigraph.addArc("4", 12);
    bigraph.addArc("4", 20);
    bigraph.addArc("4", 24);
    bigraph.addArc("5", 10);
    bigraph.addArc("5", 15);
    bigraph.addArc("5", 20);

    std::cout << "Multiples of 3: \n";
    for (const int& v : bigraph.adjacentUNodeIterator("3")){
        std::cout << "\t" << std::to_string(v) << "\n";
    }

    std::cout << "Divisors of 24: \n";
    for (const std::string& u : bigraph.adjacentVNodeIterator(24)){
        std::cout << "\t" << u << "\n";
    }

    try {
        std::cout << "\nDivisors of 22: \n";
        for (const std::string& u : bigraph.adjacentVNodeIterator(22)){
            std::cout << "\t" << u << "\n";
        }
    }
    catch(std::out_of_range& e){
        std::cout << e.what() << ": 22 not found.\n\n";
    }

    bigraph.deleteUNode("4");
    std::cout << "Divisors of 24 after removing unode 4: \n";
    for (const std::string& u : bigraph.adjacentVNodeIterator(24)){
        std::cout << "\t" << u << "\n";
    }

    bigraph.addArc("4", 22); //no arcs are added here

}
