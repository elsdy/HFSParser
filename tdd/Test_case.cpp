//
//  TDD_main.c
//  HFSParser
//
//  Created by Dongyun Shin on 3/31/14.
//  Copyright (c) 2014 elsdy. All rights reserved.
//

#include <iostream>
#include <fstream>

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

#include "Interrupt.h"
#include "Scheduler.h"

#include "ResultForFullAnalysis.h"
#include "ResultForMobile.h"

#include "Test_case.h"

#include "Debug.h"

using namespace std;

void test_macro()
{
	cout << LOG_PARSE_LIST("x") << endl;

	cout << LOG_PROCESS_PARSE_LIST("test", "test2") << endl;
	
	string test2 = "test2";
	cout << test2.c_str() << endl;	
}

#pragma mark - PLATFORM1
void test_odin_iozone1()
{
	Parser parser;
	
	parser.input("test_input/odin_iozone1.input");
	assert (parser.getParseList()->getSize() == 2670101);
	
	long modified_entry = 0;
	parser.approximateTime_inParseList(&modified_entry);
	parser.getParseList()->printList(  LOG_PARSE_LIST(PLATFORM1)  ); // debug file
	assert(modified_entry == 992);

	/// test
	assert (parser.getParseList()->getSize() == 2670101);
	
	List <ProcessInfo> *process_list = new List <ProcessInfo> ();
	parser.groupProcessParseList( process_list );
	assert (parser.getParseList()->getSize() == 2670101);
	assert(process_list->getSize() == 838);
	assert(process_list->getEntryinFirstNode()->getProcessId() == 2207);
	assert(process_list->getHead()->entry->getLogList()->getSize() == 2740);
	assert(process_list->getEntryinLastNode()->getProcessId() == 3571);
	
	
	parser.fillProcessName( process_list);
	assert (parser.getParseList()->getSize() == 2670101);
	assert(process_list->getEntryinFirstNode()->getProcessName() == "sdcard/2207");
	assert(process_list->getEntryinLastNode()->getProcessName() == "sh/3571");
	
	
	parser.loadIORule("configuration/IO_build_rule.input");
	
	/// test
	Node <Rule_SysIO> *rule1 = parser.getRuleList()->getHead();
	string ans1_1 = "sys_read_pre";
	string ans1_2 = "sys_read_post";
	string test1_1 = *(rule1->entry->getRule()->getHead()->entry);
	string test1_2 = *(rule1->entry->getRule()->getHead()->next->entry);
	assert(test1_1 == ans1_1);
	assert(test1_2 == ans1_2);
	
	/// test
	Node <Rule_SysIO> *rule8 = parser.getRuleList()->getTail();
	string ans8_1 = "sys_pwritev_pre";
	string ans8_2 = "sys_pwritev_post";
	string test8_1 = *(rule8->entry->getRule()->getHead()->entry);
	string test8_2 = *(rule8->entry->getRule()->getHead()->next->entry);
	assert(test8_1 == ans8_1);
	assert(test8_2 == ans8_2);
	
	parser.collectIORequestEventForProcesses(process_list);
	
	/// test
	assert (parser.getParseList()->getSize() == 2670101);
	
	/// test
	Node <ProcessInfo> *target_process = NULL;
	process_list->searchNodeWithKey(624, &target_process);
	assert(target_process == NULL);
	
#pragma mark 테스트중
	List <BlockInfo> *block_io_list = new List <BlockInfo> ();
	List <BlockInfo> *block_incomplete_io_list = new List <BlockInfo> ();
	List <ParseEntry> * all_emmc_event_list = new List <ParseEntry> ();

	parser.collectBlockEvent(all_emmc_event_list, block_io_list, block_incomplete_io_list);
	
	delete all_emmc_event_list;
	
	List <eMMCInfo> *eMMC_io_list = new List <eMMCInfo> ();
	List <eMMCInfo> *eMMC_incomplete_io_list = new List <eMMCInfo> ();
	List <ParseEntry> * temp_emmc_event_list = new List <ParseEntry> ();
	parser.collecteMMCEvent(temp_emmc_event_list, eMMC_io_list, eMMC_incomplete_io_list);
	
	parser.mapBlockNeMMCEvent(block_io_list, eMMC_io_list);
	
	parser.collectInterruptEvent();
	
	parser.collectExecutimeTimeEvent(process_list);
		
	delete process_list;
	delete block_io_list;
	delete block_incomplete_io_list;
	
	delete eMMC_io_list;
	delete eMMC_incomplete_io_list;

}

#pragma mark - PLATFORM2
#define DEBUG_test_nexus_iozone1 0
void test_nexus_iozone1()
{
	Parser parser("nexus_iozone1.input");
	
	long modified_entry = 0;
	parser.approximateTime_inParseList(&modified_entry);	
	assert(modified_entry == 2915331);
	parser.getParseList()->printList(  LOG_PARSE_LIST(PLATFORM2)  );

	/// test
	assert (parser.getParseList()->getSize() == 4260301);
	
	/// ParseEntry arrangement in a view of CPU
	parser.groupCpuParseList();
	assert(parser.getParseList()->getSize() == parser.getCPU()->getCntAllLog());
	
	/// ParseEntry arrangement in a view of Process
	List <ProcessInfo> *process_list = new List <ProcessInfo> ();
	parser.groupProcessParseList( process_list );
	
	// test
	assert (parser.getParseList()->getSize() == 4260301);
	assert(process_list->getSize() == 1244);
	assert(process_list->getEntryinFirstNode()->getProcessId() == 2474);
	assert(process_list->getHead()->entry->getLogList()->getSize() == 15);
	assert(process_list->getEntryinLastNode()->getProcessId() == 3219);

	
	parser.fillProcessName( process_list );
	assert (parser.getParseList()->getSize() == 4260301);
	assert(process_list->getEntryinFirstNode()->getProcessName() == "set_ftrace.sh");
	assert(process_list->getEntryinLastNode()->getProcessName() == "sh");
	
	parser.loadIORule("configuration/IO_build_rule.input");
		
	/// test
	Node <Rule_SysIO> *rule1 = parser.getRuleList()->getHead();
	string ans1_1 = "sys_read_pre";
	string ans1_2 = "sys_read_post";
	string test1_1 = *(rule1->entry->getRule()->getHead()->entry);
	string test1_2 = *(rule1->entry->getRule()->getHead()->next->entry);
	assert(test1_1 == ans1_1);
	assert(test1_2 == ans1_2);
	
	/// test
	Node <Rule_SysIO> *rule8 = parser.getRuleList()->getTail();
	string ans8_1 = "sys_pwritev_pre";
	string ans8_2 = "sys_pwritev_post";
	string test8_1 = *(rule8->entry->getRule()->getHead()->entry);
	string test8_2 = *(rule8->entry->getRule()->getHead()->next->entry);
	assert(test8_1 == ans8_1);
	assert(test8_2 == ans8_2);
	
	parser.collectIORequestEventForProcesses(process_list);
	/// test
	assert (parser.getParseList()->getSize() == 4260301);
	
	/// test
	Node <ProcessInfo> *target_process = NULL;
	process_list->searchNodeWithKey(624, &target_process);
	assert(target_process->entry->getSysfsIOList()->getSize() == 5);
	
	/// block info collected
	List <BlockInfo> *block_io_list = new List <BlockInfo> ();
	List <BlockInfo> *block_incomplete_io_list = new List <BlockInfo> ();
	
	List <ParseEntry> * all_block_event_list = new List <ParseEntry> ();
	parser.collectBlockEvent(all_block_event_list, block_io_list, block_incomplete_io_list);
	block_io_list->printList(  LOG_BLOCK_IO_LIST(PLATFORM2)  );
	assert(block_io_list->getSize() + block_incomplete_io_list->getSize() == 20835);
	delete all_block_event_list;

	/// emmc info collected
	List <eMMCInfo> *eMMC_io_list = new List <eMMCInfo> ();
	List <eMMCInfo> *eMMC_incomplete_io_list = new List <eMMCInfo> ();
	List <ParseEntry> * all_emmc_event_list = new List <ParseEntry> ();
	parser.collecteMMCEvent(all_emmc_event_list, eMMC_io_list, eMMC_incomplete_io_list);
	assert(eMMC_io_list->getSize() == 12331);
	
	all_emmc_event_list->printList( LOG_ALL_EMMC_IO_EVENT(PLATFORM2));
	eMMC_io_list->printList(  LOG_EMMC_IO_LIST(PLATFORM2)  );
	
	delete all_emmc_event_list;

	/// block <-> emmc mapping (by normal mapping)
	parser.mapBlockNeMMCEvent(block_io_list, eMMC_io_list);
	block_io_list->printList(  LOG_BLOCK_EMMC_MAPPING(PLATFORM2)  );
	
	/// block <-> emmc mapping (by packed cmd)
	parser.findPackedBlockIO(block_io_list, eMMC_io_list);
	block_io_list->printList(  LOG_BLOCK_EMMC_MAPPING_IN_PACKED_RELATION_SHIP(PLATFORM2)  );

	parser.collectExecutimeTimeEvent(process_list);

	parser.collectInterruptEvent();
	
	parser.calculatePureIOHandlingTimeInViewsOfCPUExecutionTime(process_list);
	parser.calculatePureIOHandlingTimeExcludingInterrupt(process_list);
		
	parser.showParsingResult();
	
#if DEBUG_test_nexus_iozone1
	parser.debugParseEntryNFileIOPerProcess( process_list );
#endif
	
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

	mobile_result_file.addMetaData("test_info_id");
	mobile_result_file.addMetaData("name");
	
	mobile_result_file.addMetaData("workload_execute_time");
	mobile_result_file.addMetaData("workload_cpu_time");
	mobile_result_file.addMetaData("workload_read_thput");
	mobile_result_file.addMetaData("workload_read_iops");
	mobile_result_file.addMetaData("workload_writethput");
	mobile_result_file.addMetaData("workload_writeiops");
	
	mobile_result_file.addMetaData("Totaltraffic");
	mobile_result_file.addMetaData("R_traffic");
	mobile_result_file.addMetaData("W_traffic");
	
	mobile_result_file.addMetaData("Totalcount");
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
	
	mobile_result_file.printMetaData();
	
	string process_name = "iozone";
	mobile_result_file.printProcessSummaryForMobile(process_name, process_list, parser.getTestIdx(), eMMC_io_list);
	string process_name2 = "sdcard";
	mobile_result_file.printProcessSummaryForMobile(process_name2, process_list, parser.getTestIdx(), eMMC_io_list);
		
	delete process_list;
	
	delete block_io_list;
	delete block_incomplete_io_list;
	
	delete eMMC_io_list;
	delete eMMC_incomplete_io_list;
		
}

#pragma mark -  PLATFORM3
void test_packed_command()
{
	Parser parser;
	
	parser.input("packed_command_test.input");
	
	long modified_entry = 0;
	parser.approximateTime_inParseList(&modified_entry);
//	assert(modified_entry == 2915331);
	parser.getParseList()->printList(LOG_PARSE_LIST(PLATFORM3));
		
	List <ProcessInfo> *process_list = new List <ProcessInfo> ();
	parser.groupProcessParseList( process_list );
//	assert (parser.getParseList()->getSize() == 4260301);
//	assert(process_list->getSize() == 1244);
//	assert(process_list->getEntryinFirstNode()->getProcessId() == 2474);
//	assert(process_list->getHead()->entry->getLogList()->getSize() == 15);
//	assert(process_list->getEntryinLastNode()->getProcessId() == 3219);
//	
	
	parser.fillProcessName( process_list);
//	assert (parser.getParseList()->getSize() == 4260301);
//	assert(process_list->getEntryinFirstNode()->getProcessName() == "set_ftrace.sh/2474");
//	assert(process_list->getEntryinLastNode()->getProcessName() == "sh/3219");
	
	
	parser.loadIORule("configuration/IO_build_rule.input");
	
	/// test
//	Node <Rule_SysIO> *rule1 = parser.getRuleList()->getHead();
//	string ans1_1 = "sys_read_pre";
//	string ans1_2 = "sys_read_post";
//	string test1_1 = *(rule1->entry->getRule()->getHead()->entry);
//	string test1_2 = *(rule1->entry->getRule()->getHead()->next->entry);
//	assert(test1_1 == ans1_1);
//	assert(test1_2 == ans1_2);
	
	/// test
//	Node <Rule_SysIO> *rule8 = parser.getRuleList()->getTail();
//	string ans8_1 = "sys_pwritev_pre";
//	string ans8_2 = "sys_pwritev_post";
//	string test8_1 = *(rule8->entry->getRule()->getHead()->entry);
//	string test8_2 = *(rule8->entry->getRule()->getHead()->next->entry);
//	assert(test8_1 == ans8_1);
//	assert(test8_2 == ans8_2);
	
	parser.collectIORequestEventForProcesses(process_list);
	/// test
//	assert (parser.getParseList()->getSize() == 4260301);
	
	//// test
//	Node <ProcessInfo> *target_process = NULL;
//	process_list->searchNodeWithKey(624, &target_process);
//	assert(target_process->entry->getSysfsIOList()->getSize() == 5);
	
	List <BlockInfo> *block_io_list = new List <BlockInfo> ();
	List <BlockInfo> *block_incomplete_io_list = new List <BlockInfo> ();
	List <ParseEntry> * all_block_event_list = new List <ParseEntry> ();

	parser.collectBlockEvent(all_block_event_list, block_io_list, block_incomplete_io_list);
	block_io_list->printList(  LOG_BLOCK_IO_LIST(PLATFORM3)  );
	block_incomplete_io_list->printList(  LOG_INCOMPLETE_BLOCK_IO_LIST(PLATFORM3)  );
	assert(block_io_list->getSize() == 6);
	
	delete all_block_event_list;
	
	List <eMMCInfo> *eMMC_io_list = new List <eMMCInfo> ();
	List <eMMCInfo> *eMMC_incomplete_io_list = new List <eMMCInfo> ();
	List <ParseEntry> * all_emmc_event_list = new List <ParseEntry> ();
	parser.collecteMMCEvent(all_emmc_event_list, eMMC_io_list, eMMC_incomplete_io_list);
	eMMC_io_list->printList(  LOG_EMMC_IO_LIST(PLATFORM3)  );
	all_emmc_event_list->printList( LOG_ALL_EMMC_IO_EVENT(PLATFORM3) );
	assert(eMMC_io_list->getSize() == 1);
	
	delete all_emmc_event_list;
	
#pragma mark 개발중
	/// packed command가 나오지 않는 입력 로그라서 일단 더 이상 테스트하지 않는다.
	/// 다른 로그 파일을 테스트 하는 중
	parser.mapBlockNeMMCEvent(block_io_list, eMMC_io_list);
	block_io_list->printList(  LOG_BLOCK_EMMC_MAPPING(PLATFORM3)  );
//	assert(parser.getCntBlockIONoMappedToEmmcIO() == 419);
	
	parser.findPackedBlockIO(block_io_list, eMMC_io_list);
	block_io_list->printList(  LOG_BLOCK_EMMC_MAPPING_IN_PACKED_RELATION_SHIP(PLATFORM3)  );
		
	parser.showParsingResult();
	
	delete process_list;
	delete block_io_list;
	delete block_incomplete_io_list;
	delete eMMC_io_list;
	delete eMMC_incomplete_io_list;
}

