//
//  Interrupt.h
//  HFSParser
//
//  Created by Dongyun Shin on 3/31/14.
//  Copyright (c) 2014 elsdy. All rights reserved.
//

#ifndef __HFSParser__Interrupt__
#define __HFSParser__Interrupt__

#include <iostream>

#include "Node.h"
#include "StartFinishTimeInfo.h"

class InterruptInfo: public StartFinishTimeInfo
{
private:
	int IrqNum;
	string Name;
	string HandlerName;
	
	void init()
	{
		IrqNum = -1;
		Name = "";
		HandlerName = "";
	}
	
public:
	InterruptInfo(): StartFinishTimeInfo()
	{
		init();
	}
	InterruptInfo(Node <ParseEntry> * _a, Node <ParseEntry> * _b): StartFinishTimeInfo( _a, _b)
	{
		init();
	}

	~InterruptInfo()
	{
		
	}
	
	
	int getIrqNum();
	void setIrqNum(int _value);
	string getName();
	void setName(string _value);
	string getHandlerName();
	void setHandlerName(string _value);
};

class SoftInterruptInfo: public StartFinishTimeInfo
{
private:
	int Vec;
	string Action;
	
	void init()
	{
		Vec = 0;
		Action = "";
	}
	
public:
	
	SoftInterruptInfo(): StartFinishTimeInfo()
	{
		init();
	}
	SoftInterruptInfo(Node <ParseEntry> * _a, Node <ParseEntry> * _b): StartFinishTimeInfo (_a, _b)
	{
		init();
		
		StartFinishTimeInfo::setStart(_a->entry);
		StartFinishTimeInfo::setFinish(_b->entry);
	}
	~SoftInterruptInfo()
	{
		
	}
	
	int getVec();
	void setVec(int _value);
	string getAction();
	void setAction(string _value);
};

#endif /* defined(__HFSParser__Interrupt__) */
