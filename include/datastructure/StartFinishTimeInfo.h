//
//  StartFinishTimeInfo.h
//  HFSParser
//
//  Created by Dongyun Shin on 6/11/14.
//  Copyright (c) 2014 elsdy. All rights reserved.
//

#ifndef __HFSParser__StartFinishTimeInfo__
#define __HFSParser__StartFinishTimeInfo__

#include <iostream>

#include "ParseEntry.h"
#include "Node.h"


class StartFinishTimeInfo
{
private:
	
	/// pointer to the node of the IO start in a log
	Node<ParseEntry> *IoStartLog;
	/// pointer to the node of the IO finish in a log
	Node<ParseEntry> *IoFinishLog;

	/// start event
	ParseEntry *Start;
	/// finish event
	ParseEntry *Finish;
	
	/// Type means that what this event is (refer TimeType)
#define NO_TYPE 0
#define PROCESS_CPU_TIME 1
#define SCHEDULING_TIME 2
#define PROCESS_ELAPSED_TIME 3
	
	/// time between the finish event and the start event
	double ElapsedTime;
	
	void init()
	{
		
		ElapsedTime = 0;
		Start = Finish = NULL;
		IoStartLog = IoFinishLog = NULL;

	}
	
public:
	StartFinishTimeInfo()
	{
		init();
	}

	StartFinishTimeInfo(Node <ParseEntry> *_start, Node <ParseEntry> *_finish)
	{
		init();
		
		if( _start != NULL)
		{
			IoStartLog = _start;
			Start = _start->entry;
		}
		
		if(_finish != NULL)
		{
			IoFinishLog = _finish;
			Finish = _finish->entry;
		}
		
		if( _start && _finish)
			ElapsedTime = Finish->getApproxTimeStamp() - Start->getApproxTimeStamp();
	}
	
	~StartFinishTimeInfo()
	{
	}
#pragma mark - get & set methods
	ParseEntry *getStart();
	
	void setStart(ParseEntry *_entry);
	
	ParseEntry *getFinish();
	
	void setFinish(ParseEntry *_entry);

	double getElapsedTime();
	
	void setElapsedTime(double _value);
	
	Node<ParseEntry> * getIoStartLog();
	
	void setIoStartLog(Node<ParseEntry> * _IoStartLog);
	
	Node<ParseEntry> *getIoFinishLog();
	
	void setIoFinishLog( Node <ParseEntry> * _IoFinishLog);

	
#pragma mark - for debug
	void print(ofstream &out)
	{
		if(Start)
			Start->printParseEntry(out);
		if(Finish)
			Finish->printParseEntry(out);
	}
	
	void print()
	{
		if(Start)
			Start->printParseEntry();
		if(Finish)
			Finish->printParseEntry();;
	}
	
	void printFromStartToFinish()
	{
		Node <ParseEntry> *node_iter = IoStartLog;
		
		while(node_iter != IoFinishLog)
		{
			node_iter->entry->print();
			
			node_iter = node_iter->next;
		}
		node_iter->entry->print();
	}
};


#endif /* defined(__HFSParser__StartFinishTimeInfo__) */
