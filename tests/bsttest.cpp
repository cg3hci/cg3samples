/*
 * This file is part of cg3lib: https://github.com/cg3hci/cg3lib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "bsttest.h"

#include <iostream>
#include <random>
#include <assert.h>
#include <iomanip>

#include <set>
#include <vector>

#include <cg3/data_structures/trees/bstinner.h>
#include <cg3/data_structures/trees/avlinner.h>
#include <cg3/data_structures/trees/bstleaf.h>
#include <cg3/data_structures/trees/avlleaf.h>

#include <cg3/cg3lib.h>
#include <cg3/utilities/timer.h>

#define ITERATION 1
#define INDENTSPACE 12

#define INPUTSIZE 15000
#define RANDOM_MAX (INPUTSIZE)
#define ONLYEFFICIENT (INPUTSIZE > 15000)





namespace BSTTests {


/* ----- TYPEDEFS ----- */

template <class T> using BSTInner = typename cg3::BSTInner<T>;
template <class T> using BSTLeaf = typename cg3::BSTLeaf<T>;
template <class T> using AVLInner = typename cg3::AVLInner<T>;
template <class T> using AVLLeaf = typename cg3::AVLLeaf<T>;



/* ----- FUNCTION DECLARATION ----- */


void printHeader();



template <class B>
void testCorrectness();


void doTestsOnInput(std::vector<int>& testNumbers, std::vector<int>& randomNumbers);

void testSTL(std::vector<int>& testNumbers, std::vector<int>& randomNumbers);

template <class B>
void testBST(std::vector<int>& testNumbers, std::vector<int>& randomNumbers);



/* ----- IMPLEMENTATION ----- */

void testCorrectness() {
    testCorrectness<BSTInner<int>>();
    testCorrectness<BSTLeaf<int>>();
    testCorrectness<AVLInner<int>>();
    testCorrectness<AVLLeaf<int>>();
}
template <class B>
void testCorrectness() {

    //Test correctness of iterators

    B bst1;
    B bst2;

    typename B::iterator minIt = bst1.end();
    typename B::iterator maxIt = bst1.end();
    typename B::reverse_iterator minRIt = bst1.rend();
    typename B::reverse_iterator maxRIt = bst1.rend();

    //Empty BST
    minIt = bst1.getMin();
    assert(minIt == bst1.end());
    maxIt = bst1.getMax();
    assert(maxIt == bst1.end());
    assert(bst1.end()-1 == bst1.end());

    minIt = bst1.begin();
    assert(minIt == bst1.end());
    maxIt = bst1.end()-1;
    assert(maxIt == bst1.end());
    assert(bst1.end()-1 == bst1.end());


    //Inserting data
    typename B::insert_iterator outIt = bst1.inserter();
    *outIt = 3;
    outIt++;
    *outIt = 1;
    outIt++;

    bst1.insert(2);


    std::vector<int> vec;
    vec.push_back(9);
    vec.push_back(7);
    vec.push_back(5);

    //Insert iterator correctness
    std::copy(vec.begin(), vec.end(), bst2.inserter());
    std::copy(bst1.begin()+2, bst1.end(), bst2.inserter());
    assert(*bst2.begin() == 3);
    assert(*(bst2.end()-1) == 9);
    assert(*(++bst2.begin()) == 5);

    for (typename B::reverse_iterator it = bst2.rbegin(); it != bst2.rend(); it++) {
        *it = 3;
    }
    for (typename B::const_iterator it = bst2.cbegin(); it != bst2.cend(); it++) {
        assert(*it == 3);
    }
    for (typename B::iterator it = bst2.begin(); it != bst2.end(); it++) {
        *it = 4;
    }
    for (typename B::const_reverse_iterator it = bst2.crbegin(); it != bst2.crend(); it++) {
        assert(*it == 4);
    }
    bst2.clear();
    for (typename B::iterator it = bst2.begin(); it != bst2.end(); it++) {
        assert(false);
    }
    for (typename B::const_iterator it = bst2.cbegin(); it != bst2.cend(); it++) {
        assert(false);
    }
    for (typename B::reverse_iterator it = bst2.rbegin(); it != bst2.rend(); it++) {
        assert(false);
    }
    for (typename B::const_reverse_iterator it = bst2.crbegin(); it != bst2.crend(); it++) {
        assert(false);
    }

    //Check copy constructor and move constructor
    bst2 = bst1;
    bst2.clear();

    for (int i = 0; i < 2000; i++)
        vec.push_back(i);
    bst2 = B(vec);
    bst2.insert(3);
    bst2.clear();

    B bst3(std::move(B(vec)));
    bst3.insert(2);
    bst3.clear();


    //Iterator correctness
    minIt = bst1.begin();
    assert(*minIt == 1);

    minIt++;
    assert(*minIt == 2);

    maxIt = bst1.end()-1;
    assert(*maxIt == 3);
    assert(*(maxIt-1) == 2);

    maxIt--;
    assert(*maxIt == 2);

    maxIt = bst1.end()-1;
    assert(*maxIt == 3);
    assert(*(maxIt-1) == 2);

    assert(*(bst1.end()-1) == 3);
    assert(*(bst1.end()-2) == 2);
    assert(*(bst1.end()-3) == 1);
    assert((bst1.end()-4) == bst1.end());


    //Reverse iterator correctness
    minRIt = bst1.rbegin();
    assert(*minRIt == 3);

    minRIt++;
    assert(*minRIt == 2);

    maxRIt = bst1.rend()-1;
    assert(*maxRIt == 1);
    assert(*(maxRIt-1) == 2);

    maxRIt--;
    assert(*maxRIt == 2);

    maxRIt = bst1.rend()-1;
    assert(*maxRIt == 1);
    assert(*(maxRIt-1) == 2);

    assert(*(bst1.rend()-1) == 1);
    assert(*(bst1.rend()-2) == 2);
    assert(*(bst1.rend()-3) == 3);
    assert((bst1.rend()-4) == bst1.rend());

    minRIt = bst1.rbegin();
    bst1.erase(minRIt);
    minRIt = bst1.rbegin();
    assert(*minRIt == 2);

    typename B::iterator findResult = bst1.find(2);
    try {
        bst2.erase(findResult);
        assert(false);
    }
    catch (...) {

    }

    try {
        bst1.erase(findResult);
    }
    catch (...) {
        assert(false);
    }

    minRIt = bst1.rbegin();
    assert(*minRIt == 1);

    bst1.erase(1);

    bst1.insert(3);
    bst1.insert(9);
    bst1.insert(5);

    //Lower
    assert(bst1.findLower(-1) == bst1.end());
    assert(bst1.findLower(2) == bst1.end());
    assert(*(bst1.findLower(3)) == 3);
    assert(*(bst1.findLower(4)) == 3);
    assert(*(bst1.findLower(5)) == 5);
    assert(*(bst1.findLower(6)) == 5);
    assert(*(bst1.findLower(9)) == 9);
    assert(*(bst1.findLower(10)) == 9);

    //Upper
    assert(*(bst1.findUpper(-1)) == 3);
    assert(*(bst1.findUpper(2)) == 3);
    assert(*(bst1.findUpper(3)) == 5);
    assert(*(bst1.findUpper(4)) == 5);
    assert(*(bst1.findUpper(5)) == 9);
    assert(*(bst1.findUpper(6)) == 9);
    assert(bst1.findUpper(9) == bst1.end());
    assert(bst1.findUpper(10) == bst1.end());


    //Empty BST
    bst1.clear();

    minIt = bst1.getMin();
    assert(minIt == bst1.end());
    maxIt = bst1.getMax();
    assert(maxIt == bst1.end());
    assert(bst1.end()-1 == bst1.end());

    minIt = bst1.begin();
    assert(minIt == bst1.end());
    maxIt = bst1.end()-1;
    assert(maxIt == bst1.end());
    assert(bst1.end()-1 == bst1.end());
}

void testRandom() {

    //Setup random generator
    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type>
            distIn(0,RANDOM_MAX*2);

    std::vector<int> testNumbers;
    std::vector<int> randomNumbers;

    //Random test number generation
    for (int i = 0; i < INPUTSIZE; i++) {
        int randomValue = distIn(rng)-RANDOM_MAX;
        testNumbers.push_back(randomValue);
    }

    //Random number generation
    for (int i = 0; i < INPUTSIZE; i++) {
        int randomValue = distIn(rng)-RANDOM_MAX;
        randomNumbers.push_back(randomValue);
    }


    std::cout << " ------ RANDOM ------ " << std::endl << std::endl;

    doTestsOnInput(testNumbers, randomNumbers);
}


void testMixed() {
    //Setup random generator
    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type>
            distIn(0,RANDOM_MAX*2);

    std::vector<int> testNumbers;
    std::vector<int> randomNumbers;

    //Progressive number generation
    for (int i = 0; i < INPUTSIZE/4; i++) {
        testNumbers.push_back(i);
    }
    //Random test number generation
    for (int i = 0; i < INPUTSIZE/4; i++) {
        int randomValue = distIn(rng)-RANDOM_MAX;
        testNumbers.push_back(randomValue);
    }
    //Progressive reversed number generation
    for (int i = 0; i < INPUTSIZE/4; i++) {
        testNumbers.push_back(INPUTSIZE-i);
    }
    //Random test number generation
    for (int i = 0; i < INPUTSIZE/4; i++) {
        int randomValue = distIn(rng)-RANDOM_MAX;
        testNumbers.push_back(randomValue);
    }

    //Random number generation
    for (int i = 0; i < INPUTSIZE; i++) {
        int randomValue = distIn(rng)-RANDOM_MAX;
        randomNumbers.push_back(randomValue);
    }

    std::cout << std::endl << " ------ MIXED VALUES VECTOR ------ " << std::endl << std::endl;

    doTestsOnInput(testNumbers, randomNumbers);
}


void testProgressive() {
    //Setup random generator
    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type>
            distIn(0,RANDOM_MAX*2);

    std::vector<int> testNumbers;
    std::vector<int> randomNumbers;

    //Progressive reversed number generation
    for (int i = 0; i < INPUTSIZE; i++) {
        testNumbers.push_back(INPUTSIZE-i);
    }

    //Random number generation
    for (int i = 0; i < INPUTSIZE; i++) {
        int randomValue = distIn(rng)-RANDOM_MAX;
        randomNumbers.push_back(randomValue);
    }

    std::cout << std::endl << " ------ REVERSE SORTED VECTOR ------ " << std::endl << std::endl;

    doTestsOnInput(testNumbers, randomNumbers);
}




/* ----- FUNCTION IMPLEMENTATION ----- */



void printHeader() {
    std::cout <<
         std::setw(INDENTSPACE) << std::left << "STRUCTURE" <<
         std::setw(INDENTSPACE) << std::left << "CONSTR." <<
         std::setw(INDENTSPACE) << std::left << "(NUM)" <<
         std::setw(INDENTSPACE) << std::left << "(HEIGHT)" <<
         std::setw(INDENTSPACE) << std::left << "QUERY (C)" <<
         std::setw(INDENTSPACE) << std::left << "FOUND" <<
         std::setw(INDENTSPACE) << std::left << "ITERATION" <<
         std::setw(INDENTSPACE) << std::left << "CLEAR" <<
         std::setw(INDENTSPACE) << std::left << "INSERT" <<
         std::setw(INDENTSPACE) << std::left << "(NUM)" <<
         std::setw(INDENTSPACE) << std::left << "(HEIGHT)" <<
         std::setw(INDENTSPACE) << std::left << "QUERY (D)" <<
         std::setw(INDENTSPACE) << std::left << "FOUND" <<
         std::setw(INDENTSPACE) << std::left << "ITERATION" <<
         std::setw(INDENTSPACE) << std::left << "ERASE" <<
         std::setw(INDENTSPACE) << std::left << "(NUM)" <<
         std::setw(INDENTSPACE) << std::left << "(HEIGHT)" <<
         std::setw(INDENTSPACE) << std::left << "QUERY (E)" <<
         std::setw(INDENTSPACE) << std::left << "FOUND" <<
         std::setw(INDENTSPACE) << std::left << "ERASE (IT)" <<
         std::setw(INDENTSPACE) << std::left << "TOTAL" <<
         std::endl << std::endl;
}

void doTestsOnInput(std::vector<int> &testNumbers, std::vector<int> &randomNumbers)
{
    printHeader();

    for (int t = 0; t < ITERATION; t++) {
        std::cout << std::setw(INDENTSPACE) << std::left;
        std::cout << "STL SET";

        testSTL(testNumbers, randomNumbers);
    }
    if (ITERATION > 1) {
        std::cout << std::endl;
    }



    for (int t = 0; t < ITERATION; t++) {
        std::cout << std::setw(INDENTSPACE) << std::left;
        std::cout << "AVL (I)";
        testBST<AVLInner<int>>(testNumbers, randomNumbers);
    }
    if (ITERATION > 1) {
        std::cout << std::endl;
    }



    for (int t = 0; t < ITERATION; t++) {
        std::cout << std::setw(INDENTSPACE) << std::left;
        std::cout << "AVL (L)";
        testBST<AVLLeaf<int>>(testNumbers, randomNumbers);
    }
    if (ITERATION > 1) {
        std::cout << std::endl;
    }

    if (!ONLYEFFICIENT) {

        for (int t = 0; t < ITERATION; t++) {
            std::cout << std::setw(INDENTSPACE) << std::left;
            std::cout << "BST (I)";
            testBST<BSTInner<int>>(testNumbers, randomNumbers);
        }
        if (ITERATION > 1) {
            std::cout << std::endl;
        }

        for (int t = 0; t < ITERATION; t++) {
            std::cout << std::setw(INDENTSPACE) << std::left;
            std::cout << "BST (L)";
            testBST<BSTLeaf<int>>(testNumbers, randomNumbers);
        }
        if (ITERATION > 1) {
            std::cout << std::endl;
        }
    }

    testNumbers.clear();
    randomNumbers.clear();

    std::cout << std::endl;
}




void testSTL(std::vector<int>& testNumbers, std::vector<int>& randomNumbers) {

    typedef std::set<int>::iterator Iterator;


    cg3::Timer totalTimer("Total");
    cg3::Timer timer("Step");

    totalTimer.start();


    /* Construction */

    timer.start();

    std::set<int> set(testNumbers.begin(), testNumbers.end());

    timer.stop();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << timer.delay();




    /* Number of elements */

    size_t numOfEntriesConstruction = set.size();
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << numOfEntriesConstruction;
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << "?";




    /* Query (construction) */

    timer.start();

    size_t foundConstruction = 0;

    for (const int& number : testNumbers) {
        Iterator it = set.find(number);
        bool found = (it != set.end());

        if (found)
            foundConstruction++;

        assert(found);
    }

    for (const int& number : randomNumbers) {
        Iterator it = set.find(number);
        if (it != set.end()) {
            foundConstruction++;
            assert(*it == number);
        }
    }

    timer.stop();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << timer.delay();



    /* Number of results */

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << foundConstruction;


    /* Iteration */

    timer.start();

    {
        size_t numOfEntries = 0;
        int lastNumber = INT32_MIN;
        CG3_SUPPRESS_WARNING(lastNumber);
        for (const int& number : set) {
            assert(lastNumber < number);
            lastNumber = number;
            numOfEntries++;
        }

        assert(numOfEntries == numOfEntriesConstruction);
    }

    timer.stop();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << timer.delay();



    /* Clear */

    timer.start();

    set.clear();

    timer.stop();

    assert(set.size() == 0);

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << timer.delay();




    /* Insert */

    timer.start();

    for (const int& testNumber : testNumbers) {
        set.insert(testNumber);
    }

    timer.stop();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << timer.delay();




    /* Number of elements */

    size_t numOfEntriesInsert = set.size();
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << numOfEntriesInsert;
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << "?";




    /* Query */

    timer.start();

    size_t foundInsert = 0;

    for (const int& number : testNumbers) {
        Iterator it = set.find(number);
        bool found = (it != set.end());

        if (found)
            foundInsert++;

        assert(found);
    }

    for (const int& number : randomNumbers) {
        Iterator it = set.find(number);
        if (it != set.end()) {
            foundInsert++;
            assert(*it == number);
        }
    }


    timer.stop();


    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << timer.delay();



    /* Number of results */

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << foundInsert;



    /* Iteration */

    timer.start();
    {
        size_t numOfEntries = 0;
        int lastNumber = INT32_MIN;
        CG3_SUPPRESS_WARNING(lastNumber);
        for (const int& number : set) {
            assert(lastNumber < number);
            lastNumber = number;
            numOfEntries++;
        }

        assert(numOfEntries == numOfEntriesConstruction);
    }

    timer.stop();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << timer.delay();




    /* Erase */


    timer.start();

    //Deleting second half of the vector
    for (size_t i = testNumbers.size()/2; i < testNumbers.size(); i++) {
        const int& number = testNumbers.at(i);
        set.erase(number);
    }

    timer.stop();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << timer.delay();


    /* Erase check */
    for (size_t i = testNumbers.size()/2; i < testNumbers.size(); i++) {
        const int& number = testNumbers.at(i);
        CG3_SUPPRESS_WARNING(number);
        assert(set.find(number) == set.end());
    }


    /* Number of elements */

    size_t numberOfElementsErase = set.size();
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << numberOfElementsErase;
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << "?";



    /* Query (erase) */

    timer.start();

    size_t foundErase = 0;

    for (const int& number : randomNumbers) {
        Iterator it = set.find(number);
        if (it != set.end()) {
            assert(*it == number);
            foundErase++;
        }
    }

    timer.stop();


    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << timer.delay();



    /* Number of results */

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << foundErase;



    /* Erase & insert check */
    for (size_t i = 0, j = 0; i < testNumbers.size() && j < randomNumbers.size(); i++, j++) {
        const int& testNumber = testNumbers.at(i);
        const int& randomNumber = testNumbers.at(j);
        set.erase(testNumber);
        set.insert(randomNumber);
    }



    /* Iteration (erase) */


    timer.start();


    //Erasing all elements iterating on it
    size_t numberOfElementsBeforeEraseIteration = set.size();
    size_t numOfEntries = 0;
    for (Iterator it = set.begin(); it != set.end();) {
        set.erase(it++);
        numOfEntries++;
    }
    CG3_SUPPRESS_WARNING(numberOfElementsBeforeEraseIteration);
    assert(numOfEntries == numberOfElementsBeforeEraseIteration);

    timer.stop();


    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << timer.delay();


    assert(set.empty());

    assert(set.find(5) == set.end());


    /* Total */

    totalTimer.stop();


    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << totalTimer.delay();




    std::cout << std::endl;


}

template <class B>
void testBST(std::vector<int>& testNumbers, std::vector<int>& randomNumbers) {
    B tree;

    typedef typename B::iterator iterator;


    cg3::Timer totalTimer("Total");
    cg3::Timer timer("Step");

    totalTimer.start();


    /* Construction */

    timer.start();

    tree.construction(testNumbers);

    timer.stop();


    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << timer.delay();




    /* Number of elements */

    size_t numOfEntriesConstruction = tree.size();
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << numOfEntriesConstruction;
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << tree.getHeight();




    /* Query (construction) */

    timer.start();

    size_t foundConstruction = 0;

    for (const int& number : testNumbers) {
        iterator it = tree.find(number);
        bool found = (it != tree.end());

        if (found)
            foundConstruction++;

        assert(found);
    }

    for (const int& number : randomNumbers) {
        iterator it = tree.find(number);
        if (it != tree.end()) {
            foundConstruction++;
            assert(*it == number);
        }
    }

    timer.stop();


    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << timer.delay();


    /* Number of results */

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << foundConstruction;


    /* Iteration */

    timer.start();
    {
        size_t numOfEntries = 0;
        int lastNumber = INT32_MIN;
        CG3_SUPPRESS_WARNING(lastNumber);
        CG3_SUPPRESS_WARNING(lastNumber);
        for (const int& number : tree) {
            assert(lastNumber < number);
            lastNumber = number;
            numOfEntries++;
        }

        assert(numOfEntries == numOfEntriesConstruction);
    }


    timer.stop();


    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << timer.delay();



    /* Clear */

    timer.start();

    tree.clear();

    timer.stop();

    assert(tree.size() == 0);


    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << timer.delay();




    /* Insert */

    timer.start();

    for (const int& testNumber : testNumbers) {
        tree.insert(testNumber);
    }

    timer.stop();


    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << timer.delay();




    /* Number of elements */

    size_t numOfEntriesInsert = tree.size();
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << numOfEntriesInsert;
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << tree.getHeight();




    /* Query */

    timer.start();

    size_t foundInsert = 0;

    for (const int& number : testNumbers) {
        iterator it = tree.find(number);
        bool found = (it != tree.end());

        if (found)
            foundInsert++;

        assert(found);
    }

    for (const int& number : randomNumbers) {
        iterator it = tree.find(number);
        if (it != tree.end()) {
            foundInsert++;
            assert(*it == number);
        }
    }


    timer.stop();


    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << timer.delay();



    /* Number of results */

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << foundInsert;



    /* Iteration */

    timer.start();
    {
        size_t numOfEntries = 0;
        int lastNumber = INT32_MIN;
        CG3_SUPPRESS_WARNING(lastNumber);
        for (const int& number : tree) {
            assert(lastNumber < number);
            lastNumber = number;
            numOfEntries++;
        }

        assert(numOfEntries == numOfEntriesConstruction);
    }

    timer.stop();


    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << timer.delay();




    /* Erase */


    timer.start();

    //Deleting second half of the vector
    for (size_t i = testNumbers.size()/2; i < testNumbers.size(); i++) {
        const int& number = testNumbers.at(i);
        tree.erase(number);
    }

    timer.stop();


    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << timer.delay();


    /* Erase check */
    for (size_t i = testNumbers.size()/2; i < testNumbers.size(); i++) {
        const int& number = testNumbers.at(i);
        CG3_SUPPRESS_WARNING(number);
        assert(tree.find(number) == tree.end());
    }


    /* Number of elements */

    size_t numberOfElementsErase = tree.size();
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << numberOfElementsErase;
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << tree.getHeight();



    /* Query (erase) */

    timer.start();

    size_t foundErase = 0;

    for (const int& number : randomNumbers) {
        iterator it = tree.find(number);
        if (it != tree.end()) {
            assert(*it == number);
            foundErase++;
        }
    }

    timer.stop();


    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << timer.delay();



    /* Number of results */

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << foundErase;


    /* Erase & insert check */
    for (size_t i = 0, j = 0; i < testNumbers.size() && j < randomNumbers.size(); i++, j++) {
        const int& testNumber = testNumbers.at(i);
        const int& randomNumber = testNumbers.at(j);
        tree.erase(testNumber);
        tree.insert(randomNumber);
    }


    /* Iteration (erase) */


    timer.start();


    //Erasing all elements iterating on it
    size_t numberOfElementsBeforeEraseIteration = tree.size();
    size_t numOfEntries = 0;
    for (iterator it = tree.begin(); it != tree.end();) {
        tree.erase(it++);
        numOfEntries++;
    }
    CG3_SUPPRESS_WARNING(numberOfElementsBeforeEraseIteration);
    assert(numOfEntries == numberOfElementsBeforeEraseIteration);

    timer.stop();


    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << timer.delay();


    assert(tree.empty());

    assert(tree.find(5) == tree.end());


    /* Total */

    totalTimer.stop();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << totalTimer.delay();




    std::cout << std::endl;
}












}
