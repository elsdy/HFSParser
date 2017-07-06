//
//  SysIOBuildRule.cpp
//  HFSParser
//
//  Created by Dongyun Shin on 3/22/14.
//  Copyright (c) 2014 elsdy. All rights reserved.
//

#include "SysIOBuildRule.h"

List <string> * Rule_SysIO::getRule()
{
	return Rule;
}
void Rule_SysIO::setRule( List <string> * _rule)
{
	Rule = _rule;
}

int Rule_SysIO::getIndex()
{
	return Index;
}

void Rule_SysIO::setIndex(int _index)
{
	Index = _index;
}

int Rule_SysIO::getPriority()
{
	return priority;
}
void Rule_SysIO::setPriority(int _priority)
{
	priority = _priority;
}

string  Rule_SysIO::getKey()
{
	return *Rule->getHead()->entry;
}
