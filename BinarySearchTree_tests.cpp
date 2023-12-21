#include "BinarySearchTree.hpp"
#include "unit_test_framework.hpp"
#include <iostream>
#include <cassert>  //assert
#include <functional> //less
#include <cstddef>
#include <utility>  //pair
#include <fstream>
#include <sstream>
#include <cassert>
#include <string>
#include <vector>
#include <map>
#include <regex>
#include <exception>
#include <set>


void testDefaultCtor() {
    BinarySearchTree<int> bst;
    assert(bst.empty());
}

void testEmptyAndSize() {
    BinarySearchTree<int> bst;
    assert(bst.empty());
    assert(bst.size() == 0);
}

void testCopyCtor() {
    BinarySearchTree<int> bst1;
    bst1.insert(10);
    BinarySearchTree<int> bst2(bst1);
    assert(!bst2.empty());
    assert(bst2.size() == 1);
}

void testFind() {
    BinarySearchTree<int> bst;
    bst.insert(5);
    auto it = bst.find(5);
    assert(*it == 5);
    assert(bst.find(10) == bst.end());
}

void testHeight1() {
    BinarySearchTree<int> bst;
    bst.insert(10);
    assert(bst.height() == 1);
}

void testHeight2() {
    BinarySearchTree<int> bst;
    bst.insert(10);
    bst.insert(5);
    assert(bst.height() == 2);
}

void testInorderAndPreorder() {
    BinarySearchTree<int> bst;
    bst.insert(10);
    bst.insert(5);
    bst.insert(15);

    std::stringstream ss_inorder;
    bst.traverse_inorder(ss_inorder);
    assert(ss_inorder.str() == "5 10 15 ");

    std::stringstream ss_preorder;
    bst.traverse_preorder(ss_preorder);
    assert(ss_preorder.str() == "10 5 15 ");
}

void testInsert1() {
    BinarySearchTree<int> bst;
    bst.insert(10);
    assert(!bst.empty());
    assert(bst.size() == 1);
}

void testInsert2() {
    BinarySearchTree<int> bst;
    bst.insert(10);
    bst.insert(5);
    bst.insert(15);
    assert(bst.size() == 3);
}

void testMaxElement() {
    BinarySearchTree<int> bst;
    bst.insert(10);
    bst.insert(20);
    bst.insert(5);
    auto it = bst.max_element();
    assert(*it == 20);
}

void testMinElement() {
    BinarySearchTree<int> bst;
    bst.insert(10);
    bst.insert(20);
    bst.insert(5);
    auto it = bst.min_element();
    assert(*it == 5);
}

void testMinGreaterThan1() {
    BinarySearchTree<int> bst;
    bst.insert(10);
    bst.insert(20);
    bst.insert(5);
    auto it = bst.min_greater_than(10);
    assert(*it == 20);
}

void testMinGreaterThan2() {
    BinarySearchTree<int> bst;
    bst.insert(10);
    bst.insert(20);
    bst.insert(5);
    auto it = bst.min_greater_than(15);
    assert(*it == 20);
}

void testMinGreaterThan3() {
    BinarySearchTree<int> bst;
    bst.insert(10);
    bst.insert(20);
    bst.insert(5);
    auto it = bst.min_greater_than(4);
    assert(*it == 5);
}

void testPreorder2() {
    BinarySearchTree<int> bst;
    bst.insert(10);
    bst.insert(20);
    bst.insert(5);

    std::stringstream ss;
    bst.traverse_preorder(ss);
    assert(ss.str() == "10 5 20 ");
}

void testCheckInvariants() {
    BinarySearchTree<int> bst;
    bst.insert(10);
    bst.insert(5);
    bst.insert(15);
    assert(bst.check_sorting_invariant());
}

void testFindEmptyTree() {
    BinarySearchTree<int> bst;
    assert(bst.find(10) == bst.end());
}


void testTreeBalance() {
    BinarySearchTree<int> bst;
    bst.insert(10);
    bst.insert(20);
    bst.insert(5);
    assert(bst.height() < 3);
}

void testInvariantAfterInsertions() {
    BinarySearchTree<int> bst;
    for(int i = 0; i < 10; i++) {
        bst.insert(i);
    }
    assert(bst.check_sorting_invariant());
}

void testIteratorIncrement() {
    BinarySearchTree<int> bst;
    bst.insert(5);
    bst.insert(10);
    auto it = bst.begin();
    ++it;
    assert(*it == 10);
}

void testMaxElementEmptyTree() {
    BinarySearchTree<int> bst;
    assert(bst.max_element() == bst.end());
}

void testMinElementEmptyTree() {
    BinarySearchTree<int> bst;
    assert(bst.min_element() == bst.end());
}

void testCopyConstructor() {
    BinarySearchTree<int> original;
    original.insert(10);
    original.insert(5);
    original.insert(15);

    BinarySearchTree<int> copy(original);

    ASSERT_EQUAL(original.size(), copy.size());

    std::stringstream ssOriginalIn, ssCopyIn;
    original.traverse_inorder(ssOriginalIn);
    copy.traverse_inorder(ssCopyIn);
    ASSERT_EQUAL(ssOriginalIn.str(), ssCopyIn.str());

    std::stringstream ssOriginalPre, ssCopyPre;
    original.traverse_preorder(ssOriginalPre);
    copy.traverse_preorder(ssCopyPre);
    ASSERT_EQUAL(ssOriginalPre.str(), ssCopyPre.str());

 
}

void testAssignmentOperator() {
    BinarySearchTree<int> original;
    original.insert(10);
    original.insert(20);

    BinarySearchTree<int> assigned;
    assigned.insert(5);

    assigned = original;

    std::stringstream original_ss;
    original.traverse_inorder(original_ss);
    std::stringstream assigned_ss;
    assigned.traverse_inorder(assigned_ss);

    assert(original_ss.str() == assigned_ss.str());
}

void testSelfAssignment() {
    BinarySearchTree<int> bst;
    bst.insert(10);
    bst.insert(20);

    BinarySearchTree<int> &ref_bst = bst;
    bst = ref_bst;

    std::stringstream ss;
    bst.traverse_inorder(ss);

    assert(ss.str() == "10 20 ");
}

void testDestructor() {
    {
        BinarySearchTree<int> bst;
        bst.insert(10);
        bst.insert(20);
        bst.insert(5);
    }

   
    assert(true); 
}

void testSkewedTree() {
    BinarySearchTree<int> bst;
    for(int i = 1; i <= 10; ++i) {
        bst.insert(i);
    }
    assert(bst.height() == 10); 
}

void testIteratorValidityAfterInsertion() {
    BinarySearchTree<int> bst;
    bst.insert(5);
    bst.insert(10);
    auto it = bst.find(5);
    bst.insert(15);
    assert(*it == 5); 
}

void testMinGreaterThanMaximum() {
    BinarySearchTree<int> bst;
    bst.insert(10);
    bst.insert(20);
    auto it = bst.min_greater_than(25);
    assert(it == bst.end());
}

void testMinGreaterThanInSingleElementTree() {
    BinarySearchTree<int> bst;
    bst.insert(10);
    auto it = bst.min_greater_than(5);
    assert(*it == 10); 
}

void testStress() {
    BinarySearchTree<int> bst;
    for(int i = 0; i < 10000; ++i) {
        bst.insert(i);
    }
    assert(bst.size() == 10000);
}

void testAgainstStdSet() {
    std::set<int> stdSet;
    BinarySearchTree<int> bst;

    for(int i = 0; i < 100; ++i) {
        stdSet.insert(i);
        bst.insert(i);
    }

    assert(stdSet.size() == bst.size());
    for(int i = 0; i < 100; ++i) {
        assert(stdSet.count(i) == (bst.find(i) != bst.end()));
    }
}

void testDestructorFunctionality() {
    {
        BinarySearchTree<int> bst;
        for(int i = 0; i < 1000; ++i) {
            bst.insert(i);
        }
    }
   
    assert(true); 
}



void testSingleElementTree() {
    BinarySearchTree<int> bst;
    bst.insert(10);
    ASSERT_EQUAL(bst.height(), 1);
    ASSERT_EQUAL(bst.size(), 1);
    ASSERT_EQUAL(*bst.min_element(), 10);
    ASSERT_EQUAL(*bst.max_element(), 10);
}

void testComplexTreeStructure() {
    BinarySearchTree<int> bst;
    int elements[] = {15, 10, 20, 5, 12, 17, 25};
    for (int elem : elements) {
        bst.insert(elem);
    }
    ASSERT_EQUAL(bst.height(), 3); 
    ASSERT_EQUAL(bst.size(), 7);
}




void testRandomOrderInsertion() {
    BinarySearchTree<int> bst;
    int elements[] = {20, 5, 15, 10, 25};
    for (int elem : elements) {
        bst.insert(elem);
    }
    std::stringstream ss;
    bst.traverse_inorder(ss);
    ASSERT_EQUAL(ss.str(), "5 10 15 20 25 ");
}

void testIteratorIncrementInComplexTree() {
    BinarySearchTree<int> bst;
    bst.insert(10);
    bst.insert(5);
    bst.insert(15);
    bst.insert(3);
    bst.insert(7);
    bst.insert(12);
    bst.insert(17);

    auto it = bst.begin();
    ASSERT_EQUAL(*it, 3); ++it;
    ASSERT_EQUAL(*it, 5); ++it;
    ASSERT_EQUAL(*it, 7); ++it;
    ASSERT_EQUAL(*it, 10); ++it;
    ASSERT_EQUAL(*it, 12); ++it;
    ASSERT_EQUAL(*it, 15); ++it;
    ASSERT_EQUAL(*it, 17); ++it;
    ASSERT_TRUE(it == bst.end());
}

void testWithStringType() {
    BinarySearchTree<std::string> bst;
    bst.insert("banana");
    bst.insert("apple");
    bst.insert("orange");

    ASSERT_EQUAL(bst.size(), 3);
    ASSERT_EQUAL(*bst.min_element(), "apple");
}

void testSortingInvariant() {
    BinarySearchTree<int> bst;
    bst.insert(1);
    bst.insert(0);
    
    *bst.begin() = 2;

    assert(!bst.check_sorting_invariant());
}









TEST(test_stub) {
    testDefaultCtor();
    testEmptyAndSize();
    testCopyCtor();
    testFind();
    testHeight1();
    testHeight2();
    testInorderAndPreorder();
    testInsert1();
    testInsert2();
    testMaxElement();
    testMinElement();
    testMinGreaterThan1();
    testMinGreaterThan2();
    testMinGreaterThan3();
    testPreorder2();
    testCheckInvariants();
    testFindEmptyTree();
    testTreeBalance();
    testInvariantAfterInsertions();
    testIteratorIncrement();
    testMaxElementEmptyTree();
    testMinElementEmptyTree();
    testSelfAssignment();
    testCopyConstructor();
    testAssignmentOperator();
    testDestructor();
    testSkewedTree();
    testIteratorValidityAfterInsertion();
    testMinGreaterThanMaximum();
    testMinGreaterThanMaximum();
    testStress();
    testAgainstStdSet();
    testDestructorFunctionality();
    testSingleElementTree();
    testRandomOrderInsertion();
    testComplexTreeStructure();
    testIteratorIncrementInComplexTree();
    testWithStringType();
    testSortingInvariant();



     ASSERT_TRUE(true);
}

TEST_MAIN()