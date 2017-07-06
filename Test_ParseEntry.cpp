//
//  TDD_ParseEntry.cpp
//  HFSParser
//
//  Created by Dongyun Shin on 6/3/14.
//  Copyright (c) 2014 elsdy. All rights reserved.
//

#include "Test_ParseEntry.h"


/**
 @brief
 test BitSet Class
 test ParseEntry class with its member variables isCollectedInWhichList
 */
void test_isCollectedInWhichList()
{
	ParseEntry test;
	
	test.getisCollectedInWhichList().setValue(SYS_IO);
	assert(test.getisCollectedInWhichList().compareValue(SYS_IO));
	
	test.getisCollectedInWhichList().setValue(SYS_IO|BLOCK_IO);
	assert(test.getisCollectedInWhichList().compareValue(SYS_IO));
	assert(test.getisCollectedInWhichList().compareValue(BLOCK_IO));
	
	test.getisCollectedInWhichList().clearValue(SYS_IO);
	assert(test.getisCollectedInWhichList().compareValue(BLOCK_IO));
	
	
	test.getisCollectedInWhichList().clearValue(BLOCK_IO);
	test.getisCollectedInWhichList().setValue(SYS_IO);
	assert(test.getisCollectedInWhichList().compareValue(SYS_IO));
	assert(test.getisCollectedInWhichList().compareValue(BLOCK_IO) == 0);
}
