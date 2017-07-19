/*
 * Parser.h
 *
 *  Created on: Mar 12, 2014
 *      Author: elsdy
 */

#ifndef PARSER_H_
#define PARSER_H_

#include <sys/types.h>
#include <sys/stat.h>

#include <list>
#include <assert.h>
#include <string>
#include <sstream>

#include <stdlib.h>

#include "ParseEntry.h"

#include "List.h"
#include "ProcessInfo.h"
#include "SysIOBuildRule.h"

#include "BlockLevel.h"
#include "eMMCLevel.h"

#include "Interrupt.h"
#include "IOInfo.h"

#include "Scheduler.h"

#include "Event.h"
#include "Debug.h"

#include "CPUInfo.h"
#include "ParserForFork.h"

#include <ctime>

#include "prefetcher_configuration.h"

#define PRINT_BINARY_PATH 0
#if PRINT_BINARY_PATH
#include <unistd.h>
#endif

using namespace std;

enum iotype {
	MMAP,
	HMF,
	FADVISE64,
	FADVISE64_64,
	VFSCALL, 
	OPENAT, 
	OPEN, 
	STAT,
	LSTAT,
	FSTAT,
	FORK,
	GETDENTS, 
	ACCESS, 
	FACCESSAT, 
	EXECVEAT, 
	EXECVE, 
	CREAT, 
	READLINK, 
	FSTATFS64, 
	STATFS64,
	FSTATFS, 
	STATFS, 
	TRUNCATE,
	PASS,
	NONE
};

class Parser
{
	
private:

	int analyze_mode;
#define QUIT 0
#define PROCESS_ANALYSIS 1
#define PREFETCHIGN_ANALYSIS 2


	List <ProcessInfo> *process_list;

	List <BlockInfo> *block_io_list;
	List <BlockInfo> *block_incomplete_io_list;
	List <ParseEntry> * all_block_event_list;

	List <eMMCInfo> *eMMC_io_list = new List <eMMCInfo> ();
	List <eMMCInfo> *eMMC_incomplete_io_list = new List <eMMCInfo> ();
	List <ParseEntry> * all_emmc_event_list = new List <ParseEntry> ();

	string LogVersion;
	
	string InputFileName;
	string OutputFileName;
	string InputFileDirectory;
	string OutputFileDirectory;

	string conf_file = "configuration/IO_build_rule.input";
	
	clock_t AnalysisStartTime, AnalysisFinishTime;
	long TestIdx;

	long CntMissedLogLine;
	long TotalLogLine;
	
	/// total time in a log file.
	/// calculated in input()
	double TotalLoggingTime;
	
	/// CPU info
	CPUInfo *CPU;
	
	/// core num in CPU.
	/// calculcated in input()
	int MaxCpuNum;
	
	/// Basic list structure for a log (important).
	/// allocated in constructor
	List <ParseEntry> *ParseList;
	
	/// rule list for building system call level IO.
	/// allocated in constructor
	List <Rule_SysIO> *RuleList_SysIO;
	
	/*****************/
	/** Sys IO statistics */
	/*****************/
	/// calculated in collectIORequestEventForProcesses()
	long CntTotalFileIo;
	long CntTotalFileIoWithInterrupt;
	/// calculated in collectIORequestEventForProcesses()
	long CntTotalSysIo;
	/// calculated in collectIORequestEventForProcesses()
	long CntTotalNetIo;
	long CntTotalUnknownIo;

	/************************/
	/** IO schedule event statistics */
	/************************/
	/// calculated in CntIoSchduleSequence()
	long CntIoSchduleSequence;

	/*****************/
	/** block IO statistics */
	/*****************/
	
	
	/// calculated in CntAllBlockEvent()
	long CntAllBlockEvent;
	/// the number of the Block IO with requested size Zero (statistics).
	/// calculated in checkExceptionInBlockIO()
	long CountSizeZeroBlockIO;
	/// the number of the block IO without remap request (statistics)
	/// calculated in buildAllBlockIOSequence()
	long CntBlockHasNoRemap;
	long CntBlockTransferSequence;
	long CntBlockIONoMappedToEmmcIO;
	/// calculated in findBlockIOSequence();
	long CntBlockIORequestingZeroSize;
	/// calculated in mapPackedBlockIOToEmmcIO();
	long CntBlockIOHandledInOneEmmcIO;

	/*****************/
	/** eMMC IO statistics */
	/*****************/
	/// calculated in collecteMMCEvent();
	long CntAllEmmcEvent;
	/// calculated in collecteMMCEvent();
	long CntEmmcIO;
	/// calculated in collecteMMCEvent();
	long CntIncompleteEmmcIOWithoutEMMCRequestStart;
	/// calculated in mapPackedBlockIOToEmmcIO();
	long CntPackedEmmcIO;
	/// calculated in findEmmcIOSequence();
	long EmmcIoSequenceWithNoRequestDone;
	
	/*****************/
	/** Interrupt statistics */
	/*****************/
	long CntAllInterruptEvent;
	long CntIntteruptSequence;
	
	long CntAllSoftIrqEvent;
	long CntSoftIrqSequence;
	
	/*****************/
	/** Schedule Event statistics */
	/*****************/
	long CntAllScheduleEvent;
	long CntScheduleSequence;
	long CntSchedulingStartLogMissed;
	
	/************************/
	/** CPU Execution Time Event */
	/************************/
	/// calculated in collectExecutimeTimeEvent();
	long CntCPUExecutionTimeSequence;

	ofstream prefetcher_code;
	
	void init()
	{
		analyze_mode = -1;

		LogVersion = "";
		
		InputFileName = "";
		OutputFileName = "";
		InputFileDirectory = "input/";
		OutputFileDirectory = "output/";
		
		AnalysisStartTime = 0;
		AnalysisFinishTime = 0;
		
		TestIdx = 0;
		CntMissedLogLine = 0;
		TotalLogLine = 0;
		TotalLoggingTime = 0;
		MaxCpuNum = 0;
		
		CntTotalFileIo = 0;
		CntTotalFileIoWithInterrupt = 0;
		
		CntTotalSysIo = 0;
		CntTotalNetIo = 0;
		CntTotalUnknownIo = 0;
		
		CntIoSchduleSequence = 0;
		
		CountSizeZeroBlockIO = 0;
		CntBlockHasNoRemap = 0;
		EmmcIoSequenceWithNoRequestDone = 0;
		CntAllBlockEvent = 0;
		CntBlockTransferSequence = 0;
		CntBlockIONoMappedToEmmcIO = 0;
		CntBlockIOHandledInOneEmmcIO = 0;
		CntBlockIORequestingZeroSize = 0;
		
		CntAllEmmcEvent = 0;
		CntEmmcIO = 0;
		CntIncompleteEmmcIOWithoutEMMCRequestStart = 0;
		CntPackedEmmcIO = 0;
		
		CntAllInterruptEvent = 0;
		CntIntteruptSequence = 0;
		
		CntAllSoftIrqEvent = 0;
		CntSoftIrqSequence = 0;
		
		CntAllScheduleEvent = 0;
		CntScheduleSequence = 0;
		CntSchedulingStartLogMissed = 0;
		
		CntCPUExecutionTimeSequence = 0;
	}

#pragma mark - basic methods ===
public:
#define MEASURE_PARSING_LOG 1
	Parser(string _file_to_open)
	{
		init();
		
		AnalysisStartTime = clock();
		
		InputFileName = _file_to_open;
		
		OutputFileName = InputFileName;
		OutputFileName.erase(OutputFileName.find_last_of("."));
		
		ParseList = new List <ParseEntry>;
		RuleList_SysIO = new List <Rule_SysIO>;

#if MEASURE_PARSING_LOG
		clock_t begin = clock();
		input(_file_to_open);
		clock_t end = clock();
		double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
		cout << "time for parsing a log: " << elapsed_secs << endl;

#else
		input(_file_to_open);
#endif
		
		CPU = new CPUInfo (MaxCpuNum);
		parseLog();
	}
	
	Parser()
	{
		ParseList = new List <ParseEntry>;
		RuleList_SysIO = new List <Rule_SysIO>;
		CPU = NULL;

		init();
		parseLog();
	}
	// there are two types of constructor with CPU allocated and not
	// so that destructer should be distinguish the two cases
#define PRINT_ANALYZING_TIME 1
	~Parser()
	{
		delete RuleList_SysIO;
		delete ParseList;
		
		if( CPU )
			delete CPU;

		delete process_list;

		delete block_io_list;
		delete block_incomplete_io_list;

		delete eMMC_io_list;
		delete eMMC_incomplete_io_list;

		AnalysisFinishTime = clock();
#if PRINT_ANALYZING_TIME
		cout << endl << "Analyzing Time: " << (AnalysisFinishTime - AnalysisStartTime) / CLOCKS_PER_SEC << " seconds" << endl << endl;
#endif
	}


	string getInputFileName()
	{
		return InputFileName;
	}
	void setInputFileName(string _value)
	{
		InputFileName = _value;
	}
	
	string getOutputFileName()
	{
		return OutputFileName;
	}
	void setOutputFileName(string _value)
	{
		OutputFileName = _value;
	}
	string getInputFileDirectory()
	{
		return InputFileDirectory;
	}
	void setInputFileDirectory(string _value)
	{
		InputFileDirectory = _value;
	}
	string getOutputFileDirectory()
	{
		return OutputFileDirectory;
	}
	void setOutputFileDirectory(string _value)
	{
		OutputFileDirectory = _value;
	}
	
	long getTestIdx()
	{
		return TestIdx;
	}
	void setTestIdx(long _value)
	{
		TestIdx = _value;
	}
	long getCntTotalSysIo();
	
	void setCntTotalSysIo(long _value);

	List <ParseEntry> *getParseList();
	
	List <Rule_SysIO> *getRuleList();
	
	long getCntBlockIONoMappedToEmmcIO();
	
	void setCntBlockIONoMappedToEmmcIO(long _value);
	
	long getCntCPUExecutionTimeSequence();
	
	void setCntCPUExecutionTimeSequence(long _value);
	
#pragma mark - library functions
	int is_comment_line(char * buf);
	int is_log_miss_event_line(char * buf);
	int is_cpu_wake_event_line(char *buf);
	int extractStringNextTo(string _full_str, string _search_word, string *_ret_word);
#define RET_IIR_POSITION_2_1_4_3 4
#define RET_IIR_POSITION_1_2_4_3 3
#define RET_IIR_POSITION_1_2_3_4 2
#define RET_IIR_POSITION_2_1_3_4 1
#define RET_IIR_NEVER_EXECUTE 0
#define RET_IIR_POSITION_1_4_2_3 -1
#define RET_IIR_POSITION_2_3_1_4 -2
	int isInRange(double _range_start, double _event_start, double _event_finish, double _range_finish);

#pragma mark - Input & Output
		
#define MAX_LINE 1024
#define DEBUG_input 0
	int input(string _file_to_open);
	
	void showParsingResult();
	float detectLogVersion(string _file_to_open);
	
#pragma mark - Time Approximation (approximateTime_inParseList)

 #pragma mark need to describe
	/**
	 * brief
	 * param
	 * return
	 2: success, reach the final node, which means that all interval is found
	 1: success, find a one interval
	 */
	int cutTimeInterval(  Node<ParseEntry> *_interval_start, Node<ParseEntry> *_last, Node<ParseEntry> **_interval_end);
	
	/**
	 * brief 
	 set _cpu_end's proper posigion
	 * param
	 * return
	 0:
	 1:
	 */
	int cutCPUInterval(  Node<ParseEntry> *_interval_start, Node<ParseEntry> *_interval_end
			   , Node<ParseEntry> *_cpu_start, Node<ParseEntry> **_cpu_end);
	
	/**
	 * brief
	 * param
	 * return
	 */
	int calHopCount( Node<ParseEntry> *_cpu_start, Node<ParseEntry> *_cpu_end, int *_ret);
	
	/**
	 * brief
	 * param
	 * return
	 */
	int approximateTime(  Node<ParseEntry> *_cpu_start, Node<ParseEntry> *_cpu_end, double _time_offset, long *_modified_entry);
	
	/**
	 * brief
	 * param
	 * return
	 */
	void calTimeResolution(Node <ParseEntry> *_interval_start, Node <ParseEntry> *_interval_end, double *_prev_time_resolution, double *_time_resolution);

	/**
	 * brief
	 * param
	 * return
	 */
	int approximateTime_inParseList(long *_modified_entry);
	
#pragma mark - CPU Info
	CPUInfo *getCPU();
	
	void setCPU(CPUInfo* _value);
	
	void groupCpuParseList();

#pragma mark - Process Info
	
#define DEBUG_findProcessWithName 0
#define DEBUG_findProcessWithName_TARGET_PID 10999
#define RET_FPWN_FOUND 1
#define RET_FPWN_NOT_FOUND -1
	static int findProcessWithName(const string &_process_name, List <ProcessInfo> *_process_list, ProcessInfo **_process);
	
#pragma mark - Rule & IO Info
	/**
	 * brief
	 * param
	 * return
	 */
	void generateRuleEntry( string _line, List <string> ** _rule_entry);
	
	/**
	 * brief
	 * param
	 * return
	 */
	int tokenizeRuleList(string buf, List <Rule_SysIO> * rule_list);
	
	/**
	 * brief
	 * param
	 * return
	 */
	int loadIORule(string _file);
	
	/**
	 * brief
	 * param
	 * return
	 */
	int isNetworkIO( Node <IOInfo> * _io);
	
	/**
	 * brief
	 * param
	 * return
	 */
	int isSysFsIO( Node <IOInfo> * _io);
	
	/**
	 * brief
	 * param
	 * return
	 */
	int isFileIO( Node <IOInfo> * _io);

	/**
	 * brief
	 * 
	 param
	 * return
	 */
#define RET_IFIRT_BUFFERED_IO 1
#define RET_IFIRT_PREFETCH_IO 2
#define RET_IFIRT_NEVER_REACH -1
	int identifyFIleIoReadType( Node <IOInfo> * _io);

#define RET_IFIWT_BUFFERED_IO_1 1
#define RET_IFIWT_BUFFERED_IO_2 2
#define RET_IFIWT_NEVER_REACH -1
	int identifyFIleIoWriteType( Node <IOInfo> * _io);

		
	/**
	 * brief
	 * param
	 * return
	 */
	int findIOEndFunctionName(string &_function_name, Rule_SysIO* _mapped_rule);
	
	/**
	 * brief
	 compare _fd1 with _fd2,
	 compare _size1 with _size2,
	 compare _pid1 with _pid2,
	 compare _cpuid1 with _cpuid2
	 * param
	 variables(_fd1, _fd2, _size1, _size2, _pid1, pid2, _cpuid1, _cpuid2) to be compared
	 * return
	 1: success, all pairs to be compared are the same.
	 0: failed, some are not same value.
	 */
#define RET_CIP_SAME 1
#define RET_CIP_NOT_SAME -1
	int cmpIOProperties(int _fd1, int _fd2, long _size1, long _size2, int _pid1, int _pid2);
	
#define DEBUG_findMappedIOEnd 0
#define DEBUG_findMappedIOEnd_IO_START_LINENUMBER 779621
#define RET_FMIE_FOUND_COMPOLETE_IO 1
#define RET_FMIE_NOT_FOUND_COMPOLETE_IO -1
#define RET_FMIE_NEVER_EXECUTE -2
	int findMappedIOEnd (Node <ParseEntry> *_io_start, Node <Rule_SysIO> *_mapped_rule, Node <ParseEntry> ** _ret_mapped_io_end  );
	
	int parseIOProperty(Node <ParseEntry> *_IO_start, int *_fd, long *_size, long *_pos, int *_pid, int *_cpuid);
	
	/**
	 * brief
	 * param
	 * return
	 */
	int buildIOInfoWithRule( Node <ProcessInfo> * _process );
	/**
	 * brief
	 * param
	 * return
	 */
	int categorizeIORequest(List <ProcessInfo> * _process_info)
	{
		return 1;
	}

//#define NAME_OF_FUNCTION_HAVING_FILE_NAME "generic_file"
#define NAME_OF_FUNCTION_HAVING_FILE_NAME "vfs"

	string extractFileName( Node <ParseEntry> * _io_start, Node <ParseEntry> *_io_finish)
	{
		Node <ParseEntry> *_io_log_iter = _io_start;
		string filename;
		
		while(_io_log_iter != _io_finish)
		{
			// if mode is VFS INFO, _io_start must have "file=" in the log msg.
			if( string::npos != _io_log_iter->entry->getFunctionName().find(NAME_OF_FUNCTION_HAVING_FILE_NAME))
			{
				extractStringNextTo(_io_log_iter->entry->getLogMsg(), "file=", &filename);
				break;
			}
			_io_log_iter = _io_log_iter->next;
		}
		return filename;
	}

#pragma mark - Block Info
	
	/**
	 * brief
	 identify the parameter _log as a block event
	 * param
	 _log: idenfied
	 * return
	 -1: failed
	 other values: success
	 */
	int identifyBlockLevelEvent(ParseEntry *_log);

	/**
	 * brief
	 * param
	 * return
	 */
	int collectIORequestEventForProcesses(List <ProcessInfo> * _process_info);
	
private:
	/**
	 * brief
	 * param
	 * return
	 */
#define RET_PABE_NOT_FOUND_BLOCK_EVENT -1
#define RET_PABE_FOUND_BLOCK_EVENT 1	
	int pickAllBlockEvent(List <ParseEntry>* temp_block_event_list);
	
//	jbd2/mmcblk0p23-572   [002] ...1    65.318175: block_bio_remap: 179,0 WS 3198552 + 8 <- (179,23) 20056
//	jbd2/mmcblk0p23-572   [002] ...1    65.318175: block_bio_queue: 179,0 WS 3198552 + 8 [jbd2/mmcblk0p23]

public:
	/**
	 * brief
	 extract the infos (start address, size, R_W type)
	 * param
	 _target_bio:
	 _start_addr:
	 _size:
	 _R_W:
	 * return
	 1: sucess
	 -1: failed, need to debug
	 */
#define BLOCK_READ_STRING_LABEL "b_read"
#define BLOCK_WRITE_STRING_LABEL "b_write"
	int parseBlockIOProperty( ParseEntry *_target_bio, unsigned long long *_start_addr, unsigned long long *_size, string *_R_W);
private:
	/**
	 * brief
	 judge the two block event are about the same block event (have the same start address, size, R_W type)
	 * param
	 variables: to be compared in a paired way
	 * return
	 1: success, all the paired values is same.
	 -1: failed, not all the pairs are same.
	 */
	int isSameBlockIO(unsigned long long &_start_addr, unsigned long long &_start_addr2, unsigned long long &_size, unsigned long long &_size2, string &_R_W, string &_R_W2);
	
	int isInBlockIO(unsigned long long &_start_addr, unsigned long long &_start_addr2, unsigned long long &_size, unsigned long long &_size2, string &_R_W, string &_R_W2);
	
private:
	void setBlockEventProperty(BlockInfo *new_block_info,
			      unsigned long long start_addr_bio_remap, unsigned long long size_bio_remap,
			      unsigned long long start_addr_bio_queue, unsigned long long size_bio_queue,
			      unsigned long long size_bio_midstage, unsigned long long start_addr_bio_midstage,
			      unsigned long long start_addr_bio_issue, unsigned long long size_bio_issue,
			      unsigned long long start_addr_bio_complete, unsigned long long size_bio_complete
				   );

public:

	/**
	 * brief
	 find block IO event pair
	 */
#define RET_FBIS_FOUND_BLOCKIO 1
#define RET_FBIS_FOUND_INCOMPLETE_BLOCKIO 2
#define RET_FBIS_ALREADY_COLLECTED -1
	int findBlockIOSequence( Node <ParseEntry> *_block_iter, Node <BlockInfo> ** _empty_block_node);
	
#define DEBUG_buildAllBlockIOSequence 0
//#define DEBUG_buildAllBlockIOSequence_TARGET_LINE 172430
#define DEBUG_buildAllBlockIOSequence_TARGET_LINE 47426
	void buildAllBlockIOSequence(List <ParseEntry>* _temp_block_event_list, List <BlockInfo> * _block_list, List <BlockInfo> * _incomplete_block_list);

	/**
	 * brief 
	 if the first entry in the log is incomplete, remove it from the investigation.
	 * param
	 _temp_block_event_list: block list investigated
	 * return
	 no return value, always success.
	 */
	void remove1stIncompleteBlockEvent(List <ParseEntry>* _temp_block_event_list);

public:
	/**
	 * brief
	 collect all the block event with a list
	 * param
	 all complete block IO sequence goes into _block_list
	 * return
	 1: sucess
	 -1: failed
	 */
#define RET_CBE_NO_BLOCK_EVENT -1
#define RET_CBE_BLOCK_EVENT_COLLECTED 1
	int collectBlockEvent(List <ParseEntry> * _all_block_event_list, List <BlockInfo> *_block_list, List <BlockInfo> *_incomplete_block_list);
	
	/**
	 * brief
	 * param
	 * return
	 nr
	 */
	int findPackedBlockIO(List <BlockInfo> * _block_io_list, List <eMMCInfo> * _emmc_io_list);

#pragma mark - Emmc Info
private:
	/**
	 * brief
	 identify the all emmc event
	 * param
	 * return
	 */
#define MMC_START_REQUEST_EVENT "mmc_start_request"
#define MMC_REQUEST_DONE_EVENT "mmc_request_done"
	
#define RET_IELE_MMC_START_REQUEST	1
#define RET_IELE_MMC_REQUEST_DONE	2
#define RET_IELE_NO_EMMC_EVENT -1

	int isEmmcLevelEvent(ParseEntry *_log);

#define RET_PAEE_NOT_FOUND_EMMC_EVENT -1
#define RET_PAEE_FOUND_EMMC_EVENT 1
	int pickAllEmmcEvent(List <ParseEntry>* _temp_emmc_event_list);
#define Debug_pickAllEmmcEvent 0
	
	
private:
	
	int parseEmmcIOProperty(Node <ParseEntry> *_emmc, long *_start_address, long *_block_cnt, long *_block_size, int *__cmd);
	
public:
	int isSameEmmcIO(Node <ParseEntry> *emmc_A, Node <ParseEntry> *emmc_B);
	
	/**
	 * brief
	 find emmc event node (request done) paried with _emmc_iter (request start)
	 * param
	 Node <ParseEntry> *emmc_start_request_node:
	 Node <ParseEntry> **_next_emmc_iter:
	 Node <eMMCInfo> **new_item:
	 * return
	 1: success
	 -1: failed, not found
	 */
#define RET_FEIS_FOUND_COMPLETE_EMMC_IO 1
#define RET_FEIS_NOT_FOUND_EMMC_START_REQUEST -1
#define RET_FEIS_NOT_FOUND_EMMC_REQUEST_DONE -2
#define RET_FEIS_NEVER_EXECUTE -3
	int findEmmcIOSequence(Node <ParseEntry> *_emmc_start_request_node, Node <eMMCInfo> **new_item);
	
	/**
	 * brief
	 build _emmc_list list
	 * param 
	 _emmc_list: has all emmc event paired with emmc start and emmc done
	 _temp_emmc_event_list: has all emmc event parse entry included in the log file
	 * return
	 1: success
	 -1: error
	 */
	
#define DEBUG_buildAllEmmcIOSequence 0
#define DEBUG_buildAllEmmcIOSequence_TARGET_LINE 527
	int buildAllEmmcIOSequence(List <ParseEntry>* _temp_emmc_event_list, List <eMMCInfo> * _emmc_list, List <eMMCInfo> * _incomplete_emmc_list);
	
public:
	
	void remove1stIncompleteEmmcEvent(List <ParseEntry>*  _temp_emmc_event_list);
	
#define RET_CEE_FOUND_EMMC_EVENT 1
#define RET_CEE_NOT_FOUND_EMMC_EVENT -1
	int collecteMMCEvent(List <ParseEntry> *_all_emmc_event_list, List <eMMCInfo> * _emmc_list, List <eMMCInfo> * _incomplete_emmc_list);
	
#pragma mark - Interrupt Info
	int buildInterruptEventThroughCPULog(List <ParseEntry> * _log_per_cpu, List <InterruptInfo> *_interrupt_list_per_cpu);
	
#define STR_INTERRUPT_START_FUNCTION "irq_handler_entry"
#define STR_INTERRUPT_FINISH_FUNCTION "irq_handler_exit"
#define STR_SOFTINT_START_FUNCTION "softirq_entry"
#define STR_SOFTINT_FINISH_FUNCTION "softirq_exit"
	
#define RET_IIE_IRQ_START 1
#define RET_IIE_IRQ_FINISH 2
#define RET_IIE_SOFTIRQ_START 3
#define RET_IIE_SOFTIRQ_FINISH 4
#define RET_IIE_NOT_IRQ_EVENT -1
	
	int identifyInterruptEvent(ParseEntry * _parse_entry);
	
	int collectInterruptEvent();
	
#define RET_FMIF_FOUND_IRQ_FINISH 1
#define RET_FMIF_NOT_FOUND_IRQ_FINISH -1
	int findMappedIrqFinish(Node <ParseEntry> *_log_iter, Node <ParseEntry> **_irq_end);

#define RET_FMSIF_FOUND_SOFTIRQ_FINISH 1
#define RET_FMSIF_NOT_FOUND_SOFTIRQ_FINISH -1
	int findMappedSoftIrqFinish(Node <ParseEntry> *_log_iter, Node <ParseEntry> **_soft_irq_end);
	
	int buildSoftIrqEventThroughCPULog(List <ParseEntry> * _log_per_cpu, List <SoftInterruptInfo> *_soft_irq_list_per_cpu);

#pragma mark - Schedule Info
#define STR_SCHEDULE_FUNCTION "__schedule"
#define RET_ISE_NOT_SCHEDULE_EVENT -1;
#define RET_ISE_SCHEDULE_FINISH_PROCESS_START 1
#define RET_ISE_SCHEDULE_START_PROCESS_FINISH 2
	int identifyScheduleEvent(ParseEntry *_parse_entry);
	void pickAllScheduleEvent(List <ParseEntry>* _temp_schedule_event_list);
	
#define DEBUG_collectExecutimeTimeEvent 0
#define DEBUG_collectExecutimeTimeEvent_TARGET_PID 619
	int collectExecutimeTimeEvent(List <ProcessInfo> * _process_list);

	Node <ParseEntry> * findNextScheduleFinishEventNode(Node <ParseEntry> *iter)
	{
//		iter = iter->next;
		
		while( iter )
		{
			int status = identifyScheduleEvent( iter->entry );
			
			if (status == RET_ISE_SCHEDULE_FINISH_PROCESS_START)
				return iter;
			
			iter = iter->next;
		}
		return NULL;
	}

	Node <ParseEntry> * findNextScheduleStartEventNode(Node <ParseEntry> *iter)
	{
		iter = iter->next;
		
		while( iter )
		{
			int status = identifyScheduleEvent( iter->entry );
			
			if (status == RET_ISE_SCHEDULE_START_PROCESS_FINISH)
				return iter;
			
			iter = iter->next;
		}
		return NULL;
	}
	Node <ParseEntry> * findNextScheduleEventNode(Node <ParseEntry> *iter)
	{
		iter = iter->next;
		
		while( iter )
		{
			int status = identifyScheduleEvent( iter->entry );
			
			if (status > 0)
				return iter;

			iter = iter->next;
		}
		return NULL;
	}
	void buildAllScheduleNExecutimeThroughCPULog(List <ProcessInfo> * _process_list, List <ParseEntry> * _log_per_cpu, List <ScheduleInfo> *_schedule_list_per_cpu);

	// not implemented yet, just prototype
	int findScheduleSequence(Node <ParseEntry> *_schedule_iter, Node <ScheduleInfo> ** _new_item);
		
#pragma mark - Process Info
private:
	
#define DEBUG_fillProcessName 0
#define DEBUG_fillProcessName_TARGET_PID 2053
#define RET_FPN_FOUND_PROCESS_NAME 1
#define RET_FPN_NOT_FOUND_PROCESS_NAME -1	
	int findProcessName( Node <ProcessInfo> * _process, string *name);
	
public:
#define DEBUG_groupProcessParseList 0
	int groupProcessParseList (List <ProcessInfo> *_process_info_list);
	
	int fillProcessName ( List <ProcessInfo> * _p_list);
	
	void debugParseEntryNFileIOPerProcess(List <ProcessInfo> * _process_list);
	
	
#pragma mark - Mapping Info
	
#define RET_CMBAEI_NORMAL_MAPPING 1
#define RET_CMBAEI_PACKED_CMD_MAPPING 2
#define RET_CMBAEI_NO_MAPPING -1
	int checkMappedBlockAndEmmcIO(BlockInfo *block_event, eMMCInfo *emmc_event);
	
	/**
	 * brief
	 * param
	 * return
	 */
#define RET_CEIBI_NO_EXCEPTION 			1
#define RET_CEIBI_SIZE_ZERO_BLOCK_IO 		-1
#define RET_CEIBI_INCOMPLETE_BLOCK_IO 	-2
	int checkExceptionInBlockIO( Node <BlockInfo> *_block_iter );
	
	/**
	 * brief
	 generate a map between block IO and emmc IO
	 * param
	 List <BlockInfo> *_block_list: a list having block IO event
	 List <eMMCInfo> *_eMMC_list: a list having emmc IO event
	 * return
	 0: normal, the both list (parameters) aren't filled
	 1: success, map is generated
	 */
#define DEBUG_mapBlockNeMMCEvent 0
#define DEBUG_mapBlockNeMMCEvent_TARGET_LINE 32568
	int mapBlockNeMMCEvent(List <BlockInfo> *_block_list, List <eMMCInfo> *_eMMC_list);
	BlockInfo* findBlockIOBetween(long _io_start_linenumber, long _io_finish_linenumber, List <BlockInfo> *_block_io_list);
	
	int mapSystemcallIOBlockIO(List <ProcessInfo> *_process_list, List<BlockInfo> *_block_io_list);

	int countBlockIOPackedInEmmcIO(Node <BlockInfo> * block_iter, Node <eMMCInfo> *emmc_iter);

	/**
	 * brief
	 * param
	 * return
	 return a StartFinishTimeInfo which the _target_parse_entry belongs to
	 */
	Node <StartFinishTimeInfo> * findCpuExecutionParticleInAProcess(ParseEntry *_target_parse_entry, ProcessInfo * _in_process);
	
	int calculatePureIOBlockHandlingTimeInViewsOfCPUExecutionTime(List <BlockInfo> *_block_io_list, List <ProcessInfo> *_process_list);
									      
	int isEmmcEventOccuredInBlockIO(BlockInfo *block_event, eMMCInfo *emmc_event);
	
	int mapPackedBlockIOToEmmcIO( Node <BlockInfo> * block_iter, Node <eMMCInfo> *emmc_iter );
	
#pragma mark - Pure IO Calculation
#define DEBUG_calculatePureIOHandlingTimeInViewsOfCPUExecutionTime 0
	void calculatePureIOHandlingTimeInViewsOfCPUExecutionTime(List <ProcessInfo> *_process_list);
	
#define DEBUG_calculatePureIOHandlingTimeExcludingInterrupt 0
	void calculatePureIOHandlingTimeExcludingInterrupt(List <ProcessInfo> *_process_list);
	
#define DEBUG_calculateAddIOHandlingTimeInEMMCtoPureIO 1
	void calculateAddIOHandlingTimeInEMMCtoPureIO(List <ProcessInfo> *_process_list);

#pragma mark - User Interface

	void parseLog();

	void genPrefetchingCode();

	void printTimeComposition( Node <ProcessInfo> *target_process);

#pragma mark - unorganized
	void generateTIMEComp( List <ProcessInfo> *list_process );

	void generateVFSInfo( List <ProcessInfo> *target_processes);

	void printVFSInfo();

	List <ProcessInfo> * searchProcessesWithName(List <ProcessInfo> *p_list, string pname, List <ProcessInfo> *t_list);

	Node <ParseEntry> * searchParseEntrywithTimeStamp( List <ParseEntry> *parse_list, double tp);

	void prefetchingMode( List <ProcessInfo> * p_list);
	void commandWithProcessList( List <ProcessInfo> * list_process);

	void createPrefetcherCode(string FileName, List <ProcessInfo> *target_processes);
	void writePrefetchCodeHeader();
	void generatePrefetcherCodeWithAccessedFile(List <ProcessInfo> *target_processes, map <string, string> &file_table);
	void writePrefetchCodeFooter(map <string, string> &file_table);

	void recursiveCollectPID(int parent_pid, List <ProcessInfo> *list_process, List <ProcessInfo> *target_processes );
	string correctFilePathName(string filename);

	void generatePrefetcherCodesWithOpenFiles(List <ProcessInfo> *target_processes, map <string, string> &file_table);
	void writePrefetchingCodeWithOpenedFile(ofstream &output, string filename, Node <ParseEntry> *log_iter, map <string, string> &file_table, int use_posix_fadvise);

	void prefetchOnlyAccessed(List <ProcessInfo> *target_processes, map <string, string> &file_table);
	void prefetchAllFileAccessed(List <ProcessInfo> *target_processes, map <string, string> &file_table);

	void openAndreadWholeFile(ofstream &prefetcher_code, string fd_name, size_t fd_count, string file_name, Node <ParseEntry> *log_iter, int use_posix_fadvise);

	bool isExcluded(string filename);

	iotype checkLogType(string iolog);

// 20170712
	void showMenu();
	void showProcesses(List <ProcessInfo> * list_process);
	void searchPID(List <ProcessInfo> * list_process);
	List <ProcessInfo> *setPIDtoPrefetch(List <ProcessInfo> * list_process);
	void showTargetProcesses(List<ProcessInfo> *target_processes);


};

#endif /* PARSER_H_ */
