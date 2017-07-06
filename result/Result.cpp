//
//  Result.cpp
//  HFSParser
//
//  Created by Dongyun Shin on 6/18/14.
//  Copyright (c) 2014 elsdy. All rights reserved.
//

#include "Result.h"


int Result::printMetaData()
{
	Node <string> *metadata_iter = MetaData->getHead();
	
	while(metadata_iter)
	{
		Result::getOutputFile() << metadata_iter->entry->c_str();
		
		if( metadata_iter->next == NULL)
		{
			break;
		}
		
		Result::getOutputFile() << ",";
		
		metadata_iter = metadata_iter->next;
	}
	Result::getOutputFile() << endl;

	return 1;
}

void Result::addMetaData(string _item)
{
	Node <string> *new_item = new Node <string> (_item);
	
	Result::MetaData->push_back(new_item);
}

void Result::calculateProcessCpuExecutionTime(ProcessInfo * _process, double *_cpu_execution_time)
{
	List <StartFinishTimeInfo> * cpu_execution_time_list = _process->getCpuExecutionTime();
	Node <StartFinishTimeInfo> *time_iter = cpu_execution_time_list->getHead();
	double time = 0;
	while( time_iter )
	{
		time += time_iter->entry->getElapsedTime();
		
		time_iter = time_iter->next;
	}
	
	*_cpu_execution_time = time;
}

#define THROUGHPUT_UNIT_IS_B 0
#define THROUGHPUT_UNIT_IS_KB 0
#define THROUGHPUT_UNIT_IS_MB 1
void Result::analyzeEmmcIOBetweenStartFinishTime(List <eMMCInfo> *_emmc_list, double start_time, double finish_time, int (*sizefilter)(eMMCInfo *, double, double), double _low, double _high,  int (*typefilter)(eMMCInfo *, int, int[]), int _cnt_cmd, int _cmd[], int long &TotalCount, long &TotalTraffic, double &TotaleMMCIORespTime, double &TotalWorkloadThroughput, double &TotalWorkloadLatency )
{
	Node <eMMCInfo> *emmc_iter = _emmc_list->getHead();

	while(emmc_iter)
	{
		if(start_time <= emmc_iter->entry->getStart()->getApproxTimeStamp() && emmc_iter->entry->getFinish()->getApproxTimeStamp() <= finish_time)
		{
			if ((*sizefilter)(emmc_iter->entry, _low, _high) &&
			    (*typefilter)(emmc_iter->entry, _cnt_cmd, _cmd))
			{
				TotalCount++;
				TotalTraffic += emmc_iter->entry->getBlockSz() * emmc_iter->entry->getCntBlocks();
				TotaleMMCIORespTime += emmc_iter->entry->getElapsedTime();
			}
		}
		emmc_iter = emmc_iter->next;
	}

	if(TotaleMMCIORespTime)
#if THROUGHPUT_UNIT_IS_B
		TotalWorkloadThroughput = TotalTraffic / TotaleMMCIORespTime;
#elif THROUGHPUT_UNIT_IS_KB
		TotalWorkloadThroughput = TotalTraffic / TotaleMMCIORespTime / 1024;
#elif THROUGHPUT_UNIT_IS_MB
		TotalWorkloadThroughput = TotalTraffic / TotaleMMCIORespTime / 1024 / 1024;
#endif

	if(TotalCount)
		TotalWorkloadLatency = TotaleMMCIORespTime / TotalCount ;
	
#if DEBUG_analyzeEmmcIOBetweenStartFinishTime
	
	cout << endl << "Size filter [ " << _low << ", " << _high << " ]" << endl;
	
#if THROUGHPUT_UNIT_IS_B
	printf("TotalWorkloadThroughput = TotalTraffic / TotaleMMCIORespTime ( %f = %ld / %f ) \n", TotalWorkloadThroughput, TotalTraffic, TotaleMMCIORespTime);
#elif THROUGHPUT_UNIT_IS_KB
	printf("TotalWorkloadThroughput = TotalTraffic / TotaleMMCIORespTime ( %f = %ld / %f / 1024 ) \n", TotalWorkloadThroughput, TotalTraffic, TotaleMMCIORespTime);
#elif THROUGHPUT_UNIT_IS_MB
	printf("TotalWorkloadThroughput = TotalTraffic / TotaleMMCIORespTime ( %f = %ld / %f / 1024 / 1024 ) \n", TotalWorkloadThroughput, TotalTraffic, TotaleMMCIORespTime);
#endif
	
	printf("TotalWorkloadLatency = TotaleMMCIORespTime / TotalCount ( %f = %f / %ld ) \n", TotalWorkloadLatency, TotaleMMCIORespTime, TotalCount);
#endif
}

