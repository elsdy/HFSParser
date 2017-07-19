//
//  ProcessInfo.h
//  HFSParser
//
//  Created by Dongyun Shin on 3/21/14.
//  Copyright (c) 2014 elsdy. All rights reserved.
//

#ifndef __HFSParser__ProcessInfo__
#define __HFSParser__ProcessInfo__

#include <iostream>
#include <string>

using namespace std;

#include "List.h"
#include "ParseEntry.h"

#include "IOInfo.h"
#include "StartFinishTimeInfo.h"

#include "Result.h"

class ProcessInfo
{
#pragma mark - contructor & destructor
public:
	ProcessInfo();
	~ProcessInfo();

	void init();

	/*
	 1. process id
	 2. process name
	 3. a log list from vfs.log
	 4. file IO list, collected by affecting IO_load_rule
	 5. Direct IO list (go through storage)
	 6. Buffered IO list (within file cash, not going to storage)
	 7. network IO list
	 8. Sysfs IO list
	 9. Unknown IO list
	 10. total execution time for the process to consume
	 ... various calculated time
	 */
private:
	int ProcessId; // key
	string ProcessName; // Process Name


	List <ParseEntry> *LogList;
	List <IOInfo> *FileIOList; // File IO collected in system call levels


	// # of FileIOList = # of DirectIOList + # of BufferedIOList
	List <IOInfo> *DirectIOList;
	List <IOInfo> *BufferedIOList;


	List <IOInfo> *NetIOList; // network I/O request
	List <IOInfo> *SysfsIOList; // sysfs IO request

	
	List <IOInfo> *UnknownIOList; // IO which aren't categorized in the any above


	List <StartFinishTimeInfo> * CpuExecutionTime; // the Execution time consume in CPU for the process


	double TotalCpuExecutionTime; // calculated in collectExecutimeTimeEvent(), TotalCpuExecutionTime = TotalNonFileIoHandlingTime + TotalFileIoHandlingTime
	double TotalNonFileIoHandlingTime; // TotalNonFileIoHandlingTime = TotalCpuExecutionTime (calculated) - TotalFileIoHandlingTime (calculated)
	double TotalFileIoHandlingTime; // calculated in calculatePureIOHandlingTimeExcludingInterrupt(), TotalFileIoHandlingTime = TotalInterruptHandlingtimeInFileIO + TotalSoftIrqHandlingtimeInFileIO + TotalPureFileIoHandlingTime
	double TotalInterruptHandlingtimeInFileIO; // calculated in calculatePureIOHandlingTimeExcludingInterrupt()
	double TotalSoftIrqHandlingtimeInFileIO; // calculated in calculatePureIOHandlingTimeExcludingInterrupt()
	double TotalPureFileIoHandlingTime; // TotalPureFileIoHandlingTime = TotalFileIoHandlingTime - TotalInterruptHandlingtimeInFileIO - TotalSoftIrqHandlingtimeInFileIO


#pragma mark - get & set methods
public:
	int getProcessId();
	void setProcessId(int _pid);
	int getKey(); // return Process ID, we use PID as a key for searching
	string getProcessName();
	void setProcessName(string _name);


	List <ParseEntry>* getLogList();


	List <IOInfo> *getFileIOList();
	void setFileIOList( List <IOInfo> * _file_io_list);
	

	double getTotalNonFileIoHandlingTime();
	void addTotalNonFileIoHandlingTime(double _value);


	double getTotalFileIoHandlingTime();
	void addTotalFileIoHandlingTime(double _value);


	double getTotalInterruptHandlingtimeInFileIO();
	void addTotalInterruptHandlingtimeInFileIO(double _value);


	double getTotalSoftIrqHandlingtimeInFileIO();
	void addTotalSoftIrqHandlingtimeInFileIO(double _value);


	double getTotalPureFileIoHandlingTime();
	void addTotalPureFileIoHandlingTime(double _value);


	List <IOInfo> *getNetIOList();
	void setNetIOList(List <IOInfo> *_NetIOList);


	List <IOInfo> *getSysfsIOList();
	void setSysfsIOList( List <IOInfo> *_SysfsIOList);
	

	List <StartFinishTimeInfo> *getCpuExecutionTime();
	void setCpuExecutionTime(List <StartFinishTimeInfo> * _value);
	

	double getTotalCpuExecutionTime();
	void addTotalCpuExecutionTime(double _value);


	List <IOInfo> *getDirectIOList();
	List <IOInfo> *getBufferedIOList();
	List <IOInfo> *getUnknownIOList();
	
#pragma mark - for debug
	void printLogList(ofstream &_fout);
	void print();
};

#endif /* defined(__HFSParser__ProcessInfo__) */
