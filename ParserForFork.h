//
//  ParserForFork.hpp
//  HFSParser_Xcode
//
//  Created by Dongyun Shin on 2/1/16.
//  Copyright © 2016 Dongyun Shin. All rights reserved.
//

#ifndef ParserForFork_hpp
#define ParserForFork_hpp

#include <stdio.h>
#include <iostream>
#include <string>
#include <map>
#include <sstream>
#include <fstream>
#include <iterator>
#include <regex>
#include <assert.h>
#include "Parser.h"
#include "Tokenizer.h"

using namespace std;

extern multimap <int, int> pidtree;

void buildForkTree();
void printForkTree();
void testFileNamePath();

#endif /* ParserForFork_hpp */
