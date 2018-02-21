//
//  Test_Parser.h
//  HFSParser
//
//  Created by Dongyun Shin on 4/1/14.
//  Copyright (c) 2014 elsdy. All rights reserved.
//

#ifndef __HFSParser__Test_Parser__
#define __HFSParser__Test_Parser__

#include <iostream>
#include <string>
#include <assert.h> 

#include "Parser.h"
#include "SysIOBuildRule.h"

using namespace std;

// 2014_04_08_Tue_PM_10_57_28
void test_approximateTime();

// 2014_04_06_Sun_AM_04_39_50
void test_parseBlockIOProperty();

// 2014_04_03_Thu_AM_12_58_39
void test_extractNode();

//2014_04_02_Wed_AM_07_57_45
void test_buildIOInfoWithRule();

// 2014_04_01_Tue
void test_parseIOProperty();
void test_findIOEndFunctionName();

// before 2014_04_02_Wed_AM_07_57_52
void test_extractStringNextTo();
void test_input();

// 2014_05_28_Wed_PM_09_50_16
void test_parseEmmcIOProperty();

#endif /* defined(__HFSParser__Test_Parser__) */
