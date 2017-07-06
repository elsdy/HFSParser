//
//  Scheduler.h
//  HFSParser
//
//  Created by Dongyun Shin on 3/31/14.
//  Copyright (c) 2014 elsdy. All rights reserved.
//

#ifndef __HFSParser__Scheduler__
#define __HFSParser__Scheduler__

#include <iostream>

#include "ParseEntry.h"
#include "List.h"

#include "StartFinishTimeInfo.h"

class ScheduleInfo: public StartFinishTimeInfo
{
private:
	
	void init()
	{
	}
		
public:
	ScheduleInfo(): StartFinishTimeInfo()
	{
		init();
	}
	
	ScheduleInfo(Node <ParseEntry> * _a, Node <ParseEntry> * _b): StartFinishTimeInfo(_a, _b)
	{
		init();
	}

	~ScheduleInfo()
	{
				
	}
	
#pragma mark - for debug
};
#endif /* defined(__HFSParser__Scheduler__) */
