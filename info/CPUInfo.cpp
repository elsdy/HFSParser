//
//  CPUInfo.cpp
//  HFSParser
//
//  Created by Dongyun Shin on 6/10/14.
//  Copyright (c) 2014 elsdy. All rights reserved.
//

#include "CPUInfo.h"

int CPUInfo::getMaxCpuNum()
{
	return MaxCpuNum;
}

void CPUInfo::setMaxCpuNum(int _value)
{
	MaxCpuNum = _value;
}


List <ParseEntry> *CPUInfo::getCpuLog(int _cpu_num)
{
	return CpuLog[_cpu_num];
	
}


long CPUInfo::getCntAllLog()
{
	long cnt_log = 0;
	
	for(int i = 0 ; i < MaxCpuNum; i++)
	{
		cnt_log += CpuLog[i]->getSize();
	}
	
	CntAllLog = cnt_log;
	
	return cnt_log;
}

void CPUInfo::setCntAllLog(long _value)
{
	CntAllLog = _value;
}

List <ScheduleInfo> *CPUInfo::getScheduleTime(int _cpu_num)
{
	if( _cpu_num > MaxCpuNum-1)
	{
		assert(0);
		return NULL;
	}
	return ScheduleTime[_cpu_num];
}

List <SoftInterruptInfo> *CPUInfo::getSoftIrqTime(int _cpu_num)
{
	if( _cpu_num > MaxCpuNum-1)
	{
		assert(0);
		return NULL;
	}
	return SoftIrqTime[_cpu_num];
}

List <InterruptInfo> *CPUInfo::getInterruptTime(int _cpu_num)
{
	if( _cpu_num > MaxCpuNum-1)
	{
		assert(0);
		return NULL;
	}
	
	return InterruptTime[_cpu_num];
}

List <IOInfo> *CPUInfo::getFileIo(int _cpu_num)
{
	if( _cpu_num > MaxCpuNum-1)
	{
		assert(0);
		return NULL;
	}
	
	return FileIO[_cpu_num];
}