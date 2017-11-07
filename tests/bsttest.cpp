/**
    @author Stefano Nuvoli
*/
#include "bsttest.h"

#include <iostream>
#include <random>
#include <chrono>
#include <assert.h>
#include <iomanip>

#include <set>
#include <vector>

#include "cg3/data_structures/trees/bst/bstinner.h"
#include "cg3/data_structures/trees/bst/bstleaf.h"
#include "cg3/data_structures/trees/bst/avlinner.h"
#include "cg3/data_structures/trees/bst/avlleaf.h"

#define ITERATION 1
#define INDENTSPACE 12

#define INPUTSIZE 10000
#define RANDOM_MAX (INPUTSIZE*10)
#define ONLYAUTOBALANCING (INPUTSIZE > 200000)





namespace BinarySearchTreeTests {


/* ----- TYPEDEFS ----- */

typedef std::chrono::high_resolution_clock high_resolution_clock;
typedef high_resolution_clock::time_point time_point;


template <class T> using BSTInner = typename cg3::BSTInner<T>;
template <class T> using BSTLeaf = typename cg3::BSTLeaf<T>;
template <class T> using AVLInner = typename cg3::AVLInner<T>;
template <class T> using AVLLeaf = typename cg3::AVLLeaf<T>;



/* ----- FUNCTION DECLARATION ----- */



bool intComparator(const int& o1, const int& o2);

void printHeader();


void test(std::vector<int>& testNumbers, std::vector<int>& randomNumbers);


void testSTL(std::vector<int>& testNumbers, std::vector<int>& randomNumbers);

template <class B>
void testBST(std::vector<int>& testNumbers, std::vector<int>& randomNumbers);


/* ----- FUNCTION IMPLEMENTATION ----- */


bool intComparator(const int& o1, const int& o2) {
    return o1 < o2;
}

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



void testRandom() {

    //Setup random generator
    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type>
            distIn(-RANDOM_MAX,RANDOM_MAX);

    std::vector<int> testNumbers;
    std::vector<int> randomNumbers;

    //Random test number generation
    for (int i = 0; i < INPUTSIZE; i++) {
        int randomValue = distIn(rng);
        testNumbers.push_back(randomValue);
    }

    //Random number generation
    for (int i = 0; i < INPUTSIZE; i++) {
        int randomValue = distIn(rng);
        randomNumbers.push_back(randomValue);
    }


    std::cout << " ------ RANDOM ------ " << std::endl << std::endl;

    test(testNumbers, randomNumbers);
}


void testMixed() {
    //Setup random generator
    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type>
            distIn(-RANDOM_MAX,RANDOM_MAX);

    std::vector<int> testNumbers;
    std::vector<int> randomNumbers;

    //Progressive number generation
    for (int i = 0; i < INPUTSIZE/4; i++) {
        testNumbers.push_back(i);
    }
    //Random test number generation
    for (int i = 0; i < INPUTSIZE/4; i++) {
        int randomValue = distIn(rng);
        testNumbers.push_back(randomValue);
    }
    //Progressive reversed number generation
    for (int i = 0; i < INPUTSIZE/4; i++) {
        testNumbers.push_back(INPUTSIZE-i);
    }
    //Random test number generation
    for (int i = 0; i < INPUTSIZE/4; i++) {
        int randomValue = distIn(rng);
        testNumbers.push_back(randomValue);
    }

    //Random number generation
    for (int i = 0; i < INPUTSIZE; i++) {
        int randomValue = distIn(rng);
        randomNumbers.push_back(randomValue);
    }

    std::cout << std::endl << " ------ MIXED VALUES VECTOR ------ " << std::endl << std::endl;

    test(testNumbers, randomNumbers);
}


void testProgressive() {
    //Setup random generator
    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type>
            distIn(-RANDOM_MAX,RANDOM_MAX);

    std::vector<int> testNumbers;
    std::vector<int> randomNumbers;

    //Progressive reversed number generation
    for (int i = 0; i < INPUTSIZE; i++) {
        testNumbers.push_back(INPUTSIZE-i);
    }

    //Random number generation
    for (int i = 0; i < INPUTSIZE; i++) {
        int randomValue = distIn(rng);
        randomNumbers.push_back(randomValue);
    }

    std::cout << std::endl << " ------ REVERSE SORTED VECTOR ------ " << std::endl << std::endl;

    test(testNumbers, randomNumbers);
}




void test(std::vector<int> &testNumbers, std::vector<int> &randomNumbers)
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

    if (!ONLYAUTOBALANCING) {

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


    time_point tstart = high_resolution_clock::now();

    time_point t1;
    time_point t2;


    /* Construction */

    t1 = high_resolution_clock::now();

    std::set<int> set(testNumbers.begin(), testNumbers.end());

    t2 = high_resolution_clock::now();


    auto constructionTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) constructionTime/1000;




    /* Number of elements */

    size_t numOfEntriesConstruction = set.size();
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << numOfEntriesConstruction;
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << "?";




    /* Query (construction) */

    t1 = high_resolution_clock::now();

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

    t2 = high_resolution_clock::now();

    auto constructionQueryTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) constructionQueryTime/1000;


    /* Number of results */

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << foundConstruction;


    /* Iteration */

    t1 = high_resolution_clock::now();
    {
        size_t numOfEntries = 0;
        int lastNumber = INT32_MIN;
        for (const int& number : set) {
            assert(lastNumber < number);
            lastNumber = number;
            numOfEntries++;
        }

        assert(numOfEntries == numOfEntriesConstruction);
    }


    t2 = high_resolution_clock::now();

    auto constructionIterationTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) constructionIterationTime/1000;



    /* Clear */

    t1 = high_resolution_clock::now();

    set.clear();

    t2 = high_resolution_clock::now();

    assert(set.size() == 0);

    auto constructionClearTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) constructionClearTime/1000;




    /* Insert */

    t1 = high_resolution_clock::now();

    for (const int& testNumber : testNumbers) {
        set.insert(testNumber);
    }

    t2 = high_resolution_clock::now();

    auto insertTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) insertTime/1000;




    /* Number of elements */

    size_t numOfEntriesInsert = set.size();
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << numOfEntriesInsert;
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << "?";




    /* Query */

    t1 = high_resolution_clock::now();

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


    t2 = high_resolution_clock::now();

    auto queryTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) queryTime/1000;



    /* Number of results */

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << foundInsert;



    /* Iteration */

    t1 = high_resolution_clock::now();
    {
        size_t numOfEntries = 0;
        int lastNumber = INT32_MIN;
        for (const int& number : set) {
            assert(lastNumber < number);
            lastNumber = number;
            numOfEntries++;
        }

        assert(numOfEntries == numOfEntriesConstruction);
    }

    t2 = high_resolution_clock::now();

    auto iterationTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) iterationTime/1000;




    /* Erase */


    t1 = high_resolution_clock::now();

    //Deleting second half of the vector
    for (size_t i = testNumbers.size()/2; i < testNumbers.size(); i++) {
        const int& number = testNumbers.at(i);
        set.erase(number);
    }

    t2 = high_resolution_clock::now();

    auto eraseTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) eraseTime/1000;


    /* Erase check */
    for (size_t i = testNumbers.size()/2; i < testNumbers.size(); i++) {
        const int& number = testNumbers.at(i);
        assert(set.find(number) == set.end());
    }


    /* Number of elements */

    size_t numberOfElementsErase = set.size();
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << numberOfElementsErase;
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << "?";



    /* Query (erase) */

    t1 = high_resolution_clock::now();

    size_t foundErase = 0;

    for (const int& number : randomNumbers) {
        Iterator it = set.find(number);
        if (it != set.end()) {
            assert(*it == number);
            foundErase++;
        }
    }

    t2 = high_resolution_clock::now();

    auto eraseQueryTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) eraseQueryTime/1000;



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


    t1 = high_resolution_clock::now();


    //Erasing all elements iterating on it
    size_t numberOfElementsBeforeEraseIteration = set.size();
    size_t numOfEntries = 0;
    for (Iterator it = set.begin(); it != set.end();) {
        set.erase(it++);
        numOfEntries++;
    }
    assert(numOfEntries == numberOfElementsBeforeEraseIteration);

    t2 = high_resolution_clock::now();

    auto eraseIterationTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) eraseIterationTime/1000;


    assert(set.empty());

    assert(set.find(5) == set.end());


    /* Total */

    time_point tend = high_resolution_clock::now();

    auto totalTime = std::chrono::duration_cast<std::chrono::microseconds>(tend - tstart).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) totalTime/1000;




    std::cout << std::endl;


}

template <class B>
void testBST(std::vector<int>& testNumbers, std::vector<int>& randomNumbers) {
    B tree(&intComparator);

    typedef typename B::Iterator Iterator;



    time_point tstart = high_resolution_clock::now();

    time_point t1;
    time_point t2;


    /* Construction */

    t1 = high_resolution_clock::now();

    tree.construction(testNumbers);

    t2 = high_resolution_clock::now();


    auto constructionTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) constructionTime/1000;




    /* Number of elements */

    size_t numOfEntriesConstruction = tree.size();
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << numOfEntriesConstruction;
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << tree.getHeight();




    /* Query (construction) */

    t1 = high_resolution_clock::now();

    size_t foundConstruction = 0;

    for (const int& number : testNumbers) {
        Iterator it = tree.find(number);
        bool found = (it != tree.end());

        if (found)
            foundConstruction++;

        assert(found);
    }

    for (const int& number : randomNumbers) {
        Iterator it = tree.find(number);
        if (it != tree.end()) {
            foundConstruction++;
            assert(*it == number);
        }
    }

    t2 = high_resolution_clock::now();

    auto constructionQueryTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) constructionQueryTime/1000;


    /* Number of results */

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << foundConstruction;


    /* Iteration */

    t1 = high_resolution_clock::now();
    {
        size_t numOfEntries = 0;
        int lastNumber = INT32_MIN;
        for (const int& number : tree) {
            assert(lastNumber < number);
            lastNumber = number;
            numOfEntries++;
        }

        assert(numOfEntries == numOfEntriesConstruction);
    }


    t2 = high_resolution_clock::now();

    auto constructionIterationTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) constructionIterationTime/1000;



    /* Clear */

    t1 = high_resolution_clock::now();

    tree.clear();

    t2 = high_resolution_clock::now();

    assert(tree.size() == 0);

    auto constructionClearTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) constructionClearTime/1000;




    /* Insert */

    t1 = high_resolution_clock::now();

    for (const int& testNumber : testNumbers) {
        tree.insert(testNumber);
    }

    t2 = high_resolution_clock::now();

    auto insertTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) insertTime/1000;




    /* Number of elements */

    size_t numOfEntriesInsert = tree.size();
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << numOfEntriesInsert;
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << tree.getHeight();




    /* Query */

    t1 = high_resolution_clock::now();

    size_t foundInsert = 0;

    for (const int& number : testNumbers) {
        Iterator it = tree.find(number);
        bool found = (it != tree.end());

        if (found)
            foundInsert++;

        assert(found);
    }

    for (const int& number : randomNumbers) {
        Iterator it = tree.find(number);
        if (it != tree.end()) {
            foundInsert++;
            assert(*it == number);
        }
    }


    t2 = high_resolution_clock::now();

    auto queryTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) queryTime/1000;



    /* Number of results */

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << foundInsert;



    /* Iteration */

    t1 = high_resolution_clock::now();
    {
        size_t numOfEntries = 0;
        int lastNumber = INT32_MIN;
        for (const int& number : tree) {
            assert(lastNumber < number);
            lastNumber = number;
            numOfEntries++;
        }

        assert(numOfEntries == numOfEntriesConstruction);
    }

    t2 = high_resolution_clock::now();

    auto iterationTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) iterationTime/1000;




    /* Erase */


    t1 = high_resolution_clock::now();

    //Deleting second half of the vector
    for (size_t i = testNumbers.size()/2; i < testNumbers.size(); i++) {
        const int& number = testNumbers.at(i);
        tree.erase(number);
    }

    t2 = high_resolution_clock::now();

    auto eraseTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) eraseTime/1000;


    /* Erase check */
    for (size_t i = testNumbers.size()/2; i < testNumbers.size(); i++) {
        const int& number = testNumbers.at(i);
        assert(tree.find(number) == tree.end());
    }


    /* Number of elements */

    size_t numberOfElementsErase = tree.size();
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << numberOfElementsErase;
    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << tree.getHeight();



    /* Query (erase) */

    t1 = high_resolution_clock::now();

    size_t foundErase = 0;

    for (const int& number : randomNumbers) {
        Iterator it = tree.find(number);
        if (it != tree.end()) {
            assert(*it == number);
            foundErase++;
        }
    }

    t2 = high_resolution_clock::now();

    auto eraseQueryTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) eraseQueryTime/1000;



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


    t1 = high_resolution_clock::now();


    //Erasing all elements iterating on it
    size_t numberOfElementsBeforeEraseIteration = tree.size();
    size_t numOfEntries = 0;
    for (Iterator it = tree.begin(); it != tree.end();) {
        tree.erase(it++);
        numOfEntries++;
    }
    assert(numOfEntries == numberOfElementsBeforeEraseIteration);

    t2 = high_resolution_clock::now();

    auto eraseIterationTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) eraseIterationTime/1000;


    assert(tree.empty());

    assert(tree.find(5) == tree.end());


    /* Total */

    time_point tend = high_resolution_clock::now();

    auto totalTime = std::chrono::duration_cast<std::chrono::microseconds>(tend - tstart).count();

    std::cout << std::setw(INDENTSPACE) << std::left;
    std::cout << (double) totalTime/1000;




    std::cout << std::endl;
}












}
