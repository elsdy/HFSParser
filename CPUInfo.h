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
private:
	List <ParseEntry> **CpuLog;
	
	List <IOInfo> **FileIO;
	
	List <ScheduleInfo> **ScheduleTime;
	List <InterruptInfo> **InterruptTime;
	List <SoftInterruptInfo> **SoftIrqTime;
	
	int MaxCpuNum;
	
	long CntAllLog;
	
	void init()
	{
		CpuLog = NULL;
		FileIO = NULL;
		ScheduleTime = NULL;
		InterruptTime = NULL;
		SoftIrqTime = NULL;
		
		MaxCpuNum = 0;
		CntAllLog = 0;
	}
	
public:
	CPUInfo()
	{
		init();
	}
	
	CPUInfo(int _cpu_num)
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
	~CPUInfo()
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
	
#pragma mark -  basic functions
	

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
