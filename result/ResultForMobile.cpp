//
//  ResultForMobile.cpp
//  HFSParser
//
//  Created by Dongyun Shin on 6/26/14.
//  Copyright (c) 2014 elsdy. All rights reserved.
//

#include "ResultForMobile.h"

#define FORMAT_FLOATING_POINTS 8
void ResultForMobile::printProcessSummaryForMobile(string _process_name, List<ProcessInfo> *_process_list, long _test_id, List <eMMCInfo> *_emmc_list)
{
	cout << endl << "Mobile Result for the process whose name is " << _process_name << " will be generated" << endl;
	
	long TotalCount = 0;
	long TotalTraffic = 0;
	double TotalCPUTime = 0;
	double TotalWorkloadThroughput = 0;
	double TotalWorkloadLatency = 0;

	long ReadCount = 0;
	long ReadTraffic = 0;
	double ReadCPUTime = 0;
	double ReadWorkloadThroughput = 0;
	double ReadWorkloadLatency = 0;
	
	long WriteCount = 0;
	long WriteTraffic = 0;
	double WriteCPUTime = 0;
	double WriteWorkloadThroughput = 0;
	double WriteWorkloadLatency = 0;
	
	long ReadCount_0_5k = 0, ReadCount_1k = 0, ReadCount_2k = 0, ReadCount_4k = 0, ReadCount_8k = 0,  ReadCount_16k = 0, ReadCount_32k = 0, ReadCount_64k = 0, ReadCount_128k = 0, ReadCount_256k = 0, ReadCount_512k = 0, ReadCount_MAX = 0;
	long ReadTraffic_0_5k = 0, ReadTraffic_1k = 0, ReadTraffic_2k = 0, ReadTraffic_4k = 0, ReadTraffic_8k = 0,  ReadTraffic_16k = 0, ReadTraffic_32k = 0, ReadTraffic_64k = 0, ReadTraffic_128k = 0, ReadTraffic_256k = 0, ReadTraffic_512k = 0, ReadTraffic_MAX = 0;
	double ReadCPUTime_0_5k = 0, ReadCPUTime_1k = 0, ReadCPUTime_2k = 0, ReadCPUTime_4k = 0, ReadCPUTime_8k = 0,  ReadCPUTime_16k = 0, ReadCPUTime_32k = 0, ReadCPUTime_64k = 0, ReadCPUTime_128k = 0, ReadCPUTime_256k = 0, ReadCPUTime_512k = 0, ReadCPUTime_MAX = 0;
	double ReadWorkloadThroughput_0_5k = 0, ReadWorkloadThroughput_1k = 0, ReadWorkloadThroughput_2k = 0, ReadWorkloadThroughput_4k = 0, ReadWorkloadThroughput_8k = 0,  ReadWorkloadThroughput_16k = 0, ReadWorkloadThroughput_32k = 0, ReadWorkloadThroughput_64k = 0, ReadWorkloadThroughput_128k = 0, ReadWorkloadThroughput_256k = 0, ReadWorkloadThroughput_512k = 0, ReadWorkloadThroughput_MAX = 0;
	double ReadWorkloadLatency_0_5k = 0, ReadWorkloadLatency_1k = 0, ReadWorkloadLatency_2k = 0, ReadWorkloadLatency_4k = 0, ReadWorkloadLatency_8k = 0,  ReadWorkloadLatency_16k = 0, ReadWorkloadLatency_32k = 0, ReadWorkloadLatency_64k = 0, ReadWorkloadLatency_128k = 0, ReadWorkloadLatency_256k = 0, ReadWorkloadLatency_512k = 0, ReadWorkloadLatency_MAX = 0;

	
	long WriteCount_0_5k = 0, WriteCount_1k = 0, WriteCount_2k = 0, WriteCount_4k = 0, WriteCount_8k = 0,  WriteCount_16k = 0, WriteCount_32k = 0, WriteCount_64k = 0, WriteCount_128k = 0, WriteCount_256k = 0, WriteCount_512k = 0, WriteCount_MAX = 0;
	long WriteTraffic_0_5k = 0, WriteTraffic_1k = 0, WriteTraffic_2k = 0, WriteTraffic_4k = 0, WriteTraffic_8k = 0,  WriteTraffic_16k = 0, WriteTraffic_32k = 0, WriteTraffic_64k = 0, WriteTraffic_128k = 0, WriteTraffic_256k = 0, WriteTraffic_512k = 0, WriteTraffic_MAX = 0;
	double WriteCPUTime_0_5k = 0, WriteCPUTime_1k = 0, WriteCPUTime_2k = 0, WriteCPUTime_4k = 0, WriteCPUTime_8k = 0,  WriteCPUTime_16k = 0, WriteCPUTime_32k = 0, WriteCPUTime_64k = 0, WriteCPUTime_128k = 0, WriteCPUTime_256k = 0, WriteCPUTime_512k = 0, WriteCPUTime_MAX = 0;
	double WriteWorkloadThroughput_0_5k = 0, WriteWorkloadThroughput_1k = 0, WriteWorkloadThroughput_2k = 0, WriteWorkloadThroughput_4k = 0, WriteWorkloadThroughput_8k = 0,  WriteWorkloadThroughput_16k = 0, WriteWorkloadThroughput_32k = 0, WriteWorkloadThroughput_64k = 0, WriteWorkloadThroughput_128k = 0, WriteWorkloadThroughput_256k = 0, WriteWorkloadThroughput_512k = 0, WriteWorkloadThroughput_MAX = 0;
	double WriteWorkloadLatency_0_5k = 0, WriteWorkloadLatency_1k = 0, WriteWorkloadLatency_2k = 0, WriteWorkloadLatency_4k = 0, WriteWorkloadLatency_8k = 0,  WriteWorkloadLatency_16k = 0, WriteWorkloadLatency_32k = 0, WriteWorkloadLatency_64k = 0, WriteWorkloadLatency_128k = 0, WriteWorkloadLatency_256k = 0, WriteWorkloadLatency_512k = 0, WriteWorkloadLatency_MAX = 0;


	ProcessInfo *process = NULL;
	Parser::findProcessWithName(_process_name, _process_list, &process);
	if(process == NULL)
	{
		cout << "- A Result for " << _process_name << " is canceled because of the lack of its log" << endl;
		return;
	}
	
	double start_time = process->getCpuExecutionTime()->getHead()->entry->getStart()->getApproxTimeStamp();
	double finish_time = process->getCpuExecutionTime()->getTail()->entry->getFinish()->getApproxTimeStamp();
	
	double TotalWorkloadExecutionTime = finish_time - start_time;
	
	double TotalWorkloadCpuTime = 0;
	calculateProcessCpuExecutionTime(process, &TotalWorkloadCpuTime );
	
	cout << "Logging period: " << TotalWorkloadExecutionTime << " (" << start_time << " ~ " << finish_time << " )" << endl;
	cout << "Lines in the Log: " << process->getCpuExecutionTime()->getHead()->entry->getStart()->getLineNumber() << " ~ " << process->getCpuExecutionTime()->getTail()->entry->getFinish()->getLineNumber();
	
	/// total / read / write
	int count_cmd_read_write = 6;
	int cmd_read_write[] = {18,19,53, 24,25,54};
	// Result::analyzeEmmcIOBetweenStartFinishTime(_emmc_list, start_time, finish_time, Result::SizeFilter, -1, -1, Result::TypeFilter, -1, NULL, TotalCount, TotalTraffic, TotalCPUTime, TotalWorkloadThroughput, TotalWorkloadLatency );
	Result::analyzeEmmcIOBetweenStartFinishTime(_emmc_list, start_time, finish_time, Result::SizeFilter, -1, -1, Result::TypeFilter, count_cmd_read_write, cmd_read_write, TotalCount, TotalTraffic, TotalCPUTime, TotalWorkloadThroughput, TotalWorkloadLatency );
	
	int count_cmd_read = 3;
	int cmd_read[] = {18,19,53};
	Result::analyzeEmmcIOBetweenStartFinishTime(_emmc_list, start_time, finish_time, Result::SizeFilter, -1, -1, Result::TypeFilter, count_cmd_read, cmd_read, ReadCount, ReadTraffic, ReadCPUTime, ReadWorkloadThroughput, ReadWorkloadLatency );
	
	int count_cmd_write = 3;
	int cmd_write[] = {24,25,54};
	Result::analyzeEmmcIOBetweenStartFinishTime(_emmc_list, start_time, finish_time, Result::SizeFilter, -1, -1, Result::TypeFilter, count_cmd_write, cmd_write, WriteCount, WriteTraffic, WriteCPUTime, WriteWorkloadThroughput, WriteWorkloadLatency );
	
	/// size categorization for read
	Result::analyzeEmmcIOBetweenStartFinishTime(_emmc_list, start_time, finish_time, Result::SizeFilter, 0, 0.5, Result::TypeFilter, count_cmd_read, cmd_read, ReadCount_0_5k, ReadTraffic_0_5k, ReadCPUTime_0_5k, ReadWorkloadThroughput_0_5k, ReadWorkloadLatency_0_5k );
	Result::analyzeEmmcIOBetweenStartFinishTime(_emmc_list, start_time, finish_time, Result::SizeFilter, 0.5, 1, Result::TypeFilter, count_cmd_read, cmd_read, ReadCount_1k, ReadTraffic_1k, ReadCPUTime_1k, ReadWorkloadThroughput_1k, ReadWorkloadLatency_1k );
	Result::analyzeEmmcIOBetweenStartFinishTime(_emmc_list, start_time, finish_time, Result::SizeFilter, 1, 2, Result::TypeFilter, count_cmd_read, cmd_read, ReadCount_2k, ReadTraffic_2k, ReadCPUTime_2k, ReadWorkloadThroughput_2k, ReadWorkloadLatency_2k );
	Result::analyzeEmmcIOBetweenStartFinishTime(_emmc_list, start_time, finish_time, Result::SizeFilter, 2, 4, Result::TypeFilter, count_cmd_read, cmd_read, ReadCount_4k, ReadTraffic_4k, ReadCPUTime_4k, ReadWorkloadThroughput_4k, ReadWorkloadLatency_4k );
	Result::analyzeEmmcIOBetweenStartFinishTime(_emmc_list, start_time, finish_time, Result::SizeFilter, 4, 8, Result::TypeFilter, count_cmd_read, cmd_read, ReadCount_8k, ReadTraffic_8k, ReadCPUTime_8k, ReadWorkloadThroughput_8k, ReadWorkloadLatency_8k );
	Result::analyzeEmmcIOBetweenStartFinishTime(_emmc_list, start_time, finish_time, Result::SizeFilter, 8, 16, Result::TypeFilter, count_cmd_read, cmd_read, ReadCount_16k, ReadTraffic_16k, ReadCPUTime_16k, ReadWorkloadThroughput_16k, ReadWorkloadLatency_16k );
	Result::analyzeEmmcIOBetweenStartFinishTime(_emmc_list, start_time, finish_time, Result::SizeFilter, 16, 32, Result::TypeFilter, count_cmd_read, cmd_read, ReadCount_32k, ReadTraffic_32k, ReadCPUTime_32k, ReadWorkloadThroughput_32k, ReadWorkloadLatency_32k );
	Result::analyzeEmmcIOBetweenStartFinishTime(_emmc_list, start_time, finish_time, Result::SizeFilter, 32, 64, Result::TypeFilter, count_cmd_read, cmd_read, ReadCount_64k, ReadTraffic_64k, ReadCPUTime_64k, ReadWorkloadThroughput_64k, ReadWorkloadLatency_64k );
	Result::analyzeEmmcIOBetweenStartFinishTime(_emmc_list, start_time, finish_time, Result::SizeFilter, 64, 128, Result::TypeFilter, count_cmd_read, cmd_read, ReadCount_128k, ReadTraffic_128k, ReadCPUTime_128k, ReadWorkloadThroughput_128k, ReadWorkloadLatency_128k );
	Result::analyzeEmmcIOBetweenStartFinishTime(_emmc_list, start_time, finish_time, Result::SizeFilter, 128, 256, Result::TypeFilter, count_cmd_read, cmd_read, ReadCount_256k, ReadTraffic_256k, ReadCPUTime_256k, ReadWorkloadThroughput_256k, ReadWorkloadLatency_256k );
	Result::analyzeEmmcIOBetweenStartFinishTime(_emmc_list, start_time, finish_time, Result::SizeFilter, 256, 512, Result::TypeFilter, count_cmd_read, cmd_read, ReadCount_512k, ReadTraffic_512k, ReadCPUTime_512k, ReadWorkloadThroughput_512k, ReadWorkloadLatency_512k );
	Result::analyzeEmmcIOBetweenStartFinishTime(_emmc_list, start_time, finish_time, Result::SizeFilter, 512, -1, Result::TypeFilter, count_cmd_read, cmd_read, ReadCount_MAX, ReadTraffic_MAX, ReadCPUTime_MAX, ReadWorkloadThroughput_MAX, ReadWorkloadLatency_MAX );

	
	/// size categorization for write
	Result::analyzeEmmcIOBetweenStartFinishTime(_emmc_list, start_time, finish_time, Result::SizeFilter, 0, 0.5, Result::TypeFilter, count_cmd_write, cmd_write, WriteCount_0_5k, WriteTraffic_0_5k, WriteCPUTime_0_5k, WriteWorkloadThroughput_0_5k, WriteWorkloadLatency_0_5k );
	Result::analyzeEmmcIOBetweenStartFinishTime(_emmc_list, start_time, finish_time, Result::SizeFilter, 0.5, 1, Result::TypeFilter, count_cmd_write, cmd_write, WriteCount_1k, WriteTraffic_1k, WriteCPUTime_1k, WriteWorkloadThroughput_1k, WriteWorkloadLatency_1k );
	Result::analyzeEmmcIOBetweenStartFinishTime(_emmc_list, start_time, finish_time, Result::SizeFilter, 1, 2, Result::TypeFilter, count_cmd_write, cmd_write, WriteCount_2k, WriteTraffic_2k, WriteCPUTime_2k, WriteWorkloadThroughput_2k, WriteWorkloadLatency_2k );
	Result::analyzeEmmcIOBetweenStartFinishTime(_emmc_list, start_time, finish_time, Result::SizeFilter, 2, 4, Result::TypeFilter, count_cmd_write, cmd_write, WriteCount_4k, WriteTraffic_4k, WriteCPUTime_4k, WriteWorkloadThroughput_4k, WriteWorkloadLatency_4k );
	Result::analyzeEmmcIOBetweenStartFinishTime(_emmc_list, start_time, finish_time, Result::SizeFilter, 4, 8, Result::TypeFilter, count_cmd_write, cmd_write, WriteCount_8k, WriteTraffic_8k, WriteCPUTime_8k, WriteWorkloadThroughput_8k, WriteWorkloadLatency_8k );
	Result::analyzeEmmcIOBetweenStartFinishTime(_emmc_list, start_time, finish_time, Result::SizeFilter, 8, 16, Result::TypeFilter, count_cmd_write, cmd_write, WriteCount_16k, WriteTraffic_16k, WriteCPUTime_16k, WriteWorkloadThroughput_16k, WriteWorkloadLatency_16k );
	Result::analyzeEmmcIOBetweenStartFinishTime(_emmc_list, start_time, finish_time, Result::SizeFilter, 16, 32, Result::TypeFilter, count_cmd_write, cmd_write, WriteCount_32k, WriteTraffic_32k, WriteCPUTime_32k, WriteWorkloadThroughput_32k, WriteWorkloadLatency_32k );
	Result::analyzeEmmcIOBetweenStartFinishTime(_emmc_list, start_time, finish_time, Result::SizeFilter, 32, 64, Result::TypeFilter, count_cmd_write, cmd_write, WriteCount_64k, WriteTraffic_64k, WriteCPUTime_64k, WriteWorkloadThroughput_64k, WriteWorkloadLatency_64k );
	Result::analyzeEmmcIOBetweenStartFinishTime(_emmc_list, start_time, finish_time, Result::SizeFilter, 64, 128, Result::TypeFilter, count_cmd_write, cmd_write, WriteCount_128k, WriteTraffic_128k, WriteCPUTime_128k, WriteWorkloadThroughput_128k, WriteWorkloadLatency_128k );
	Result::analyzeEmmcIOBetweenStartFinishTime(_emmc_list, start_time, finish_time, Result::SizeFilter, 128, 256, Result::TypeFilter, count_cmd_write, cmd_write, WriteCount_256k, WriteTraffic_256k, WriteCPUTime_256k, WriteWorkloadThroughput_256k, WriteWorkloadLatency_256k );
	Result::analyzeEmmcIOBetweenStartFinishTime(_emmc_list, start_time, finish_time, Result::SizeFilter, 256, 512, Result::TypeFilter, count_cmd_write, cmd_write, WriteCount_512k, WriteTraffic_512k, WriteCPUTime_512k, WriteWorkloadThroughput_512k, WriteWorkloadLatency_512k );
	Result::analyzeEmmcIOBetweenStartFinishTime(_emmc_list, start_time, finish_time, Result::SizeFilter, 512, -1, Result::TypeFilter, count_cmd_write, cmd_write, WriteCount_MAX, WriteTraffic_MAX, WriteCPUTime_MAX, WriteWorkloadThroughput_MAX, WriteWorkloadLatency_MAX );
	
#if DEBUG_printProcessSummaryForMobile
	long long_tmp = 0;
	double double_tmp  = 0;
#endif

#if DEBUG_printProcessSummaryForMobile
	long_tmp = ReadCount + WriteCount;
#endif
	assert(TotalCount == ReadCount + WriteCount);

#if DEBUG_printProcessSummaryForMobile
	long_tmp = ReadCount_0_5k + ReadCount_1k + ReadCount_2k + ReadCount_4k + ReadCount_8k + ReadCount_16k + ReadCount_32k + ReadCount_64k + ReadCount_128k + ReadCount_256k + ReadCount_512k + ReadCount_MAX;
#endif
	assert(ReadCount == ReadCount_0_5k + ReadCount_1k + ReadCount_2k + ReadCount_4k + ReadCount_8k + ReadCount_16k + ReadCount_32k + ReadCount_64k + ReadCount_128k + ReadCount_256k + ReadCount_512k + ReadCount_MAX);

#if DEBUG_printProcessSummaryForMobile
	long_tmp = ReadTraffic_0_5k + ReadTraffic_1k + ReadTraffic_2k + ReadTraffic_4k + ReadTraffic_8k + ReadTraffic_16k + ReadTraffic_32k + ReadTraffic_64k + ReadTraffic_128k + ReadTraffic_256k + ReadTraffic_512k + ReadTraffic_MAX;
#endif
	assert(ReadTraffic == ReadTraffic_0_5k + ReadTraffic_1k + ReadTraffic_2k + ReadTraffic_4k + ReadTraffic_8k + ReadTraffic_16k + ReadTraffic_32k + ReadTraffic_64k + ReadTraffic_128k + ReadTraffic_256k + ReadTraffic_512k + ReadTraffic_MAX);

#if DEBUG_printProcessSummaryForMobile
	double_tmp = ReadCPUTime_0_5k + ReadCPUTime_1k + ReadCPUTime_2k + ReadCPUTime_4k + ReadCPUTime_8k + ReadCPUTime_16k + ReadCPUTime_32k + ReadCPUTime_64k + ReadCPUTime_128k + ReadCPUTime_256k + ReadCPUTime_512k + ReadCPUTime_MAX;
#endif
	assert(ReadCPUTime == ReadCPUTime_0_5k + ReadCPUTime_1k + ReadCPUTime_2k + ReadCPUTime_4k + ReadCPUTime_8k + ReadCPUTime_16k + ReadCPUTime_32k + ReadCPUTime_64k + ReadCPUTime_128k + ReadCPUTime_256k + ReadCPUTime_512k + ReadCPUTime_MAX);

#if DEBUG_printProcessSummaryForMobile
	long_tmp = WriteCount_0_5k + WriteCount_1k + WriteCount_2k + WriteCount_4k + WriteCount_8k + WriteCount_16k + WriteCount_32k + WriteCount_64k + WriteCount_128k + WriteCount_256k + WriteCount_512k + WriteCount_MAX;
#endif
	assert(WriteCount == WriteCount_0_5k + WriteCount_1k + WriteCount_2k + WriteCount_4k + WriteCount_8k + WriteCount_16k + WriteCount_32k + WriteCount_64k + WriteCount_128k + WriteCount_256k + WriteCount_512k + WriteCount_MAX);
	
#if DEBUG_printProcessSummaryForMobile
	long_tmp = WriteTraffic_0_5k + WriteTraffic_1k + WriteTraffic_2k + WriteTraffic_4k + WriteTraffic_8k + WriteTraffic_16k + WriteTraffic_32k + WriteTraffic_64k + WriteTraffic_128k + WriteTraffic_256k + WriteTraffic_512k + WriteTraffic_MAX;
#endif
	assert(WriteTraffic == WriteTraffic_0_5k + WriteTraffic_1k + WriteTraffic_2k + WriteTraffic_4k + WriteTraffic_8k + WriteTraffic_16k + WriteTraffic_32k + WriteTraffic_64k + WriteTraffic_128k + WriteTraffic_256k + WriteTraffic_512k + WriteTraffic_MAX);
	
#if DEBUG_printProcessSummaryForMobile
	double_tmp = WriteCPUTime_0_5k + WriteCPUTime_1k + WriteCPUTime_2k + WriteCPUTime_4k + WriteCPUTime_8k + WriteCPUTime_16k + WriteCPUTime_32k + WriteCPUTime_64k + WriteCPUTime_128k + WriteCPUTime_256k + WriteCPUTime_512k + WriteCPUTime_MAX;
#endif
	assert(WriteCPUTime == WriteCPUTime_0_5k + WriteCPUTime_1k + WriteCPUTime_2k + WriteCPUTime_4k + WriteCPUTime_8k + WriteCPUTime_16k + WriteCPUTime_32k + WriteCPUTime_64k + WriteCPUTime_128k + WriteCPUTime_256k + WriteCPUTime_512k + WriteCPUTime_MAX);
	
	
	// ************************** /
	// **** print a resutl for mobile ****/
	// ************************** /
	
	Result::getOutputFile() << fixed;
	Result::getOutputFile().precision(FORMAT_FLOATING_POINTS);
	
	Result::preparePrintData();

	Result::printData(_test_id);
	Result::printData(_process_name);
	Result::printData(TotalWorkloadExecutionTime);
	Result::printData(TotalWorkloadCpuTime);
	Result::printData(ReadWorkloadThroughput);
	Result::printData(ReadWorkloadLatency);
	Result::printData(WriteWorkloadThroughput);
	Result::printData(WriteWorkloadLatency);
	
	Result::printData(TotalTraffic);
	Result::printData(ReadTraffic);
	Result::printData(WriteTraffic);
	
	Result::printData(TotalCount);
	Result::printData(ReadCount);
	Result::printData(WriteCount);
	

	Result::printData(ReadWorkloadThroughput_0_5k);
	Result::printData(ReadWorkloadThroughput_1k);
	Result::printData(ReadWorkloadThroughput_2k);
	Result::printData(ReadWorkloadThroughput_4k);
	Result::printData(ReadWorkloadThroughput_8k);
	Result::printData(ReadWorkloadThroughput_16k);
	Result::printData(ReadWorkloadThroughput_32k);
	Result::printData(ReadWorkloadThroughput_64k);
	Result::printData(ReadWorkloadThroughput_128k);
	Result::printData(ReadWorkloadThroughput_256k);
	Result::printData(ReadWorkloadThroughput_512k);
	Result::printData(ReadWorkloadThroughput_MAX);

	Result::printData(ReadWorkloadLatency_0_5k);
	Result::printData(ReadWorkloadLatency_1k);
	Result::printData(ReadWorkloadLatency_2k);
	Result::printData(ReadWorkloadLatency_4k);
	Result::printData(ReadWorkloadLatency_8k);
	Result::printData(ReadWorkloadLatency_16k);
	Result::printData(ReadWorkloadLatency_32k);
	Result::printData(ReadWorkloadLatency_64k);
	Result::printData(ReadWorkloadLatency_128k);
	Result::printData(ReadWorkloadLatency_256k);
	Result::printData(ReadWorkloadLatency_512k);
	Result::printData(ReadWorkloadLatency_MAX);

	
	Result::printData(WriteWorkloadThroughput_0_5k);
	Result::printData(WriteWorkloadThroughput_1k);
	Result::printData(WriteWorkloadThroughput_2k);
	Result::printData(WriteWorkloadThroughput_4k);
	Result::printData(WriteWorkloadThroughput_8k);
	Result::printData(WriteWorkloadThroughput_16k);
	Result::printData(WriteWorkloadThroughput_32k);
	Result::printData(WriteWorkloadThroughput_64k);
	Result::printData(WriteWorkloadThroughput_128k);
	Result::printData(WriteWorkloadThroughput_256k);
	Result::printData(WriteWorkloadThroughput_512k);
	Result::printData(WriteWorkloadThroughput_MAX);

	Result::printData(WriteWorkloadLatency_0_5k);
	Result::printData(WriteWorkloadLatency_1k);
	Result::printData(WriteWorkloadLatency_2k);
	Result::printData(WriteWorkloadLatency_4k);
	Result::printData(WriteWorkloadLatency_8k);
	Result::printData(WriteWorkloadLatency_16k);
	Result::printData(WriteWorkloadLatency_32k);
	Result::printData(WriteWorkloadLatency_64k);
	Result::printData(WriteWorkloadLatency_128k);
	Result::printData(WriteWorkloadLatency_256k);
	Result::printData(WriteWorkloadLatency_512k);
	Result::printData(WriteWorkloadLatency_MAX);

	long data_left = Result::checkHowManyDataShouldBePrinted();
	if( data_left != 0 )
		assert(0);

}

void ResultForMobile::loadTargetWorkloadNameForMobile(string _configuration_file)
{
	cout << endl << "loading the name set of workload to be analyzed" << endl;
	char buf[1024];
	
	ifstream conf_file;
	conf_file.open(_configuration_file.c_str(), ios::out);
	
	
	while (!conf_file.eof()) {
		conf_file.getline(buf, 1024);
#if DEBUG_loadTargetWorkloadName
		cout << buf << endl;
#endif
		if (strlen (buf))
		{
			Node <string> *new_item = new Node <string> (buf);
			getTargetProcessName()->push_back(new_item);
		}
	}
	conf_file.close();
}

