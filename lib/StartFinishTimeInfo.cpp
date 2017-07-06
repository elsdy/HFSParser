//
//  StartFinishTimeInfo.cpp
//  HFSParser
//
//  Created by Dongyun Shin on 6/11/14.
//  Copyright (c) 2014 elsdy. All rights reserved.
//

#include "StartFinishTimeInfo.h"

ParseEntry *StartFinishTimeInfo::getStart()
{
	return Start;
}
void StartFinishTimeInfo::setStart(ParseEntry *_entry)
{
	Start = _entry;
}

ParseEntry *StartFinishTimeInfo::getFinish()
{
	return Finish;
}
void StartFinishTimeInfo::setFinish(ParseEntry *_entry)
{
	Finish = _entry;
}


double StartFinishTimeInfo::getElapsedTime()
{
	return ElapsedTime;
}


void StartFinishTimeInfo::setElapsedTime(double _value)
{
	ElapsedTime = _value;
}

Node<ParseEntry> * StartFinishTimeInfo::getIoStartLog()
{
	return IoStartLog;
}

void StartFinishTimeInfo::setIoStartLog(Node<ParseEntry> * _IoStartLog)
{
	IoStartLog = _IoStartLog;
}

Node<ParseEntry> *StartFinishTimeInfo::getIoFinishLog()
{
	return IoFinishLog;
}
void StartFinishTimeInfo::setIoFinishLog( Node <ParseEntry> * _IoFinishLog)
{
	IoFinishLog = _IoFinishLog;
}
