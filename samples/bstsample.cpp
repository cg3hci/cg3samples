/**
    @author Stefano Nuvoli
*/
#include "bstsample.h"

#include <iostream>

#include "cg3/data_structures/trees/bst.h"
//#include "trees/avlleaf.h"
//#include "trees/avlinner.h"
//#include "trees/bstinner.h"
//#include "trees/bstleaf.h"

/* Choose the one you prefer, you can use each one in the same way! */

typedef cg3::BST<int> BSTInt; //Default BST

//    typedef cg3::AVLLeaf<int> BSTInt;
//    typedef cg3::BSTLeaf<int> BSTInt;

//    typedef cg3::AVLInner<int> BSTInt;
//    typedef cg3::BSTInner<int> BSTInt;


/* Choose the one you prefer, you can use each one in the same way! */

typedef cg3::BST<int, std::string> BSTIntString; //Default BST

//    typedef cg3::AVLLeaf<int, std::string> BSTIntString;
//    typedef cg3::BSTLeaf<int, std::string> BSTIntString;

//    typedef cg3::AVLInner<int, std::string> BSTIntString;
//    typedef cg3::BSTInner<int, std::string> BSTIntString;


bool reverseComparator(const int& o1, const int& o2) {
    return o1 > o2;
}


void BinarySearchTreeSamples::sampleBST()
{
    //Creating BST (int) with default (<) comparator
    std::cout << "Creating BST (int keys and values) with default (<) comparator..." << std::endl;
    BSTInt bst;

    //Insert numbers: 2, 10, 3, 25, 12, 15, 45, 65, 100, 52
    std::cout << "Inserting numbers: 2, 10, 3, 25, 12, 15, 45, 65, 100, 52" << std::endl;
    bst.insert(2);
    bst.insert(10);
    bst.insert(3);
    bst.insert(25);
    bst.insert(12);
    bst.insert(15);
    bst.insert(45);
    bst.insert(65);
    bst.insert(100);
    bst.insert(52);

    //Get min and max (through iterators)
    BSTInt::Iterator minIterator = bst.getMin();
    int& minValue = *minIterator;
    std::cout << "Minimum node is: " << minValue << std::endl;

    std::cout << "Maximum node is: " << *(bst.getMax()) << std::endl;

    //Find number 2
    BSTInt::Iterator queryIterator = bst.find(2);
    if (queryIterator != bst.end())
        std::cout << "Number 2 is in the binary search tree!" << std::endl;
    else
        std::cout << "Number 2 is NOT in the binary search tree!" << std::endl;

    //Erase
    std::cout << "Erasing number 2..." << std::endl;    
    bst.erase(2);

    //Find number 2, after it has been erased
    if (bst.find(2) != bst.end())
        std::cout << "Number 2 is in the binary search tree!" << std::endl;
    else
        std::cout << "Number 2 is NOT in the binary search tree!" << std::endl;

    //Iteration
    std::cout << "The BST contains:" << std::endl << "    ";
    for (int& number : bst)
        std::cout << number << " ";
    std::cout << std::endl;


    //Range query in the interval 8 - 45
    std::cout << "Range query in the interval 8 - 45:" << std::endl << "    ";
    std::vector<BSTInt::Iterator> rangeQueryResults;
    bst.rangeQuery(8, 45, rangeQueryResults);
    for (BSTInt::Iterator& it : rangeQueryResults) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;


    std::cout << "Erasing the next of 52..." << std::endl;
    //Erase the next of 10 (erase with iterators)
    BSTInt::Iterator it52 = bst.find(52);
    it52++;
    bst.erase(it52);

    //Erase values less than 30 (erase with iterators
    std::cout << "Erasing numbers less than 30..." << std::endl;
    for (BSTInt::Iterator it = bst.begin(); it != bst.end();) {
        if (*it < 30)
            bst.erase(it++); //When an iterator is erased, iterator is automatically incremented!
        else
            it++;
    }

    //Iteration with explicit iterators
    std::cout << "The BST contains:" << std::endl << "    ";
    for (BSTInt::Iterator it = bst.begin(); it != bst.end(); it++) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    //Size
    std::cout << "The BST contains " << bst.size() << " elements" << std::endl;

    //Clear
    std::cout << "Clear BST..." << std::endl;
    bst.clear();



    std::cout << std::endl;








    //BST (int, string) with custom comparator (> instead of <) and different key/value types
    std::cout << "Creating BST (int keys and string values) with reverse (>) comparator..." << std::endl;
    BSTIntString bst2(reverseComparator);

    //Insert numbers: 1, 5, 3, 10, 7 with their name as value
    std::cout << "Inserting numbers: 1, 5, 3, 10, 7" << std::endl;
    bst2.insert(1, "one");
    bst2.insert(5, "five");
    bst2.insert(3, "three");
    bst2.insert(10, "ten");
    bst2.insert(7, "seven");

    //Iteration
    std::cout << "The BST with reverse comparator contains:" << std::endl << "    ";
    for (std::string& number : bst2)
        std::cout << number << " ";
    std::cout << std::endl;

    //No need to clear: clear with constructor

    std::cout << std::endl;
}
