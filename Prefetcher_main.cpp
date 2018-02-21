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

void extractSchedInfo(char *log_name)
{
    char buf[100];
    snprintf(buf, 100, "grep sched_process_fork %s > fork.log",log_name);
	system(buf);
}

void CleanUp()
{
	system("rm fork.log");
    system("rm fault.log");
}

void extractFaultInfo(char *log_name)
{
    char buf[100];
    snprintf(buf, 100, "grep -v \"vfs\" -v %s | grep -v \"sched_process_fork\" > fault.log", log_name);
    system(buf);
}

// 1. generate a file named "fork.log" which is extracted from the vfs.log" with a line "sched_process_fork"
// - the fork.log file used for tracking parent and child relationship.
// - the ralationship is used for supporting
//   (1) parent only prefetching mode
//   (2) parent and all children prefetching mode

int main(int argc, const char * argv[])
{
    // char filename[20];
    // cout << "input file name: ";
    // scanf("%s", filename);
    
        genParentChildTree();
        printForkTree();

    char filename[20]="trace8";

	extractSchedInfo(filename);
    extractFaultInfo(filename);
    cout << "Analyzing " << filename << " is done." << endl;
    
    //    analyis is prepared well and now result will be generated
    Parser parser(filename);
    parser.genPrefetchingCode();

     CleanUp();

        testFileNamePath();

	return 0;
}
