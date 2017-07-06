//
//  ResultForFullAnalysis.h
//  HFSParser
//
//  Created by Dongyun Shin on 6/26/14.
//  Copyright (c) 2014 elsdy. All rights reserved.
//

#ifndef __HFSParser__ResultForFullAnalysis__
#define __HFSParser__ResultForFullAnalysis__

#include <iostream>
#include "Result.h"
class ResultForFullAnalysis: public Result
{
private:
	
	int All;
	//	int EntryType;
	//	long LineNumber = 0;;
	//	string ProcessName;
	//	string OriginProcessName;
	//	long Pid;
	//	long OriginPid;
	//	double StartTime;
	//	double FinishTime;
	//	string RW;
	//	int IoSize;
	//	string FileName;
	//	long Inode;
	//	long MappedSysIOIndex;
	//	int AccessAddress;
	
#define ENTRY_TYPE_CPU_EXECUTE -1
#define ENTRY_TYPE_SYSIO 1
#define ENTRY_TYPE_IO_SCHEUDLER 2
#define ENTRY_TYPE_BLOCK_IO_TRANSFER 3
#define ENTRY_TYPE_EMMC_IO 4
	
	void init()
	{
		All = 1;
		//		EntryType = 0;
		//		LineNumber = 0;
		//		ProcessName = "";
		//		OriginProcessName = "";
		//		Pid = 0;
		//		OriginPid = 0;
		//		StartTime = FinishTime = 0;
		//		RW = "";
		//		IoSize = -1;
		//		FileName = "";
		//		Inode = 0;
		//		MappedSysIOIndex = 0;
		//		AccessAddress =-1;
	}
	
public:
	ResultForFullAnalysis(string _file_name_to_open): Result(_file_name_to_open)
	{
		cout << "Result for Full version analysis will be generated" << endl;
		
		init();
		
	}
	~ResultForFullAnalysis()
	{
		
	}
	
	/**
	 * brief
	 * param
	 * return
	 the number of total Sys IO
	 */
#define DEBUG_printIOFromProcess 0
	void printIOFromProcess(List <ProcessInfo> *_process_list);
	
	
	void printIOSchedule(List <BlockInfo> *_block_io_list, List <ProcessInfo> *_process_list);
	void printBlockIO(List <BlockInfo> *_block_io_list, List <ProcessInfo> *_process_list);
	void printEmmcIO(List <eMMCInfo> *_emmc_io_list, List <ProcessInfo> *_process_list);
	
	/**
	 * return the number of total execution time event for all process
	 */
	void printCpuExecutionTime(List <ProcessInfo> *_process_list);
	
	
	void printIOInfoResult(Node <IOInfo> *_io, Node <ProcessInfo> * _process);
	void printIoScheduleResult(Node <BlockInfo> *_block_io_info, List <ProcessInfo> *_process_list);
	void printBlockInfoResult(Node <BlockInfo> *_block_io_info, List <ProcessInfo> *_process_list);
	void printEmmcInfoResult( Node <eMMCInfo> *_emmc_io_info, List <ProcessInfo> *_process_list);
	
	/**
	 * brief
	 * param
	 * return
	 
	 */
	int printCPUExecutioTimeInfo(Node <StartFinishTimeInfo> *_cpu_execution_time, Node <ProcessInfo> *_process);
	
	
	
#pragma mark - calculating eMMC Level Result
	void countEmmcIO(List <eMMCInfo> *_emmc_io_list, long &_result);
	void calculateEmmcIORequestSize(List <eMMCInfo> * _emmc_io_list, long &_result);
	
	int calculateEmmcTotalResponseTime(List <eMMCInfo> *_emmc_io_list, double & _result);
};


#endif /* defined(__HFSParser__ResultForFullAnalysis__) */
