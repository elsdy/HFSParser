//
//  CPUInfo.h
//  HFSParser
//
//  Created by Dongyun Shin on 6/10/14.
//  Copyright (c) 2014 elsdy. All rights reserved.
//

#ifndef __HFSParser__CPUInfo__
#define __HFSParser__CPUInfo__

#include <iostream>

#include "ParseEntry.h"
#include "List.h"
#include "StartFinishTimeInfo.h"
#include "IOInfo.h"

#include "Scheduler.h"
#include "Interrupt.h"

class CPUInfo
{
public:
	CPUInfo();
	CPUInfo(int _cpu_num);
	~CPUInfo();

private:
	int MaxCpuNum;

	List <ParseEntry> **CpuLog;

	List <IOInfo> **FileIO;

	List <ScheduleInfo> **ScheduleTime;

	List <InterruptInfo> **InterruptTime;

	List <SoftInterruptInfo> **SoftIrqTime;

	
	long CntAllLog;
	
	void init();
	
public:
#pragma mark -  basic methods

	int getMaxCpuNum();
	void setMaxCpuNum(int _value);
	
	List <ParseEntry> *getCpuLog(int _cpu_num);
	
	List <IOInfo> *getFileIo(int _cpu_num);
	
	List <ScheduleInfo> *getScheduleTime(int _cpu_num);

	List <InterruptInfo> *getInterruptTime(int _cpu_num);
	
	List <SoftInterruptInfo> *getSoftIrqTime(int _cpu_num);

	long getCntAllLog();
	void setCntAllLog(long _value);
};
#endif /* defined(__HFSParser__CPUInfo__) */
