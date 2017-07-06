/*
 * ParseEntry.h
 *
 *  Created on: Mar 12, 2014
 *      Author: elsdy
 */

#ifndef PARSEENTRY_H_
#define PARSEENTRY_H_

#include <iostream>

#include <cstdlib>
#include <string>
#include <cstring>
#include <list>
#include <fstream>
#include <assert.h>
#include <regex>

#include "Bitset.h"

using namespace std;

class ParseEntry {
private:
	/// line number
	long LineNumber;

	/// PID
	int ProcessId;

	/// Log Message
	string LogMsg;

	/// Function Name
	string FunctionName;

	/// CPU ID
	int CpuId;

	/// Process Name
	string ProcessName;

	/// Time Stamp when log message is recorded
	double TimeStamp;
	double ApproxTimeStamp;
	
	// 2014_06_03_Tue_PM_05_42_58
	// for checking if this parse entry is collected in a intended list.
#define SYS_IO		1 << 0
#define BLOCK_IO		1 << 1
#define EMMC_IO		1 << 2
#define INTERRUT		1 << 3
#define SCHEDULE		1 << 4
	BitSet isCollectedInWhichList;
	
protected:

#define DEBUG_basic_parse_line_for_log_v1 0
	void basic_parse_line_for_log_v1(const char *line, long _line_number);
#define DEBUG_basic_parse_line_for_log_v2 0
	void basic_parse_line_for_log_v2(const char *line, long _line_number);
#define DEBUG_basic_parse_line_for_log_v2_1 0
	void basic_parse_line_for_log_v2_1(const char *_line, long _line_number);

	void basic_parse_line_for_log_with_regex(const char *_line, long _line_number);

	char * strrstr(const char *str, const char *strSearch);

public:
	ParseEntry()
	{
		init();
	}
	ParseEntry(string _line)
	{
		const char *buf = _line.c_str();
		init();
		basic_parse_line_for_log_v1(buf, 0);
	}
	
	ParseEntry(string _line, long _line_number, string _log_version)
	{
		const char *buf = _line.c_str();
		
		init();
		
		if(_log_version == "1.0")
		{
			basic_parse_line_for_log_v1(buf, _line_number);
//			basic_parse_line_for_log_with_regex(buf, _line_number);
		}
		else if(_log_version == "2.0")
		{	basic_parse_line_for_log_v2(buf, _line_number);
		}
		else if(_log_version == "2.1")
		{
			basic_parse_line_for_log_v2_1(buf, _line_number);
		}
		else{
			basic_parse_line_for_log_with_regex(buf, _line_number);
		}
	}

	~ParseEntry()
	{
	}

	void init();
	
	long getLineNumber();
	void setLineNumber(long _ln);
	int getProcessID();
	void setProcessID(int _pid);
	string getLogMsg();
	void setLogMsg(string _log);
	string getFunctionName();
	void setFunctionName(string _fn);
	int getCpuId();
	void setCpuId(int _cpu_id);
	double getTimeStamp();
	void setTimeStamp(double _ts);
	double getApproxTimeStamp();
	void setApproxTimeStamp(double _ts);
	
	BitSet &getisCollectedInWhichList();
	
	void setisCollectedInWhichList(BitSet _bs);
	
	string getProcessName()
	{
		return ProcessName;
	}
	
	void setProcessName(string _value)
	{
		ProcessName = _value;
	}
	
#pragma mark  - for debug
	void printParseEntry(ofstream &fout);
	void printParseEntry();
	
	int print();
	int print(ofstream &_output);
	int recoverLogFormat(ofstream &_output);


};

#endif /* PARSEENTRY_H_ */
