/*
 * main.cpp
 *
 *  Created on: Mar 12, 2014
 *      Author: elsdy
 */

#include <iostream>
#include <fstream>
#include <sstream>

#include <assert.h>

// data structure
#include "List.h"
#include "Node.h"

// basic class for parsing
#include "ParseEntry.h"
#include "Parser.h"

// info for process
#include "ProcessInfo.h"

// info for IO in each level
#include "IOInfo.h"
#include "BlockLevel.h"
#include "eMMCLevel.h"
#include "ParserForFork.h"

using namespace std;

#include "Interrupt.h"
#include "Scheduler.h"

#include "string.h"

#include "ResultForFullAnalysis.h"
#include "ResultForMobile.h"
#include "ResultForProcessInfo.h"

#define DEBUG_do_analysis_for_LG_project 0
void do_analysis_for_LG_project(string _input_file)
{
	Parser parser(_input_file);
	
	long modified_entry = 0;
	parser.approximateTime_inParseList(&modified_entry);
	
	/// ParseEntry arrangement in a view of CPU
	parser.groupCpuParseList();
	
	/// ParseEntry arrangement in a view of Process
	List <ProcessInfo> *process_list = new List <ProcessInfo> ();
	parser.groupProcessParseList( process_list );
	parser.fillProcessName( process_list );

	parser.loadIORule("configuration/IO_build_rule.input");
	parser.collectIORequestEventForProcesses(process_list);

	/// block info collected
	List <BlockInfo> *block_io_list = new List <BlockInfo> ();
	List <BlockInfo> *block_incomplete_io_list = new List <BlockInfo> ();
	List <ParseEntry> * all_block_event_list = new List <ParseEntry> ();
	parser.collectBlockEvent(all_block_event_list, block_io_list, block_incomplete_io_list);
	delete all_block_event_list;
	
	/// emmc info collected
	List <eMMCInfo> *eMMC_io_list = new List <eMMCInfo> ();
	List <eMMCInfo> *eMMC_incomplete_io_list = new List <eMMCInfo> ();
	List <ParseEntry> * all_emmc_event_list = new List <ParseEntry> ();
	parser.collecteMMCEvent(all_emmc_event_list, eMMC_io_list, eMMC_incomplete_io_list);
	delete all_emmc_event_list;
	
	/// mapping
	/// block <-> emmc mapping (1. normal mapping)
	parser.mapBlockNeMMCEvent(block_io_list, eMMC_io_list);
	/// block <-> emmc mapping (2. packed cmd mapping)
	parser.findPackedBlockIO(block_io_list, eMMC_io_list);
	
	/// system call IO <-> block IO mapping
	parser.mapSystemcallIOBlockIO(process_list, block_io_list);
	
	/// block IO and eMMC IO mapping integrity check
	//	parser.checkAlleMMCIOmappedBlockIO(eMMC_io_list);
	
	/// other events collected
	/// 1. CPU execution time
	parser.collectExecutimeTimeEvent(process_list);
	/// 2. Interrupt Event
	parser.collectInterruptEvent();
		
	
	/// calulcating Pure IO Handling Time of File I/O
	parser.calculatePureIOHandlingTimeInViewsOfCPUExecutionTime(process_list);
	parser.calculatePureIOHandlingTimeExcludingInterrupt(process_list);
	
	/// calulcating Pure IO Handling Time of Block I/O
	parser.calculatePureIOBlockHandlingTimeInViewsOfCPUExecutionTime(block_io_list, process_list);
	
	parser.calculateAddIOHandlingTimeInEMMCtoPureIO(process_list);

	//	analyis is prepared well and now result will be generated
	parser.userInterface();

	parser.summary();
	
#if DEBUG_do_analysis_for_LG_project
	parser.debugParseEntryNFileIOPerProcess( process_list );
#endif
	
	ResultForProcessInfo process_result_file (parser.getOutputFileDirectory() + parser.getOutputFileName() + "_processinfo.csv");
	process_result_file.addMetaData("PrNm");
	process_result_file.addMetaData("PID");
	process_result_file.addMetaData("StartTm");
	process_result_file.addMetaData("EndTm");
	process_result_file.addMetaData("ElapsTm");
	process_result_file.addMetaData("CpuElaspTm");
	process_result_file.addMetaData("PrCpuTm");
	process_result_file.addMetaData("PrIoCpuTm");
	process_result_file.addMetaData("PrIoIrqtTm");
	process_result_file.addMetaData("PrIoSoftirqTm");
	process_result_file.addMetaData("PrPureIoTm");
	process_result_file.addMetaData("PrNonIoTm");
	process_result_file.addMetaData("OtherPrTm");
#if PRINT_ADDITIONAL_INFO
	process_result_file.addMetaData("PrCpuTm_CpuElaspTm");
	process_result_file.addMetaData("PrPureIoTm_CpuElaspTm");
	process_result_file.addMetaData("PrPureIoTm_ElapsTm");
#endif
	process_result_file.printMetaData();
	
	process_result_file.printTimeCompositionForProcess( process_list, parser.getCPU()->getMaxCpuNum() );
	
	ResultForFullAnalysis result_file (parser.getOutputFileDirectory() + parser.getOutputFileName() + ".csv");
	result_file.addMetaData("all");
	result_file.addMetaData("entry_type");
	result_file.addMetaData("linenumber");
	result_file.addMetaData("processname");
	result_file.addMetaData("origin_processname");
	result_file.addMetaData("pid");
	result_file.addMetaData("origin_pid");
	result_file.addMetaData("starttime");
	result_file.addMetaData("endtime");
	result_file.addMetaData("rw");
	result_file.addMetaData("iosize");
	result_file.addMetaData("filename");
	result_file.addMetaData("inode");
	result_file.addMetaData("mapped_syscall_io");
	result_file.addMetaData("access_address");
	result_file.addMetaData("IoRespTime");
	result_file.addMetaData("PureIoRespTime1");
	result_file.addMetaData("PureIoRespTime2");
	result_file.printMetaData();
	
	result_file.printIOFromProcess(process_list);
	result_file.printIOSchedule(block_io_list, process_list);
	result_file.printBlockIO(block_io_list, process_list);
	result_file.printEmmcIO(eMMC_io_list, process_list);
	result_file.printCpuExecutionTime(process_list);
	
	ResultForMobile mobile_result_file (parser.getOutputFileDirectory() + parser.getOutputFileName() + "_mobile" + ".csv");
	
	mobile_result_file.addMetaData("info_id");
	mobile_result_file.addMetaData("workload_name");
	
	mobile_result_file.addMetaData("execute_time");
	mobile_result_file.addMetaData("cpu_time");
	mobile_result_file.addMetaData("read_thput");
	mobile_result_file.addMetaData("read_iops");
	mobile_result_file.addMetaData("write_thput");
	mobile_result_file.addMetaData("write_iops");
	
	mobile_result_file.addMetaData("total_traffic");
	mobile_result_file.addMetaData("R_traffic");
	mobile_result_file.addMetaData("W_traffic");
	
	mobile_result_file.addMetaData("total_count");
	mobile_result_file.addMetaData("R_count");
	mobile_result_file.addMetaData("W_count");
	
	mobile_result_file.addMetaData("R_thput_0_5k");
	mobile_result_file.addMetaData("R_thput_1k");
	mobile_result_file.addMetaData("R_thput_2k");
	mobile_result_file.addMetaData("R_thput_4k");
	mobile_result_file.addMetaData("R_thput_8k");
	mobile_result_file.addMetaData("R_thput_16k");
	mobile_result_file.addMetaData("R_thput_32k");
	mobile_result_file.addMetaData("R_thput_64k");
	mobile_result_file.addMetaData("R_thput_128k");
	mobile_result_file.addMetaData("R_thput_256k");
	mobile_result_file.addMetaData("R_thput_512k");
	mobile_result_file.addMetaData("R_thput_512k_");
	
	mobile_result_file.addMetaData("R_iops_0_5k");
	mobile_result_file.addMetaData("R_iops_1k");
	mobile_result_file.addMetaData("R_iops_2k");
	mobile_result_file.addMetaData("R_iops_4k");
	mobile_result_file.addMetaData("R_iops_8k");
	mobile_result_file.addMetaData("R_iops_16k");
	mobile_result_file.addMetaData("R_iops_32k");
	mobile_result_file.addMetaData("R_iops_64k");
	mobile_result_file.addMetaData("R_iops_128k");
	mobile_result_file.addMetaData("R_iops_256k");
	mobile_result_file.addMetaData("R_iops_512k");
	mobile_result_file.addMetaData("R_iops_512k_");


	mobile_result_file.addMetaData("W_thput_0_5k");
	mobile_result_file.addMetaData("W_thput_1k");
	mobile_result_file.addMetaData("W_thput_2k");
	mobile_result_file.addMetaData("W_thput_4k");
	mobile_result_file.addMetaData("W_thput_8k");
	mobile_result_file.addMetaData("W_thput_16k");
	mobile_result_file.addMetaData("W_thput_32k");
	mobile_result_file.addMetaData("W_thput_64k");
	mobile_result_file.addMetaData("W_thput_128k");
	mobile_result_file.addMetaData("W_thput_256k");
	mobile_result_file.addMetaData("W_thput_512k");
	mobile_result_file.addMetaData("W_thput_512k_");

	
	mobile_result_file.addMetaData("W_iops_0_5k");
	mobile_result_file.addMetaData("W_iops_1k");
	mobile_result_file.addMetaData("W_iops_2k");
	mobile_result_file.addMetaData("W_iops_4k");
	mobile_result_file.addMetaData("W_iops_8k");
	mobile_result_file.addMetaData("W_iops_16k");
	mobile_result_file.addMetaData("W_iops_32k");
	mobile_result_file.addMetaData("W_iops_64k");
	mobile_result_file.addMetaData("W_iops_128k");
	mobile_result_file.addMetaData("W_iops_256k");
	mobile_result_file.addMetaData("W_iops_512k");
	mobile_result_file.addMetaData("W_iops_512k_");
	
	mobile_result_file.printMetaData();
	
	mobile_result_file.loadTargetWorkloadNameForMobile("configuration/target_workload_name.conf");
	mobile_result_file.printProcessSummaryForMobileWithWorkloadSet( mobile_result_file.getTargetProcessName(),  process_list, parser.getTestIdx(), eMMC_io_list);
	
	delete process_list;
	
	delete block_io_list;
	delete block_incomplete_io_list;
	
	delete eMMC_io_list;
	delete eMMC_incomplete_io_list;
}

#define INPUT_FILE_NAME "trace.input"

#ifdef BUILD_HFSPARSER
int main(int argc, const char * argv[]) {
	if(argc == 1)
		do_analysis_for_LG_project(INPUT_FILE_NAME);
	else
	{
		do_analysis_for_LG_project(argv[1]);
	}

	return 0;
}
#endif
