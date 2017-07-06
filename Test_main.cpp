//
//  Test_main.cpp
//  HFSParser_Xcode
//
//  Created by Dongyun Shin on 5/5/16.
//  Copyright © 2016 Dongyun Shin. All rights reserved.
//

#include <stdio.h>

#include "Test_Parser.h"
#include "Test_case.h"
#include "Test_Parser.h"
#include "Test_STL_string.h"
#include "Test_List.h"
#include "Test_ParseEntry.h"


void TDD_test_functions()
{
	test_extractStringNextTo();
	test_substr();
	test_tokenize();
	test_List();
	//	test_input();
	//	test_parseIOProperty();
	//	test_findIOEndFunctionName();
	//	test_buildIOInfoWithRule();
	//	test_extractNode();
	test_find_last_not_of();
	test_find_last_not_of2();
	//	test_parseBlockIOProperty();
	test_List_removeFromTo();
	//	test_approximateTime();
	//	test_parseEmmcIOProperty();
	test_isCollectedInWhichList();
	test_macro();

	string test = "test1";

	cout << test << endl;
}
void TDD_test_cases()
{
	// 잠시 테스트를 미룸, packed command가 없는 테스트 케이스
	//	test_nexus_iozone1();
	//	test_odin_iozone1();

	// packed command가 있는 테스트 케이스
	//	test_packed_command();
	
}

#ifdef BUILD_TDD
int main(int argc, const char * argv[]) {

	TDD_test_functions();
	TDD_test_cases();

	return 0;
}
#endif
