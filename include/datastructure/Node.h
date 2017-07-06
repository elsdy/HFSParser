//
//  Node.h
//  HFSParser
//
//  Created by Dongyun Shin on 3/25/14.
//  Copyright (c) 2014 elsdy. All rights reserved.
//

#ifndef __HFSParser__Node__
#define __HFSParser__Node__

#include <iostream>
#include <string>

#include "ParseEntry.h"

template <typename T>
class Node
{
private:
public:
	T *entry;
	Node <T> *prev, *next;
	
	int ShouldDeleteEntry;
	
public:
	Node() {
		entry = new T();
		prev = next = NULL;
		
		ShouldDeleteEntry = 1;
	}

	Node( T *_entry)
	{
		entry = _entry;
		prev = next = NULL;
		
		ShouldDeleteEntry = 0;
	}
	
	
//	Node(string _str, long _line_number) {
//		entry = new T(_str, _line_number);
//		prev = next = NULL;
//		
//		ShouldDeleteEntry = 1;
//	}
	
	Node(string _str, long _line_number, string _log_version) {
		entry = new T(_str, _line_number, _log_version);
		prev = next = NULL;
		
		ShouldDeleteEntry = 1;
	}

	Node (string _str)
	{
		entry = new T(_str);
		prev = next = NULL;
		
		ShouldDeleteEntry = 1;
	}

	Node( Node <ParseEntry> * _a, Node <ParseEntry> * _b)
	{
		entry = new T( _a, _b);
		prev = next = NULL;
		
		ShouldDeleteEntry = 1;
	}
	
	Node( Node <ParseEntry> * _io_start, Node <ParseEntry> * _io_end, int _fd, long _size, long _pos, string _filename)
	{
		entry = new T( _io_start, _io_end, _fd, _size, _pos, _filename);
		prev = next = NULL;
		
		ShouldDeleteEntry = 1;
	}
	
	~Node() {
		if(ShouldDeleteEntry)
			delete entry;
		
		prev = next = NULL;
		entry = NULL;
	}
	
	int getShouldDeleteEntry()
	{
		return ShouldDeleteEntry;
	}

	void setShouldDeleteEntry(int _sd)
	{
		ShouldDeleteEntry = _sd;
	}
	
	void linkSameEntry( Node <T> * _other_node)
	{
		entry = _other_node->entry;
	}

	Node <T> * jumpNext(int jump_cnt)
	{
		int cnt = 0;
		Node <T> * ret = this;

		while( ret != NULL )
		{
			ret = ret->next;
			if(cnt == jump_cnt)
				break;
		}

		return ret;
	}

	Node <T> * jumpPrev(long jump_cnt)
	{
		int cnt = 0;
		Node <T> * ret = this;

		while( ret != NULL )
		{
			ret = ret->prev;
			if(cnt == jump_cnt)
				break;
		}

		return ret;
	}
		
#define PRINT_FULL_VERSION_StartFinishTimeInfo 1
	void printFromSelfToNode( Node <T> * _to_node)
	{
		Node <T> *iter = this;
		while(iter && iter != _to_node)
		{
#if PRINT_FULL_VERSION_StartFinishTimeInfo
			iter->entry->printFromStartToFinish();
#else
			iter->entry->print();
#endif
			iter = iter->next;
		}
#if PRINT_FULL_VERSION_StartFinishTimeInfo
		iter->entry->printFromStartToFinish();
#else
		iter->entry->print();
#endif
	}
};



#endif /* defined(__HFSParser__Node__) */
