//
//  Interrupt.cpp
//  HFSParser
//
//  Created by Dongyun Shin on 3/31/14.
//  Copyright (c) 2014 elsdy. All rights reserved.
//

#include "Interrupt.h"

#pragma mark - class InterruptInfo
int InterruptInfo::getIrqNum()
{
	return IrqNum;
}

void InterruptInfo::setIrqNum(int _value)
{
	IrqNum = _value;
}

string InterruptInfo::getName()
{
	return Name;
}

void InterruptInfo::setName(string _value)
{
	Name = _value;
}

string InterruptInfo::getHandlerName()
{
	return HandlerName;
}

void InterruptInfo::setHandlerName(string _value)
{
	HandlerName = _value;
}


#pragma mark - class SoftInterruptInfo
int SoftInterruptInfo::getVec()
{
	return Vec;
}

void SoftInterruptInfo::setVec(int _value)
{
	Vec = _value;
}

string SoftInterruptInfo::getAction()
{
	return Action;
}

void SoftInterruptInfo::setAction(string _value)
{
	Action = _value;
}



