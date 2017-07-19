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
	#pragma mark - constructor & destructor
public:
	InterruptInfo();
	InterruptInfo(Node <ParseEntry> * _a, Node <ParseEntry> * _b);
	~InterruptInfo();
	void init();


private:
	int IrqNum;
	string Name;
	string HandlerName;


	#pragma mark - getter & setter
public:
	int getIrqNum();
	void setIrqNum(int _value);
	string getName();
	void setName(string _value);
	string getHandlerName();
	void setHandlerName(string _value);
};

class SoftInterruptInfo: public StartFinishTimeInfo
{
public:
	SoftInterruptInfo();
	SoftInterruptInfo(Node <ParseEntry> * _a, Node <ParseEntry> * _b);
	~SoftInterruptInfo();


private:
	void init();


private:
	int Vec;
	string Action;
	

public:
	int getVec();
	void setVec(int _value);
	string getAction();
	void setAction(string _value);
};

#endif /* defined(__HFSParser__Interrupt__) */
