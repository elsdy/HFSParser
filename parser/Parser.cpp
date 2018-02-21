/*
 * Parser.cpp
 *
 *  Created on: Mar 12, 2014
 *      Author: elsdy ###
 */

#include "Parser.h"

List <ParseEntry> *Parser::getParseList()
{
	return ParseList;
}

List <Rule_SysIO> *Parser::getRuleList()
{
	return RuleList_SysIO;
}

long Parser::getCntBlockIONoMappedToEmmcIO()
{
	return CntBlockIONoMappedToEmmcIO;
}

void Parser::setCntBlockIONoMappedToEmmcIO(long _value)
{
	CntBlockIONoMappedToEmmcIO = _value;
}

CPUInfo *Parser::getCPU()
{
	return CPU;
}

void Parser::setCPU(CPUInfo* _value)
{
	CPU = _value;
}

long Parser::getCntTotalSysIo()
{
	return CntTotalSysIo;
}

void Parser::setCntTotalSysIo(long _value)
{
	CntTotalSysIo = _value;
}


int Parser::is_comment_line(char * buf) {
	if (buf[0] == '#' || buf[0] == '\n' || buf[0] == '\0')
		return 1;
	else
		return 0;
}

int Parser::is_log_miss_event_line(char * buf)
{
	if (buf[0] == 'C' && buf[1] == 'P' && buf[2] == 'U')
		return 1;
	else
		return 0;
}

int Parser::is_cpu_wake_event_line(char *buf)
{
	if(buf[0] == '#' && buf[1] =='#' && buf[2] == '#' && buf[3] =='#' && buf[4] =='#' && buf[5] ==' ' && buf[6] == 'C')
		return 1;
	else
		return 0;
}

long Parser::getCntCPUExecutionTimeSequence()
{
	return CntCPUExecutionTimeSequence;
}

void Parser::setCntCPUExecutionTimeSequence(long _value)
{
	CntCPUExecutionTimeSequence = _value;
}

int Parser::extractStringNextTo(string _full_str, string _search_word, string *_ret_word)
{
	string::size_type pos  = _full_str.find(_search_word);
	
	if(pos == string::npos)
		return -1;
	else
	{
		string::size_type offset = _search_word.size();

		if( _full_str[offset] == ' ' )
		{
			*_ret_word = "";
			return 1;
		}
		
		string substr = _full_str.substr(pos+offset);
		
		stringstream stream(substr);
		string token;
		
		stream >> token;
		
		*_ret_word = token;
		
		return 1;
	}
}

float Parser::detectLogVersion(string _file_to_open)
{
	float ret = -1;

	ifstream input_file;
	string full_input_file_path = InputFileDirectory + _file_to_open;
	input_file.open(full_input_file_path.c_str(), ios::in);
	
#if PRINT_BINARY_PATH
    char *path=NULL;
    size_t size;
    path=getcwd(path,size);
    cout << "current Path" << path << endl;
#endif
    
	if (!input_file.is_open()) {
		cout << "the Input file isn't in the logs/ directory." << endl;
		assert(0);
	}
	
	char buf[MAX_LINE] = { 0 };
	
	// read the first line of a log
	// verify the version of log format
	input_file.getline(buf, MAX_LINE);
	
	if(buf[0] == '#')
	{
		LogVersion = "1.0";
		cout << "Log version 1.0" << endl;
		ret = 1;
	}
	else
	{
		TestIdx = atol(buf);
		
		input_file.getline(buf, MAX_LINE);
		
		int i = 0;
		int count = 0;
		while( buf[i] != '\0')
		{
			if (buf[i] == '|')
				count++;
				
			i++;
		}
		
		if(count == 4)
		{
			LogVersion = "2.0";
			cout << "Log version 2.0" << endl;
			ret = 2;
		}
		else if(count == 3)
		{
			LogVersion = "2.1";
			cout << "Log version 2.1" << endl;
			ret = 2.1;
		}
		else{
			cout << "Unknown Log Format" << endl;
			cout << "Force to parse?" << endl;
		}
	}
	
	input_file.close();

	return ret;
}
#define PRINT_MISSED_LOG_LINE 0
#define PRINT_CntMissedLogLine 1
#define MEASURE_PARSING_A_LINE 0
int Parser::input(string _file_to_open) {
	float LogFormat = 0;

	cout << "[Starting Analysis for \"" << _file_to_open << "\"]" << endl << endl;
	cout << "Parsing the input file is in progress" << endl;
	
	LogFormat = detectLogVersion(_file_to_open);
	
	ifstream input_file;
	string full_input_file_path = InputFileDirectory + _file_to_open;
	input_file.open(full_input_file_path.c_str(), ios::in);
	
	if (!input_file.is_open()) {
		assert(0);
	}
	
	char buf[MAX_LINE] = { 0 };
	long line_number = 0;
	
	// 로그 버젼 정보는 이미 탐지 되었으므로 첫라인은 버린다.
	// LogFormat
	// -1, error
	if(LogFormat != 0)
	{
		input_file.getline(buf, MAX_LINE);
		line_number++;
	}

	while (!input_file.eof()) {
		input_file.getline(buf, MAX_LINE);
		line_number++;
		
#if DEBUG_input
		if(line_number == 1775666)
			cout << "debug to stop, line: " << buf << endl;
#endif
		
		if(is_cpu_wake_event_line(buf))
		{
			cout << endl << "WANRING: CPU Wake Up Event Occured in this Log at line number " << line_number << endl;
			cout << "buf: " << buf << endl;
			continue;
		}
		else if (is_comment_line(buf)) {
			continue;
		}
		else if(is_log_miss_event_line(buf))
		{
			string temp_buf = buf;
			string word = "";
			extractStringNextTo(temp_buf, "[LOST ", &word);
			CntMissedLogLine += atol(word.c_str());

			cout << endl << "WANRING: Log miss event occured in line " << line_number << endl;
			continue;
		}

#if MEASURE_PARSING_A_LINE
		clock_t begin = clock();
		// basic_input_line
		Node <ParseEntry> *new_node =  new Node <ParseEntry> (buf, line_number, LogVersion);
		clock_t end = clock();
		double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
		cout << "parsing time for a line in the log: " << elapsed_secs << endl;
#else
		Node <ParseEntry> *new_node =  new Node <ParseEntry> (buf, line_number, LogVersion);
#endif

		/// count the max num of CPU
		if( MaxCpuNum < new_node->entry->getCpuId() )
		{
			MaxCpuNum = new_node->entry->getCpuId();
		}
		
		ParseList->push_back(new_node);
	}
	
	
	// adjust the number of cpu, (e.g. 3->4)
	MaxCpuNum++;
	TotalLogLine = line_number;
	
	TotalLoggingTime = ParseList->getTail()->entry->getTimeStamp() - ParseList->getHead()->entry->getTimeStamp();
	
#if PRINT_CntMissedLogLine
	if (CntMissedLogLine)
	{
		cout << fixed;
		cout.precision(4);
		cout << endl << "WARNING: Count Missed Log Event / Total Log Event - " << CntMissedLogLine << " / " << TotalLogLine  << endl;
		cout << "WARNING: Log Lost Ratio (lost / remain): " << (double) (CntMissedLogLine*100)  / TotalLogLine << " %" << endl;
	}
	else
	{
		cout << endl << "NOTICE: No Count Missed Log Event" << endl;
	}
#endif
	
	input_file.close();
	
	cout << endl;
	
	return 1;
}

void Parser::showParsingResult()
{
	int sm_idx = 1;
	
	cout << endl << "Parsing and Analysis result is summarized" << endl;
	
	cout << endl << "[ Hierarchical Analyzer Start, Log version " << LogVersion << ", " << MaxCpuNum << " Cores ]" << endl;
	cout << "Input file Name: " << InputFileName << endl;
	cout << "Output file Name: " << OutputFileName << "csv" << endl;
	cout << "Log Data Period: " << ParseList->getTail()->entry->getTimeStamp() - ParseList->getHead()->entry->getTimeStamp() << " (" << ParseList->getHead()->entry->getTimeStamp() << "~" << ParseList->getTail()->entry->getTimeStamp() << ")" << endl;
	
	cout << "Total Line in the Log: " << TotalLogLine << endl;

	// SYS IO EVENT SUMMARY
	cout << endl << sm_idx++ << ". SYS IO EVENT SUMMARY" << endl;
	cout << "CntTotalFileIo: " << CntTotalFileIo << endl;
	cout << "CntTotalFileIoWithInterrupt: " << CntTotalFileIoWithInterrupt << endl;
	cout << "CntTotalSysIo: " << CntTotalSysIo << endl;
	cout << "CntTotalNetIo: " << CntTotalNetIo << endl;
	cout << "CntTotalUnknownIo: " << CntTotalUnknownIo << endl;
	
	// IO SCHEDULE EVENT SUMMARY
	cout << endl << sm_idx++ << ". IO SCHEDULE EVENT SUMMARY" << endl;
	cout << "CntIoSchduleSequence: " << CntIoSchduleSequence << endl;
	
	// BLOCK IO EVENT SUMMARY
	cout << endl << sm_idx++ << ". BLOCK IO EVENT SUMMARY" << endl;
	cout << "CntAllBlockEvent : " << CntAllBlockEvent << endl;
	cout << "CountSizeZeroBlockIO: " << CountSizeZeroBlockIO << endl;
	cout << "CntBlockHasNoRemap event: " << CntBlockHasNoRemap << endl;
	cout << "CntBlockTransferSequence: " << CntBlockTransferSequence << endl;
	cout << "CntBlockIONoMappedToEmmcIO: " << CntBlockIONoMappedToEmmcIO << endl;
	cout << "CntBlockIOHandledInOneEmmcIO: " << CntBlockIOHandledInOneEmmcIO << endl;
	
	cout << "CntBlockIORequestingZeroSize: " << CntBlockIORequestingZeroSize << endl;
	
	// EMMC IO EVENT SUMMARY
	cout << endl << sm_idx++ << ". EMMC IO EVENT SUMMARY" << endl;
	cout << "CntAllEmmcEvent : " << CntAllEmmcEvent << endl;
	cout << "CntEmmcIO: " << CntEmmcIO << endl;
	cout << "CntIncompleteEmmcIOWithoutEMMCRequestStart: " << CntIncompleteEmmcIOWithoutEMMCRequestStart << endl;
	cout << "CntPackedEmmcIO: " << CntPackedEmmcIO << endl;
	cout << "EmmcIoSequenceWithNoRequestDone: " << EmmcIoSequenceWithNoRequestDone << endl;
	
	cout << endl << sm_idx++ << ". OTHER EVENTS SUMMARY" << endl;

	// INTERRUPT EVENT SUMMARY
	cout << "CntAllInterruptEvent: " << CntAllInterruptEvent << endl;
	cout << "CntIntteruptSequence: " << CntIntteruptSequence << endl;
	
	cout << "CntAllSoftIrqEvent: " << CntAllSoftIrqEvent << endl;
	cout << "CntSoftIrqSequence: " << CntSoftIrqSequence << endl;
	
	// SCHEDULING EVENT SUMMARY
	cout << "CntAllScheduleEvent: " << CntAllScheduleEvent << endl;
	cout << "CntScheduleSequence: " << CntScheduleSequence << endl;
	cout << "CntSchedulingStartLogMissed: " << CntSchedulingStartLogMissed << endl;
	
	// PROCESS EXECUTION TIME EVENT SUMMARY
	cout << "CntCPUExecutionTimeSequence : " << CntCPUExecutionTimeSequence << endl;
	
	cout << "[ Statistics analysis Finishes ]" << endl << endl;;
}


int Parser::cutTimeInterval(  Node<ParseEntry> *_interval_start, Node<ParseEntry> *_last, Node<ParseEntry> **_interval_end)
{
	Node <ParseEntry> *cur = _interval_start;
	
	while ( cur != NULL)
	{
		if(  _interval_start->entry->getTimeStamp() != cur->entry->getTimeStamp() )
		{
			*_interval_end = cur->prev;
			return 1;
		}
		
		cur = cur->next;
	}
	
	*_interval_end = _last;
	return 0;
}

int Parser::cutCPUInterval(  Node<ParseEntry> *_interval_start, Node<ParseEntry> *_interval_end
		   , Node<ParseEntry> *_cpu_start, Node<ParseEntry> **_cpu_end)
{
	Node<ParseEntry> *cur = _cpu_start;
	while(1)
	{
		if(cur == _interval_end)
		{
			*_cpu_end = cur;
			return 0;
		}
		
		else
		{
			if( cur->entry->getCpuId() != cur->next->entry->getCpuId() )
			{
				*_cpu_end = cur;
				return 1;
			}
		}
		cur = cur->next;
	}
	return 1;
}

int Parser::calHopCount( Node<ParseEntry> *_cpu_start, Node<ParseEntry> *_cpu_end, int *_ret)
{
	Node <ParseEntry> *cur = _cpu_start;
	int count = 1;
	
	while(  cur != _cpu_end)
	{
		cur = cur->next;
		count++;
	}
	*_ret = count;
	
	return 1;
}


int Parser::approximateTime(  Node<ParseEntry> *_cpu_start, Node<ParseEntry> *_cpu_end, double _time_offset, long *num_modified_entry)
{
	Node <ParseEntry> *cur = _cpu_start;
	double base_timestamp = cur->entry->getTimeStamp();
	int i = 0;
	
	while(1)
	{
		double gap = base_timestamp + i * _time_offset;
		cur->entry->setApproxTimeStamp( gap );
		if(cur->entry->getTimeStamp() != cur->entry->getApproxTimeStamp())
		{
			//				cout.precision(9);
			//				cout << cur->entry->getTimeStamp() << " -> " << cur->entry->getApproxTimeStamp() << endl;
			(*num_modified_entry)++;
		}
		if (cur == _cpu_end)
		{
			break;
		}
		cur = cur->next;
		i++;
	}
	return 1;
}

int Parser::approximateTime_inParseList(long *num_modified_entry)
{
	cout << endl << "Approximating time due to the limited timer resulution is in progress" << endl;
	
	Node<ParseEntry> *list_end = ParseList->getTail();
	
	int status = 0;
	
	double prev_time_resolution = 0;
	
	*num_modified_entry = 0;
	
	// loop init
	Node<ParseEntry> *interval_start = ParseList->getHead();
	Node<ParseEntry> *interval_end = NULL;
	while(1)
	{
		status = cutTimeInterval(interval_start, list_end, &interval_end);
		if( status < 0)
		{
			assert(0);
		}
		
		Node <ParseEntry> *cpu_start = NULL, *cpu_end = NULL;
		
		cpu_start = interval_start;
		while(1)
		{
			status = cutCPUInterval(interval_start, interval_end, cpu_start, &cpu_end);
			if ( status < 0 )
			{
				assert(0);
			}
			
			
			int hop = 0;
			status = calHopCount(cpu_start, cpu_end, &hop);
			if ( status < 0)
			{
				assert(0);
			}
			
			double time_resolution = 0;
			calTimeResolution(interval_start, interval_end, &prev_time_resolution, &time_resolution);
			
			double time_offset = time_resolution  / hop;
			long modified_entry = 0;
			
			status  = approximateTime(cpu_start, cpu_end, time_offset, &modified_entry);
			if (status < 0 )
			{
				assert(0);
			}
			*num_modified_entry += modified_entry;
			
			if(cpu_end == interval_end)
			{
				break;
			}
			cpu_start = cpu_end->next;
		}
		if(interval_end == list_end)
		{
			assert ( * num_modified_entry <= ParseList->getSize() );
	
			return 0;
		}
		interval_start = interval_end->next;
	}
}


void Parser::calTimeResolution(Node <ParseEntry> *_interval_start, Node <ParseEntry> *_interval_end, double *_prev_time_resolution, double *_time_resolution)
{
	if(_interval_end->next != NULL)
	{
		*_time_resolution = *_prev_time_resolution = _interval_end->next->entry->getTimeStamp() - _interval_start->entry->getTimeStamp();
	}
	else
	{
		*_time_resolution = *_prev_time_resolution;
	}
	
	
}

void Parser::buildAllBlockIOSequence(List <ParseEntry>* _temp_block_event_list, List <BlockInfo> * _block_list, List <BlockInfo> * _incomplete_block_list)
{
	int status = 0;
	
	Node <ParseEntry> *block_iter = _temp_block_event_list->getHead();
	while (block_iter)
	{
		int block_event = -1;
		block_event = identifyBlockLevelEvent( block_iter->entry );
		
		if(block_event == BLOCK_BIO_REMAP || block_event == BLOCK_BIO_QUEUE)
		{
			Node <BlockInfo> *new_item = NULL;
			status = findBlockIOSequence(block_iter, &new_item);
			if(status == RET_FBIS_ALREADY_COLLECTED)
			{
				block_iter = block_iter->next;
				continue;
			}
			else if(status == RET_FBIS_FOUND_INCOMPLETE_BLOCKIO)
			{
				_incomplete_block_list->push_back(new_item);
				block_iter = block_iter->next;
				continue;
			}
			
			_block_list->push_back(new_item);
			
			block_iter = block_iter->next;
			continue;
		}
		block_iter = block_iter->next;
	}
	
}

int Parser::findBlockIOSequence( Node <ParseEntry> *_block_iter, Node <BlockInfo> ** _empty_block_node)
{
	int status = 0;
	int block_event = -1;
	
	Node <ParseEntry> *bio = _block_iter;
	
// remap:
	unsigned long long start_addr_bio_remap = 0, size_bio_remap = 0;
	
	string R_W_bio_remap;
	ParseEntry *bio_remap = NULL;
	block_event = identifyBlockLevelEvent(bio->entry);
	
	if( block_event == BLOCK_BIO_REMAP)
	{
		if(bio->entry->getisCollectedInWhichList().compareValue(BLOCK_IO))
			return RET_FBIS_ALREADY_COLLECTED;
		
		status = parseBlockIOProperty( bio->entry, &start_addr_bio_remap, &size_bio_remap, &R_W_bio_remap);
		if( status < 0)
		{
			assert(0);
		}
		
		// keep remap data
		bio_remap = bio->entry;
		
		bio = bio->next;
	}
	
// queue:
	unsigned long long start_addr_bio_queue= 0, size_bio_queue = 0;
	string R_W_bio_queue;
	ParseEntry *bio_queue = NULL;
	while(bio)
	{
		if(bio->entry->getisCollectedInWhichList().compareValue(BLOCK_IO))
			return RET_FBIS_ALREADY_COLLECTED;
		
		block_event = identifyBlockLevelEvent(bio->entry);
		if ( block_event == BLOCK_BIO_QUEUE )
		{
			status = parseBlockIOProperty(bio->entry, &start_addr_bio_queue, &size_bio_queue, &R_W_bio_queue);
			if( status < 0)
			{
				assert(0);
			}
			
			// exception
			if( bio_remap == NULL)
			{
				bio_queue = bio->entry;
				bio = bio->next;
				
				CntBlockHasNoRemap++;
				break;
			}
			
			status = isSameBlockIO(start_addr_bio_remap, start_addr_bio_queue, size_bio_remap, size_bio_queue, R_W_bio_remap, R_W_bio_queue);
			if( status < 0)
			{
				bio = bio->next;
				continue;
			}
			
			// keep queue data
			bio_queue = bio->entry;
			bio = bio->next;
			
			break;
		}
		bio = bio->next;
	}
	
// getrq_or_merge:
	// remap -> queue -> getrq
	// queue -> getrq
	// queue
	unsigned long long start_addr_bio_midstage= 0, size_bio_midstage = 0;
	string R_W_bio_midstage;
	ParseEntry *bio_getrq = NULL, *bio_backmerge = NULL, *bio_frontmerge = NULL;
	while(bio)
	{
		block_event = identifyBlockLevelEvent(bio->entry);
		if ( block_event == BLOCK_GETRQ )
		{
			status = parseBlockIOProperty(bio->entry, &start_addr_bio_midstage, &size_bio_midstage, &R_W_bio_midstage);
			if(status <0)
			{
				assert(0);
			}
			bio_getrq = bio->entry;
			
			bio = bio->next;
			break;
		}
		else if ( block_event == BLOCK_BIO_BACKMERGE)
		{
			status = parseBlockIOProperty(bio->entry, &start_addr_bio_midstage, &size_bio_midstage, &R_W_bio_midstage);
			if(status < 0)
			{
				assert(0);
			}
			status = isSameBlockIO(start_addr_bio_queue, start_addr_bio_midstage, size_bio_queue, size_bio_midstage, R_W_bio_queue, R_W_bio_midstage);
			if(status < 0)
			{
				bio = bio->next;
				continue;
			}
			bio_backmerge = bio->entry;
			
			bio = bio->next;
			break;
		}
		else if ( block_event == BLOCK_BIO_FRONTMERGE )
		{
			status = parseBlockIOProperty(bio->entry, &start_addr_bio_midstage, &size_bio_midstage, &R_W_bio_midstage);
			if(status < 0)
			{
				assert(0);
			}
			status = isSameBlockIO(start_addr_bio_queue, start_addr_bio_midstage, size_bio_queue, size_bio_midstage, R_W_bio_queue, R_W_bio_midstage);
			if(status < 0)
			{
				bio = bio->next;
				continue;
			}
			bio_frontmerge = bio->entry;
			
			bio = bio->next;
			break;
		}
		else
		{
			goto stage_issue;
		}
//		bio = bio->next; // never reaced
	}
	
stage_issue:
	unsigned long long start_addr_bio_issue= 0, size_bio_issue = 0;
	string R_W_bio_issue;
	ParseEntry *bio_issue = NULL;
	while(bio != NULL)
	{
		block_event = identifyBlockLevelEvent(bio->entry);
		if(block_event == BLOCK_RQ_ISSUE)
		{
			status = parseBlockIOProperty(bio->entry, &start_addr_bio_issue, &size_bio_issue, &R_W_bio_issue);
			if(status < 0)
			{
				assert(0);
			}
			status = isInBlockIO(start_addr_bio_queue, start_addr_bio_issue, size_bio_queue, size_bio_issue, R_W_bio_queue, R_W_bio_issue);
			if(status < 0)
			{
				bio = bio->next;
				continue;
			}
			bio_issue = bio->entry;
			
			bio = bio->next;
			break;
		}
		
		bio = bio->next;
		
	}
// stage_complete:
	unsigned long long start_addr_bio_complete= 0, size_bio_complete = 0;
	string R_W_bio_complete;
	ParseEntry *bio_complete = NULL;
	while(bio)
	{
		block_event = identifyBlockLevelEvent(bio->entry);
		if(block_event == BLOCK_RQ_COMPLETE)
		{
			status = parseBlockIOProperty(bio->entry, &start_addr_bio_complete, &size_bio_complete, &R_W_bio_complete);
			if(status < 0)
			{
				assert(0);
			}
			status = isSameBlockIO(start_addr_bio_issue, start_addr_bio_complete, size_bio_issue, size_bio_complete, R_W_bio_issue, R_W_bio_complete);
			if(status < 0)
			{
				bio = bio->next;
				continue;
			}
			bio_complete = bio->entry;
			bio = bio->next;
			break;
		}
		
		bio = bio->next;
	}
	
	
// generate_block_event:
	Node <BlockInfo> *new_block_info = new Node <BlockInfo> ();
	status = new_block_info->entry->setBlockEventParseEntry(bio_remap, bio_queue, bio_getrq, bio_backmerge, bio_frontmerge, bio_issue, bio_complete);
	if(status < 0)
	{
		assert(0);
	}
		
	setBlockEventProperty(new_block_info->entry,
			      start_addr_bio_remap, size_bio_remap,
			      start_addr_bio_queue, size_bio_queue,
			      start_addr_bio_midstage, start_addr_bio_midstage,
			      start_addr_bio_issue, size_bio_issue,
			      start_addr_bio_complete, size_bio_complete
			      );
	if(size_bio_complete == 0)
		CntBlockIORequestingZeroSize++;
	
	*_empty_block_node = new_block_info;
	
	if(new_block_info->entry->getBlock_rq_complete() == NULL)
	{
		return RET_FBIS_FOUND_INCOMPLETE_BLOCKIO;
	}
	
	return RET_FBIS_FOUND_BLOCKIO;
}

/**
 * brief
 해당 블록 I/O (B_IO)로부터 시작하여 몇개의 블록 I/O가 합쳐져서 
 E_IO에 의해 packed 되었는 지 알아봄
 
 맵핑 관계 형성
 
 * param
 파라미터로 넘겨 받은 블록 I/O 정보 (B_IO)
 파라미터로 넘겨 받은 eMMC I/O 정보 (E_IO)
 * return
 */
int Parser::countBlockIOPackedInEmmcIO(Node <BlockInfo> * block_iter, Node <eMMCInfo> *emmc_iter)
{
	return 1;
}

/**
 * brief
 실제로 다수의 블록 I/O와 하나의 eMMC I/O를 연결
 * param
 * return
 */
int Parser::mapPackedBlockIOToEmmcIO( Node <BlockInfo> * block_iter, Node <eMMCInfo> *emmc_iter )
{
	long packed_size_in_emmc_io = emmc_iter->entry->getCntBlocks();
	long size_block_io = 0;
	
	while(block_iter)
	{
		CntBlockIOHandledInOneEmmcIO++;
//		CntBlockIONoMappedToEmmcIO--;
		
		// 1:1 mapping views of block io
		block_iter->entry->setMappedEmmc(emmc_iter->entry);
		// 1:n mapping
		emmc_iter->entry->pushMappedBlockInfo(block_iter->entry);
		
		size_block_io = block_iter->entry->getSize_RqIssue();
		packed_size_in_emmc_io -= size_block_io;
		
		if(packed_size_in_emmc_io == 1)
		{
			CntPackedEmmcIO++;

			break;
		}
		
		block_iter = block_iter->next;
	}
	
	return 1;
}

int Parser::findPackedBlockIO(List <BlockInfo> * _block_io_list, List <eMMCInfo> * _emmc_io_list)
{
	cout << endl << "Finding Packed eMMC IOs is in progress" << endl;
	
	int status = 0;
	int CntPackedBlockIOFound = 0;
	
	Node <BlockInfo> * block_iter = _block_io_list->getHead();
	
	while(block_iter)
	{
		Node <eMMCInfo> *emmc_iter = _emmc_io_list->getHead();
		
		while(emmc_iter)
		{
			status = checkMappedBlockAndEmmcIO(block_iter->entry, emmc_iter->entry);
			
			if(status == RET_CMBAEI_PACKED_CMD_MAPPING)
			{
				mapPackedBlockIOToEmmcIO(block_iter, emmc_iter);
			}
			
			emmc_iter = emmc_iter->next;
		}
		
		block_iter = block_iter->next;
	}
		
	return CntPackedBlockIOFound;
}

Node <StartFinishTimeInfo> * Parser::findCpuExecutionParticleInAProcess(ParseEntry *_target_parse_entry, ProcessInfo * _in_process)
{
	Node <StartFinishTimeInfo> * cpu_execution_time_iter = _in_process->getCpuExecutionTime()->getHead();
	
	while(cpu_execution_time_iter)
	{
		if(cpu_execution_time_iter->entry->getStart()->getLineNumber() <= _target_parse_entry->getLineNumber() && _target_parse_entry->getLineNumber() <= cpu_execution_time_iter->entry->getFinish()->getLineNumber())
		{
			return cpu_execution_time_iter;
		}
		cpu_execution_time_iter = cpu_execution_time_iter->next;
	}
	
	return NULL;
}

#define DEBUG_calculatePureIOBlockHandlingTimeInViewsOfCPUExecutionTime 0
int Parser::calculatePureIOBlockHandlingTimeInViewsOfCPUExecutionTime(List <BlockInfo> *_block_io_list, List <ProcessInfo> *_process_list)
{
	cout << endl << "started calculatePureIOBlockHandlingTimeInViewsOfCPUExecutionTime" << endl;

	Node <BlockInfo> *block_io_iter = _block_io_list->getHead();
	while (block_io_iter)
	{
		
		// calculating between time
		// remap ~ issue or queue ~ issue
		if( block_io_iter->entry->getBlock_bio_remap() )
			block_io_iter->entry->setTimeFromRemapToIssue(block_io_iter->entry->getBlock_rq_issue()->getApproxTimeStamp() - block_io_iter->entry->getBlock_bio_remap()->getApproxTimeStamp());
		else if ( block_io_iter->entry->getBlock_bio_queue() )
			block_io_iter->entry->setTimeFromRemapToIssue(block_io_iter->entry->getBlock_rq_issue()->getApproxTimeStamp() - block_io_iter->entry->getBlock_bio_queue()->getApproxTimeStamp());
		else
		{
			assert(0);
		}
		// issue ~ complete
		block_io_iter->entry->setTimeFromIssueToComplete(block_io_iter->entry->getBlock_rq_complete()->getApproxTimeStamp() - block_io_iter->entry->getBlock_rq_issue()->getApproxTimeStamp());

#if DEBUG_calculatePureIOBlockHandlingTimeInViewsOfCPUExecutionTime
		cout << endl;
		block_io_iter->entry->print();
#endif
		
		/// pure handling time calculated
		// for block io issue
		Node <ProcessInfo> *process = NULL;
		_process_list->searchNodeWithKey(block_io_iter->entry->getBlock_rq_issue()->getProcessID(), &process);
		if(process == NULL)
		{
			assert(0);
		}

		Node <StartFinishTimeInfo> * block_issue_cpu_particle = findCpuExecutionParticleInAProcess(block_io_iter->entry->getBlock_rq_issue(), process->entry);
		
		// for block io complete
		_process_list->searchNodeWithKey(block_io_iter->entry->getBlock_rq_complete()->getProcessID(), &process);
		if(process == NULL)
		{
			assert(0);
		}
		
		Node <StartFinishTimeInfo> * block_complete_cpu_particle = findCpuExecutionParticleInAProcess(block_io_iter->entry->getBlock_rq_complete(), process->entry);
#if DEBUG_calculatePureIOBlockHandlingTimeInViewsOfCPUExecutionTime
		cout << endl;
		block_issue_cpu_particle->printFromSelfToNode(block_complete_cpu_particle);
#endif
		// calculating between time
		// remap ~ issue or queue ~ issue
		if( block_io_iter->entry->getBlock_bio_remap() )
			block_io_iter->entry->setPureTimeFromRemapToIssue(block_io_iter->entry->getBlock_rq_issue()->getApproxTimeStamp() - block_io_iter->entry->getBlock_bio_remap()->getApproxTimeStamp());
		else if ( block_io_iter->entry->getBlock_bio_queue() )
			block_io_iter->entry->setPureTimeFromRemapToIssue(block_io_iter->entry->getBlock_rq_issue()->getApproxTimeStamp() - block_io_iter->entry->getBlock_bio_queue()->getApproxTimeStamp());
		else
		{
			assert(0);
		}
		
		block_io_iter->entry->setPureTimeFromIssueToComplete(block_issue_cpu_particle->entry->getFinish()->getApproxTimeStamp() - block_io_iter->entry->getBlock_rq_issue()->getApproxTimeStamp() + block_io_iter->entry->getBlock_rq_complete()->getApproxTimeStamp() - block_complete_cpu_particle->entry->getStart()->getApproxTimeStamp());
		
#if DEBUG_calculatePureIOBlockHandlingTimeInViewsOfCPUExecutionTime
		block_issue_cpu_particle->printFromSelfToNode(block_complete_cpu_particle);
						
		cout << "TimeFromRemapToIssue: " << block_io_iter->entry->getTimeFromRemapToIssue() << endl;
		cout << "PureTimeFromRemapToIssue: " << block_io_iter->entry->getPureTimeFromRemapToIssue() << endl;
		cout << "TimeFromIssueToComplete: " << block_io_iter->entry->getTimeFromIssueToComplete() << endl;
		cout << "PureTimeFromIssueToComplete: " << block_io_iter->entry->getPureTimeFromIssueToComplete() << endl;
		
		cout << "In Queue Time ( Pure / Normal ): " << (double) (100 * block_io_iter->entry->getPureTimeFromRemapToIssue() / block_io_iter->entry->getTimeFromRemapToIssue() ) << " %" <<  endl;
		cout << "In Transfer Time ( Pure / Normal ): " << (double) (100 * block_io_iter->entry->getPureTimeFromIssueToComplete() / block_io_iter->entry->getTimeFromIssueToComplete() ) <<  " %" << endl;
#endif
		
		block_io_iter = block_io_iter->next;
	}
	return 1;
}

#define RET_IEEOIBI_OCCUR_IN 1
#define RET_IEEOIBI_OCCUR_OUT -1;
int Parser::isEmmcEventOccuredInBlockIO(BlockInfo *block_event, eMMCInfo *emmc_event)
{
	// Line Number check
	if( block_event->getBlock_rq_issue()->getLineNumber() < emmc_event->getStart()->getLineNumber()
	   &&
	   emmc_event->getFinish()->getLineNumber() < block_event->getBlock_rq_complete()->getLineNumber() )
		return RET_IEEOIBI_OCCUR_IN;
	else
		return RET_IEEOIBI_OCCUR_OUT;
	
}

int Parser::checkMappedBlockAndEmmcIO(BlockInfo *block_event, eMMCInfo *emmc_event)
{
		
	if( isEmmcEventOccuredInBlockIO(block_event, emmc_event)  )
	{
		// start addr & size check
		if( block_event->getStartAddr_RqComplete() == emmc_event->getStartAddress() )
		{
			if( block_event->getSize_RqComplete() == emmc_event->getCntBlocks() )
				return RET_CMBAEI_NORMAL_MAPPING;
			else
				return RET_CMBAEI_PACKED_CMD_MAPPING;
		}
		else
		{
			return RET_CMBAEI_NO_MAPPING;
		}
	}
	else
	{
		return RET_CMBAEI_NO_MAPPING;
	}
}

int Parser::identifyScheduleEvent(ParseEntry *_parse_entry)
{
	if(_parse_entry->getFunctionName() == STR_SCHEDULE_FUNCTION)
	{
		if(_parse_entry->getLogMsg() == "" || _parse_entry->getLogMsg() == " " )
			return RET_ISE_SCHEDULE_FINISH_PROCESS_START;
		else
			return RET_ISE_SCHEDULE_START_PROCESS_FINISH;
	}
	else
	{
		return RET_ISE_NOT_SCHEDULE_EVENT;
	}
}

void Parser::pickAllScheduleEvent(List <ParseEntry>* _temp_schedule_event_list)
{
	Node <ParseEntry> * log_iter = ParseList->getHead();
	
	while( log_iter != NULL)
	{
		if( identifyScheduleEvent(log_iter->entry) > 0 )
		{
			Node <ParseEntry> *linked_node = new Node<ParseEntry> (log_iter->entry);
			_temp_schedule_event_list->push_back(linked_node);
		}
		
		log_iter = log_iter->next;
	}
}

int Parser::findScheduleSequence(Node <ParseEntry> *_schedule_iter, Node <ScheduleInfo> ** _new_item)
{
	return 1;
}

/**
 * brief
 스케쥴링 이벤트를 수집한다.
 * param
 * return
 */
int Parser::collectExecutimeTimeEvent(List <ProcessInfo> * _process_list)
{
	cout << endl << "Collecting CPU execution time event for each process is in progress" << endl;
	int status = 0;
	
	// 일단 순회하면서
	Node <ProcessInfo> *process_iter = _process_list->getHead();
	
	/// 프로세스 별로 순회
	while (process_iter)
	{
#if DEBUG_collectExecutimeTimeEvent
		if( process_iter->entry->getProcessId() == DEBUG_collectExecutimeTimeEvent_TARGET_PID)
			cout << "[DEBUG_collectExecutimeTimeEvent_TARGET_PID] target process id: " << process_iter->entry->getProcessId() << endl;
		
		cout << endl << "process name: " << process_iter->entry->getProcessName() << "(" << process_iter->entry->getProcessId() << ")" << endl;
		cout << "process log's size: " << process_iter->entry->getLogList()->getSize() << endl;
#endif
		Node <ParseEntry> *log_iter = process_iter->entry->getLogList()->getHead();
		
		/// 예외 처리 (프로세스의 로그 크기가 1이하인 경우)
		// 프로세스의 로그가 없는 경우 (0)
		// 구간 설정이 안되서 수집을 할 수 없는 경우 (1)
		// 다음 프로세스로 진행, 이런 경우는 아예 프로세스 정보가 수집되지 않을 텐데, assert로 테스트 바람
		if (process_iter->entry->getLogList()->getSize() <= 1)
		{
#if DEBUG_collectExecutimeTimeEvent
			cout << "log size is too mall so that go to next process" << endl;
#endif
			process_iter = process_iter->next;
			
			continue;
		}
#if DEBUG_collectExecutimeTimeEvent
		cout << "process execution time analysis start" << endl;
#endif
		
		status = identifyScheduleEvent(log_iter->entry);
		
		/// 전처리가 필요한 지 판단
		if(status < 0 )
		{
#if DEBUG_collectExecutimeTimeEvent
			/// 전처리가 필요한 경우
			cout << "first node isn't a schedule event" << endl;
			log_iter->entry->print();
			cout << "start finding next schedule event node" << endl;
#endif
			
			log_iter = findNextScheduleEventNode( log_iter );
			if(log_iter == NULL)
			{
#if DEBUG_collectExecutimeTimeEvent
				cout << "process's log dumped" << endl;
				process_iter->entry->getLogList()->printList();
#endif
				process_iter = process_iter->next;
				continue;
			}
			status = identifyScheduleEvent( log_iter->entry );
			
#if DEBUG_collectExecutimeTimeEvent
			cout << "found next schedule event: ";
			log_iter->entry->print();
#endif

			if( status == RET_ISE_SCHEDULE_FINISH_PROCESS_START ) // 발생하지 않을 것으로 예상했으나 발생함
			{
				Node <StartFinishTimeInfo> *new_item = new Node <StartFinishTimeInfo> (process_iter->entry->getLogList()->getHead(), log_iter->prev);
				process_iter->entry->getCpuExecutionTime()->push_back(new_item);
				process_iter->entry->addTotalCpuExecutionTime(new_item->entry->getElapsedTime());
				
#if 1
				cout << endl << "interrupt during scheduling" << endl;
				cout << "cpu execution time period: " << endl;
				cout << "start: ";
				process_iter->entry->getLogList()->getHead()->entry->print();
				cout << "finish: ";
				log_iter->prev->entry->print();
#endif
			}
			else if (status == RET_ISE_SCHEDULE_START_PROCESS_FINISH ) // 실행 시간 이벤트
			{
				Node <StartFinishTimeInfo> *new_item = new Node <StartFinishTimeInfo> (process_iter->entry->getLogList()->getHead(), log_iter);
				process_iter->entry->getCpuExecutionTime()->push_back(new_item);
				process_iter->entry->addTotalCpuExecutionTime(new_item->entry->getElapsedTime());
				
#if DEBUG_collectExecutimeTimeEvent
				cout << endl << "cpu execution time period: " << endl;
				cout << "start: ";
				process_iter->entry->getLogList()->getHead()->entry->print();
				cout << "finish: ";
				log_iter->entry->print();
#endif
			}
		}

		/// 전처리가 필요없는 경우
		// 시작부터 스케쥴 이벤트가 나오는 경우
		// 첫 실행 시간 이벤트를 찾는다.
		while(log_iter)
		{
			// start와 finish중에 하나의 이벤트를 못 찾은 경우
			Node <ParseEntry> *cpu_execution_start = findNextScheduleFinishEventNode(log_iter);
			if(cpu_execution_start == NULL)
			{
				// 더이상 cpu를 사용하지 않는 경우
				// 더이상 나오지 않는 경우
#if DEBUG_collectExecutimeTimeEvent
				cout << "cpu_execution_start isn't found" << endl;
#endif
				break;
			}
			Node <ParseEntry> *cpu_execution_finish = findNextScheduleStartEventNode(cpu_execution_start);
			if(cpu_execution_finish == NULL)
			{
				// 로그끝이 잘려서 더이상 찾을 수 없는 경우
				// 마지막 불완전한 한조각을 넣어야 한다.
				Node <StartFinishTimeInfo> *new_item = new Node <StartFinishTimeInfo> (cpu_execution_start, process_iter->entry->getLogList()->getTail());
				process_iter->entry->getCpuExecutionTime()->push_back(new_item);
				process_iter->entry->addTotalCpuExecutionTime(new_item->entry->getElapsedTime());
#if DEBUG_collectExecutimeTimeEvent
				cout << "cpu_execution_finish isn't found" << endl;
#endif
				break;
			}
			
			Node <StartFinishTimeInfo> *new_item = new Node <StartFinishTimeInfo> (cpu_execution_start, cpu_execution_finish);
			
			process_iter->entry->getCpuExecutionTime()->push_back(new_item);
			process_iter->entry->addTotalCpuExecutionTime(new_item->entry->getElapsedTime());
			
#if DEBUG_collectExecutimeTimeEvent
			cout << endl << "cpu execution time period: " << endl;
			cout << "start: ";
			cpu_execution_start->entry->print();
			cout << "finish: ";
			cpu_execution_finish->entry->print();
#endif
			log_iter = cpu_execution_finish;
		}

#if DEBUG_collectExecutimeTimeEvent
		cout << "collecting CPU execution time event for the process finished" << endl;
#endif
		CntCPUExecutionTimeSequence += process_iter->entry->getCpuExecutionTime()->getSize();

		process_iter = process_iter->next;
	}
#if 0
	double log_time = 0;

	// test
	Node <ProcessInfo> *process_iter = _process_list->getHead();
	// traverse processes in a process list
	while(process_iter)
	{
		Node <StartFinishTimeInfo> *time_iter = process_iter->entry->getCpuExecutionTime()->getHead();
		
		// traverse time info in a process
		while( time_iter)
		{
			log_time += time_iter->entry->getElapsedTime();
			
			time_iter = time_iter->next;
		}
		process_iter = process_iter->next;
	}
	
	assert(TotalLoggingTime >= log_time/MaxCpuNum);
#endif
	
	return 1;
}


#define findProcessName_V2
int Parser::findProcessName( Node <ProcessInfo> * _process, string *name)
{
	List <ParseEntry> *log =  _process->entry->getLogList();

#ifdef findProcessName_V2
	name->assign(log->getTail()->entry->getProcessName());

	return RET_FPN_FOUND_PROCESS_NAME;
#else
	Node <ParseEntry> *cur = log->getTail();

	while( cur != NULL)
	{
		if ( identifyScheduleEvent( cur->entry ) == RET_ISE_SCHEDULE_START_PROCESS_FINISH )
		{
			if(LogVersion == "1.0")
				*name = cur->entry->getLogMsg();
			else if(LogVersion == "2.0")
				*name = cur->entry->getProcessName();
			else if(LogVersion == "2.1")
				*name = cur->entry->getLogMsg();
			
			return RET_FPN_FOUND_PROCESS_NAME;
		}
		cur = cur->prev;
	}
	
	return RET_FPN_NOT_FOUND_PROCESS_NAME;
#endif
}

void Parser::groupCpuParseList()
{
	cout << endl << "Grouping parsed logs per CPU ID is in progress" << endl;
	
	Node <ParseEntry> *log_iter = this->getParseList()->getHead();
	
	while(log_iter)
	{
		int cpu_num = log_iter->entry->getCpuId();
		
		Node <ParseEntry> *linked_log = new Node <ParseEntry> (log_iter->entry);
		CPU->getCpuLog(cpu_num)->push_back(linked_log);
		
		log_iter = log_iter->next;
	}
}

void Parser::generateRuleEntry( string _line, List <string> ** _rule_entry)
{
	List <string> * rule_entry = new List <string> ();
	
	string token;
	stringstream stream(_line);
	
	while (stream >> token)
	{
		Node <string> * rule_token = new Node <string> (token);
		rule_entry->push_back(rule_token);
	}
	
	*_rule_entry = rule_entry;
}


int Parser::tokenizeRuleList(string buf, List <Rule_SysIO> * rule_list)
{
	List <string> * rule_value = new List <string> ();
	
	string token;
	stringstream stream(buf);
	
	while (stream >> token)
	{
		Node <string> * rule = new Node <string> (token);
		rule_value->push_back(rule);
		//			cout << token<< endl;
	}
	
	Node <string> *priority;
	rule_value->pop_back(&priority);
	int pri = atoi(priority->entry->c_str());
	delete priority;
	
	Node<Rule_SysIO> *rule_node = new Node <Rule_SysIO>;
	rule_node->entry->setRule(rule_value);
	rule_node->entry->setPriority(pri);
	
	rule_list->push_back(rule_node);
	return 1;
}

#define DEBUG_loadIORule 0
int Parser::loadIORule(string _file)
{
	cout << endl << "Loading the IO build rule whose name is \"" << _file << "\"" << endl;
	
	char buf[1024];
	
	ifstream fin;
	fin.open(_file.c_str(), ios::out);
	
	int status = 0;
	
	if(fin.is_open())
	{
		while(1)
		{
			if(fin.eof())
				break;
			
			fin.getline(buf, 1024);
			
			if( is_comment_line(buf) )
				continue;
			
			string temp = buf;
			
			status = tokenizeRuleList(temp, RuleList_SysIO);
			if( status < 0 )
			{
				;
			}
		}
#if DEBUG_loadIORule
		RuleList_SysIO->printList();
#endif
		return 1;
	}
	else
	{
		return -1;
		
	}
}

int Parser::isNetworkIO( Node <IOInfo> * _io)
{
	IOInfo *io = _io->entry;
	Node <ParseEntry> *io_end = io->getIoFinishLog();
	
	Node <ParseEntry> *io_iter = io->getIoStartLog();
	while (io_iter != io_end)
	{
		if( string::npos != io_iter->entry->getFunctionName().find("sock_aio") ) // sock_aio_read || sock_aio_write
		{
			return 1;
		}
		io_iter = io_iter->next;
	}
	return 0;
}

int Parser::isSysFsIO( Node <IOInfo> * _io)
{
	IOInfo *io = _io->entry;
	Node <ParseEntry> *io_end = io->getIoFinishLog();
	Node <ParseEntry> *io_iter = io->getIoStartLog();
	
	while (io_iter != io_end)
	{
		if( string::npos != io_iter->entry->getFunctionName().find("do_sync"))
		{
			return 0;
		}
		io_iter = io_iter->next;
	}
	return 1;
}

int Parser::isFileIO( Node <IOInfo> * _io)
{
	IOInfo *io = _io->entry;
	Node <ParseEntry> *io_end = io->getIoFinishLog();
	Node <ParseEntry> *io_iter = io->getIoStartLog();
	
	while (io_iter != io_end)
	{
		if( string::npos != io_iter->entry->getFunctionName().find("generic_file_aio_read") || string::npos != io_iter->entry->getFunctionName().find("ext4_file_write"))
		{
			return 1;
		}
		io_iter = io_iter->next;
	}
	return 0;
}

#define DEBUG_identifyFIleIoReadType 0
/*
 get start, finish log
 traverse from start to finish 
 
 generic_file_aio_read
 가 나오면 Cache Miss & Page Fault -> Prefetced IO
 안나오면 Cache Hit -> Buffered IO
 */
int Parser::identifyFIleIoReadType( Node <IOInfo> * _io)
{
	Node <ParseEntry> * io_log_iter = _io->entry->getIoStartLog();
	Node <ParseEntry> * io_log_finish = _io->entry->getIoFinishLog();
	
	while (io_log_iter != io_log_finish )
	{
		string in_cache;
		if( io_log_iter->entry->getFunctionName() == "generic_file_aio_read"|| io_log_iter->entry->getFunctionName() == "filemap_fault")
		{
			if( io_log_iter->entry->getLogMsg() == "CM&PF")
				return RET_IFIRT_PREFETCH_IO;
			else if( io_log_iter->entry->getLogMsg() == "CH")
				return RET_IFIRT_BUFFERED_IO;
		}
		io_log_iter = io_log_iter->next;
	}
	return RET_IFIRT_NEVER_REACH;
}

#define DEBUG_identifyFIleIoWriteType 0
int Parser::identifyFIleIoWriteType( Node <IOInfo> * _io)
{
	Node <ParseEntry> * io_log_iter = _io->entry->getIoStartLog();
	Node <ParseEntry> * io_log_finish = _io->entry->getIoFinishLog();
	
	while (io_log_iter != io_log_finish)
	{
		string in_cache;
		if( io_log_iter->entry->getFunctionName() == "ext4_ext_in_cache")
		{
			extractStringNextTo(io_log_iter->entry->getLogMsg(),"ret ", &in_cache);
			
			if( in_cache == "1")
				return RET_IFIWT_BUFFERED_IO_1;
			else if( in_cache == "0")
				return RET_IFIWT_BUFFERED_IO_2;
			else
				RET_IFIWT_NEVER_REACH;
		}
		io_log_iter = io_log_iter->next;
	}
	return RET_IFIWT_NEVER_REACH;
}

int Parser::findIOEndFunctionName(string &_function_name, Rule_SysIO* _mapped_rule)
{
	Node <string> * _io_end_func_name = _mapped_rule->getRule()->getHead()->next;
	
	while( _io_end_func_name != NULL)
	{
		if( _function_name == *(_io_end_func_name->entry)  )
		{
			return 1;
		}
		
		_io_end_func_name = _io_end_func_name->next;
	}
	
	return -1;
	
}

int Parser::cmpIOProperties(int _fd1, int _fd2, long _size1, long _size2, int _pid1, int _pid2 )
{
	if(  (_fd1 == _fd2) && (_size1 == _size2) && (_pid1 == _pid2) )
		return RET_CIP_SAME;
	else
		return RET_CIP_NOT_SAME;
}

int Parser::findMappedIOEnd (Node <ParseEntry> *_io_start, Node <Rule_SysIO> *_mapped_rule, Node <ParseEntry> ** _ret_mapped_io_end  )
{
	int status = 0;
	
	int fd = -1;
	long size = -1;
	long pos = -1;
	int pid = -1;
	int cpuid = -1;
	Node <ParseEntry> *cur = _io_start->next;
	
#if DEBUG_findMappedIOEnd
	if(_io_start->entry->getLineNumber() == DEBUG_findMappedIOEnd_IO_START_LINENUMBER)
		cout << "debug to stop" << endl;
		
#endif
	// for io start
	parseIOProperty(_io_start, &fd, &size, &pos, &pid, &cpuid);
	
	while (cur != NULL)
	{
		string cmp_funcname = cur->entry->getFunctionName();
		
		Rule_SysIO * _rule = _mapped_rule->entry;
		
		status = findIOEndFunctionName(  cmp_funcname, _rule  );
		if( status < 0) // if cur is not the mapped io end
		{
			cur = cur->next;
			continue;
		}
		// if cur is the mapped io end
		int cmp_fd = 0;
		long cmp_size = 0;
		long cmp_pos = 0;
		int cmp_pid = 0;
		int cmp_cpuid = 0;
		// for io end
		status = parseIOProperty(cur, &cmp_fd, &cmp_size, &cmp_pos, &cmp_pid, &cmp_cpuid);
		if( status < 0 )
		{
			RET_FMIE_NEVER_EXECUTE; // never reached
		}
		
		// compare properties
//		if(analyze_mode != VFS_INFO)
//		{
//			status = cmpIOProperties(fd, cmp_fd, size, cmp_size
//						 , pid, cmp_pid);
//			
//			if( status < 0)
//			{
//				cur = cur->next;
//				continue;
//			}
//		}

		* _ret_mapped_io_end = cur;
		return RET_FMIE_FOUND_COMPOLETE_IO;
	}
	
	return RET_FMIE_NOT_FOUND_COMPOLETE_IO;
}


int Parser::parseIOProperty(Node <ParseEntry> *_IO_start, int *_fd, long *_size, long * _pos, int *_pid, int *_cpuid)
{
	int status = 0;
	
	string str_fd;
	status = extractStringNextTo(_IO_start->entry->getLogMsg(), "file=", &str_fd);
	if( status < 0)
		return -1;
	
	string str_size;
	status = extractStringNextTo(_IO_start->entry->getLogMsg(), "size=", &str_size);
	if( status < 0)
		return -1;

	string str_pos;
	status = extractStringNextTo(_IO_start->entry->getLogMsg(), "pos=", &str_pos);
	if( status < 0)
		return -1;
	
	*_fd = atoi(str_fd.c_str());
	*_size = atoi(str_size.c_str());
	*_pos = atoi(str_pos.c_str());

	*_pid = _IO_start->entry->getProcessID();
	*_cpuid = _IO_start->entry->getCpuId();
	
	return 1;
}

#define DEBUG_buildIOInfoWithRule 0
#define DEBUG_buildIOInfoWithRule_LineNumber 83998
int Parser::buildIOInfoWithRule( Node <ProcessInfo> * _process )
{
	int status  = 0;
	Node <ParseEntry> * log_iter = _process->entry->getLogList()->getHead();
	
	// find a io_start candidate
	while ( log_iter != NULL )
	{
		Node <ParseEntry> *IO_start = NULL, *IO_end = NULL;
		Node <Rule_SysIO> *applied_rule = NULL;

#if DEBUG_buildIOInfoWithRule
		if(log_iter->entry->getLineNumber() == DEBUG_buildIOInfoWithRule_LineNumber)
		{
			cout << "target line: " << DEBUG_buildIOInfoWithRule_LineNumber << endl;
		}
#endif
		string f_name = log_iter->entry->getFunctionName();
		// find a mapped rule corresponding the cur
		status = RuleList_SysIO->searchNodeWithKey(f_name, &applied_rule);
		
		if (status == RET_SNWK_NOT_FOUND ) // if a matched rule isn't found
		{
			log_iter = log_iter->next;
			continue;
		}

		// if a matched rule is found (means that IO start is found)
		IO_start = log_iter;
		
		// find the mapped IO end
		status = findMappedIOEnd( IO_start, applied_rule, &IO_end );
		
		if (status < 0) // can't found a mapped io_end
		{
			log_iter = log_iter->next;
			continue;
		}
		
		// found a matched IO_end
		int fd = 0;
		long size = 0;
		long pos = 0;
		int pid = 0;
		int cpuid = 0;
		
		// get io _end propert
//		parseIOProperty(IO_end, &fd, &size, &pid, &cpuid);
		parseIOProperty(IO_start, &fd, &size, &pos, &pid, &cpuid);
		
		// gen io info and push into the list
		string filename = extractFileName(IO_start, IO_end);
		if(string::npos != filename.find("size=") || string::npos != filename.find("*ppos="))
			filename = "";
		
		Node <IOInfo> *new_io = new Node <IOInfo> (IO_start, IO_end, fd, size, pos, filename);

		// categorizing file I/Os
		if( isFileIO(new_io)  ) // if the new_io is file I/O
		{
#if DEBUG_buildIOInfoWithRule
			cout << endl << "[File IO]" << endl;
			new_io->entry->printFromStartToFinish();
#endif
			// if the new_io is file io and the process has a list of File I/O which isn't empty.
			if(_process->entry->getFileIOList()->getHead() != NULL)
			{
				ParseEntry * last_io_finish = _process->entry->getFileIOList()->getTail()->entry->getFinish();
			
			// if wrong io collectin gprocedure occurd
			// example)
			/**
			index #208
			[16959] (65.214833 -> 65.214845) [cpu2] [sys_read_pre/646]: fd=3 size=262224
			[46224] (66.295895 -> 66.2959563) [cpu0] [sys_read_post/646]: fd=3 size=262224
			
			index #209
			[46171] (66.294613 -> 66.294613) [cpu0] [sys_writev_pre/646]: fd=3 size=2
			[46178] (66.294766 -> 66.294766) [cpu0] [sys_writev_post/646]: fd=3 size=2
			
			index #210
			[46179] (66.294766 -> 66.294781) [cpu0] [sys_read_pre/646]: fd=3 size=262224
			[46224] (66.295895 -> 66.2959563) [cpu0] [sys_read_post/646]: fd=3 size=262224
			 */
				if( new_io->entry->getStart()->getLineNumber() <= last_io_finish->getLineNumber())
				{
					cout << endl << "WANRING: Wrong io collection is canceled due to log miss" << endl;
					cout << "wrong io info to be deleted: " << endl;
					_process->entry->getFileIOList()->getTail()->entry->print();
					cout << "new io info to be inserted: " << endl;
					new_io->entry->print();
					
					Node <IOInfo> * io_to_delete = NULL;
					_process->entry->getFileIOList()->pop_back(&io_to_delete);
					delete io_to_delete;
				}
			}
			
			_process->entry->getFileIOList()->push_back(new_io);
			
#if DEBUG_buildIOInfoWithRule_LineNumber
			if( new_io->entry->getStart()->getLineNumber() == DEBUG_buildIOInfoWithRule_LineNumber)
				cout << "bp" << endl;
#endif

			/// copy the analyzed io and put it into CPU File IO
			Node <IOInfo> *linked_io = new Node <IOInfo> (new_io->entry);
			CPU->getFileIo(cpuid)->push_back(linked_io);
			
			if(new_io->entry->getR_W_type() == "sys_read")
			{
				status = identifyFIleIoReadType(new_io);
				if(status == RET_IFIRT_BUFFERED_IO)
				{
#if DEBUG_buildIOInfoWithRule
					cout << "====> [Buffered IO Read]" << endl;
#endif
					Node <IOInfo> *copied_io = new Node <IOInfo> (new_io->entry);
					_process->entry->getBufferedIOList()->push_back(copied_io);
				}
				else if (status == RET_IFIRT_PREFETCH_IO)
				{
#if DEBUG_buildIOInfoWithRule
					cout << "====> [Direct IO Read]" << endl;
#endif
					Node <IOInfo> *copied_io = new Node <IOInfo> (new_io->entry);
					_process->entry->getDirectIOList()->push_back(copied_io);
				}
				else
				{
#if DEBUG_buildIOInfoWithRule
					cout << "What is this type?" << endl;
#endif
					Node <IOInfo> *copied_io = new Node <IOInfo> (new_io->entry);
					_process->entry->getUnknownIOList()->push_back(copied_io);
//					new_io->entry->printFromStartToFinish();
				}
			}
			else if ( new_io->entry->getR_W_type() == "sys_write" )
			{
				status = identifyFIleIoWriteType(new_io);
				if(status == RET_IFIWT_BUFFERED_IO_1)
				{
#if DEBUG_buildIOInfoWithRule
					cout << "====> [Buffered IO Write 1]" << endl;
#endif
					Node <IOInfo> *copied_io = new Node <IOInfo> (new_io->entry);
					_process->entry->getBufferedIOList()->push_back(copied_io);
				}
				else if (status == RET_IFIWT_BUFFERED_IO_2)
				{
#if DEBUG_buildIOInfoWithRule
					cout << "====> [Buffered IO Write 2]" << endl;
#endif
					Node <IOInfo> *copied_io = new Node <IOInfo> (new_io->entry);
					_process->entry->getBufferedIOList()->push_back(copied_io);
				}
				else
				{
#if DEBUG_buildIOInfoWithRule
					cout << "====> [Buffered IO Write 3]" << endl;
#endif
					Node <IOInfo> *copied_io = new Node <IOInfo> (new_io->entry);
					_process->entry->getBufferedIOList()->push_back(copied_io);
				}
			}
			else {
				cout << "Not read or write?" << endl;
				assert (0);
			}
		}
//		else if( isNetworkIO( new_io) )
//		{
//			// if the new_io is network IO
//			_process->entry->getNetIOList()->push_back( new_io );
//		}
//		else if( isSysFsIO(new_io) )
//		{
//			// if the new_io is sysfs io
//			_process->entry->getSysfsIOList()->push_back( new_io );
//		}
		else
		{
#if DEBUG_buildIOInfoWithRule
//			cout << endl;
//			new_io->entry->printFromStartToFinish();
//			//			cout << new_io->entry->getFileName() << " ";
#endif
			_process->entry->getUnknownIOList()->push_back(new_io);
		}
		
		log_iter = log_iter->next;
	}

	return 1;
	
}

int Parser::identifyBlockLevelEvent(ParseEntry *_log)
{
	if( _log->getFunctionName() == BLOCK_BIO_REMAP_EVENT)
	{
		return BLOCK_BIO_REMAP;
	}
	else if( _log->getFunctionName() == BLOCK_BIO_QUEUE_EVENT)
	{
		return BLOCK_BIO_QUEUE;
	}
	else if( _log->getFunctionName() == BLOCK_GETRQ_EVENT)
	{
		return BLOCK_GETRQ;
	}
	else if( _log->getFunctionName() == BLOCK_PLUG_EVENT)
	{
		return BLOCK_PLUG;
	}
	else if( _log->getFunctionName() == BLOCK_BIO_BACKMERGE_EVENT)
	{
		return BLOCK_BIO_BACKMERGE;
		
	}
	else if( _log->getFunctionName() == BLOCK_BIO_FRONTMERGE_EVENT)
	{
		return BLOCK_BIO_FRONTMERGE;
		
	}
	else if( _log->getFunctionName() == BLOCK_RQ_INSERT_EVENT)
	{
		return BLOCK_RQ_INSERT;
		
	}
	else if( _log->getFunctionName() == BLOCK_UNPLUG_EVENT)
	{
		return BLOCK_UNPLUG;
		
	}
	else if( _log->getFunctionName() == BLOCK_RQ_ISSUE_EVENT)
	{
		return BLOCK_RQ_ISSUE;
		
	}
	else if( _log->getFunctionName() == BLOCK_RQ_COMPLETE_EVENT)
	{
		return BLOCK_RQ_COMPLETE;
		
	}
	else
	{
		return -1;
	}
}

#define DEBUG_collectIORequestEventForProcesses 0
#define TARGET_PID 2681
int Parser::collectIORequestEventForProcesses(List <ProcessInfo> * _process_info)
{
	cout << endl << "Collecting IO requested from apps in system call level is in progress" << endl;
	
	Node <ProcessInfo> * process_iter =  _process_info->getHead();
	
	while(  process_iter != NULL  )
	{
#if DEBUG_collectIORequestEventForProcesses
		if( process_iter->entry->getProcessId() == TARGET_PID)
		{
			cout << "target pid: " << TARGET_PID << endl;
		}
#endif
		buildIOInfoWithRule(  process_iter  );
		   
		CntTotalFileIo += process_iter->entry->getFileIOList()->getSize();
		CntTotalNetIo += process_iter->entry->getNetIOList()->getSize();
		CntTotalSysIo += process_iter->entry->getSysfsIOList()->getSize();
		CntTotalUnknownIo += process_iter->entry->getUnknownIOList()->getSize();
		
		process_iter = process_iter->next;
	}
	
	if(!CntTotalFileIo)
		cout << endl << "NOTICE: File I/O request from applications canceled because log has no block level event." << endl;

	/// test routine (passed)
//	long count_total_file_io = 0;
//	for(int i = 0; i < MaxCpuNum; i++)
//	{
//		count_total_file_io += CPU->getFileIo(i)->getSize();
//	}
//	assert(CntTotalFileIo == count_total_file_io);
	
	return 1;
}

int Parser::pickAllBlockEvent(List <ParseEntry>* temp_block_event_list)
{
	Node <ParseEntry> * log_iter = ParseList->getHead();
	
	while( log_iter != NULL)
	{
		if( identifyBlockLevelEvent(log_iter->entry) > 0 )
		{
			Node <ParseEntry> *linked_node = new Node<ParseEntry> (log_iter->entry);
			temp_block_event_list->push_back(linked_node);
		}
		
		log_iter = log_iter->next;
	}
	
	if( temp_block_event_list->getSize() == 0)
		return RET_PABE_NOT_FOUND_BLOCK_EVENT;
	else
		return RET_PABE_FOUND_BLOCK_EVENT;
}


int Parser::parseBlockIOProperty( ParseEntry *_target_bio, unsigned long long *_start_addr, unsigned long long *_size, string *_R_W)
{
	string log = _target_bio->getLogMsg();
	
	string::size_type posPlus = log.find_first_of("+");
	if (posPlus == string::npos )
	{
		return -1;
	}
	
	string::size_type posNum = log.find_last_not_of(" 0123456789+", posPlus);
	posNum = log.find_first_of("0123456789", posNum);
	if (posNum == string::npos )
	{
		return -1;
	}
	
	string::size_type posNumEnd = log.find_last_not_of("+", posPlus);
	if (posNumEnd == string::npos )
	{
		return -1;
	}
	
	// extract the access address
	string start_addr = log.substr(posNum, posNumEnd-posNum);
	if(start_addr == "18446744073709551615")
		*_start_addr = 0;
	else
		* _start_addr = atoll(start_addr.c_str());
	
	
	posNum = log.find_first_of("0123456789", posPlus);
	if (posNum == string::npos )
	{
		return -1;
	}
	
	posNumEnd = log.find_first_not_of("0123456789", posNum);
	if (posNumEnd == string::npos )
	{
		return -1;
	}
	
	// extract the access size
	string size = log.substr(posNum, posNumEnd - posNum);
	if(size == "18446744073709551615")
		*_size = 0;
	else
		* _size = atoll(size.c_str());
	
#if 0
	string::size_type posR_WEnd = log.find_last_not_of(" 0123456789+", posPlus);
	string::size_type posR_W = log.find_last_of(" ", posR_WEnd);
	posR_W +=1;
#else
	string::size_type posR_W = log.find_first_not_of(" ,0123456789");
	string::size_type posR_WEnd = log.find_first_of(" ", posR_W);
	
#endif
	// type
	
	string R_W = log.substr(posR_W, posR_WEnd-posR_W);
	
	if ( string::npos != R_W.find("R"))
	{
		*_R_W = BLOCK_READ_STRING_LABEL;
		
	}
	else if ( string::npos != R_W.find("W"))
	{
		*_R_W = BLOCK_WRITE_STRING_LABEL;
	}
	
	return 1;
}


int Parser::isSameBlockIO(unsigned long long &_start_addr, unsigned long long &_start_addr2, unsigned long long &_size, unsigned long long &_size2, string &_R_W, string &_R_W2)
{
	if( _start_addr != _start_addr2 )
	{
		return -1;
	}
	
	if( _size != _size2)
	{
		return -1;
	}
	
	if ( _R_W != _R_W2)
	{
		return -1;
	}
	
	return 1;
}


int Parser::isInBlockIO(unsigned long long &_start_addr, unsigned long long &_start_addr2, unsigned long long &_size, unsigned long long &_size2, string &_R_W, string &_R_W2)
{
	if( _R_W != _R_W2)
	{
		return -1;
		
	}
	if( (_start_addr2 <= _start_addr) && (_start_addr + _size <= _start_addr2 + _size2) )
	{
		return 1;
	}
	return -1;
}

void Parser::setBlockEventProperty(BlockInfo *new_block_info,
			   unsigned long long start_addr_bio_remap, unsigned long long size_bio_remap,
			   unsigned long long start_addr_bio_queue, unsigned long long size_bio_queue,
			   unsigned long long size_bio_midstage, unsigned long long start_addr_bio_midstage,
			   unsigned long long start_addr_bio_issue, unsigned long long size_bio_issue,
			   unsigned long long start_addr_bio_complete, unsigned long long size_bio_complete
			   )
{
	new_block_info->setStartAddr_BioRemap(start_addr_bio_remap);
	new_block_info->setSize_BioRemap(size_bio_remap);

	new_block_info->setStartAddr_BioQueue(start_addr_bio_queue);
	new_block_info->setSize_BioQueue(size_bio_queue);
	
	if(new_block_info->getBlock_getrq())
	{
		new_block_info->setStartAddr_GetRq(start_addr_bio_midstage);
		new_block_info->setSIze_GetRq(start_addr_bio_midstage);
	}
	else if(new_block_info->getBlock_bio_backmerge())
	{
		new_block_info->setStartAddr_BioBackMerge(start_addr_bio_midstage);
		new_block_info->setSize_BioBackMerge(start_addr_bio_midstage);
	}
	else if(new_block_info->getBlock_bio_frontmerge())
	{
		new_block_info->setStartAddr_BioFrontMerge(start_addr_bio_midstage);
		new_block_info->setSize_BioFrontMerge(start_addr_bio_midstage);
	}
	
	new_block_info->setStartAddr_RqIssue(start_addr_bio_issue);
	new_block_info->setSize_RqIssue(size_bio_issue);
	
	new_block_info->setStartAddr_RqComplete(start_addr_bio_complete);
	new_block_info->setSize_RqComplete(size_bio_complete);
}


void Parser::remove1stIncompleteBlockEvent(List <ParseEntry>* _temp_block_event_list)
{
	Node <ParseEntry> *block_iter = _temp_block_event_list->getHead();
		
	int block_event = identifyBlockLevelEvent( block_iter->entry );
	if(block_event == BLOCK_BIO_REMAP || block_event == BLOCK_BIO_QUEUE)
	{
		return;
	}
	
	block_iter = block_iter->next;
	while (block_iter)
	{
		block_event = -1;
		block_event = identifyBlockLevelEvent( block_iter->entry );
		
		if(block_event == BLOCK_BIO_REMAP || block_event == BLOCK_BIO_QUEUE)
		{
			_temp_block_event_list->removeFromTo(_temp_block_event_list->getHead(), block_iter->prev);
			return;
		}
		block_iter = block_iter->next;
	}
}

int Parser::collectBlockEvent(List <ParseEntry> * _all_block_event_list, List <BlockInfo> *_block_list, List <BlockInfo> *_incomplete_block_list)
{
	cout << endl << "Collecting block events is in progress" << endl;
	
	int status = 0;
	int ret = 0;
	
	status = pickAllBlockEvent(_all_block_event_list);
	
	if(status == RET_PABE_NOT_FOUND_BLOCK_EVENT)
	{
		cout << endl << "NOTICE: Block layer analysis canceled because log has no block level event." << endl;
		ret = RET_CBE_NO_BLOCK_EVENT;
	}
	else{
		remove1stIncompleteBlockEvent(_all_block_event_list);
		buildAllBlockIOSequence(_all_block_event_list, _block_list, _incomplete_block_list);
		ret = RET_CBE_BLOCK_EVENT_COLLECTED;
	}

	CntAllBlockEvent = _all_block_event_list->getSize();
	CntIoSchduleSequence = _block_list->getSize();
			
	return ret;
}

int Parser::isEmmcLevelEvent(ParseEntry *_log)
{
	if( _log->getFunctionName() == MMC_START_REQUEST_EVENT)
	{
		return RET_IELE_MMC_START_REQUEST;
	}
	else if( _log->getFunctionName() == MMC_REQUEST_DONE_EVENT)
	{
		return RET_IELE_MMC_REQUEST_DONE;
	}
	else
	{
		return RET_IELE_NO_EMMC_EVENT;
	}
}


int Parser::pickAllEmmcEvent(List <ParseEntry>* _temp_emmc_event_list)
{
	int status = 0;
	Node <ParseEntry> * log_iter = ParseList->getHead();
	
	while( log_iter != NULL)
	{
		status = isEmmcLevelEvent(log_iter->entry) ;
		
		if( status < 0)
		{
			log_iter = log_iter->next;
			continue;
		}
		
		Node <ParseEntry> *linked_node = new Node<ParseEntry> (log_iter->entry);
		_temp_emmc_event_list->push_back(linked_node);
		
		log_iter = log_iter->next;
	}
	
	if( _temp_emmc_event_list->getSize() == 0)
		return RET_PAEE_NOT_FOUND_EMMC_EVENT;
	else
		return RET_PAEE_FOUND_EMMC_EVENT;

}


int Parser::parseEmmcIOProperty(Node <ParseEntry> *_emmc, long *_start_address, long *_block_cnt, long *_block_size, int *__cmd)
{
	string start_address;
	string arg = "arg ";
	extractStringNextTo(_emmc->entry->getLogMsg(), arg, &start_address);
	*_start_address = atol(start_address.c_str());
	
	string size;
	string blocks = "blocks ";
	extractStringNextTo(_emmc->entry->getLogMsg(), blocks, &size);
	*_block_cnt = atol(size.c_str());
	
	string block_size;
	string blksz = "blksz";
	extractStringNextTo(_emmc->entry->getLogMsg(), blksz, &block_size);
	*_block_size = atol(block_size.c_str());
	
	string cmd;
	string CMD = "CMD";
	extractStringNextTo(_emmc->entry->getLogMsg(), CMD, &cmd);
	*__cmd = atoi(cmd.c_str());
	
	return 1;
}

int Parser::isSameEmmcIO(Node <ParseEntry> *emmc_A, Node <ParseEntry> *emmc_B)
{
	long start_address_A = 0;
	long block_cnt_A = 0;
	int cmd_A = 0;
	long block_size_A = 0;
	parseEmmcIOProperty(emmc_A, &start_address_A, &block_cnt_A, &block_size_A, &cmd_A);
	
	long start_address_B = 0;
	long block_cnt_B = 0;
	int cmd_B = 0;
	long block_size_B = 0;
	parseEmmcIOProperty(emmc_B, &start_address_B, &start_address_B, &block_size_B, &cmd_B);
	
	if( start_address_A == start_address_B
	   && block_cnt_A == block_cnt_B
	   && cmd_A == cmd_B)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


int Parser::findEmmcIOSequence(Node <ParseEntry> *_emmc_start_request_node, Node <eMMCInfo> **new_item)
{
	int status = -1;
	
	Node <ParseEntry> * emmc_request_done = _emmc_start_request_node->next;
	
	// no more emmc event
	if(emmc_request_done == NULL)
	{
		*new_item = new Node <eMMCInfo> (_emmc_start_request_node, NULL);
		
		/// need to be refactored
		long start_address = 0;
		long block_cnt = 0;
		long block_size = 0;
		int cmd = 0;
		
		parseEmmcIOProperty(_emmc_start_request_node, &start_address, &block_cnt, &block_size, &cmd);
		
		(*new_item)->entry->setStartAddress(start_address);
		(*new_item)->entry->setCntBlocks(block_cnt);
		(*new_item)->entry->setBlockSz(block_size);
		(*new_item)->entry->setCMD(cmd);
		
		EmmcIoSequenceWithNoRequestDone++;
		
		return RET_FEIS_NOT_FOUND_EMMC_REQUEST_DONE;
	}
	
	int emmc_event = isEmmcLevelEvent(emmc_request_done->entry);
	
	// found the matched emmc event
	if(emmc_event == RET_IELE_MMC_REQUEST_DONE)
	{
		status = isSameEmmcIO(_emmc_start_request_node, emmc_request_done);
		
		if(status < 0)
		{
			assert(0);
		}
		
		*new_item = new Node <eMMCInfo> (_emmc_start_request_node, emmc_request_done);
		
		/// need to be refactored
		long start_address = 0;
		long block_cnt = 0;
		long block_size = 0;
		int cmd = 0;
		
		parseEmmcIOProperty(_emmc_start_request_node, &start_address, &block_cnt, &block_size, &cmd);
		
		(*new_item)->entry->setStartAddress(start_address);
		(*new_item)->entry->setCntBlocks(block_cnt);
		(*new_item)->entry->setBlockSz(block_size);
		(*new_item)->entry->setCMD(cmd);
		
		return RET_FEIS_FOUND_COMPLETE_EMMC_IO;
	}
	// emmc_request_done event log is missed
	else if(emmc_event ==  RET_IELE_MMC_START_REQUEST)
	{
		*new_item = new Node <eMMCInfo> (_emmc_start_request_node, NULL);
		
		/// need to be refactored
		long start_address = 0;
		long block_cnt = 0;
		long block_size = 0;
		int cmd = 0;
		
		parseEmmcIOProperty(_emmc_start_request_node, &start_address, &block_cnt, &block_size, &cmd);
		
		(*new_item)->entry->setStartAddress(start_address);
		(*new_item)->entry->setCntBlocks(block_cnt);
		(*new_item)->entry->setBlockSz(block_size);
		(*new_item)->entry->setCMD(cmd);
		
		EmmcIoSequenceWithNoRequestDone++;
		
		return RET_FEIS_NOT_FOUND_EMMC_REQUEST_DONE;
	}
	// never happen
	else{
		assert(0);
		return RET_FEIS_NEVER_EXECUTE;
	}
}

#define PRINT_ERROR_LINE_DURING_EMMC_IO_COLLECTED 0
#define PRINT_ALL_EMMC_CMD_TYPE 1
#define PRINT_INCOMPLETE_EMMC_EVENT 1
#define COUNT_INCOMPLETE_EMMC_EVENT 1
int Parser::buildAllEmmcIOSequence(List <ParseEntry>* _temp_emmc_event_list, List <eMMCInfo> * _emmc_list, List <eMMCInfo> * _incomplete_emmc_list)
{
	/// return status
	int status = -1;
	
	Node <ParseEntry> *emmc_iter = _temp_emmc_event_list->getHead();
	while (emmc_iter)
	{
#if DEBUG_buildAllEmmcIOSequence
		if ( emmc_iter->entry->getLineNumber() == DEBUG_buildAllEmmcIOSequence_TARGET_LINE)
		{
			cout << "DEBUG: break" << endl;
		}
		emmc_iter->entry->print();
#endif
		
		int emmc_event = -1;
		emmc_event = isEmmcLevelEvent(emmc_iter->entry );
		
		if(emmc_event == RET_IELE_MMC_START_REQUEST)
		{
			Node <eMMCInfo> *new_item = NULL;
			status = findEmmcIOSequence(emmc_iter, &new_item);
			
			/// found the matched request done emmc event.
			if (status == RET_FEIS_FOUND_COMPLETE_EMMC_IO)
			{
				_emmc_list->push_back(new_item);
				
				emmc_iter = emmc_iter->next;
				continue;
			}
			/// no more emmc event or next emmc event is a start request event.
			else if(status == RET_FEIS_NOT_FOUND_EMMC_REQUEST_DONE)
			{
				_incomplete_emmc_list->push_back(new_item);
				
				emmc_iter = emmc_iter->next;
				
				continue;
			}
			else // never reached
			{
				assert (0);
			}
		}
		else if(emmc_event == RET_IELE_MMC_REQUEST_DONE)
		{
			int prev_emmc_event = 0;

			prev_emmc_event = isEmmcLevelEvent(emmc_iter->prev->entry );
			if(prev_emmc_event == RET_IELE_MMC_REQUEST_DONE) // a emmc request start log may be missed.
			{
				Node <eMMCInfo> *new_item = new Node <eMMCInfo> (NULL, emmc_iter);
				_incomplete_emmc_list->push_back(new_item);

#if PRINT_ERROR_LINE_DURING_EMMC_IO_COLLECTED
				cout << "Emmc request done event continuesly happeded." << endl;
				emmc_iter->entry->print();
#endif
				
#if 0
#define LOOP_ITERATION 8
				for(int i = 0; i < LOOP_ITERATION; i++)
				{
					emmc_iter = emmc_iter->prev;
				}
				for(int i = 0; i < LOOP_ITERATION+1; i++)
				{
					emmc_iter->entry->print();
					emmc_iter = emmc_iter->next;
				}
#elseif 0
				Node <ParseEntry> *debug_emmc_iter = _temp_emmc_event_list->getHead();
				while(debug_emmc_iter)
				{
					debug_emmc_iter->entry->print();
					debug_emmc_iter = debug_emmc_iter->next;
				}
#endif
			}
		}
		else
		{
			assert(0);
		}
		emmc_iter = emmc_iter->next;
	}
	
	return 1;
}

void Parser::remove1stIncompleteEmmcEvent(List <ParseEntry>*  _temp_emmc_event_list)
{
	// parameter check
	if (_temp_emmc_event_list->getSize() == 0)
	{
		assert(0);
	}
	
	int status = isEmmcLevelEvent( _temp_emmc_event_list->getHead()->entry);
	
	if ( status == RET_IELE_MMC_REQUEST_DONE)
	{
		_temp_emmc_event_list->removeNode(_temp_emmc_event_list->getHead());
	}
}


int Parser::collecteMMCEvent(List <ParseEntry> *_all_emmc_event_list, List <eMMCInfo> * _emmc_list, List <eMMCInfo> * _incomplete_emmc_list)
{
	cout << endl << "Collecting eMMC Event is in progress" << endl;
	
	int ret = 0;
	int status = 0;
	status = pickAllEmmcEvent(_all_emmc_event_list);
	//		temp_emmc_event_list->printList("debug/temp_emmc_event_list_1phase.txt");
	if( status == RET_PAEE_NOT_FOUND_EMMC_EVENT)
	{
		ret = RET_CEE_NOT_FOUND_EMMC_EVENT;
	}
	else
	{
		remove1stIncompleteEmmcEvent(_all_emmc_event_list);
		//		temp_emmc_event_list->printList("debug/temp_emmc_event_list_2phase.txt");
		buildAllEmmcIOSequence(_all_emmc_event_list, _emmc_list, _incomplete_emmc_list);
		
		ret = RET_CEE_FOUND_EMMC_EVENT;
	}
	
	CntAllEmmcEvent = _all_emmc_event_list->getSize();
	CntEmmcIO = _emmc_list->getSize();
	CntIncompleteEmmcIOWithoutEMMCRequestStart = _incomplete_emmc_list->getSize();
		
	return ret;
}

int Parser::identifyInterruptEvent(ParseEntry * _parse_entry)
{
	if(_parse_entry->getFunctionName() == STR_INTERRUPT_START_FUNCTION)
	{
		return RET_IIE_IRQ_START;
	}
	if(_parse_entry->getFunctionName() == STR_INTERRUPT_FINISH_FUNCTION)
	{
		return RET_IIE_IRQ_FINISH;
	}
	if(_parse_entry->getFunctionName() == STR_SOFTINT_START_FUNCTION)
	{
		return RET_IIE_SOFTIRQ_START;
	}
	if(_parse_entry->getFunctionName() == STR_SOFTINT_FINISH_FUNCTION)
	{
		return RET_IIE_SOFTIRQ_FINISH;
	}
	else
	{
		return RET_IIE_NOT_IRQ_EVENT;
	}
}

int Parser::findMappedIrqFinish(Node <ParseEntry> *_log_iter, Node <ParseEntry> **_irq_end)
{
	Node <ParseEntry> *log_iter = _log_iter->next;
	
	while(log_iter)
	{
		int interrupt_event =identifyInterruptEvent(log_iter->entry);
		
		if(interrupt_event == RET_IIE_IRQ_FINISH)
		{
			*_irq_end = log_iter;
			return RET_FMIF_FOUND_IRQ_FINISH;
		}
		
		log_iter = log_iter->next;
	}
	
	return RET_FMIF_NOT_FOUND_IRQ_FINISH;
	
}

int Parser::findMappedSoftIrqFinish(Node <ParseEntry> *_log_iter, Node <ParseEntry> **_soft_irq_end)
{
	Node <ParseEntry> *log_iter = _log_iter->next;
	
	while(log_iter)
	{
		int interrupt_event =identifyInterruptEvent(log_iter->entry);
		
		if(interrupt_event == RET_IIE_SOFTIRQ_FINISH)
		{
			*_soft_irq_end = log_iter;
			return RET_FMSIF_FOUND_SOFTIRQ_FINISH;
		}
		
		log_iter = log_iter->next;
	}
	
	return RET_FMSIF_NOT_FOUND_SOFTIRQ_FINISH;
	
}


int Parser::buildInterruptEventThroughCPULog(List <ParseEntry> * _log_per_cpu, List <InterruptInfo> *_interrupt_list_per_cpu)
{
	Node <ParseEntry> *log_iter = _log_per_cpu->getHead();
	
	int status = 0;
	
	while(log_iter)
	{
		Node <ParseEntry> *irq_end = NULL;
		
		int interrupt_event = identifyInterruptEvent(log_iter->entry);
		
		if(interrupt_event == RET_IIE_IRQ_START)
		{
			CntAllInterruptEvent++;
			
			status = findMappedIrqFinish(log_iter, &irq_end);
			
			if(status < 0)
			{
				cout << "Maybe this is the last irq event in the log: " << endl;
				log_iter->entry->print();
				
				log_iter = log_iter->next;
				continue;
			}
			
			CntAllInterruptEvent++;

			Node <InterruptInfo> *irq_new_item = new Node <InterruptInfo> (log_iter, irq_end);
			
			_interrupt_list_per_cpu->push_back(irq_new_item);
			
			log_iter = log_iter->next;
			
			continue;
		}
		
		
		log_iter = log_iter->next;
	}
	
	CntIntteruptSequence += _interrupt_list_per_cpu->getSize();
	
	return 1;
}


int Parser::buildSoftIrqEventThroughCPULog(List <ParseEntry> * _log_per_cpu, List <SoftInterruptInfo> *_soft_irq_list_per_cpu)
{
	Node <ParseEntry> *log_iter = _log_per_cpu->getHead();
	
	int status = 0;
	
	while(log_iter)
	{
		Node <ParseEntry> *soft_irq_end = NULL;
		
		int interrupt_event = identifyInterruptEvent(log_iter->entry);
		
		if(interrupt_event == RET_IIE_SOFTIRQ_START)
		{
			CntAllSoftIrqEvent++;
			
			status = findMappedSoftIrqFinish(log_iter, &soft_irq_end);
			if(status < 0)
			{
				cout << "Maybe this is the last soft irq event in the log: " << endl;
				log_iter->entry->print();
				
				log_iter = log_iter->next;
				continue;
			}
			
			CntAllSoftIrqEvent++;
			
			Node <SoftInterruptInfo> *irq_new_item = new Node <SoftInterruptInfo> (log_iter, soft_irq_end);
			
			_soft_irq_list_per_cpu->push_back(irq_new_item);
			
			log_iter = log_iter->next;
			
			continue;
		}
		
		log_iter = log_iter->next;
	}
	
	CntSoftIrqSequence += _soft_irq_list_per_cpu->getSize();
	return 1;
}


int Parser::collectInterruptEvent()
{
	cout << endl << "Collecting interrupt and soft interrupt is in progress" << endl;
	
	for(int i = 0; i < MaxCpuNum; i++)
	{
		buildInterruptEventThroughCPULog(CPU->getCpuLog(i), CPU->getInterruptTime(i));
		buildSoftIrqEventThroughCPULog(CPU->getCpuLog(i), CPU->getSoftIrqTime(i));
	}
		
	return 1;
}

int Parser::groupProcessParseList (List <ProcessInfo> *_process_info_list)
{
	cout << endl << "Grouping ParseList into the corresponding process" << endl;
	
	int status = 0;
	
	Node <ParseEntry> *cur_log = ParseList->getHead();
	while (cur_log != NULL)
	{
#if DEBUG_groupProcessParseList
		if ( cur_log->entry->getLineNumber() == 552)
			cout << "stop to debug" << endl;
#endif
		Node <ProcessInfo> *target_process = NULL;
		int pid = cur_log->entry->getProcessID();
		status = _process_info_list->searchNodeWithKey(pid, &target_process);
		if( status < 0 ) // not found
		{
			target_process = new Node <ProcessInfo>();
			//				Node <ParseEntry> *new_node_with_same_entry =
			target_process->entry->setProcessId(pid);
			_process_info_list->push_back(target_process);
			
			Node <ParseEntry> *linked_node = new Node<ParseEntry> (cur_log->entry);
			target_process->entry->getLogList()->push_back(linked_node);
		}
		else // found
		{
			Node <ParseEntry> *linked_node = new Node<ParseEntry> (cur_log->entry);
			target_process->entry->getLogList()->push_back(linked_node);
		}
		//			if(cur_log ->entry->getLineNumber() == 4260314)
		//				printf("bp\n");
		cur_log = cur_log->next;
	}
	
	return 1;
}


/**
 * brief
 1. get _process_list
 2.
 * param
 * return
 */
#define PRINT_PID_HAVING_NO_PROCESS_NAME 1
#define PRINT_LINE_HAVING_NO_PROCESS_NAME 0
int Parser::fillProcessName ( List <ProcessInfo> * _process_list)
{
	cout << endl << "Finding process names is in progress" << endl;
	
	Node <ProcessInfo> * cur = _process_list->getHead();
	
	int status = 0;
	
	while( cur != NULL)
	{
		string process_name;
		
#if DEBUG_fillProcessName
		if( cur->entry->getProcessId() == DEBUG_fillProcessName_TARGET_PID)
			cout << "DEBUG for process id : " << DEBUG_fillProcessName_TARGET_PID << endl;
#endif
		
		status = findProcessName( cur , &process_name );
//		string::size_type pos = p_name.find("/");
		if( status == RET_FPN_NOT_FOUND_PROCESS_NAME)
		{
#if PRINT_PID_HAVING_NO_PROCESS_NAME
			cout << "ERROR: Process name couldn't found for pid-" << cur->entry->getProcessId() << endl;
#endif
#if PRINT_LINE_HAVING_NO_PROCESS_NAME
			cur->entry->getLogList()->getHead()->entry->print();
#endif
			// exception handling
			cur->entry->setProcessName("?");

		}
		else if( status == RET_FPN_FOUND_PROCESS_NAME)
		{
			if(LogVersion == "1.0")
			{
				size_t pos = process_name.find_last_of("/");
				if( string::npos != pos)
				{
					process_name.erase(pos);
				}
			}
			
			cur->entry->setProcessName(process_name);
		}
		cur = cur->next;
	}

	return 1;
}

void Parser::debugParseEntryNFileIOPerProcess(List <ProcessInfo> * _process_list)
{
	cout << endl << "Information File for debugging is generated" << endl;
	
	Node <ProcessInfo> *process_iter = _process_list->getHead();
	string dir_path = DEBUG_DIRECTORY + getInputFileName() + "/";
	
	mkdir(dir_path.c_str(),  0700);
	
	while( process_iter )
	{
		stringstream pid_strm;;
		pid_strm << process_iter->entry->getProcessId();
		
		string file_path = dir_path + pid_strm.str() + "_" + process_iter->entry->getProcessName() + "_ParseList.tmp";
		process_iter->entry->getLogList()->printList( file_path.c_str() );
		
		string tmp_file_name = dir_path + pid_strm.str() + "_" + process_iter->entry->getProcessName() + "_file_io.tmp";
		process_iter->entry->getFileIOList()->printList(tmp_file_name);
		
		process_iter = process_iter->next;
	}
}


int Parser::checkExceptionInBlockIO( Node <BlockInfo> *_block_iter )
{
	// case 1. when the requested size is 0
	if(_block_iter->entry->getStartAddr_BioQueue() == 0)
	{
		_block_iter = _block_iter->next;
		CountSizeZeroBlockIO++;
		return RET_CEIBI_SIZE_ZERO_BLOCK_IO;
	}
	
	// case 2. incomplete block event
	else if( (_block_iter->entry->getBlock_bio_remap() == NULL && _block_iter->entry->getBlock_bio_queue() == NULL) || _block_iter->entry->getBlock_rq_complete() == NULL)
	{
		_block_iter = _block_iter->next;
		return RET_CEIBI_INCOMPLETE_BLOCK_IO;
	}
	return RET_CEIBI_NO_EXCEPTION;
}

int Parser::mapBlockNeMMCEvent(List <BlockInfo> *_block_list, List <eMMCInfo> *_eMMC_list)
{
	cout << endl << "Mapping between block events and eMMC events is in progress" << endl;
	
	Node <BlockInfo> *block_iter = _block_list->getHead();
	Node <eMMCInfo> *emmc_iter = _eMMC_list->getHead();
	
	int status = 0;
	
	if(_block_list->getSize() == 0)
	{
		cout << endl << "NOTICE: No block event occured so that mapping procedure couldn't do" << endl << endl;
		return 0;
	}
	else if ( _eMMC_list->getSize() == 0)
	{
		cout << endl << "NOTICE: No eMMC event occured so that mapping procedure couldn't do" << endl << endl;
		return 0;
	}
	
	while (block_iter)
	{		
		/// exception handling
		if( checkExceptionInBlockIO(block_iter) < 0 )
		{
			block_iter = block_iter->next;
			continue;
		}
		
		// no exception found
		emmc_iter = _eMMC_list->getHead();
		while(emmc_iter)
		{
			// if a emmc event is mapped to the block event
			status = checkMappedBlockAndEmmcIO(block_iter->entry, emmc_iter->entry);
			if (status == RET_CMBAEI_NO_MAPPING)
			{
				emmc_iter = emmc_iter->next;
				
				continue;;
			}
			
			block_iter->entry->setMappedEmmc(emmc_iter->entry);
			emmc_iter->entry->pushMappedBlockInfo(block_iter->entry);
			
			break; // check for the next block event;
		}
		if(emmc_iter == NULL ) // if the block event can't be mapped to a emmc event
		{
			CntBlockIONoMappedToEmmcIO++;
		}
		
		block_iter = block_iter->next;
	}
	
	return 1;
}

BlockInfo* Parser::findBlockIOBetween(long _io_start_linenumber, long _io_finish_linenumber, List <BlockInfo> *_block_io_list)
{
	Node <BlockInfo> *block_io_iter = _block_io_list->getHead();
	
	while(block_io_iter
	      &&  block_io_iter->entry->getBlock_rq_issue()->getLineNumber() < _io_finish_linenumber )
	{
		if(_io_start_linenumber < block_io_iter->entry->getBlock_rq_issue()->getLineNumber()
		   && block_io_iter->entry->getBlock_rq_complete()->getLineNumber())
		{
			return block_io_iter->entry;
		}
		
		block_io_iter = block_io_iter->next;
	}
	return NULL;
}

#define DEBUG_mapSystemcallIOBlockIO 0
int Parser::mapSystemcallIOBlockIO(List <ProcessInfo> *_process_list, List<BlockInfo> *_block_io_list)
{
	cout << endl << "started mapSystemcallIOBlockIO()" << endl;
	
	Node <ProcessInfo> *process_iter = _process_list->getHead();
	
	while (process_iter)
	{
		/// for direct IO which means the IO go to Media Stora
		Node <IOInfo> *file_io_iter = process_iter->entry->getDirectIOList()->getHead();
		
		while( file_io_iter )
		{
			if(file_io_iter->entry->getR_W_type() == "sys_read")
			{
#if DEBUG_mapSystemcallIOBlockIO
				file_io_iter->entry->printFromStartToFinish();
				cout << endl;
#endif
				BlockInfo *mapped_block_io = findBlockIOBetween(file_io_iter->entry->getStart()->getLineNumber(), file_io_iter->entry->getFinish()->getLineNumber(), _block_io_list);
				
				if( mapped_block_io == NULL)
				{
#if DEBUG_mapSystemcallIOBlockIO
					cout << endl << "problematic codes are the followings" << endl;
					file_io_iter->entry->printFromStartToFinish();
#endif
					cout << "No mapped block io exists?" << endl;
//					assert(0); // This is not an error.
					
					// new type of count required?
					file_io_iter = file_io_iter->next;
					continue;
				}
				else
				{
				
#if DEBUG_mapSystemcallIOBlockIO
					mapped_block_io->print();
					cout << endl;
#endif
					file_io_iter->entry->setMappedBlockIO(mapped_block_io);
					mapped_block_io->setMappedIO(file_io_iter);
				}
			}
			else if(file_io_iter->entry->getR_W_type() == "sys_write")
			{
				assert(0);
			}
			else
			{
				assert(0);
			}
			file_io_iter = file_io_iter->next;
		}
		
		process_iter = process_iter->next;
	}
	return 1;
}

int Parser::isInRange(double _range_start, double _event_start, double _event_finish, double _range_finish)
{
#if DEBUG_calculatePureIOHandlingTimeInViewsOfCPUExecutionTime
	cout << fixed;
	cout.precision(6);
#endif

	if( _range_start <= _event_start && _event_finish <= _range_finish)
	{
#if DEBUG_calculatePureIOHandlingTimeInViewsOfCPUExecutionTime
		cout << _range_start << "(1) " << _event_start << "(2) " << _event_finish << "(3) " << _range_finish << "(4) " << endl;
#endif
		return RET_IIR_POSITION_1_2_3_4;
	}
	else if (_range_start <= _event_start && _event_start < _range_finish && _range_finish < _event_finish) // normal case 3
	{
#if DEBUG_calculatePureIOHandlingTimeInViewsOfCPUExecutionTime
		cout << _range_start << "(1) " << _event_start << "(2) " << _range_finish << "(4) " << _event_finish << "(3) " << endl;
#endif
		return RET_IIR_POSITION_1_2_4_3;
	}
	else if ( _event_start < _range_start && _range_finish < _event_finish) // a reverse case versus 1
	{
#if DEBUG_calculatePureIOHandlingTimeInViewsOfCPUExecutionTime
		cout << _event_start << "(2) " << _range_start << "(1) " << _range_finish << "(4) " << _event_finish << "(3) " << endl;
#endif
		return RET_IIR_POSITION_2_1_4_3;
	}
	else if (_event_start < _range_start && _range_start < _event_finish && _event_finish <= _range_finish ) // normal case 2
	{
#if DEBUG_calculatePureIOHandlingTimeInViewsOfCPUExecutionTime
		cout << _event_start << "(2) " << _range_start << "(1) " << _event_finish << "(3) " << _range_finish << "(4) " << endl;
#endif
		return RET_IIR_POSITION_2_1_3_4;
	}
	else if (_range_finish < _event_start)
	{
#if DEBUG_calculatePureIOHandlingTimeInViewsOfCPUExecutionTime
		cout << _range_start << "(1) " << _range_finish << "(4) " << _event_start << "(2) " << _event_finish << "(3) " << endl;
#endif
		return RET_IIR_POSITION_1_4_2_3;
	}
	else if(_event_finish < _range_start)
	{
#if DEBUG_calculatePureIOHandlingTimeInViewsOfCPUExecutionTime
		cout << _event_start << "(2) " << _event_finish << "(3) " << _range_start << "(1) " << _range_finish << "(4) " << endl;
#endif
		return RET_IIR_POSITION_2_3_1_4;
	}
	else
	{
		return RET_IIR_NEVER_EXECUTE;
	}
}


void Parser::calculatePureIOHandlingTimeInViewsOfCPUExecutionTime(List <ProcessInfo> *_process_list)
{
#if DEBUG_calculatePureIOHandlingTimeInViewsOfCPUExecutionTime
	cout << endl << "calculatePureIOHandlingTimeInViewsOfCPUExecutionTime started" << endl;
#endif

	Node <ProcessInfo> *process_iter = _process_list->getHead();
	
	int status = 0;
	
	// traver file io through processes
	while(process_iter)
	{
		
#if DEBUG_calculatePureIOHandlingTimeInViewsOfCPUExecutionTime
#if 1
		if( process_iter->entry->getProcessName() == "sdcard")
			cout << "stop to debug" << endl;
#endif
#endif
		if( process_iter->entry->getFileIOList()->getSize() == 0 )
		{
			process_iter = process_iter->next;
			continue;
		}
#if DEBUG_calculatePureIOHandlingTimeInViewsOfCPUExecutionTime
		cout << endl << "process name: " << process_iter->entry->getProcessName() << "(" << process_iter->entry->getProcessId() << ")" << endl;
		cout << "process execution time logs (" << process_iter->entry->getLogList()->getSize() << ") are tested if the sequence in is time order" << endl;
		cout << "process cpu execution time event: " << process_iter->entry->getCpuExecutionTime()->getSize() << endl;
		cout << "process file i/o: " << process_iter->entry->getFileIOList()->getSize() << endl;
#endif
		// cpu execution time 이벤트는 반드시 시간 오름차순이여야 함
		Node <ParseEntry> *log_iter = process_iter->entry->getLogList()->getHead();
		while( log_iter )
		{
			if(log_iter->next)
			{
				if(log_iter->entry->getApproxTimeStamp() > log_iter->next->entry->getApproxTimeStamp() )
				{
#if DEBUG_calculatePureIOHandlingTimeInViewsOfCPUExecutionTime
					log_iter->entry->print();
					log_iter->next->entry->print();
#endif
//					assert(0);
				}
			}
			log_iter = log_iter->next;
		}
		
		// 최초 파일 I/O가 나타나는 CPU execution time을 찾는다.
		Node <StartFinishTimeInfo> *cpu_exec_iter = process_iter->entry->getCpuExecutionTime()->getHead();
		Node <IOInfo> *first_file_io = process_iter->entry->getFileIOList()->getHead();
		
		while(cpu_exec_iter)
		{
			if(cpu_exec_iter->entry->getStart()->getApproxTimeStamp() < first_file_io->entry->getStart()->getApproxTimeStamp()
			   && first_file_io->entry->getFinish()->getApproxTimeStamp() < cpu_exec_iter->entry->getFinish()->getApproxTimeStamp()
			   )
			{
#if DEBUG_calculatePureIOHandlingTimeInViewsOfCPUExecutionTime
				cout << "the first file IO in this cpu exec event" << endl;
				first_file_io->entry->getStart()->print();
				cout << "the cpu exec event" << endl;
				cpu_exec_iter->entry->print();
				cout << "- starting to find cpu execution time particles in the file I/O" << endl;
#endif
				break;
			}
			cpu_exec_iter = cpu_exec_iter->next;
		}
		
		/// variables initialization
		Node <IOInfo> *file_io_iter = process_iter->entry->getFileIOList()->getHead();
		
		Node <StartFinishTimeInfo> * execution_time_iter = process_iter->entry->getCpuExecutionTime()->getHead();
		
		//		Node <InterruptInfo> **interrupt_iter_in_cpu = new Node <InterruptInfo> *[MaxCpuNum];
		//
		//		for(int i =0; i< MaxCpuNum; i++)
		//		{
		//			interrupt_iter_in_cpu[i]= CPU->getInterruptTime(i)->getHex`ad();
		//		}
		
		/// routine for calculating pure IO hadnling time
		while( file_io_iter && execution_time_iter )
		{
#if DEBUG_calculatePureIOHandlingTimeInViewsOfCPUExecutionTime
			if ( file_io_iter->entry->getStart()->getLineNumber() == 1775688 )
				cout << "stop to debug: line number " << file_io_iter->entry->getStart()->getLineNumber() << endl;

#endif
			double io_start = file_io_iter->entry->getStart()->getApproxTimeStamp();
			double io_finish = file_io_iter->entry->getFinish()->getApproxTimeStamp();
			
			double execution_start = execution_time_iter->entry->getStart()->getApproxTimeStamp();
			double execution_finish = execution_time_iter->entry->getFinish()->getApproxTimeStamp();
			
#if DEBUG_calculatePureIOHandlingTimeInViewsOfCPUExecutionTime
			cout << endl << "file io info:" << endl;
			file_io_iter->entry->print();
			cout << "execution time info:" << endl;
			execution_time_iter->entry->print();
#endif
			
			status = isInRange (execution_start, io_start, io_finish, execution_finish);
			if(status == RET_IIR_POSITION_1_2_4_3)
			{
#if DEBUG_calculatePureIOHandlingTimeInViewsOfCPUExecutionTime
				cout << "[pattern1] the first cpu execution time part is found for the io" << endl;
#endif
				Node <StartFinishTimeInfo> *pure_io_handling_time = new Node <StartFinishTimeInfo> (file_io_iter->entry->getIoStartLog(), execution_time_iter->entry->getIoFinishLog());
				
				file_io_iter->entry->getPureIoHandlingParticle()->push_back(pure_io_handling_time);
				
				file_io_iter->entry->addPureIoHandlingTime(execution_finish - io_start);
				
				execution_time_iter = execution_time_iter->next;
				continue;
			}
			else if (status == RET_IIR_POSITION_2_1_4_3)
			{
#if DEBUG_calculatePureIOHandlingTimeInViewsOfCPUExecutionTime
				cout << "[pattern2] more cpu execution time part is required" << endl;
#endif
				
				Node <StartFinishTimeInfo> *pure_io_handling_time = new Node <StartFinishTimeInfo> (execution_time_iter->entry->getIoStartLog(), execution_time_iter->entry->getIoFinishLog());
				
				file_io_iter->entry->getPureIoHandlingParticle()->push_back(pure_io_handling_time);
				
				file_io_iter->entry->addPureIoHandlingTime(execution_finish - execution_start);
				
				execution_time_iter = execution_time_iter->next;
				continue;
			}
			else if(status == RET_IIR_POSITION_2_1_3_4)
			{
#if DEBUG_calculatePureIOHandlingTimeInViewsOfCPUExecutionTime
				cout << "[pattern3] final cpu execution part is found" << endl;
#endif
				
				Node <StartFinishTimeInfo> *pure_io_handling_time = new Node <StartFinishTimeInfo> (execution_time_iter->entry->getIoStartLog(), file_io_iter->entry->getIoFinishLog());
				
				file_io_iter->entry->getPureIoHandlingParticle()->push_back(pure_io_handling_time);

				file_io_iter->entry->addPureIoHandlingTime(io_finish - execution_start);
				process_iter->entry->addTotalFileIoHandlingTime(io_finish - execution_start);
				
				file_io_iter = file_io_iter->next;
				
				continue;
			}
			else if(status == RET_IIR_POSITION_1_2_3_4)
			{
#if DEBUG_calculatePureIOHandlingTimeInViewsOfCPUExecutionTime
				cout << "[pattern4] only 1 cpu execution part is included in the file io time period" << endl;
#endif
				Node <StartFinishTimeInfo> *pure_io_handling_time = new Node <StartFinishTimeInfo> (file_io_iter->entry->getIoStartLog(), file_io_iter->entry->getIoFinishLog());
				
				file_io_iter->entry->getPureIoHandlingParticle()->push_back(pure_io_handling_time);
				
				file_io_iter->entry->addPureIoHandlingTime(io_finish - io_start);
				process_iter->entry->addTotalFileIoHandlingTime(io_finish - io_start);
				
				file_io_iter = file_io_iter->next;
				continue;
			}
			else if (status == RET_IIR_POSITION_1_4_2_3) // exec before io
			{
#if DEBUG_calculatePureIOHandlingTimeInViewsOfCPUExecutionTime
				cout << "[pattern5] exec before io" << endl;
#endif
				execution_time_iter = execution_time_iter->next;
				continue;
				
			}
			else if (status == RET_IIR_POSITION_2_3_1_4) // io before exec
			{
#if DEBUG_calculatePureIOHandlingTimeInViewsOfCPUExecutionTime
				cout << "[pattern6] io before exec" << endl;
#endif
				Node <IOInfo> * tmp = NULL;
				process_iter->entry->getFileIOList()->extractNode(file_io_iter, &tmp);
				
				// incomplete file io occured due to the log miss in the CPU execution log (very important)
				cout << endl << "WARNING: incomplete file io occured due to the log miss in the CPU execution log (very important)" << endl;
				file_io_iter->entry->print();
				
				file_io_iter = tmp;
				
				continue;
			}
			else
			{
#if DEBUG_calculatePureIOHandlingTimeInViewsOfCPUExecutionTime
				cout << "[pattern7] uncategoried pattern" << endl;
#endif
			}
			
			execution_time_iter = execution_time_iter->next;
		}
		
		
//		// debug routine
//		file_io_iter = process_iter->entry->getFileIOList()->getHead();
//		ofstream debug;
//		debug.open("debug/file_io.txt");
//		while(file_io_iter)
//		{
//			debug << "file io" << endl;
//			file_io_iter->entry->print(debug);
//			file_io_iter->entry->printPureIoHandling(debug);
//			
//			file_io_iter = file_io_iter->next;
//		}
//		debug.close();
		
		process_iter = process_iter->next;
	}
	
}

void Parser::calculatePureIOHandlingTimeExcludingInterrupt(List <ProcessInfo> *_process_list)
{
	cout << endl << "calculatePureIOHandlingTimeExcludingInterrupt started" << endl;
	
#if DEBUG_calculatePureIOHandlingTimeExcludingInterrupt
	cout << fixed;
	cout.precision(7);
#endif
	int status = 0;
	
	/// 프로세스마다
	Node <ProcessInfo> *process_iter = _process_list->getHead();
	
	while( process_iter )
	{
#if DEBUG_calculatePureIOHandlingTimeInViewsOfCPUExecutionTime
#if 1
		if( process_iter->entry->getProcessName() == "sdcard")
			cout << "stop to debug" << endl;
#endif
#endif

		if( process_iter->entry->getFileIOList()->getSize() == 0 )
		{
			process_iter = process_iter->next;
			continue;
		}

#if DEBUG_calculatePureIOHandlingTimeExcludingInterrupt
		cout << endl << "process name: " << process_iter->entry->getProcessName() << endl;
		cout << "process's file io count: " << process_iter->entry->getFileIOList()->getSize() << endl;
#endif
		Node <IOInfo> * file_io_iter = process_iter->entry->getFileIOList()->getHead();
		
//		// test
//		Node <ParseEntry> *process_log_iter = file_io_iter->entry->getIoStartLog();
//		while(process_log_iter)
//		{
//			process_log_iter->entry->print();
//			process_log_iter = process_log_iter->next;
//		}
		
		/// 프로세스의 파일 I/O 마다
		while( file_io_iter )
		{
#if DEBUG_calculatePureIOHandlingTimeExcludingInterrupt
			cout << endl << "file io start : " << endl;
			file_io_iter->entry->getStart()->print();
			file_io_iter->entry->getFinish()->print();
			cout << endl;
#endif
			/// 프로세스의 파일 I/O의 CPU  execution time 조각 마다
			Node <ParseEntry> *process_log_iter = file_io_iter->entry->getIoStartLog();
			Node <ParseEntry> *file_io_finish = file_io_iter->entry->getIoFinishLog();
			
			while(process_log_iter != file_io_finish)
			{
#if DEBUG_calculatePureIOHandlingTimeExcludingInterrupt
				process_log_iter->entry->print();
#endif
				int interrupt_event = identifyInterruptEvent(process_log_iter->entry);
				
				if(interrupt_event == RET_IIE_IRQ_START)
				{
					Node <ParseEntry> *irq_end = NULL;
					status = findMappedIrqFinish(process_log_iter, &irq_end);
					if(status < 0)
					{
						assert(0);
					}
					
					Node <InterruptInfo> *irq_new_item = new Node <InterruptInfo> (process_log_iter, irq_end);
					
					file_io_iter->entry->getInterrupt()->push_back(irq_new_item);
					file_io_iter->entry->addTotalInterruptHandlingTime(irq_end->entry->getApproxTimeStamp() - process_log_iter->entry->getApproxTimeStamp());
					process_iter->entry->addTotalInterruptHandlingtimeInFileIO(irq_end->entry->getApproxTimeStamp() - process_log_iter->entry->getApproxTimeStamp());
#if DEBUG_calculatePureIOHandlingTimeExcludingInterrupt
					cout << "- interrupt info -" << endl;
#endif
				}
				else if(interrupt_event == RET_IIE_SOFTIRQ_START)
				{
					Node <ParseEntry> *soft_irq_end = NULL;

					status = findMappedSoftIrqFinish(process_log_iter, &soft_irq_end);
					if(status < 0)
					{
						assert(0);
					}
					
					Node <SoftInterruptInfo> *irq_new_item = new Node <SoftInterruptInfo> (process_log_iter, soft_irq_end);
					
					file_io_iter->entry->getSoftInterrupt()->push_back(irq_new_item);
					file_io_iter->entry->addTotalSoftInterruptHandlingTime(soft_irq_end->entry->getApproxTimeStamp() - process_log_iter->entry->getApproxTimeStamp());
					process_iter->entry->addTotalSoftIrqHandlingtimeInFileIO(soft_irq_end->entry->getApproxTimeStamp() - process_log_iter->entry->getApproxTimeStamp());
#if DEBUG_calculatePureIOHandlingTimeExcludingInterrupt
					cout << "- soft interrupt info -" << endl;
#endif

				}
				
				process_log_iter = process_log_iter->next;
			}
			
#if DEBUG_calculatePureIOHandlingTimeExcludingInterrupt
			double io_response_time = file_io_iter->entry->getFinish()->getApproxTimeStamp()- file_io_iter->entry->getStart()->getApproxTimeStamp();
			double pure_io_response_time = file_io_iter->entry->getPureIoHandlingTime();
			double total_interrupt_time = file_io_iter->entry->getTotalInterruptHandlingTime();
			double total_soft_interrupt_time = file_io_iter->entry->getTotalSoftInterruptHandlingTime();
			
			double pure_io_response_time_execluing_iterrrupt = pure_io_response_time - total_interrupt_time - total_soft_interrupt_time;
			
			if (file_io_iter->entry->getInterrupt()->getSize())
			{
				cout << "This file IO has " << file_io_iter->entry->getInterrupt()->getSize() << " interrupts" << endl;
				cout << "Total interrupt handling time: " << file_io_iter->entry->getTotalInterruptHandlingTime() << endl;
			}
			if (file_io_iter->entry->getSoftInterrupt()->getSize())
			{
				cout << "This file IO has " << file_io_iter->entry->getSoftInterrupt()->getSize() << " soft interrupts" << endl;
				cout << "Total soft interrupt handling time: " << file_io_iter->entry->getTotalSoftInterruptHandlingTime() << endl;
			}
#endif
			if(file_io_iter->entry->getInterrupt()->getSize() || file_io_iter->entry->getSoftInterrupt()->getSize())
			{
				CntTotalFileIoWithInterrupt++;
				
#if DEBUG_calculatePureIOHandlingTimeExcludingInterrupt
				cout << endl << "IO response time: " << io_response_time << endl;
				cout << "Pure IO resp time: " << pure_io_response_time;
				cout.precision(2);
				cout << "(" <<  (100 *pure_io_response_time) / io_response_time << "%, PureIO:IO )" << endl;
				cout.precision(7);
				cout << "Pure IO resp time execluding interrupt: " << pure_io_response_time_execluing_iterrrupt;
				cout.precision(2);
				cout << "( " << 100 * pure_io_response_time_execluing_iterrrupt / io_response_time << "%, PureIONoIRQ:IO / " << 100 * pure_io_response_time_execluing_iterrrupt / pure_io_response_time << "%, PureIONoIRQ:PureIO )"<< endl;
				cout.precision(7);
#endif
			}
			file_io_iter = file_io_iter->next;
		}
		

		process_iter = process_iter->next;
	}
}

void Parser::calculateAddIOHandlingTimeInEMMCtoPureIO(List <ProcessInfo> *_process_list)
{
	int status = 0;
	
	Node <ProcessInfo> *process_iter = _process_list->getHead();
	
	while (process_iter)
	{
		Node <IOInfo> *file_io_iter = process_iter->entry->getFileIOList()->getHead();
		
		while(file_io_iter)
		{
			Node <ParseEntry> *parse_node_iter = file_io_iter->entry->getIoStartLog();
			Node <ParseEntry> *parse_node_io_finish = file_io_iter->entry->getIoFinishLog();
			
			while( parse_node_iter !=  parse_node_io_finish)
			{
				status = isEmmcLevelEvent(parse_node_iter->entry );
				if( status == RET_IELE_NO_EMMC_EVENT)
				{
					parse_node_iter = parse_node_iter->next;
					continue;
				}
				else if (status == RET_IELE_MMC_START_REQUEST)
				{
#if DEBUG_calculateAddIOHandlingTimeInEMMCtoPureIO
					file_io_iter->entry->getStart()->print();
					parse_node_iter->entry->print();
					parse_node_io_finish->entry->print();
#endif
					assert(0);
				}
				else if (status == RET_IELE_MMC_REQUEST_DONE)
				{
#if DEBUG_calculateAddIOHandlingTimeInEMMCtoPureIO
//					file_io_iter->entry->getStart()->print();
//					parse_node_iter->entry->print();
//					parse_node_io_finish->entry->print();
#endif
					parse_node_iter = parse_node_iter->next;
					continue;
				}
				
				parse_node_iter = parse_node_iter->next;
			}
				
			file_io_iter = file_io_iter->next;
		}
		process_iter = process_iter->next;
	}
}

int Parser::findProcessWithName(const string &_process_name, List <ProcessInfo> *_process_list, ProcessInfo **_process)
{
	Node <ProcessInfo> *process_iter = _process_list->getHead();
	
	
	while(process_iter)
	{
#if DEBUG_findProcessWithName
		if( process_iter->entry->getProcessId() == DEBUG_findProcessWithName_TARGET_PID)
			cout << "test" << endl;
#endif
		// find the target process with the _process name, parameter
		//		if (_process_name == process_iter->entry->getProcessName() )
		if( strstr(process_iter->entry->getProcessName().c_str(), _process_name.c_str()) )
		{
			*_process = process_iter->entry;
			return RET_FPWN_FOUND;
		}
		process_iter = process_iter->next;
	}
	
	return RET_FPWN_NOT_FOUND;
}

void Parser::printVFSInfo()
{

}

// assuming that target_processes include all the processes to be printed out in a file having name with "Output File Name"
void Parser::generateVFSInfo( List <ProcessInfo> *target_processes)
{
	Node <ProcessInfo> *target_process = NULL;
	List <IOInfo> *list_fileio = NULL;
	Node <IOInfo> *iter = NULL;

	long cnt_vfs_io = 0;

	string OutputFileName;
	cout << "Output file Name: ";
	cin >> OutputFileName;

	string FilePath = "result/" + OutputFileName;
	FILE * fout = fopen(FilePath.c_str(), "w");

	target_process = target_processes->getHead();

	while(target_process)
	{
		cout << target_process->entry->getProcessName() << "/" << target_process->entry->getProcessId() << endl;
		list_fileio = target_process->entry->getUnknownIOList();

		iter = list_fileio->getHead();
		while ( iter )
		{
			if(iter->entry->getFileName() != "")
			{
				cnt_vfs_io++;
				fprintf(fout, "%f,%s,%s,%ld,%ld\n",
						iter->entry->getStart()->getTimeStamp(), // time
						target_process->entry->getProcessName().c_str(), // process name
						iter->entry->getFileName().c_str(), // file name
						iter->entry->getSize(), // access size
						iter->entry->getPos()// offset
						);
			}
			else
				printf("%f,%s,%s,%ld,%ld\n",
					   iter->entry->getStart()->getTimeStamp(), // time
					   target_process->entry->getProcessName().c_str(), // process name
					   iter->entry->getFileName().c_str(), // file name
					   iter->entry->getSize(), // access size
					   iter->entry->getPos()// offset
					   );
			
			iter = iter->next;
		}
		target_process = target_process->next;
	}

	cout << endl << "========= VFS Info generated ( " << cnt_vfs_io << " IOs)=========" << endl;
	fclose(fout);
}


void Parser::parseLog()
{
	long modified_entry = 0;
	approximateTime_inParseList(&modified_entry);

	/// ParseEntry arrangement in a view of CPU
	groupCpuParseList();

	/// ParseEntry arrangement in a view of Process
	process_list = new List <ProcessInfo> ();
    groupProcessParseList( process_list );
    fillProcessName( process_list );

	string conf_file = "configuration/IO_build_rule.input";
	loadIORule(conf_file);
	collectIORequestEventForProcesses(process_list);

	/// block info collected
	block_io_list = new List <BlockInfo> ();
	block_incomplete_io_list = new List <BlockInfo> ();
	List <ParseEntry>* all_block_event_list = new List <ParseEntry> ();
	collectBlockEvent(all_block_event_list, block_io_list, block_incomplete_io_list);
	delete all_block_event_list;


	/// emmc info collected
	eMMC_io_list = new List <eMMCInfo> ();
	eMMC_incomplete_io_list = new List <eMMCInfo> ();

	List <ParseEntry> * all_emmc_event_list = new List <ParseEntry> ();
	collecteMMCEvent(all_emmc_event_list, eMMC_io_list, eMMC_incomplete_io_list);
	delete all_emmc_event_list;


	/// Mapping

	/// block <-> emmc mapping (1. normal mapping)
	mapBlockNeMMCEvent(block_io_list, eMMC_io_list);

	/// block <-> emmc mapping (2. packed cmd mapping)
	findPackedBlockIO(block_io_list, eMMC_io_list);

	/// system call IO <-> block IO mapping
	mapSystemcallIOBlockIO(process_list, block_io_list);

	/// block IO and eMMC IO mapping integrity check
	// parser.checkAlleMMCIOmappedBlockIO(eMMC_io_list);


	/// other events collected
	/// 1. CPU execution time
	collectExecutimeTimeEvent(process_list);
	/// 2. Interrupt Event
	collectInterruptEvent();


	/// calulcating Pure IO Handling Time of File I/O
	calculatePureIOHandlingTimeInViewsOfCPUExecutionTime(process_list);
	calculatePureIOHandlingTimeExcludingInterrupt(process_list);


	/// calulcating Pure IO Handling Time of Block I/O
	calculatePureIOBlockHandlingTimeInViewsOfCPUExecutionTime(block_io_list, process_list);

	calculateAddIOHandlingTimeInEMMCtoPureIO(process_list);

	showParsingResult();
}

void Parser::genPrefetchingCode()
{
	cout << "generating prefetch codes" << endl;

    genParentChildTree();
    menuWithProcessList(process_list);
}

List <ProcessInfo> * Parser::searchProcessesWithName(List <ProcessInfo> *p_list, string pname, List <ProcessInfo> *t_list)
{
	Node <ProcessInfo> * p_iter = p_list->getHead();

	while(p_iter)
	{
		if(p_iter->entry->getProcessName() == pname)
		{
			t_list->push_back(p_iter);
		}
		p_iter = p_iter->next;
	}

	return t_list;
}


void printTotalSizeofReadinVFS( Node <ProcessInfo> *target_process )
{
	char menu;
	Node<IOInfo> *iter = target_process->entry->getUnknownIOList()
	->getHead();
	long iosize = 0;

	cout << "0. excluding file with noname?" << endl;
	cout << "1. including file with noname?" << endl;
	cin >> menu;

	while(iter != NULL)
	{
		if(iter->entry->getFileName() != "" || menu)
			iosize += iter->entry->getSize();

		iter = iter->next;
	}

	cout << "the total size of file io: "  << iosize << " Byte" << endl;
}

void Parser::printTimeComposition( Node <ProcessInfo> *target_process)
{
	cout << endl << "total cpu execution time: " << target_process->entry->getTotalCpuExecutionTime() << endl;
	cout << "total non file io handling time: " << target_process->entry->getTotalNonFileIoHandlingTime() << endl;
	cout << "total file io handling itme: " << target_process->entry->getTotalFileIoHandlingTime() << endl;
	cout << "total pure file io handling time: " << target_process->entry->getTotalPureFileIoHandlingTime() << endl;
	cout << "total interrupt handling time: " << target_process->entry->getTotalInterruptHandlingtimeInFileIO() << endl;
	cout << "total soft irq handling time: " << target_process->entry->getTotalSoftIrqHandlingtimeInFileIO() << endl;
}

#define DIV_PIECE 10
Node <ParseEntry> * Parser::searchParseEntrywithTimeStamp( List <ParseEntry> *parse_list, double tp)
{
	Node <ParseEntry> *iter = parse_list->getTail();
	Node <ParseEntry> *checkpoint = NULL;
	long jump_cnt = 0;

	if(iter == NULL)
		return NULL;

	if(tp < parse_list->getHead()->entry->getTimeStamp()
	   || parse_list->getTail()->entry->getTimeStamp() < tp)
		return NULL;

	if(TotalLogLine < DIV_PIECE)
		jump_cnt = 1;
	else
		jump_cnt = TotalLogLine / DIV_PIECE;

	// coarse grained backword search
	while(iter != NULL)
	{
		if(iter->entry->getTimeStamp() < tp)
		{
			checkpoint = iter;
			break;
		}

		iter = iter->jumpPrev(jump_cnt);
	}

	if(iter == NULL)
		checkpoint = parse_list->getHead();

	// fine grained forward search

	iter = checkpoint;

	while(1)
	{
		if(tp == iter->entry->getTimeStamp())
		{
			iter->entry->printParseEntry();
			iter->next->entry->printParseEntry();
			iter->next->next->entry->printParseEntry();
			iter->next->next->next->entry->printParseEntry();
			iter->next->next->next->next->entry->printParseEntry();
			break;
		}

		iter = iter->next;
	}

	return iter;
}

#define DEBUG_recursiveCollectPID 0
/**
1. compare with the values of parent pid with and of child pid

- if they are same, done
- else if parent pid and child pid are different, then queue the process in the queue searched

2. recursively call self with iter->second (might be child pid)
- the same searching will be done with different pid (child)

 @param parent_pid the first process searching started from
 @param list_process searching is doing in the list_process
 @param target_processes processes searched as a result go into this list (rw)
 */
void Parser::recursiveCollectPID(int parent_pid, List <ProcessInfo> *list_process, List <ProcessInfo> *target_processes )
{
	Node <ProcessInfo> *target_process = NULL;
    
    // find target process with pid (no exception)
    list_process->searchNodeWithKey(parent_pid, &target_process);
    if(target_process  == NULL)
        assert(0);
    
    // insert a found target process into the list
    target_processes->copy_and_push_back(target_process);
    
    // iterate table
    // if find a entry with parent_pid in iter_pair.first
    // then call recursively with the new parent_pid (iter_pair.second)
    
    multimap<int,int>::iterator iter;
    for ( iter = )
    {
        if( )
        {
            recursiveCollectPID(iter->second, list_process, target_processes);
        }
    }

	// no results
	if( iter_pair.first == iter_pair.second ) {
		return;
	} else { // results exits
		
		for( iter = iter_pair.first ; iter != iter_pair.second ; iter++)
		{
			
			recursiveCollectPID(iter->second, list_process, target_processes);
		}
	}
}



void Parser::showProcesses(List <ProcessInfo> * list_process)
{
	Node<ProcessInfo> *p_iter = list_process->getHead();
	while (p_iter)
	{
		cout << p_iter->entry->getProcessName() << "/" << p_iter->entry->getProcessId() << endl;
		p_iter = p_iter->next;
	}
}



void Parser::searchPID(List <ProcessInfo> * list_process)
{
	int pid = 0;
	Node<ProcessInfo> *target_process = NULL;

	cout << "pid to search: ";
	cin >> pid;

	if (list_process->searchNodeWithKey(pid, &target_process) == RET_SNWK_NOT_FOUND)
		cout << "process [" << pid << "] isn't found" << endl;
	else
		cout << "the process exist" << endl;
}

/**
1. input pid

2. search the process having pid in the list_process

3. select a mode for generating prefetching code
	- target process only
	- include all child processes

4. write a prefetching code. 

@brief set pid of process for a writing prefetching code
@param list_process List <ProcessInfo> * list_process
@return Node <ProcessInfo> * target_processes
*/
#define DEBUG_setPIDtoPrefetch 1
List <ProcessInfo> *Parser::setPIDtoPrefetch( List <ProcessInfo> * list_process )
{
	Node <ProcessInfo> *target_process = NULL;
	List <ProcessInfo> *target_processes = new List <ProcessInfo> ();

	cout << "pid: ";
	int pid = 0;
	cin >> pid;
	cout << endl;

	list_process->searchNodeWithKey(pid, &target_process);
	if (target_process == NULL)
	{
		cout << "no process of the pid" << endl;
		return target_processes;
	}
	cout << "target_process name: " << target_process->entry->getProcessName() << endl;
	cout << "target_process pid: " << target_process->entry->getProcessId() << endl;
	target_processes->copy_and_push_back(target_process);

	cout << endl;
	cout << "1. target process" << endl;
	cout << "2. target process + all children" << endl;
	cout << ": ";

	int option = 0;
	cin >> option;
	cout << endl;

#define TARGET_PROCESS_ONLY 1 
#define INCLUDE_CHILD_PRCESSES 2
	switch (option)
	{
	// assuming that pid is a unique key
	case TARGET_PROCESS_ONLY:
		cout << "not implemented yet" << endl;
		break;
	case INCLUDE_CHILD_PRCESSES:
		cout << "process list's size: #" << process_list->getSize() << endl;
            
		// process_list->printList();
		recursiveCollectPID(pid, process_list, target_processes);
		cout << "the number of all the process: #" << target_processes->getSize() << endl;
		break;
	}
#if DEBUG_setPIDtoPrefetch
    target_processes->printList();
#endif
	return target_processes;
}

void Parser::showTargetProcesses(List<ProcessInfo> *target_processes)
{
	cout << "show the list in the target processes" << endl;
	Node<ProcessInfo> *p_iter = target_processes->getHead();
        
	while (p_iter)
	{
		cout << p_iter->entry->getProcessName() << "/" << p_iter->entry->getProcessId() << endl;
		p_iter = p_iter->next;
	}
}

void Parser::searchProcessAndPrintProcessInfo(string process_name)
{
	Node <ProcessInfo> *p_iter = process_list->getHead();
	while(p_iter)
	{
		if(p_iter->entry->getProcessName() == process_name)
		{
			cout << "matched process name: " << p_iter->entry->getProcessName() << endl;
			cout << "matched process id: " << p_iter->entry->getProcessId() << endl;
			cout << endl;
		}
		p_iter = p_iter->next;
	}
}


enum MenusInPrefetchingCode {
    LIST_PROCESSES = 1,         // 1
    SEARCH_PID,                 // 2
    SEARCH_PROCESSNAME,         // 3
    SET_TARGET,                 // 4
    SHOW_TARGET_PROCESSES,      // 5
    WRITE_PREFETCHING_CODES,    // 6
    EXIT                        // 7
};



#define DEBUG_generatePrefetchingCodes 1
void Parser::menuWithProcessList( List <ProcessInfo> * list_process)
{
    int mode = (int)EXIT;
    
	do
	{
        cout << endl << "select a target process" << endl;
        cout << LIST_PROCESSES << ". to list all the process info" << endl;
        cout << SEARCH_PID << ". search the process with pid" << endl;
        cout << SEARCH_PROCESSNAME << ". search the process with process name" << endl;
        cout << SET_TARGET << ". set target with pid" << endl;
        cout << SHOW_TARGET_PROCESSES << ". print out the processes in the target list" << endl;
        cout << WRITE_PREFETCHING_CODES << ". write a prefetching code" << endl;
        cout << EXIT << ". quit" << endl;
        cout << ": ";
        
        cin >> mode;
        cout << endl;
        
        List <ProcessInfo> *target_processes = NULL;
        
        switch (mode)
        {
            case LIST_PROCESSES:
            {
                showProcesses(list_process);
                break;
            }
                
            case SEARCH_PID:
            {
                searchPID(list_process);
                break;
            }
                
            case SET_TARGET:
            {                
                target_processes = setPIDtoPrefetch(list_process);
                break;
            }
            case SEARCH_PROCESSNAME:
            {
                string process_name;
                cout << "process name to search: ";
                cin >> process_name;
                searchProcessAndPrintProcessInfo(process_name);
                
                break;
            }
            case SHOW_TARGET_PROCESSES:
            {
            	if (target_processes == NULL)
            		target_processes = setPIDtoPrefetch(list_process);

                showTargetProcesses(target_processes);
                break;
            }
            case WRITE_PREFETCHING_CODES:
            {
                if (target_processes == NULL)
                {
                    cout << "target processes is NUL. you should input pid to generate prefetch code" << endl;
                    target_processes = setPIDtoPrefetch(list_process);
                }
                
                createPrefetcherCode(PREFECHING_CODE, target_processes);
                break;
            }
                
            case EXIT:
            {
                break;
            }
                
            default:
            {
                cout << "input correct menu id" << endl;
                break;
            }
        }
	} while (mode != EXIT);
}



// output the code parts
void Parser::openAndreadWholeFile(ofstream &prefetcher_code, string fd_name, size_t fd_count, string filename, Node <ParseEntry> *log_iter, int use_posix_fadvise)
{
		prefetcher_code << "\tint " << fd_name + to_string(fd_count) <<
		" = open (\"" << filename << "\", O_RDONLY); // system call, LN-" << log_iter->entry->getLineNumber() << endl;
		prefetcher_code << "\tif( " << fd_name + to_string(fd_count) << " < 0 ) { open_file_error_count++;" << endl;
		prefetcher_code << "\tprintf(\"open error: " << filename << "\\n\"); }" << endl;
		prefetcher_code << "\telse {" << endl;
		prefetcher_code << "\t\topen_file_count++;" << endl;
		prefetcher_code << "\t\tprintf(\"reading " << filename << "\\n\");" << endl;
		prefetcher_code << endl;
	
		if(use_posix_fadvise == 1) // 1 = posix_fadvise, 2 = read
		{
			prefetcher_code << "\t\tstatus = fstat(" << fd_name + to_string(fd_count) << ", &s);" << endl;
			prefetcher_code << "\t\tsize = s.st_size;" << endl;

			prefetcher_code << "\t\tposix_fadvise(" << fd_name + to_string(fd_count) << ", " << "size" << ", " << "0" << ", POSIX_FADV_WILLNEED); // system call, LN-" << log_iter->entry->getLineNumber() << endl;
		}
		else {
			prefetcher_code << "\t\twhile( read(" << fd_name + to_string(fd_count) << ", buf, BUFF_SIZE) == BUFF_SIZE ) printf(\".\");" << endl;
			prefetcher_code << "\t\tprintf(\"\\n\");" << endl;
		}
		prefetcher_code << "\t}" << endl;
}

void Parser::writePrefetchingCodeWithOpenedFile(ofstream &prefetcher_code, string filename, Node <ParseEntry> *log_iter, map <string, string> &file_table, int use_posix_fadvise)
{
	string fd_name = "fd";
	size_t fd_count = file_table.size();
	map <string, string>::iterator fd_iter;

	// find file entries in the file list
	fd_iter = file_table.find(filename);

	if(file_table.end() == fd_iter) { // no search results
      	file_table.insert(make_pair(filename, fd_name + to_string(fd_count)));		
      	openAndreadWholeFile(prefetcher_code, fd_name, fd_count, filename, log_iter, use_posix_fadvise);
	}
}

bool Parser::isExcluded(string filename) {
	if (filename.find("/dev/") != std::string::npos
		|| filename.find("/sys/") != std::string::npos
		|| filename.find("/proc/") != std::string::npos )
	{
		cout << filename << " is excluded" << endl;
		return 1;
	}
	else
		return 0;
}

void Parser::generatePrefetcherCodesWithOpenFiles(List <ProcessInfo> *target_processes, map <string, string> &file_table)
{
	Node <ProcessInfo> * process_iter = target_processes->getHead();
	int use_posix_fadvise = 0;

	cout << endl;
	cout << "use posix_fadvise() or lseek() & read()" << endl;
	cout << "1. posix_fadvise()" << endl;
	cout << "2. read() (no need of lseek)" << endl;
	cout << ": ";
    
	cin >> use_posix_fadvise;
	cout << endl;

	prefetcher_code << "\tstruct stat s;" << endl;
	prefetcher_code << "\tint status;" << endl;
	prefetcher_code << "\tsize_t size;" << endl;

	while(process_iter)
	{
		List <ParseEntry> *log_list = process_iter->entry->getLogList();
		Node <ParseEntry> *log_iter = log_list->getHead();
		string log;
		while( log_iter)
		{
			log = log_iter->entry->getLogMsg();
			iotype type = checkLogType(log);

			switch(type)
			{
				case NONE:
				{
					assert(0);
					break;
				}
				case PASS:
				{
					assert(1);
					break;
				}
				case OPEN:
				{
					regex log_regex("openfile=(.*)"); // file=ptmx size=16384 *pos=0
					string filename_replace("$1");
					string  filename = regex_replace(log, log_regex, filename_replace, regex_constants::format_no_copy);
					if (filename == "")
					{
						goto inc_iter;
					}
					else
					{
						if(isExcluded(filename))
							goto inc_iter;
						else
						{
							writePrefetchingCodeWithOpenedFile(prefetcher_code, filename, log_iter, file_table, use_posix_fadvise);
						}
					}
					break;
				}



				case FADVISE64_64:
				case FADVISE64:
				case VFSCALL:
				{
					regex systemcall_regex("file=([a-zA-Z0-9.\\-_:/]*) size=([0-9]+) \\*pos=([0-9]+)"); // file=ptmx size=16384 *pos=0
					string filename_replace("$1"), pos_replace("$3"), size_replace("$2");
					string  filename = regex_replace(log, systemcall_regex, filename_replace, regex_constants::format_no_copy);

					if (filename == "")
					{
						goto inc_iter;
					}
					else
					{
						if(isExcluded(filename))
							goto inc_iter;
						else
						{
							writePrefetchingCodeWithOpenedFile(prefetcher_code, filename, log_iter, file_table, use_posix_fadvise);
						}
					}

					break;
				}

				case MMAP:
				{
					regex mmap_regex("file=([a-zA-Z0-9.\\-_:/]*) pgoff=([0-9]+) pgsize=([0-9]+)"); // file=cat, pgoff=11, pgsize=32
					string filename_replace("$1"), pgoff_replace("$2"), pgsize_replace("$3");
					string  filename = regex_replace(log, mmap_regex, filename_replace, regex_constants::format_no_copy);

					if (filename == "")
					{
						goto inc_iter;
					}
					else
					{
						if(isExcluded(filename))
							goto inc_iter;
						else
							writePrefetchingCodeWithOpenedFile(prefetcher_code, filename, log_iter, file_table, use_posix_fadvise);
					}

					break;
				}
                case TLB:
                {
                    writePrefetchingCodeWithOpenedFile(prefetcher_code, "error", log_iter, file_table, use_posix_fadvise);
                    break;
                }
				
				case FORK:
				{
					break;
				}
				case STAT:
				break;

				case LSTAT:
				break;

				case FSTAT:
				break;
				
				case ACCESS:
				break;

				case GETDENTS:
				break;

				case FACCESSAT:
				break;

				case EXECVE:
				break;

				case CREAT:
				break;

				case OPENAT:
				break;

				case EXECVEAT:
				break;

				case READLINK:
				break;

				case FSTATFS:
				break;

				case FSTATFS64:
				case STATFS64:
				break;

				case STATFS:
				break;

				case TRUNCATE:
				break;

				case HMF:
					break;
			}

		inc_iter:
			log_iter = log_iter->next;
		}
		process_iter = process_iter->next;
	}

#if FUNCTION_PRINT_FILE_SET
	ofstream file_set;
	map <string, string>::iterator fd_iter;
	cout << "FUNCTION ON: print file set" << endl;
	file_set.open("output/file_set_generatePrefetcherCodesWithAllOpenFile.log", ofstream::out);
	for( fd_iter = file_table.begin(); fd_iter != file_table.end(); fd_iter++ )
		file_set << fd_iter->first << endl;
	file_set.close();
#endif
}

enum prefetch_domain {ACCESSEDFILE=1, OPENFILE};
void Parser::createPrefetcherCode(string FileName, List <ProcessInfo> *target_processes)
{
	map <string, string> file_table;

	prefetcher_code.open(OutputFileDirectory + FileName, ofstream::out);
	writePrefetchCodeHeader();

	int mode = ACCESSEDFILE;

	cout << endl;
	cout << ACCESSEDFILE << ". include only accessed files"  << endl;
	cout << OPENFILE << ". include all opened files" << endl;
	cout << ": ";
	cin >> mode;
	cout << endl;

	switch (mode) {
		case ACCESSEDFILE:
            generatePrefetcherCodeWithAccessedFile(target_processes, file_table);
            break;

		case OPENFILE:
			generatePrefetcherCodesWithOpenFiles(target_processes, file_table);
			break;

		default:
			break;
	}
	writePrefetchCodeFooter(file_table);
}

void Parser::writePrefetchCodeHeader()
{
	prefetcher_code << "#include <stdio.h>" << endl;
	prefetcher_code << "#include <unistd.h>" << endl;
	prefetcher_code << "#include <dirent.h>" << endl;
	prefetcher_code << "#include <stdlib.h>" << endl;
	prefetcher_code << "#include <fcntl.h>" << endl;
	prefetcher_code << "#include <sys/types.h>" << endl;
	prefetcher_code << "#include <sys/stat.h>" << endl;
	prefetcher_code << "#include <sys/vfs.h>" << endl;
	prefetcher_code << endl;
	prefetcher_code << "#define BUFF_SIZE " << BUFF_SIZE << endl;
	prefetcher_code << endl;
	prefetcher_code << "int main(void)" << endl;
	prefetcher_code << "{ " << endl;
	prefetcher_code << "\tint open_file_count = 0;" << endl;			// 1. open 
	prefetcher_code << "\tint open_file_error_count = 0;" << endl;	
	prefetcher_code << "\tint stat_count = 0;" << endl;					// 2. stat
	prefetcher_code << "\tint stat_error_count = 0;" << endl;
	prefetcher_code << "\tint lstat_count = 0;" << endl;				// 3. lstat
	prefetcher_code << "\tint lstat_error_count = 0;" << endl;
	prefetcher_code << "\tint open_file_for_fstat_count = 0;" << endl;	// 4. open for fstat
	prefetcher_code << "\tint open_file_error_for_fstat_count = 0;" << endl;
	prefetcher_code << "\tint fstat_count = 0;" << endl;				// 5. fstat
	prefetcher_code << "\tint fstat_error_count = 0;" << endl;
	prefetcher_code << "\tint open_dir_count = 0;" << endl;				// 6. open dir
	prefetcher_code << "\tint open_dir_error_count = 0;" << endl;
	prefetcher_code << "\tint access_count = 0;" << endl;				// 7. access
	prefetcher_code << "\tint access_error_count = 0;" << endl;
	prefetcher_code << "\tint statfs_count = 0;" << endl;				// 8. statfs
	prefetcher_code << "\tint statfs_error_count = 0;" << endl;

	prefetcher_code << "\tchar  buf[BUFF_SIZE];" << endl;
	prefetcher_code << "\tFILE *fp;" << endl;
	prefetcher_code << "\tDIR *dirp;" << endl;
	prefetcher_code << "\tint error;" << endl;
	prefetcher_code << "\tstruct stat sb;" << endl;
	prefetcher_code << "\tstruct statfs statfs_buf;" << endl;
	prefetcher_code << endl;

}

string Parser::correctFilePathName(string filename)
{
	Tokenizer target_str (filename, ":");
	string token;
	string str_ret;
	while((token = target_str.next()) != "")
	{
		if(token == "/")
			break;
		str_ret = token + str_ret;
		str_ret = "/" + str_ret;
	}
	return str_ret;
//	cout << str_ret << endl << endl;
}

#define DEBUG_printPrefetcherCodes 1

enum menuAccessedFile {ONLY_ACCESSED=1, ALL_PART};
void Parser::generatePrefetcherCodeWithAccessedFile(List <ProcessInfo> *target_processes, map <string, string> &file_table)
{
	int menu;

	cout << ONLY_ACCESSED << ". prefetch the only part to be accessed" << endl;
	cout << ALL_PART << ". prefetch all the parts of the files" << endl;
	cout << ": ";
	cin >> menu;
	cout << endl;

	switch(menu) 
	{
		case ONLY_ACCESSED:
		{
			prefetcher_code << "\tint i = 0;" << endl;
 			prefetchOnlyAccessed(target_processes, file_table);
			break;
		}
		
		case ALL_PART:
		{
			prefetchAllFileAccessed(target_processes, file_table);
			break;
		}
	}
}

// ! need to be refactored
// - A IO log is included in only one type
iotype Parser::checkLogType(string iolog)
{
	size_t found = string::npos;
    
#if PREFETCH_TLB
    found = iolog.find("filename=");
    if (found != string::npos)
        return TLB;
#endif

#if PREFETCH_HMF
    found = iolog.find("[HMF]");
    if (found != string::npos)
		return HMF;
#endif

#if PREFETCH_MMAP
    found = iolog.find("pgsize=");
    if (found != string::npos)
		return MMAP;
#endif

#if PREFETCH_VFSCALL
    found = iolog.find("pos=");
    if (found != string::npos)
		return VFSCALL;
#endif

#if PREFETCH_OPENAT
    found = iolog.find("openat=");
    if (found != string::npos)
		return OPENAT;
#endif

#if PREFETCH_OPEN
    found = iolog.find("open=");
    if (found != string::npos)
		return OPEN;
#endif

    
#if PREFETCH_LSTAT_STAT
    found = iolog.find("fstateat=");
    if (found != string::npos) {
        found = iolog.find("flag=256");
        if(found != string::npos)
			return LSTAT;
	}
#endif

#if PREFETCH_FORK
    found = iolog.find("child_pid=");
    if(found != string::npos)
		return FORK;
#endif

#if PREFETCH_GETDENTS
    found = iolog.find("getdents=");
    if(found != string::npos)
		return GETDENTS;
#endif

#if PREFETCH_ACCESS
    found = iolog.find("access=");
    if(found != string::npos)
		return ACCESS;
#endif


#if PREFETCH_FACCESSAT
    found = iolog.find("faccessat=");
    if(found != string::npos)
		return FACCESSAT;
#endif

#if PREFETCH_EXECVEAT
    found = iolog.find("execveat=");
    if(found != string::npos)
		return EXECVEAT;
#endif

#if PREFETCH_EXECVE
    found = iolog.find("execve=");
    if(found != string::npos)
		return EXECVE;
#endif

#if PREFETCH_CREAT
    found = iolog.find("creat=");
    if(found != string::npos)
		return CREAT;
#endif

#if PREFETCH_READLINK
    found = iolog.find("readlink=");
    if(found != string::npos)
		return READLINK;
#endif

#if PREFETCH_FSTATFS64
    found = iolog.find("fstatfs64=");
    if(found != string::npos)
		return FSTATFS64;
#endif

#if PREFETCH_STATFS64
    found = iolog.find("statfs64=");
    if(found != string::npos)
		return STATFS64;
#endif

#if PREFETCH_FSTATFS
    found = iolog.find("fstatfs=");
    if(found != string::npos)
		return FSTATFS;
#endif

#if PREFETCH_STATFS
    found = iolog.find("statfs=");
    if(found != string::npos)
		return STATFS;
#endif

#if PREFETCH_TRUNCATE
    found = iolog.find("trunfile=");
    if(found != string::npos)
		return TRUNCATE;
#endif

#if PREFETCH_FADVISE64_64
    found = iolog.find("fadvise64_64=");
    if(found != string::npos)
		return FADVISE64_64;
#endif

#if PREFETCH_FADVISE64
    found = iolog.find("fadvise64=");
    if(found != string::npos)
		return FADVISE64;
#endif

#if PREFETCH_FSTAT
    found = iolog.find("vfs_getattr=");
    if(found != string::npos)
		return FSTAT;
#endif

	return PASS;
}

// int fd2 = open ("/usr/share/X11/locale/locale.alias", O_RDONLY); // system call, LN-515
// 	if( fd2 < 0 ) { open_file_error_count++;  printf("open error: /usr/share/X11/locale/locale.alias\n"); }
// 	else open_file_count++;
// 	lseek(fd2, 0, SEEK_SET);
// 	read(fd2, buf, 4096);
// 	lseek(fd2, 4096, SEEK_SET);
// 	read(fd2, buf, 4096);
// 	
void Parser::prefetchAllFileAccessed(List <ProcessInfo> *target_processes, map <string, string> &file_table)
{
	Node <ProcessInfo> *process_iter = target_processes->getHead();
	map <string, string>::iterator fd_iter;
	string fd_name = "fd";
	size_t fd_count = 0;
	long total_sz_prefetching = 0;
	long max_buf_size = 0;

	cout << "use posix_fadvise() or lseek() & read()" << endl;
	cout << "0. lseek() & read()" << endl;
	cout << "1. posix_fadvise() (not support)" << endl;
	cout << endl;

	while (process_iter != NULL)
	{
		List <ParseEntry> *log_list = process_iter->entry->getLogList();
		Node <ParseEntry> *log_iter = log_list->getHead();
		while (log_iter != NULL)
		{
			iotype type = NONE;
			string iolog = log_iter->entry->getLogMsg();

			type = checkLogType(iolog);

			switch (type)
			{
				case NONE:
				{
					assert(0);
					break;
				}

				case PASS:
				{
					assert(1);
					break;
				}

				case FADVISE64_64:
				case FADVISE64:
				case VFSCALL:
				{
					////
					// file name
					regex systemcall_regex("file=([a-zA-Z0-9.-_:/]*) size=([0-9]+) \\*pos=([0-9]+)"); // file=ptmx size=16384 *pos=0
					string filename_replace("$1"), pos_replace("$3"), size_replace("$2");
					string  filename = regex_replace(iolog, systemcall_regex, filename_replace, regex_constants::format_no_copy);


					////
					// size
					string size = regex_replace(iolog, systemcall_regex, size_replace, regex_constants::format_no_copy);
					long l_size = stol(size);
					total_sz_prefetching += l_size;
					if( max_buf_size < l_size )
						max_buf_size = l_size;

					////
					// pos
					string pos = regex_replace(iolog, systemcall_regex, pos_replace, regex_constants::format_no_copy);


#if DEBUG_printPrefetcherCodes
					cout << "filename: " << filename << endl;

					cout << "pos: " << pos << endl;
#endif
					////
					// find file entries in the file list
					fd_iter = file_table.find(filename);

					////
					// if file isn't opened yet,
					if(file_table.end() == fd_iter) { // no search results, add file_table and write "open"
                    	fd_count = file_table.size();
						file_table.insert(make_pair(filename, fd_name + to_string(fd_count)));
						//	int fd1 = open("/lib/x86_64-linux-gnu/ld-2.19.so", O_RDONLY);
						openAndreadWholeFile(prefetcher_code, fd_name, fd_count, filename, log_iter, 0);
					}
					break;
				}

				case MMAP:
				{
					regex mmap_regex("file=([a-zA-Z0-9.-_:/]*) pgoff=([0-9]+) pgsize=([0-9]+)"); // file=cat, pgoff=11, pgsize=32
					string filename_replace("$1"), pgoff_replace("$2"), pgsize_replace("$3");
					string  filename = regex_replace(iolog, mmap_regex, filename_replace, regex_constants::format_no_copy);
                    

					string  pgoff = regex_replace(iolog, mmap_regex, pgoff_replace, regex_constants::format_no_copy);
					string  pgsize = regex_replace(iolog, mmap_regex, pgsize_replace, regex_constants::format_no_copy);

					fd_iter = file_table.find(filename);
					if(file_table.end() == fd_iter) { // no search results, add file_table and write "open"
                    	fd_count = file_table.size();
						file_table.insert(make_pair(filename, fd_name + to_string(fd_count)));
						//	int fd1 = open("/lib/x86_64-linux-gnu/ld-2.19.so", O_RDONLY);
						
						openAndreadWholeFile(prefetcher_code, fd_name, fd_count, filename, log_iter, 0);
					}

					break;
				}

				case TLB:
				{
					regex mmap_regex("filename=([a-zA-Z0-9.-_:/]*) pg_idx=([0-9]+)"); // file=cat, pgoff=11, pgsize=32
					string filename_replace("$1"), pgoff_replace("$2");

                    string  filename = regex_replace(iolog, mmap_regex, filename_replace, regex_constants::format_no_copy);
					string  pgoff = regex_replace(iolog, mmap_regex, pgoff_replace, regex_constants::format_no_copy);
					string  pgsize = "1";
                    
                    if (filename == "")
                        break;

					fd_iter = file_table.find(filename);
					if(file_table.end() == fd_iter) { // no search results, add file_table and write "open"
                    	fd_count = file_table.size();
						file_table.insert(make_pair(filename, fd_name + to_string(fd_count)));
						//	int fd1 = open("/lib/x86_64-linux-gnu/ld-2.19.so", O_RDONLY);
						
						openAndreadWholeFile(prefetcher_code, fd_name, fd_count, filename, log_iter, 0);
					}

					break;
				}

				case OPEN:
					break;
				case FORK:
					break;

				case STAT:
					break;
				case LSTAT:
					break;
				case FSTAT:
					break;

				case ACCESS:
					break;
				case GETDENTS:
					break;
				case FACCESSAT:
					break;
				case EXECVE:
					break;
				case CREAT:
					break;

				case OPENAT:
				break;

				case EXECVEAT:
				break;

				case READLINK:
				break;

				case FSTATFS:
				break;

				case FSTATFS64:
				case STATFS64:
				break;

				case STATFS:
				break;

				case TRUNCATE:
				break;

				case HMF:
					break;

			}
			log_iter = log_iter->next;
		}
		process_iter = process_iter->next;
	}

	cout << "total_sz_prefetching(KB): " << total_sz_prefetching / 1024  << endl;

#if FUNCTION_PRINT_FILE_SET
	ofstream file_set;
	cout << "FUNCTION ON: print file set" << endl;
	file_set.open("output/file_set_AllPartOfFile.log", ofstream::out);
	for( fd_iter = file_table.begin(); fd_iter != file_table.end(); fd_iter++ )
		file_set << fd_iter->first << endl;
	file_set.close();
#endif
}

#define DEBUG_OnlyAccessedPartOfFile 0
#define DEBUGING_CODE 0
void Parser::prefetchOnlyAccessed(List <ProcessInfo> *target_processes, map <string, string> &file_table)
{
	Node <ProcessInfo> *process_iter = target_processes->getHead();
	map <string, string>::iterator fd_iter;
	string fd_name = "fd";
	size_t fd_count = file_table.size();
	long total_sz_prefetching = 0;
	long max_buf_size = 0;
	int use_posix_fadvise = 0;

#ifdef FUNCTION_PRINT_FILE_SET
	ofstream file_set;
	cout << "FUNCTION ON: print file set" << endl;
	file_set.open("output/filenameFromPrefetcherC.log", ofstream::out);
#endif


	cout << endl;
	cout << "use posix_fadvise() or lseek() & read()" << endl;
	cout << "0. lseek() & read()" << endl;
	cout << "1. posix_fadvise()" << endl;
	cout << ": ";
	cin >> use_posix_fadvise;
	cout << endl;

	while (process_iter != NULL)
	{
		List <ParseEntry> *log_list = process_iter->entry->getLogList();
		Node <ParseEntry> *log_iter = log_list->getHead();
		while (log_iter != NULL)
		{
			iotype type = NONE;
			string iolog = log_iter->entry->getLogMsg();

#if DEBUG_OnlyAccessedPartOfFile
			if(log_iter->entry->getLineNumber() == 8721)
				cout << "break" << endl;
#endif

			type = checkLogType(iolog);

			switch (type)
			{
				case NONE:
				{
					assert(0);
					break;
				}

				case PASS:
				{
					assert(1);
					break;
				}
                    
                case TLB:
                {
                    assert(1);
                    break;
                }
                    
				case FADVISE64_64:
				case FADVISE64:
				case VFSCALL:
				{
					string filename;
					extractStringNextTo(iolog, "file=", &filename);
					if(filename == "")
						break;
					else
					{
						filename = correctFilePathName(filename);
					}

					////
					// size
					string  size;
					extractStringNextTo(iolog, "size=", &size);

					long l_size = stol(size);
					total_sz_prefetching += l_size;
					if( max_buf_size < l_size )
						max_buf_size = l_size;

					////
					// pos
					string pos; 
					extractStringNextTo(iolog, "pos=", &pos);

#if DEBUG_printPrefetcherCodes
					cout << "filename: " << filename << endl;
					cout << "size: " << size << endl;
					cout << "pos: " << pos << endl;
#endif
					////
					// find file entries in the file list
					fd_iter = file_table.find(filename);

					////
					// if file isn't opened yet,
					if(file_table.end() == fd_iter) { // no search results, add file_table and write "open"
	                    fd_count = file_table.size();
						file_table.insert(make_pair(filename, fd_name + to_string(fd_count)));
						//	int fd1 = open("/lib/x86_64-linux-gnu/ld-2.19.so", O_RDONLY);

						prefetcher_code << "\tint " << fd_name + to_string(fd_count) << " = open (\"" << filename << "\", O_RDONLY); // system call, LN-" << log_iter->entry->getLineNumber() << endl;
						prefetcher_code << "\tif( " << fd_name + to_string(fd_count) << " < 0 ) {" << endl;
						prefetcher_code << "\t\topen_file_error_count++;" << endl;
						prefetcher_code << "\t\tprintf(\"open file error: " << filename << "\\n\");" << endl;
						prefetcher_code <<"\t}" << endl;
						prefetcher_code << "\telse {" << endl;
						prefetcher_code << "\t\topen_file_count++;" << endl;
#if DEBUGING_CODE
						prefetcher_code << "\tprintf(\"the line number is: %d\\n\", __LINE__);" << endl;
#endif
						prefetcher_code << "\t}";

						prefetcher_code << endl;
					}

					fd_iter = file_table.find(filename);
					// posix_fadvise(fd1, 128, 0, POSIX_FADV_WILLNEED);
					if (use_posix_fadvise) {
						prefetcher_code << "\tposix_fadvise(" << fd_iter->second << ", " << size << ", " << pos << ", POSIX_FADV_WILLNEED); // system call, LN-" << log_iter->entry->getLineNumber() << endl;
					}
					else
					{
						prefetcher_code << "\tlseek(" << fd_iter->second << ", " << pos << ", SEEK_SET);" << endl;
						prefetcher_code << "\tfor(i=0; i<" << l_size / BUFF_SIZE << "; i++) read(" << fd_iter->second << ", buf, BUFF_SIZE);" << endl;
						if(l_size % BUFF_SIZE) prefetcher_code << "\tread(" << fd_iter->second << ", buf, " << l_size % BUFF_SIZE << ");" << endl;
#if DEBUGING_CODE
						prefetcher_code << "\tprintf(\"the line number is: %d\\n\", __LINE__);" << endl;
#endif
						prefetcher_code << endl;
					}

#if FUNCTION_PRINT_FILE_SET
					file_set << filename << " [" << log_iter->entry->getLineNumber() << "," << "vfscall" << "]" << endl;
#endif
					break;
				}

				case MMAP:
				{
					string filename;
					extractStringNextTo(iolog, "file=", &filename);

					if(filename == "") 	break;
					else
					{
						filename = correctFilePathName(filename);
					}
					string  pgoff ;
					extractStringNextTo(iolog, "pgoff=",  &pgoff);
					string pgsize;
					extractStringNextTo(iolog, "pgsize=",  &pgsize);
#if DEBUG_printPrefetcherCodes
					cout << "filename: " << filename << endl;
					cout << "pgoff: " << pgoff << endl;
					cout << "pgsize: " << pgsize << endl;
#endif
					fd_iter = file_table.find(filename);
					if(file_table.end() == fd_iter) { // no search results, add file_table and write "open"
						fd_count = file_table.size();
						file_table.insert(make_pair(filename, fd_name + to_string(fd_count)));
						//	int fd1 = open("/lib/x86_64-linux-gnu/ld-2.19.so", O_RDONLY);

						prefetcher_code << "\tint " << fd_name + to_string(fd_count) << " = open (\"" << filename << "\", O_RDONLY); // mmap, LN-" << log_iter->entry->getLineNumber() << endl;
						prefetcher_code << "\tif( " << fd_name + to_string(fd_count) << " < 0 ) { " << endl;
						prefetcher_code << "\t\topen_file_error_count++;" << endl;
						prefetcher_code << "\t\tprintf(\"open file error: " << filename << "\\n\"); }" << endl;
						prefetcher_code << "\telse ";
						prefetcher_code << "\t\topen_file_count++;" << endl;
						prefetcher_code << endl;
					}

					fd_iter = file_table.find(filename);
					// posix_fadvise(fd1, 128, 0, POSIX_FADV_WILLNEED);
					long l_pgsize = stol(pgsize);
					l_pgsize *= 4096;
					total_sz_prefetching += l_pgsize;
					if( max_buf_size < l_pgsize )
						max_buf_size = l_pgsize;


					long l_pgoff = stol(pgoff);
					l_pgoff *= 4096;

					if (use_posix_fadvise) {
						prefetcher_code << "\tposix_fadvise(" << fd_iter->second << ", " << l_pgsize << ", " << l_pgoff << ", POSIX_FADV_WILLNEED); // mmap, LN-" << log_iter->entry->getLineNumber() << endl;
						prefetcher_code << endl;
					}
					else
					{
						prefetcher_code << "\tlseek(" << fd_iter->second << ", " << l_pgoff << ", SEEK_SET);" << endl;
						prefetcher_code << "\tfor(i=0; i<" << l_pgsize / BUFF_SIZE << "; i++) read(" << fd_iter->second << ", buf, BUFF_SIZE);" << endl;
						if(l_pgsize % BUFF_SIZE) prefetcher_code << "\tread(" << fd_iter->second << ", buf, " << l_pgsize % BUFF_SIZE << ");" << endl;
						prefetcher_code << endl;
					}

#if FUNCTION_PRINT_FILE_SET
					file_set << filename << " [" << log_iter->entry->getLineNumber() << "," << "mmap" << "]" << endl;
#endif
					break;
				}

				case STAT:
				{
					string filename;
					extractStringNextTo(iolog, "file=", &filename);

					if(filename == "") 	break;

					prefetcher_code << "\tif( stat(\"" << filename << "\", &sb) < 0 ) {" << endl;
					prefetcher_code << "\t\tstat_error_count++;" << endl;
					prefetcher_code << "\t\tprintf(\"stat_error_count: " << filename << "\\n\");" << endl;
					prefetcher_code << "\t}" << endl;
					prefetcher_code << "\telse {" << endl;
					prefetcher_code << "\t\tstat_count++;" << endl;
					prefetcher_code << "\t}" << endl;

#if FUNCTION_PRINT_FILE_SET
					file_set << filename << " [" << log_iter->entry->getLineNumber() << "," << "stat" << "]" << endl;
#endif

					break;
				}

				case LSTAT:
				{
					string filename;

					extractStringNextTo(iolog, "file=", &filename);
					if(filename == "") 	break;

					prefetcher_code << "\tif( lstat(\"" << filename << "\", &sb) < 0 ) {" << endl;
					prefetcher_code << "\t\tlstat_error_count++;" << endl;
					prefetcher_code << "\t\tprintf(\"lstat_error_count: " << filename << "\\n\");" << endl;
					prefetcher_code << "\t}" << endl;
					prefetcher_code << "\telse {" << endl;
					prefetcher_code << "\t\tlstat_count++;" << endl;
					prefetcher_code << "\t}" << endl;
					prefetcher_code << endl;

#if FUNCTION_PRINT_FILE_SET
					file_set << filename << " [" << log_iter->entry->getLineNumber() << "," << "lstat" << "]" << endl;
#endif

					break;
				}

				case FSTAT:
				{
					string filename;

					extractStringNextTo(iolog, "vfs_getattr=", &filename);
					if(filename == "") 	break;
					else
					{
						filename = correctFilePathName(filename);
					}

					// check if the file for fstat is already opened or not
					// 1. opened
					// - file descriptor number is used again.
					// 2. or not
					// - open the file
					// -  the file descriptor number is used

					fd_iter = file_table.find(filename);
					string filename_to_open_for_fstat;
					if(file_table.end() == fd_iter) { // no search results, add file_table and write "open"
						fd_count = file_table.size();
						file_table.insert(make_pair(filename, fd_name + to_string(fd_count)));

						prefetcher_code << "\tint " << fd_name + to_string(fd_count) << " = open (\"" << filename << "\", O_RDONLY); // system call, LN-" << log_iter->entry->getLineNumber() << endl;
						prefetcher_code << "\tif( " << fd_name + to_string(fd_count) << " < 0 ) {" << endl;
						prefetcher_code << "\t\topen_file_error_for_fstat_count++;" << endl;
						prefetcher_code << "\t\tprintf(\"open file for fstat error: " << filename << "\\n\");" << endl;
						prefetcher_code <<"\t}" << endl;
						prefetcher_code << "\telse {" << endl;
						prefetcher_code << "\t\topen_file_for_fstat_count++;" << endl;
						prefetcher_code << "\t}" << endl;
#if DEBUGING_CODE
						prefetcher_code << "\tprintf(\"the line number is: %d\\n\", __LINE__);" << endl;
#endif
						filename_to_open_for_fstat = fd_name + to_string(fd_count);
					}
					else{
						filename_to_open_for_fstat = fd_iter->second;
					}
					prefetcher_code << "\tif( fstat(" << filename_to_open_for_fstat  << ", &sb) < 0 ) {" << endl;
					prefetcher_code << "\t\tfstat_error_count++;" << endl;
					prefetcher_code << "\t\tprintf(\"fstat_error_count: " << filename << "\\n\");" << endl;
					prefetcher_code << "\t}" << endl;
					prefetcher_code << "\telse {" << endl;
					prefetcher_code << "\t\tfstat_count++;" << endl;
					prefetcher_code << "\t}" << endl;
					prefetcher_code << endl;

#if FUNCTION_PRINT_FILE_SET
					file_set << filename << " [" << log_iter->entry->getLineNumber() << "," << "fstat" << "]" << endl;
#endif

					break;
				}

				case FORK:
					break;

				case FACCESSAT:
				{
					string filename;
					extractStringNextTo(iolog, "faccesats=", &filename);
					if(filename == "") break;

					string mode;
					extractStringNextTo(iolog, "mode=", &mode);
					if(mode == "") assert(0);

					prefetcher_code << "\tif( access(\"" << filename << "\", " << mode << ") < 0) {" << endl;
					prefetcher_code << "\t\taccess_error_count++;" << endl;
					prefetcher_code << "\t\tprintf(\"access_error_count: " << filename << " mode=" << mode << "\\n\");" << endl;
					prefetcher_code << "\t}" << endl;
					prefetcher_code << "\telse {" << endl;
					prefetcher_code << "\t\taccess_count++;" << endl;
					prefetcher_code << "\t}" << endl;
					prefetcher_code << endl;

#if FUNCTION_PRINT_FILE_SET
					file_set << filename << " [" << log_iter->entry->getLineNumber() << "," << "faccessat" << "]" << endl;
#endif

					break;
				}

				case ACCESS:
				{
					string filename;
					extractStringNextTo(iolog, "access=", &filename);
					if(filename == "") break;

					string mode;
					extractStringNextTo(iolog, "mode=", &mode);
					if(mode == "") assert(0);

					prefetcher_code << "\tif( access(\"" << filename << "\", " << mode << ") < 0) {" << endl;
					prefetcher_code << "\t\taccess_error_count++;" << endl;
					prefetcher_code << "\t\tprintf(\"access_error_count: " << filename << " mode=" << mode << "\\n\");" << endl;
					prefetcher_code << "\t}" << endl;
					prefetcher_code << "\telse {" << endl;
					prefetcher_code << "\t\taccess_count++;" << endl;
					prefetcher_code << "\t}" << endl;
					prefetcher_code << endl;

#if FUNCTION_PRINT_FILE_SET
					file_set << filename << " [" << log_iter->entry->getLineNumber() << "," << "access" << "]" << endl;
#endif

					break;
				}

				case GETDENTS:
				{
					string filename;
					extractStringNextTo(iolog, "getdents=", &filename);

					filename = correctFilePathName(filename);

					prefetcher_code << "\tdirp = opendir(\"" << filename << "\"); // LN-" << log_iter->entry->getLineNumber() <<endl;
					prefetcher_code << "\tif(dirp != NULL) {" << endl;
					prefetcher_code << "\t\twhile(readdir(dirp));" << endl;
					prefetcher_code << "\t\tclosedir(dirp);" << endl;
					prefetcher_code << "\t\topen_dir_count++;" << endl;
					prefetcher_code << "\t}" << endl;
					prefetcher_code << "\telse {" << endl;
					prefetcher_code << "\t\tprintf(\"opendir error: " << filename << "\\n\");" << endl;
					prefetcher_code << "\t\topen_dir_error_count++;" << endl;
					prefetcher_code << "\t}" << endl;
					prefetcher_code << endl;

#if FUNCTION_PRINT_FILE_SET
					file_set << filename << " [" << log_iter->entry->getLineNumber() << "," << "getdents" << "]" << endl;
#endif

					break;
				}

				case CREAT:
					assert(0);
					break;

				case OPEN:
				case OPENAT:
				{
					string filename;
					extractStringNextTo(iolog, "=", &filename);

					fd_iter = file_table.find(filename);

					// if file isn't opened yet,
					if(file_table.end() == fd_iter) { // no search results, add file_table and write "open"
						fd_count = file_table.size();
						file_table.insert(make_pair(filename, fd_name + to_string(fd_count)));
						//	int fd1 = open("/lib/x86_64-linux-gnu/ld-2.19.so", O_RDONLY);

						prefetcher_code << "\tint " << fd_name + to_string(fd_count) << " = open (\"" << filename << "\", O_RDONLY); // system call, LN-" << log_iter->entry->getLineNumber() << endl;
						prefetcher_code << "\tif( " << fd_name + to_string(fd_count) << " < 0 ) {" << endl;
						prefetcher_code << "\t\topen_file_error_count++;" << endl;
						prefetcher_code << "\t\tprintf(\"open file error: " << filename << "\\n\");" << endl;
						prefetcher_code <<"\t}" << endl;
						prefetcher_code << "\telse {" << endl;
						prefetcher_code << "\t\topen_file_count++;" << endl;
						prefetcher_code << "\t};" << endl;
						prefetcher_code << endl;
					}

#if FUNCTION_PRINT_FILE_SET
					file_set << filename << " [" << log_iter->entry->getLineNumber() << "," << "open" << "]" << endl;
#endif

					break;
				}

				case EXECVE:
				case EXECVEAT:
				{
					string filename;
					extractStringNextTo(iolog, "=", &filename);
					if(filename == "") break;

					writePrefetchingCodeWithOpenedFile(prefetcher_code, filename, log_iter, file_table, use_posix_fadvise);

#if FUNCTION_PRINT_FILE_SET
					file_set << filename << " [" << log_iter->entry->getLineNumber() << "," << "execve" << "]" << endl;
#endif

				}
				break;

				case READLINK:
					assert(0);
					break;

				case FSTATFS:
				case FSTATFS64:
				case STATFS64:
				case STATFS:
				{
					string filename;
					extractStringNextTo(iolog, "=", &filename);
					if(filename == "")
						break;
					else
					{
						filename = correctFilePathName(filename);
					}

					prefetcher_code << "\tif( statfs(\"" << filename << "\", &statfs_buf) < 0 ) {" << endl;
					prefetcher_code << "\t\tprintf(\"statfs error: " << filename << "\\n\");" << endl;
					prefetcher_code << "\t\tstatfs_error_count++; }" << endl;
					prefetcher_code << "\telse {" << endl;
					prefetcher_code << "\t\tstatfs_count++; } " << endl << endl;

#if FUNCTION_PRINT_FILE_SET
					file_set << filename << " [" << log_iter->entry->getLineNumber() << "," << "statfs" << "]" << endl;
#endif
				}

				break;

				case TRUNCATE:
				break;

				case HMF:
					string filename;
					extractStringNextTo(iolog, "filename=", &filename);

					if(filename == "") 	break;
					else
					{
						filename = correctFilePathName(filename);
					}
//					string  pgoff ;
//					extractStringNextTo(iolog, "pos=",  &pgoff);
					string pgoff;
					extractStringNextTo(iolog, "pgoff=",  &pgoff);
#if DEBUG_printPrefetcherCodes
					cout << "filename: " << filename << endl;
					cout << "pgoff: " << pgoff << endl;
#endif
					fd_iter = file_table.find(filename);
					if(file_table.end() == fd_iter) { // no search results, add file_table and write "open"
						fd_count = file_table.size();
						file_table.insert(make_pair(filename, fd_name + to_string(fd_count)));
						//	int fd1 = open("/lib/x86_64-linux-gnu/ld-2.19.so", O_RDONLY);

						prefetcher_code << "\tint " << fd_name + to_string(fd_count) << " = open (\"" << filename << "\", O_RDONLY); // mmap, LN-" << log_iter->entry->getLineNumber() << endl;
						prefetcher_code << "\tif( " << fd_name + to_string(fd_count) << " < 0 ) { " << endl;
						prefetcher_code << "\t\topen_file_error_count++;" << endl;
						prefetcher_code << "\t\tprintf(\"open file error: " << filename << "\\n\"); }" << endl;
						prefetcher_code << "\telse ";
						prefetcher_code << "\t\topen_file_count++;" << endl;
						prefetcher_code << endl;
					}

					// inserted the new opened file and re-search it in the table (map).

					fd_iter = file_table.find(filename);
					// posix_fadvise(fd1, 128, 0, POSIX_FADV_WILLNEED);

					// MHF's behavior doesn't imply page size, which means that pgsize is can be 1.
					long l_pgsize = 1;
					long l_filesize = l_pgsize * 4096; // for fetching the 1 page which have the data.
										

					total_sz_prefetching += l_filesize; // ?
					if( max_buf_size < l_pgsize )
						max_buf_size = l_pgsize;


					long l_pgoff = stol(pgoff); // pgoff can be used in the original string which is converted to long type
					long l_byteoff = l_pgoff * 4096;

					if (use_posix_fadvise) {
						prefetcher_code << "\tposix_fadvise(" << fd_iter->second << ", " << l_pgsize << ", " << l_pgoff << ", POSIX_FADV_WILLNEED); // mmap, LN-" << log_iter->entry->getLineNumber() << endl;
						prefetcher_code << endl;
					}
					else
					{
						// [ ] fd_iter->second might be the file descriptor?
#if DEBUG_printPrefetcherCodes
						prefetcher_code << "\t// system call, LN-" << log_iter->entry->getLineNumber() << endl;

#endif
						prefetcher_code << "\tlseek(" << fd_iter->second << ", " << l_byteoff << ", SEEK_SET);" << endl;
						prefetcher_code << "\tfor(i=0; i<" << l_pgsize / BUFF_SIZE << "; i++) read(" << fd_iter->second << ", buf, BUFF_SIZE);" << endl;
						if(l_pgsize % BUFF_SIZE) prefetcher_code << "\tread(" << fd_iter->second << ", buf, " << l_pgsize % BUFF_SIZE << ");" << endl;
						prefetcher_code << endl;
					}

#if FUNCTION_PRINT_FILE_SET
					file_set << filename << " [" << log_iter->entry->getLineNumber() << "," << "mmap" << "]" << endl;
#endif
					break;
			}
			log_iter = log_iter->next;
		}
		process_iter = process_iter->next;
	}

	cout << "total_sz_prefetching(KB): " << total_sz_prefetching / 1024  << endl;
#if FUNCTION_PRINT_FILE_SET
	file_set.close();
#endif
}

void Parser::writePrefetchCodeFooter(map <string, string> &file_table)
{
	map <string, string>::iterator fd_iter = file_table.begin();
	prefetcher_code << endl;

	for( ; fd_iter != file_table.end(); ++fd_iter)
	{
		prefetcher_code << "\tif(" << fd_iter->second << ">=0)\tclose(" << fd_iter->second << ");" << endl;
	}


	prefetcher_code << "\tprintf(\"====================================\\n\");" << endl;

	// 1. open
	prefetcher_code << "\tprintf(\"opened file #: %d\\n\", open_file_count);" << endl;
	prefetcher_code << "\tprintf(\"open file error #: %d\\n\", open_file_error_count);" << endl;

	// 2. stat
	prefetcher_code << "\tprintf(\"opened stat #: %d\\n\", stat_count);" << endl;
	prefetcher_code << "\tprintf(\"open stat error #: %d\\n\", stat_error_count);" << endl;

	// 3. lstat
	prefetcher_code << "\tprintf(\"opened lstat #: %d\\n\", lstat_count);" << endl;
	prefetcher_code << "\tprintf(\"open lstat error #: %d\\n\", lstat_error_count);" << endl;

	// 4. open for fstat
	prefetcher_code << "\tprintf(\"open_file_for_fstat_count #: %d\\n\", open_file_for_fstat_count);" << endl;
	prefetcher_code << "\tprintf(\"open_file_error_for_fstat_count #: %d\\n\", open_file_error_for_fstat_count);" << endl;

	// 5. fstat
	prefetcher_code << "\tprintf(\"opened fstat #: %d\\n\", fstat_count);" << endl;
	prefetcher_code << "\tprintf(\"open fstat error #: %d\\n\", fstat_error_count);" << endl;

	// 6. open dif
	prefetcher_code << "\tprintf(\"opened opendir #: %d\\n\", open_dir_count);" << endl;
	prefetcher_code << "\tprintf(\"open opendir error #: %d\\n\", open_dir_error_count);" << endl;

	// 7. access
	prefetcher_code << "\tprintf(\"access #: %d\\n\", access_count);" << endl;
	prefetcher_code << "\tprintf(\"access error #: %d\\n\", access_error_count);" << endl;

	// 8. statfs
	prefetcher_code << "\tprintf(\"statfs_count #: %d\\n\", statfs_count);" << endl;
	prefetcher_code << "\tprintf(\"statfs_error_count #: %d\\n\", statfs_error_count);" << endl;

	prefetcher_code << "\treturn 0;" << endl;

	prefetcher_code << "}" << endl;
	prefetcher_code.close();
}
