//
//  SysIOBuildRule.h
//  HFSParser
//
//  Created by Dongyun Shin on 3/22/14.
//  Copyright (c) 2014 elsdy. All rights reserved.
//

#ifndef __HFSParser__SysIOBuildRule__
#define __HFSParser__SysIOBuildRule__

#include <iostream>

#include <string>
#include <sstream>
#include <stdio.h>

#include "List.h"

using namespace std;

class Rule_SysIO
{
private:
	int Index;
	
	int priority;
	
	List <string> *Rule;
public:
	Rule_SysIO()
	{
		Index = 0;
		priority = 0;
		Rule = new List <string> ();
	}
	Rule_SysIO(int _index, int _priority, List <string> *_rule)
	{
		Index = _index;
		priority = _priority;
		Rule = _rule;
	}
	Rule_SysIO(List <string> *_rule)
	{
		Rule = _rule;
	}
	~Rule_SysIO()
	{
		Index = -1;
		priority = -1;
		delete Rule;
	}
	
	List <string> * getRule();
	
	void setRule( List <string> * _rule);
		
	int getIndex();
	
	void setIndex(int _index);
	
	int getPriority();
	
	void setPriority(int _priority);
	
	string  getKey();

	void print()
	{
		Node <string> *iter = Rule->getHead();
		while(iter)
		{
			printf("%s / ", iter->entry->c_str());
//			cout << iter->entry << " / ";
			iter = iter->next;
		}
		cout << endl;
	}
};

#endif /* defined(__HFSParser__SysIOBuildRule__) */
