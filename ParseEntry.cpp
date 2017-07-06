;/*
 * ParseEntry.cpp
 *
 *  Created on: Mar 12, 2014
 *      Author: elsdy
 */


#include "ParseEntry.h"

#pragma mark - Class ParseEntry get / set Methods

void ParseEntry::init()
{
	ProcessId = 0;
	LogMsg = "";
	FunctionName = "";
	CpuId = 0;
	ProcessName = "";
	TimeStamp = 0;
	ApproxTimeStamp = 0;
	LineNumber = 0;
}

long ParseEntry::getLineNumber()
{
	return LineNumber;
}
void ParseEntry::setLineNumber(long _ln)
{
	LineNumber = _ln;
}

int ParseEntry::getProcessID()
{
	return ProcessId;
}
void ParseEntry::setProcessID(int _pid)
{
	ProcessId = _pid;
}
string ParseEntry::getLogMsg()
{
	return LogMsg;
}
void ParseEntry::setLogMsg(string _log)
{
	string LogMsg(_log);
}
string ParseEntry::getFunctionName()
{
	return FunctionName;
}
void ParseEntry::setFunctionName(string _fn)
{
	string FunctionName(_fn);
}
int ParseEntry::getCpuId()
{
	return CpuId;
}
void ParseEntry::setCpuId(int _cpu_id)
{
	CpuId =_cpu_id;
}
double ParseEntry::getTimeStamp()
{
	return TimeStamp;
}
void ParseEntry::setTimeStamp(double _ts)
{
	TimeStamp = _ts;
}

double ParseEntry::getApproxTimeStamp()
{
	return ApproxTimeStamp;
}

void ParseEntry::setApproxTimeStamp(double _ts)
{
	ApproxTimeStamp = _ts;
}

BitSet & ParseEntry::getisCollectedInWhichList()
{
	return isCollectedInWhichList;
}

void ParseEntry::setisCollectedInWhichList(BitSet _bs)
{
	isCollectedInWhichList = _bs;
}




#pragma mark - Class ParseEntry Methods

/**
 This function reads a line in the input file and parses it. Make a ParseEntry  with them extracted during parsing
 */
//           gmain-1177  [000] d... 92692.160582: : file=[eventfd] size=16 *pos=0
void ParseEntry::basic_parse_line_for_log_v1(const char *_line, long _line_number)
{
#if DEBUG_basic_parse_line_for_log_v1
	cout << "line: " << _line << endl;
#endif

	char buf[4096];

	char *token = NULL;
	char *token_temp = NULL;

	char *ptr = &buf[0];
	char *ptr_temp = NULL;

	char *process_name_start = NULL;
	char *process_name_end = NULL;
	char process_name[100];

	LineNumber = _line_number;

	strcpy(buf, _line);

	// 1. 프로세스 id를 찾기 위한 움직임
	//  pool-1-thread-1-2262  [002] d..1    43.174821: sys_pread64_pre: fd=64
	//  Smack Packet Wr-1745  [003] ...1    24.059850: finish_task_switch:
	// 위의 두가지 경우를 해결하기 위한 방안
	// 가정은 무조건 cpu 번호는 [ ]으로 둘려싸여지고
	// pid값은 프로세스 이름 다음에 나온다는 가정
	// 이 경우는? AsyncTask #2-835 [002] ...1 37.441630: __schedule: AsyncTask #2:

	while(*ptr == ' ' || *ptr == '\t')
		ptr++;

	process_name_start = ptr;

	while(*ptr != ']')
		ptr++;
	while(*ptr != '-')
		ptr--;

	process_name_end = ptr;

	memcpy(process_name, process_name_start, process_name_end - process_name_start);
	process_name[process_name_end - process_name_start] = '\0';
	setProcessName(process_name);

	token = strtok_r(ptr, " -\t", &ptr);
	ProcessId = atoi(token);

	token = strtok_r(NULL, "[] \t", &ptr);
	CpuId = atoi(token);

	token = strtok_r(NULL, " \t", &ptr);

	token = strtok_r(NULL, " -\t:", &ptr);

	TimeStamp = atof(token);

	token = strtok_r(NULL, ":", &ptr);
	ptr_temp = ptr;
	FunctionName = token;
	FunctionName.erase(FunctionName.begin());

	token = strtok_r(NULL, " \n", &ptr);

	if(token != NULL)
	{
		token_temp = strrstr(_line, token);
		
//		LogMsg = string(token_temp);
		LogMsg = token_temp;
	}
}
void ParseEntry::basic_parse_line_for_log_v2(const char *_line, long _line_number)
{
#if DEBUG_basic_parse_line_for_log_v2
	cout << "line: " << _line << endl;
#endif
	
	char buf[4096];
	
	char *token = NULL;
	char *token_temp = NULL;
	
	char *ptr = &buf[0];
	
	
	LineNumber = _line_number;
	
	strcpy(buf, _line);
		
	token = strtok_r(ptr, "|", &ptr);
	ProcessName = token;
	
	token = strtok_r(ptr, "|", &ptr);
	ProcessId = atoi(token);
	
	token = strtok_r(ptr, "|", &ptr);
	CpuId = atoi(token);
	
	token = strtok_r(ptr, "|", &ptr);
	TimeStamp = atof(token);
	
	token = strtok_r(ptr, ":", &ptr);
	FunctionName = token;
	
	token = strtok_r(NULL, " \n", &ptr);
	
	if(token != NULL)
	{
		token_temp = strrstr(_line, token);
		
		//		LogMsg = string(token_temp);
		LogMsg = token_temp;
	}
}

void ParseEntry::basic_parse_line_for_log_with_regex(const char *_line, long _line_number)
{
#if DEBUG_basic_parse_line_for_log_v2_1
	cout << "line: " << _line << endl;
#endif
	
	char buf[4096];
	
	LineNumber = _line_number;
	
	strcpy(buf, _line);

	//	bash-2935  [002] d...   713.629083: : file=0 size=1 *pos=0
	//   systemd-udevd-389   [002] ....  1700.863909: sched_process_fork: comm=systemd-udevd pid=389 child_comm=systemd-udevd child_pid=17192
	//	regex log_tokenizer("^\\s+([a-zA-Z0-9<>/_:.-]+(\\s*[a-zA-Z0-9<>/_:.#])+)-([0-9]+)\\s+\\[([0-9]+)\\]\\s+[a-zA-Z.]+\\s+([0-9.]+): ([a-zA-Z0-9_]*):\\s+"); // v1.0 too long time required
//		regex log_tokenizer("^\\s+(.*)-([0-9]+)\\s+\\[([0-9]+)\\]\\s+[a-zA-Z.]+\\s+([0-9.]+): ([a-zA-Z0-9_]*):\\s+"); // v2.0 (?)
		regex log_tokenizer("^\\s+(.*)-([\\d]+) +\\[([\\d]+)\\] +[\\w.]+ +([\\d.]+): +(.*): (.*)"); // v2.1 - time per a line = 0.0016

	// $1 = processname
	// $2 = pid
	// $3 = cpu id
	// $5 = time stamp
	// $6 = function name
	// $7 = msg

	string pro_name_rp("$1"), pid_rp("$2"), cpuid_rp("$3");
	string time_stamp_rp("$4"), functionname_rp("$5");
	string log_rp("$6");

	ProcessName = regex_replace(buf, log_tokenizer, pro_name_rp, regex_constants::format_no_copy);

	string  str_pid = regex_replace(buf, log_tokenizer, pid_rp, regex_constants::format_no_copy);
	ProcessId = stoi(str_pid);

	string  str_cpuid = regex_replace(buf, log_tokenizer, cpuid_rp, regex_constants::format_no_copy);
	CpuId = stoi(str_cpuid);
	
	string  str_time_stamp = regex_replace(buf, log_tokenizer, time_stamp_rp, regex_constants::format_no_copy);
	TimeStamp = stof(str_time_stamp);
	
	FunctionName = regex_replace(buf, log_tokenizer, functionname_rp, regex_constants::format_no_copy);

	LogMsg = regex_replace(buf, log_tokenizer, log_rp, regex_constants::format_no_copy);
}

void ParseEntry::basic_parse_line_for_log_v2_1(const char *_line, long _line_number)
{
#if DEBUG_basic_parse_line_for_log_v2_1
	cout << "line: " << _line << endl;
#endif

	char buf[4096];

	char *token = NULL;
	char *token_temp = NULL;

	char *ptr = &buf[0];


	LineNumber = _line_number;

	strcpy(buf, _line);

	token = strtok_r(ptr, "|", &ptr);
	ProcessId = atoi(token);

	token = strtok_r(ptr, "|", &ptr);
	CpuId = atoi(token);

	token = strtok_r(ptr, "|", &ptr);
	TimeStamp = atof(token);

	token = strtok_r(ptr, ":", &ptr);
	FunctionName = token;

	token = strtok_r(NULL, " \n", &ptr);

	if(token != NULL)
	{
		token_temp = strrstr(_line, token);

		//		LogMsg = string(token_temp);
		LogMsg = token_temp;
	}
}

char * ParseEntry::strrstr(const char *str, const char *strSearch)
{
	char *ptr, *last=NULL;
	ptr = (char*)str;
	while((ptr = strstr(ptr, strSearch))) last = ptr++;
	return last;
}


int ParseEntry::recoverLogFormat(ofstream &_output)
{
	_output << "           <" << ProcessName << ">-" << ProcessId << "\t .... \t" << TimeStamp << ": " << FunctionName << ": " << LogMsg << endl;
	return 1;
}

#pragma mark - for debug
int ParseEntry::print()
{
	printParseEntry();
	return 1;
}

int ParseEntry::print(ofstream &_output)
{
	printParseEntry(_output);
	return 1;
}

void ParseEntry::printParseEntry(ofstream &_fout)
{
	//	if(_fout.is_open())
	//	{
	_fout << fixed;
	_fout.precision(6);
	
	_fout << "[" << LineNumber << "] (" << TimeStamp << " -> " << ApproxTimeStamp << ") [cpu" << CpuId << "] [" << FunctionName << "/" << ProcessId <<"]: " << LogMsg << endl;
	//	}
}

void ParseEntry::printParseEntry()
{
	cout << fixed;
	cout.precision(6);
	
	cout << "[" << LineNumber << "] (" << TimeStamp << " -> " << ApproxTimeStamp << ") [cpu" << CpuId << "] [" << FunctionName << "/" << ProcessId <<"]: " << LogMsg << endl;
}

