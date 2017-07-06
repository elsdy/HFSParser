  //
//  Test_Parser.cpp
//  HFSParser
//
//  Created by Dongyun Shin on 4/1/14.
//  Copyright (c) 2014 elsdy. All rights reserved.
//

#include "Test_Parser.h"

void test_approximateTime()
{
	Parser parser;
	
	parser.input("test_input/test_approximate.input");
	
	long modified_entry = 0;
	parser.approximateTime_inParseList(&modified_entry);

}

void test_parseBlockIOProperty()
{
	Parser parser;
	
	Node <ParseEntry> *test = new Node<ParseEntry> ("	jbd2/mmcblk0p23-572   [002] ...1    65.318175: block_bio_remap: 179,0 WS 3198552 + 8 <- (179,23) 20056");
	
	unsigned long long start_addr = 0;
	unsigned long long size = 0;
	string R_W = "";
	
	parser.parseBlockIOProperty(test->entry, &start_addr, &size, &R_W);
	
	assert(start_addr == 3198552);
	assert(size == 8);
	assert(R_W == BLOCK_WRITE_STRING_LABEL);
	
//	cout << start_addr << endl <<size << endl << R_W << endl;

	Node <ParseEntry> *test2 = new Node<ParseEntry> ("         mmcqd/0-123   [000] d..2    65.024721: block_rq_issue: 179,0 RM 0 () 7380712 + 8 [mmcqd/0]");
	
	parser.parseBlockIOProperty(test2->entry, &start_addr, &size, &R_W);
	
	assert(start_addr == 7380712);
	assert(size == 8);
	assert(R_W == BLOCK_READ_STRING_LABEL);

//	cout << start_addr << endl <<size << endl << R_W << endl;
}

void test_extractNode()
{
	Parser parser;
	
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
	
	Node <string> * new_pos = NULL;
	
	test_list->extractNode(a, &new_pos);
	assert( *(a->entry) == "a");
	assert( *(new_pos->entry) == "b");
	assert( *(test_list->getHead()->entry) == "b");
	
	test_list2->push_back(a);
	assert( *(test_list2->getTail()->entry) == "a");

	test_list2->extractNode(cc, &new_pos);
	assert( *(cc->entry) == "cc");
	assert( *(new_pos->entry) == "dd");
	test_list->push_back(cc);
	assert( *(test_list->getTail()->entry) == "cc");

}

void test_buildIOInfoWithRule()
{
	Parser parser;
	
	parser.input("test_input/odin_iozone1.input");
	
	long nr_modified_entry = 0;
	parser.approximateTime_inParseList(&nr_modified_entry);
	
	List <ProcessInfo> *process_list = new List <ProcessInfo>();
	parser.groupProcessParseList( process_list );
	parser.fillProcessName( process_list );
		
	parser.loadIORule("configuration/IO_build_rule.input");
	
	// being tested
	Node <ProcessInfo> * head_process =  process_list->getHead();

	parser.buildIOInfoWithRule(  head_process  );
	
	// assert
	assert ( head_process->entry->getFileIOList()->getTail()->entry->getFd() == 5);
	assert ( head_process->entry->getFileIOList()->getTail()->entry->getSize() == 262224);
	assert ( head_process->entry->getFileIOList()->getSize() == 389);
}

void test_extractStringNextTo()
{
	Parser parser;
	string temp = "           <...>-619   [001] d..1    65.010713: sys_writev_pre: fd=3 size=2";
	
	string search_str = "fd=";
	string ret;
	
	parser.extractStringNextTo(temp, search_str, &ret);
	int number = atoi(ret.c_str());
	assert(number == 3);
	
	string search_str2 = "size=";
	parser.extractStringNextTo(temp, search_str2, &ret);
	number = atoi(ret.c_str());
	assert(number == 2);
}

void test_input()
{
	Parser parser;
	
	parser.input("test_input/test1.input");
	assert(parser.getParseList()->getSize() == 9);
}


void test_parseIOProperty()
{
	Parser parser;
	
	Node <ParseEntry> *test  = new Node <ParseEntry> ("           <...>-2474  [000] d..1    65.010743: sys_read_pre: fd=24 size=512");
	
	int fd = 0;
	long size = 0;
	long pos = 0;
	int pid = 0;
	int cpu_id = 0;
	parser.parseIOProperty(test, &fd, &size, &pos, &pid, &cpu_id);
	
	// test
	assert(fd == 24);
	assert(size == 512);
	assert(pid == 2474);
	assert(cpu_id == 0);
	
	delete test;
}

void test_findIOEndFunctionName()
{
	int status = 0;
	
	Parser parser;
	
	
	Rule_SysIO * rule_entry = new Rule_SysIO();
	
	List <string> *rule_value = NULL;
	
	parser.generateRuleEntry("sys_read_pre  sys_read_post  1", &rule_value);
	rule_entry->setRule(rule_value);
	
	string cmp_funcname = "sys_writev_post";
	status = parser.findIOEndFunctionName(cmp_funcname, rule_entry);
	assert(  status < 0 );
	
	string cmp_funcname2 = "sys_read_post";
	status = parser.findIOEndFunctionName(cmp_funcname2, rule_entry);
	assert(  status == 1 );
	
	delete rule_entry;
}

void test_parseEmmcIOProperty()
{
	Parser parser;
	
	Node <ParseEntry> *emmc_A  = new Node <ParseEntry> ("         mmcqd/0-123   [002] ....    65.264825: mmc_start_request: mmc0: starting CMD18 arg 6226600 cmd->flags 000000b5 mmc0 blksz 512 blocks 8 data->flags 00000200 tsac 10 ms nsac 0");
	
	Node <ParseEntry> *emmc_B  = new Node <ParseEntry> ("           <...>-1698  [000] ..s2    65.265253: mmc_request_done: mmc0: req done (CMD18) arg 6226600 cmd->flags 000000b5 mmc0 blksz 512 blocks 8 data->flags 00000200 tsac 10 ms nsac 0 xfered=4096");
	
	assert(parser.isSameEmmcIO(emmc_A, emmc_B));
}
