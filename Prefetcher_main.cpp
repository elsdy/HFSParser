//
//  Prefetcher_main.cpp
//  HFSParser_Xcode
//
//  Created by Dongyun Shin on 5/4/16.
//  Copyright © 2016 Dongyun Shin. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <sstream>

#include <assert.h>

// data structure
#include "List.h"
#include "Node.h"

// basic class for parsing
#include "ParseEntry.h"
#include "Parser.h"

// info for process
#include "ProcessInfo.h"

// info for IO in each level
#include "IOInfo.h"
#include "BlockLevel.h"
#include "eMMCLevel.h"
#include "ParserForFork.h"

using namespace std;

#include "Interrupt.h"
#include "Scheduler.h"

#include "Test_case.h"
#include "Test_Parser.h"
#include "Test_STL_string.h"
#include "Test_List.h"
#include "Test_ParseEntry.h"

#include "string.h"

#include "ResultForFullAnalysis.h"
#include "ResultForMobile.h"
#include "ResultForProcessInfo.h"

#include <stdio.h>

void do_analysis_in_interface_mode(string _input_file)
{
	Parser parser(_input_file);

	parser.parseLog();

	parser.summary();

	//	analyis is prepared well and now result will be generated
	parser.userInterface();
}

// 1. generate a file named "fork.log" which is extracted from the vfs.log" with a line "sched_process_fork"
// - the fork.log file used for tracking parent and child relationship.
// - the ralationship is used for supporting
//   (1) parent only prefetching mode
//   (2) parent and all children prefetching mode

#ifdef BUILD_PREFETCHER
int main(int argc, const char * argv[])
{
	//	buildForkTree();
	//	printForkTree();
	system("grep sched_process_fork input/vfs.log > input/fork.log");
	do_analysis_in_interface_mode(argv[1]);
	system("rm input/fork.log");
	//	testFileNamePath();

	return 0;
}
#endif
