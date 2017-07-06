//
//  Result.h
//  HFSParser
//
//  Created by Dongyun Shin on 6/18/14.
//  Copyright (c) 2014 elsdy. All rights reserved.
//

#ifndef __HFSParser__Result__
#define __HFSParser__Result__

#include <iostream>

#include "List.h"
#include "eMMCLevel.h"

#include <string>
#include <climits>

#include "StartFinishTimeInfo.h"

class ProcessInfo;

#include "ProcessInfo.h"

#include "BlockLevel.h"
#include "eMMCLevel.h"

#include "IOInfo.h"

class Result
{
private:
	ofstream OutputFile;
	
	List <string> *MetaData;
	
	int IdxData;

	List <string> *TargetProcessName;
	
public:
	
	void init()
	{
		getOutputFile() << fixed;
		getOutputFile().precision(6);

		MetaData = new List <string> ();
		IdxData = 0;
		TargetProcessName = new List <string> ();
	}
	Result()
	{
		init();
	}
	Result(string _file_name_to_open)
	{
		init();
		
		OutputFile.open(_file_name_to_open.c_str(), ios::out);
	}
	~Result()
	{
		delete MetaData;
		delete TargetProcessName;
		
		OutputFile.close();
	}
	
	ofstream & getOutputFile()
	{
		return OutputFile;
	}
	
	void addMetaData(string _item);
	
	int printMetaData();
	
	template <class T>
	void printData(T _value)
	{		
		OutputFile << _value;
		IdxData++;
		if(IdxData == MetaData->getSize())
		{
			OutputFile << endl;
		}
		else if (IdxData < MetaData->getSize())
		{
			OutputFile << ",";
		}
		else
		{
			assert("over printed");
		}
	}
	template <class T>
	void printData(T _value, string _format)
	{
		OutputFile << _value;
		IdxData++;
		
		OutputFile << _format;
		
		if(IdxData == MetaData->getSize())
		{
			OutputFile << endl;
		}
		else if (IdxData < MetaData->getSize())
		{
			OutputFile << ",";
		}
		else
		{
			assert("over printed");
		}
	}

	
	long checkHowManyDataShouldBePrinted()
	{
		long ret = MetaData->getSize() - IdxData;
		
		return ret;
	}
	void preparePrintData()
	{
		IdxData = 0;
	}
	
	List <string> *getMetaData()
	{
		return MetaData;
	}
	
	List <string> * getTargetProcessName()
	{
		return TargetProcessName;
	}

	void calculateProcessCpuExecutionTime(ProcessInfo * _process, double *_cpu_execution_time);
	
	/**
	 @brief
	 @param
	 int (*sizefilter)(eMMCInfo*, double, double): size filter function
	 double _low: passed to the above sizefilter function as the first double parameter
	 double _high: passed to the above sizefilter function as the second double parameter
	 
	 int (*typefilter)(eMMCInfo*, int, int[]): type filter function. type include various cmd number
	 int _cnt_cmd: the number of cmd to be monitored
	 int __cmd[]: an array include monitored cmd
	 @return
	 long &TotalCount:
	 long &TotalTraffic:
	 double &TotalCPUTime:
	 double &TotalWorkloadThroughput:
	 double &TotalWorkloadIops:
	 */
#define DEBUG_analyzeEmmcIOBetweenStartFinishTime 0
	void analyzeEmmcIOBetweenStartFinishTime(List <eMMCInfo> *_emmc_list, double start_time, double finish_time, int (*sizefilter)(eMMCInfo*, double, double), double _low, double _high, int (*typefilter)(eMMCInfo*, int, int[]), int _cnt_cmd, int __cmd[], long &TotalCount, long &TotalTraffic, double &TotalCPUTime, double &TotalWorkloadThroughput, double &TotalWorkloadIops );
	
#define KB 1024
#define MB KB*KB
#define BLOCK_SIZE 512
	static int SizeFilter( eMMCInfo *_emmc_io, double low, double high)
	{
		long request_byte = BLOCK_SIZE * _emmc_io->getBlockSz() * _emmc_io->getCntBlocks();
		if( low == -1 )
			low = 0;
		if (high == -1)
			high = LONG_MAX/KB;
		
		if( low*KB <  request_byte && request_byte <= high*KB )
			return 1;
		else
			return 0;
	}
	
	
	static int TypeFilter( eMMCInfo *_emmc_io, int _cnt_cmd, int __cmd[] )
	{
		if( _cnt_cmd == -1)
			return 1;
		
		for( int i = 0; i < _cnt_cmd; i++)
		{
			if( __cmd[i] == _emmc_io->getCMD() )
			{
				return 1;
			}
			
		}
		return 0;
	}
};
#endif /* defined(__HFSParser__Result__) */
