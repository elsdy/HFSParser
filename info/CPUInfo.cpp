//
//  CPUInfo.cpp
//  HFSParser
//
//  Created by Dongyun Shin on 6/10/14.
//  Copyright (c) 2014 elsdy. All rights reserved.
//

#include "CPUInfo.h"

CPUInfo::CPUInfo()
{
	init();
}

CPUInfo::CPUInfo(int _cpu_num)
{
	CPUInfo();

	MaxCpuNum = _cpu_num;
	CntAllLog = 0;

	/// generate list for build logs for all core in a cpu
	CpuLog = new List <ParseEntry> *[_cpu_num];

	// _cpu_num 개수 만큼 CpuLog를 할당
	for(int i = 0 ; i < _cpu_num; i++)
	{
		CpuLog[i] = new List <ParseEntry>;
	}

	/// generate list for build logs for all core in a cpu
	FileIO = new List <IOInfo> *[_cpu_num];

	// _cpu_num 개수 만큼 CpuLog를 할당
	for(int i = 0 ; i < _cpu_num; i++)
	{
		FileIO[i] = new List <IOInfo>;
	}

	/// generate list for build scheduling time info for all core in a cpu
	ScheduleTime = new List <ScheduleInfo> *[_cpu_num];

	// _cpu_num 개수 만큼 CpuLog를 할당
	for(int i = 0 ; i < _cpu_num; i++)
	{
		ScheduleTime[i] = new List <ScheduleInfo> ();
	}

	/// generate list for build interrupt time info for all core in a cpu
	InterruptTime = new List <InterruptInfo> *[_cpu_num];

	// _cpu_num 개수 만큼 CpuLog를 할당
	for(int i = 0 ; i < _cpu_num; i++)
	{
		InterruptTime[i] = new List <InterruptInfo> ();
	}

	/// generate list for build interrupt time info for all core in a cpu
	SoftIrqTime = new List <SoftInterruptInfo> *[_cpu_num];

	// _cpu_num 개수 만큼 CpuLog를 할당
	for(int i = 0 ; i < _cpu_num; i++)
	{
		SoftIrqTime[i] = new List <SoftInterruptInfo> ();
	}
}

CPUInfo::~CPUInfo()
{
	for(int i = 0; i < MaxCpuNum; i++ )
	{
		delete CpuLog[i];
		delete FileIO[i];
		delete ScheduleTime[i];
		delete InterruptTime[i];
		delete SoftIrqTime[i];
	}

	delete [] CpuLog;
	delete [] FileIO;
	delete [] ScheduleTime;
	delete [] InterruptTime;
	delete [] SoftIrqTime;

}

void CPUInfo::init()
{
	CpuLog = NULL;
	FileIO = NULL;
	ScheduleTime = NULL;
	InterruptTime = NULL;
	SoftIrqTime = NULL;

	MaxCpuNum = 0;
	CntAllLog = 0;
}

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
