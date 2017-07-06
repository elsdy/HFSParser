//
//  List_Test.c
//  HFSParser
//
//  Created by Dongyun Shin on 3/20/14.
//  Copyright (c) 2014 elsdy. All rights reserved.
//

#include <stdio.h>

#include "Test_List.h"

// extractNode also tested in this case.
void test_List_removeFromTo()
{
	List <string> *test_list = new List <string> ();
	
	Node <string> *a = new Node<string> ("a");
	Node <string> *b = new Node<string> ("b");
	Node <string> *c = new Node<string> ("c");
	
	test_list->push_back(a);
	test_list->push_back(b);
	test_list->push_back(c);
	
	List <string > *test_list2 = new List <string> ();
	
	Node <string> *aa = new Node<string> ("aa");
	Node <string> *bb = new Node<string> ("bb");
	Node <string> *cc = new Node<string> ("cc");
	Node <string> *dd = new Node<string> ("dd");
	
	test_list2->push_back(aa);
	test_list2->push_back(bb);
	test_list2->push_back(cc);
	test_list2->push_back(dd);
	
	test_list->removeFromTo(a,b);
	assert( *(test_list->getHead()->entry) == "c");
	
	test_list2->removeFromTo(cc,dd);
	assert( *(test_list2->getHead()->entry) == "aa");
	assert( *(test_list2->getTail()->entry) == "bb");

}

void test_List_push_back()
{
	List <ParseEntry> list;
	
	Node <ParseEntry> *a = new Node <ParseEntry> ();
	list.push_back(a);
	
	Node <ParseEntry> *b = new Node <ParseEntry>;
	list.push_back(b);
	
	list.printList();
	
	assert(list.getSize() == 2);
}

void test_List()
{
	List <ParseEntry> *test_list = new List <ParseEntry> ();
	
	Node <ParseEntry> *a = new Node <ParseEntry> ();
	test_list->push_back(a);
	
	Node <ParseEntry> *b = new Node <ParseEntry> ();
	test_list->push_back(b);
	
	Node <ParseEntry> *c = new Node <ParseEntry> ();
	test_list->push_back(c);
	
	delete test_list;
}